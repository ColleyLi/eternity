#ifndef OBJ_FILE_PARSE_H
#define OBJ_FILE_PARSE_H

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <vector>
#include <map>
using namespace std ;

#include "GameWindow.h"
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

  // WHY is this char* you ask?  Well,
  // this is really an experiment in performance.

  // With a char* I can use memsets to clear
  // this object out, with a string member,
  // I do not think its a good idea to memset it
  // because I don't know its internal implementation
  // if it has a v-table or not.

  // after its done I'll try swapping out all char*
  // with std::strings in these objects and see
  // if there's a performance hit, however marginal.



  // The CARSIM file format actually specifies
  // the texture file as "map_Kd", line looks like 
  /*
  
  map_Kd tread.tga
  
  */
  char *textureFilename ;

  /// Because this is the eternity engine,
  /// we may as well store a reference to
  /// the spriteId like, its SPRITE.
  int spriteId ;

  D3DXVECTOR3 Ka, Kd, Ks ;

  int illum ;

  /// `d` is diffusion, for
  /// things like glass.  presently unused.
  float d ;

  /// Specularity
  float Ns ;

public:
  #pragma region ctor and copy ctor
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

    cstrcpy( name, other.name ) ;
    cstrcpy( textureFilename, other.textureFilename ) ;
  }
  #pragma endregion

  char* getName()
  {
    if( name )
      return name ;
    else
      return "__UNNAMED__" ;
  }

  void setName( char* iName )
  {
    cstrfree(name);
    cstrcpy(name,iName);
  }

  // You don't need to read the next 3 lines ;)
  D3DXVECTOR3 getKa(){return Ka;}
  D3DXVECTOR3 getKd(){return Kd;}
  D3DXVECTOR3 getKs(){return Ks;}

  bool checkErr( char *fnName, char* str, int numArgsGot, int numArgsExpected )
  {
    if( numArgsGot != numArgsExpected )
    {
      warning( "%s failed to parse %d args from `%s`, parsed %d", fnName, numArgsExpected, str, numArgsGot ) ;
      return false ;
    }
    else
      return true ;
  }
  
  #pragma region set vectors
  /// You set Ka from a string
  /// that you read from a file
  /// @param str The string to parse the
  /// 3 values of Ka from
  /// @return True if the parse was successful,
  /// false if it failed (3 params not extracted)
  bool setKa( char* str )
  {
    int res = sscanf( str, "Ka %f %f %f", &Ka.x, &Ka.y, &Ka.z ) ;
    return checkErr( "setKa", str, res, 3 ) ;
  }

  bool setKd( char* str )
  {
    int res = sscanf( str, "Kd %f %f %f", &Kd.x, &Kd.y, &Kd.z ) ;
    return checkErr( "setKd", str, res, 3 ) ;
  }
  
  bool setKs( char* str )
  {
    int res = sscanf( str, "Ks %f %f %f", &Ks.x, &Ks.y, &Ks.z ) ;
    return checkErr( "setKs", str, res, 3 ) ;
  }
  #pragma endregion

  #pragma region non-vector get/sets
  int getIllum(){ return illum ; }
  float getD(){ return d ; }
  float getNs(){ return Ns ; }

  bool setIllum( char *str )
  {
    int res = sscanf( str, "illum %d", &illum ) ;
    return checkErr( "setIllum", str, res, 1 ) ;
  }

  bool setD( char *str )
  {
    int res = sscanf( str, "d %f", &d ) ;
    return checkErr( "setD", str, res, 1 ) ;
  }

  bool setNs( char *str )
  {
    int res = sscanf( str, "Ns %f", &Ns ) ;
    return checkErr( "setNs", str, res, 1 ) ;
  }
  #pragma endregion

  void setSpriteId( int iSpriteId )
  {
    spriteId = iSpriteId ;
  }

  char* getTextureFilename()
  {
    if( textureFilename )
      return textureFilename ;
    else
      return "__NO TEXTURE__" ;
  }

  void setTextureFilename( char *iFilename )
  {
    cstrfree( textureFilename ) ;
    cstrcpy( textureFilename, iFilename ) ;
  }


  ~Material()
  {
    cstrfree( name ) ;
    cstrfree( textureFilename ) ;
  }
} ;

class ObjFile ;

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

  /// The parent file, which
  /// contains the actual vertices
  /// referred to by facesVertices,
  /// facesNormals and facesTexture
  ObjFile *parentFile ;

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


  /// Each group uses a material,
  /// depending on which PART of the
  /// It seems that every time a
  /// Group is selected, a material
  /// is also specified.
  Material material ;

  Group( ObjFile *parent )
  {
    name = NULL ;
    parentFile = parent ;
  }

  char* getName()
  {
    if( name )
      return name ;
    else
      return "__UNNAMED__" ;
  }

  void setName( char *iName )
  {
    cstrfree( name ) ;
    cstrcpy( name, iName ) ;
  }

  ~Group()
  {
    cstrfree( name ) ;
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
#define MTL_TEXTURE_NAME "map_Kd"
// There could be others used by other formats
#define MTL_TEXTURE_NAME_LEN 6

/// Switch material 
#define OBJ_CHANGE_MATERIAL "usemtl"

/// A group is created by "g"
#define OBJ_GROUP_NAME      "g"

typedef map<string, Material*> /* as simply */ MatMap ;
typedef map<string, Material*>::iterator /* as simply */ MatMapIter ;

typedef map<string, Group*> /* as simply */ GroupMap ;
typedef map<string, Group*>::iterator /* as simply */ GroupMapIter ;

class ObjFile
{
  GameWindow * window ;

  string originalObjFilename, originalMtllibName ;

  /// Materials by Name, Material
  MatMap materials ;

  /// FACE tie togethers actually
  /// occur in groups.
  GroupMap groups ;

  /// Using D3DXVECTOR3's for
  /// storage of both verts and normals
  vector<D3DXVECTOR3> vertices ;
  vector<D3DXVECTOR3> normals ;

  /// We're not expecting volumetric
  /// textures..
  vector<D3DXVECTOR2> texCoords ;



public:

  // Ok, a bit of a kludge here.
  // For this object to load
  // texture objects seemlessly,
  // I actually need your GameWindow
  // instance.
  // If GameWindow were a true singleton
  // then accessing the one and only instance
  // of it shouldn't be so tough..
  // EVEN IF an extern declaration of it
  // is in GameWindow.h, and it is assumed
  // to exist, that would be fine as well..

  ObjFile( GameWindow* gameWindow, char* objFilename )
  {
    // Save off the reference
    window = gameWindow ;

    if( !objFilename )
    {
      error( "I can't open a NULL pointer filename!" ) ;
      return ;
    }

    originalObjFilename = objFilename ;

    // Try and open the file
    FILE * objFile = fopen( objFilename, "r" ) ;
    if( !objFile )
      error( "ObjFile: %s not found", objFilename ) ;

    // PASS #1
    firstPass( objFile ) ;

    rewind( objFile ) ; // eventually people won't
    // understand this function when there are
    // no more cassette or VHS tapes.

    // PASS #2
    secondPass( objFile ) ;
    
    

    
  }

  

  void firstPass( FILE* f )
  {
    // Get the material library filename,
    // open and load all materials
    int numVerts=0, numNormals=0, numTexCoords=0, numFaces=0 ;

    // Count up the number of faces
    // stored in each group.
    map<string, int> numFacesByGroup ;
    
    string currentGroup = ""; // we must know what
    // group we're on at all times.

    char buf[ 300 ] ;

    #pragma region parse file
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
      if( buf[0] == '#' )
      {
        // Its a comment, pass it thru
        info( buf ) ;
      }
      else if( buf[0] == 'v' && buf[1] == 'n' )
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
        numVerts++ ;
      }
      else if( buf[0] == 'f' && IS_WHITE(buf[1]) )
      {
        // face
        numFaces++ ;

        // Increase the numFaces count
        // FOR THE CURRENT GROUP

        // if currentGroup has a value
        // then it will already have been
        // inserted into the `numFacesByGroup`
        // map
        map<string,int>::iterator iter = numFacesByGroup.find( currentGroup ) ;
        if( iter == numFacesByGroup.end() )
        {
          error( "Obj file parse pass #1:  face specified "
            "to join a group that doesn't exist `%s`", currentGroup.c_str() ) ;
        }
        else
        {
          // The group name exists already, so increment face count
          iter->second = iter->second + 1 ;
        }
      }
      else if( buf[0] == 'g' && IS_WHITE(buf[1]) )
      {
        // Count DISTINCT groups, which'll just be groups.size() when
        // thsi is all done

        // A group.  Create it now, unless it already exists

        // Get the name
        char *groupName = buf+2 ; // pass "g ", name goes to end of line

        cstrnulllastnl(groupName) ;

        currentGroup = groupName ;

        //!! two problems here to do with file anomolies.
        // 1 Sometimes files contain an EMPTY "g",
        // usually at the very top of the file.
        // I'm not sure what this means but its
        // an empty group.. and the "g" statements
        // aren't usually followed by a material name either.
        // 2 The second problem is when a file has
        // "more than one name" in the group name string.
        // I'm not sure whta this means either, but it
        // ends up creating a "group" that has a unique name
        // (in this implemenation!)
        // instead of being a combination...
        // I'm going to leave it that way since it doesn't
        // seem to make sense to define groups of faces
        // that belong to more than just one group..
        // (although in Maya I think you CAN do this..)


        Group *group = getGroup( groupName ) ;
        if( group )
        {
          // The group exists
          // so skip.
          
        }
        else
        {
          // create the group and add it to the groups collection
          Group * g = new Group( this ) ;
          g->setName( groupName ) ;

          //!! I think I should wrap groupName in string(groupName)
          // but I haven't here.
          groups.insert( make_pair(
            groupName, g
          ) ) ;

          // Initialize numFaces @ 0
          numFacesByGroup.insert( make_pair( currentGroup, 0 ) ) ;
        }
      }
      else if( !strncmp( buf, MTL_LIBRARY, MTL_LIBRARY_LEN ) )
      {
        // Its the material library filename!  grab it!
        
        // Now we may as well parse this up here,
        // on the first pass.
        loadMtlFile( buf+(MTL_LIBRARY_LEN+1) ) ;
      }
    }
    #pragma endregion

    info( "verts=%d, normals=%d, texCoords=%d, faces=%d, groups=%d",
          numVerts, numNormals, numTexCoords, numFaces, groups.size() ) ;
    info( "First pass on %s complete", originalObjFilename.c_str() ) ;

    // Allocate space in the vertices, normals, texCoords arrays
    vertices.resize( numVerts ) ;
    normals.resize( numNormals ) ;
    texCoords.resize( numTexCoords ) ;

    // faces are divided up into the different groups,
    // so we can't really say right now faces.resize()..
    for( map<string, int>::iterator iter = numFacesByGroup.begin() ;
         iter != numFacesByGroup.end() ;
         ++iter )
    {
      info( "Group `%s` has %d faces", iter->first.c_str(), iter->second ) ;
    }

    // IF THERE WAS NO MATERIAL FILENAME
    // BY EOF, THEN CREATE A DEFAULT MATERIAL
  }

  void secondPass( FILE* file )
  {
    // In the second pass we actually
    // read up the vertices and save them.


  }

  Group* getGroup( string groupName )
  {
    GroupMapIter groupEntry = groups.find( groupName ) ;
    if( groupEntry == groups.end() )
    {
      // It doesn't exist
      return NULL ;
    }
    else
    {
      return groupEntry->second ;
    }
  }

  void loadMtlFile( char *mtllibName )
  {

    if( !mtllibName )
    {
      error( "Mtllibname was null" ) ;
      return ;
    }

    // Save this filename forever :)
    originalMtllibName = mtllibName ;

    FILE *mtlFile = fopen( mtllibName, "r" ) ;
    if( !mtlFile )
    {
      error( "Couldn't open mtllib %s", mtllibName ) ;
      return ;
    }

    char buf[ 300 ] ;
    Material *currentMaterial = NULL ;

    while( !feof( mtlFile ) )
    {
      fgets( buf, 300, mtlFile ) ;

      if( buf[0] == '#' )
      {
        // its a comment
        //info( buf ) ;
      }

      // if it starts with newmtl,
      // its a new material
      else if( !strncmp( buf, MTL_NEW, MTL_NEW_LEN ) )
      {
        // It means we're starting a new
        // material..

        // If the currentMaterial
        // isn't NULL then, we should save it
        // before re-creating a new one
        if( currentMaterial )
        {
          // Save it, here keyed
          // by its name
          materials.insert(
            make_pair(
              currentMaterial->getName(),
              currentMaterial
            )
          ) ;

          info( "Loaded material `%s`, texture: `%s`",
            currentMaterial->getName(),
            currentMaterial->getTextureFilename() ) ;

          // Make a NEW material
          // / reset the material
          currentMaterial = new Material() ;
        }

        char *mtlName = buf+(MTL_NEW_LEN+1) ;

        // Retrieve the new material's name and save it
        currentMaterial->setName( mtlName ) ;
      }

      // The rest of these just affect the
      // CURRENT MATERIAL, whatever that is.
      else
      {
        // For this to RUN, there MUST have been
        // a CREATE MATERIAL stmt previously
        // This'll only happen if there are BLANK LINES
        // above the first newmtl stmt
        // in the worst case you'll see MATERIAL STATEMENTS here
        // and so you'll know something is wrong
        if( !currentMaterial )
          warning( "material parse: no material yet (a line had to be ignored, `%s`)", buf ) ;

        // The ambient term is being specified
        else if( !strncmp( buf, MTL_AMBIENT, MTL_AMBIENT_LEN ) )
          currentMaterial->setKa( buf ) ;

        // The ambient term is being specified
        else if( !strncmp( buf, MTL_DIFFUSION, MTL_DIFFUSION_LEN ) )
          currentMaterial->setKd( buf ) ;

        // The specularity term is being specified
        else if( !strncmp( buf, MTL_SPECULARITY, MTL_SPECULARITY_LEN ) )
          currentMaterial->setKs( buf ) ;

        // The illum term is being specified
        else if( !strncmp( buf, MTL_ILLUM, MTL_ILLUM_LEN ) )
          currentMaterial->setIllum( buf ) ;

        // The Ns term is being specified
        else if( !strncmp( buf, MTL_NSPEC, MTL_NSPEC_LEN ) )
          currentMaterial->setNs( buf ) ;

        else if( !strncmp( buf, MTL_D, MTL_D_LEN ) )
          currentMaterial->setD( buf ) ;

        else if( !strncmp( buf, MTL_TEXTURE_NAME, MTL_TEXTURE_NAME_LEN ) )
        {
          // We found a texture filename
          // We need to assign it the next
          // available texture id.
          char *spriteFile = buf+(MTL_TEXTURE_NAME_LEN+1) ;
          int spriteId = window->getNextSpriteId() ;

          window->loadSprite( spriteId, spriteFile ) ;
          
          // Save the sprite id
          currentMaterial->setSpriteId( spriteId ) ;
          currentMaterial->setTextureFilename( spriteFile ) ;

        }

      }
    }

    // Save the last material
    if( currentMaterial )
    {
      materials.insert(
        make_pair(
          currentMaterial->getName(),
          currentMaterial
        )
      ) ;
    }

    info( "%d materials loaded", materials.size() ) ;

  }

  
  void computeNormals()
  {
    // Computes normals on loaded vertex set
  }

  
} ;

#endif // OBJ_FILE_PARSE_H