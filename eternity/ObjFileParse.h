#ifndef OBJ_FILE_PARSE_H
#define OBJ_FILE_PARSE_H

#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <map>
using namespace std ;


#include "helperFunctions.h"


/// returns TRUE of a character x
/// is a whitespace character
///    9 == horizontal tab
///   10 == line feed,
///   11 == "vertical tab" (not that anyone uses it..)
///   13 == carriage return
///   32 == space
#define IS_WHITE(x) (x==9 || x==10 || x==11 || x==13 || x==32)

struct Material
{
private:
  /// Material changes are referenced
  /// by name.
  char* name ; // This is the only variable
  // we need guard since setting it requires
  // freeing its old value.

public:
  D3DXVECTOR3 Ka, Kd, Ks ;

  int illum ;

  /// This is diffusion, for
  /// things like glass.  presently unused.
  float d ;

  /// Specularity
  float Ns ;

  Material()
  {
    /*
    name = 0 ; // the "nothing"
    // material means.. a material
    // hasn't been defined yet

    Ka.x = Ka.y = Ka.z = 
      Kd.x = Kd.y = Kd.z =
      Ks.x = Ks.y = Ks.z = 0.0f ;

    illum = 0 ;
    d = Ns = 0.0f ;

    */
    // As long as this doesn't contain
    // a vtable, you can memset it like this
    // no problem.
    memset( this, 0, sizeof( Material ) ) ;
  }

  Material( const Material& other )
  {
    // The only thing that needs to be
    // copied specially is the name
  
    // Straight memcopy the whole
    // struct, then do a strcpy
    // for the string in a new object
    memcpy( this, &other, sizeof(Material) ) ;

    name = (char*)malloc( strlen( other.name ) + 1 ) ;
    strcpy( this->name, other.name ) ;
  }

  char* getName()
  {
    return name ;
  }

  void setName( char* iName )
  {
    if( name )
      free(name) ;
    name = NULL ;

    if( !iName )
      return ; // user meant to clear
    // out old name, so that's done now
    // no new name to save

    name = (char*)malloc( strlen(iName)+1 ) ;
    strcpy( name, iName ) ;
  }

  ~Material()
  {
    if( name )
    {
      free( name ) ;
      name = NULL ;
    }
  }
} ;


/// Groups are not just
/// vertices -- instead
/// they are collections
/// of faces
/// The file switches between
/// groups as it parses thru
/// -- while running thru the
/// file faces will be added to different groups
struct Group
{
private:
  /// The name of the group
  /// as it appears in the obj file
  char *name ;

public:
  /// Using D3DXVECTOR3's for
  /// storage of both verts and normals
  vector<D3DXVECTOR3> vertices ;
  vector<D3DXVECTOR3> normals ;

  /// We're not expecting volumetric
  /// textures..
  vector<D3DXVECTOR2> texCoords ;

  /// Each group uses a material,
  /// depending on which PART of the
  /// It seems that every time a
  /// Group is selected, a material
  /// is also specified.
  Material material ;

  Group()
  {
    name = NULL ;

    /// The collection of verts+normals
    /// will grow as we parse thru the file
    vertices.resize( 0 ) ;
    normals.resize( 0 ) ;
    texCoords.resize( 0 ) ;
    material = Material() ; // redundant, I know.
  }

  char* getName()
  {
    return name ;
  }

  void setName( char *iName )
  {
    if( name )
    {
      free( name ) ;
      name = NULL ;
    }

    if( !iName )
      return ; // user wanted to unset name only

    name = (char*)malloc( strlen(iName)+1 ) ;
    strcpy( name, iName ) ;

  }

  ~Group()
  {
    if( name )
    {
      free( name ) ;
      name = NULL ;
    }
  }
} ;

// Define the dictionary words
// and their lengths 
#define MTL_LIBRARY     "mtllib"
#define MTL_LIBRARY_LEN 6
#define MTL_NEW         "newmtl"
#define MTL_NEW_LEN     6
#define MTL_AMBIENT     "Ka"
#define MTL_AMBIENT_LEN 2
#define MTL_DIFFUSION   "Kd"
#define MTL_DIFFUSION_LEN 2
#define MTL_SPECULARITY "Ks"
#define MTL_SPECULARITY_LEN 2
#define MTL_ILLUM       "illum"
#define MTL_ILLUM_LEN   6
#define MTL_NSPEC       "Ns"
#define MTL_NSPEC_LEN   2
#define MTL_D           "d"
#define MTL_D_LEN       1

/// Switch material 
#define OBJ_CHANGE_MATERIAL "usemtl"

/// A group is created by "g"
#define OBJ_GROUP_NAME      "g"

typedef map<char*, Material> /* as simply */ MatMap ;
typedef map<char*, Material>::iterator /* as simply */ MatMapIter ;

typedef map<char*, Group> /* as simply */ GroupMap ;
typedef map<char*, Group>::iterator /* as simply */ GroupMapIter ;

class ObjFile
{
  /// Materials by Name, Material
  MatMap materials ;

  /// FACE tie togethers actually
  /// occur in groups.
  GroupMap groups ;

  /// The faces array is an index buffer
  /// indexing the vertices and normals
  /// arrays
  vector<int> facesVertices ;

  /// The tie-together for normals is
  /// completely independent of the
  /// tie-together for vertices.. hence
  /// why the #normals != #vertices necessarily.
  vector<int> facesNormals ;

  /// Texture coordinate lookup
  vector<int> facesTexture ;

public:

  Group* getGroup( char *groupName )
  {
    GroupMapIter groupEntry = groups.find( groupName ) ;
    if( groupEntry == groups.end() )
    {
      // It doesn't exist
      return NULL ;
    }
    else
    {
      return &(groupEntry->second) ;
    }
  }

  void firstPass( FILE* f )
  {
    // Get the material library filename,
    // open and load all materials
    int numVerts, numNormals, numTexCoords ;
    int numFaces ;
    
    char buf[ 300 ] ;

    while( !feof( f ) )
    {
      // Count normals,
      // Count texture coords,
      // count verticies,
      // count faces

      // For each line, we have to
      // read and determine if it is
      // a v, vn, vt, f
      
      fgets( buf, 300, f ) ;
      if( buf[0] == 'v' && buf[1] == 'n' )
      {
        // vertex normal
        numNormals++ ;
      }
      else if( buf[0] == 'v' && buf[1] == 't' )
      {
        //texture coordinate
        numTexCoords++ ;
      }
      else if( buf[0] == 'v' && buf[1] == 'p' )
      {
        // parameter space vertices
        warning( "Sorry, parameter space vertices not supported, ignoring" ) ;
      }
      else if( buf[0] == 'v' && IS_WHITE(buf[1]) )
      {
        // The only other thing that starts 
        // with 'v' is a vertex itself.
        // the buf[1] character will be
        // either a SPACE or a TAB
        numVertices++ ;
      }
      else if( buf[0] == 'f' && IS_WHITE(buf[1]) )
      {
        // face
        numFaces++ ;
      }
      else if( buf[0] == 'g' && IS_WHITE(buf[1]) )
      {
        // A group.  Create it now, unless it already exists

        // Get the name
        char *groupName = buf+2 ; // pass "g ", name goes to end of line

        Group *group = getGroup( groupName ) ;
        if( group )
        {
          // The group exists
          // so skip.
          //continue ; // there's nothign else to process this line
          // (though there's no need for the continue stmt!)
        }
        else
        {
          // create the group and add it to the groups collection
          Group g ;
          g.setName( groupName ) ;
          /// !!there is too much copy construction going on here.
          /// we need to change the map to taking a POINTER to a Group.
          groups.insert( make_pair(
            g.getName(), g
          ) ) ;
        }
      }
      

      // Count DISTINCT groups,
      

    }


    // IF THERE WAS NO MATERIAL FILENAME
    // BY EOF, THEN CREATE A DEFAULT MATERIAL
  }

  void secondPass( FILE* file )
  {
  }

  ObjFile( char* objFilename )
  {
    // Open a file and scan thru
    // for the vertex set.

    // 2 passes:
    
    // PASS #1:  
    



    // First, gather up the materials
    FILE * objFile = fopen( objFilename, "r" ) ;
    if( !objFile )
      error( "ObjFile: %s not found", objFilename ) ;

    firstPass( objFile ) ;

    rewind( objFile ) ; // eventually people won't
    // understand this function when there are
    // no more cassette or VHS tapes.

    secondPass( objFile ) ;
    
    char buf[ 300 ] ;
    Material currentMaterial ;

    while( !feof( mtlFile ) )
    {
      fgets( buf, 300, mtlFile ) ;

      // if it starts with newmtl,
      // its a new material
      if( !strncmp( buf, MTL_NEW, MTL_NEW_LEN ) )
      {
        // It means we're starting a new
        // material..

        // If the currentMaterial
        // isn't the NOTHING material,
        // then, we should save it
        // before re-creating a new one
        if( currentMaterial.name )
        {
          // Save it, here keyed
          // by its name
          materials.insert(
            make_pair(
              currentMaterial.name,
              currentMaterial
            )
          ) ;

          // Make a NEW material
          // / reset the material
          currentMaterial = Material() ;
        }


        // Retrieve the new name
        // and save it

        // From position 7 in the
        // array will be the material's name
        currentMaterial.setName( buf+7 ) ;
        // We're making some assumptions here
        // about not more than one space etc
        // but this is a FILE FORMAT, so I
        // think its safe to do so.
      }

      // I wonder if this can be done
      // with less hard comparisons by
      // checking if sscanf( "Ka ..." ) __fails__
      // If it does then it means this isn't
      // the Ka term..
      if( !strncmp( buf, MTL_AMBIENT, MTL_AMBIENT_LEN ) )
      {
        // The ambient term is being specified
        sscanf( buf, "Ka %f %f %f",
          &currentMaterial.Ka.x,
          &currentMaterial.Ka.y,
          &currentMaterial.Ka.z ) ;
      }

      if( !strncmp( buf, MTL_DIFFUSION, MTL_DIFFUSION_LEN ) )
      {
        // The ambient term is being specified
        sscanf( buf, "Kd %f %f %f",
          &currentMaterial.Kd.x,
          &currentMaterial.Kd.y,
          &currentMaterial.Kd.z ) ;
      }

      if( !strncmp( buf, MTL_SPECULARITY, MTL_SPECULARITY_LEN ) )
      {
        // The ambient term is being specified
        sscanf( buf, "Ks %f %f %f",
          &currentMaterial.Ks.x,
          &currentMaterial.Ks.y,
          &currentMaterial.Ks.z ) ;
      }

      if( !strncmp( buf, MTL_ILLUM, MTL_ILLUM_LEN ) )
      {
        // The ambient term is being specified
        sscanf( buf, "illum %d",
          &currentMaterial.illum ) ;
      }

      if( !strncmp( buf, MTL_NSPEC, MTL_NSPEC_LEN ) )
      {
        // The ambient term is being specified
        sscanf( buf, "Ns %f",
          &currentMaterial.Ns ) ;
      }

      if( !strncmp( buf, MTL_D, MTL_D_LEN ) )
      {
        // The ambient term is being specified
        sscanf( buf, "d %f",
          &currentMaterial.d ) ;
      }
    }

    FILE * objFile = fopen( objFilename, "r" ) ;
    if( !objFile )
      error( "ObjFile: %s not found", objFilename ) ;

    
  }

  void computeNormals()
  {
    // Computes normals on loaded vertex set
  }

  
} ;

#endif // OBJ_FILE_PARSE_H