#ifndef OBJ_FILE_PARSE_H
#define OBJ_FILE_PARSE_H

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <vector>
#include <map>
using namespace std ;

#include "GamePak.h"
#include "helperFunctions.h"


/// returns TRUE of a character x
/// is a whitespace character
///    9 == horizontal tab
///   10 == line feed,
///   11 == "vertical tab" (not that anyone uses it..)
///   13 == carriage return
///   32 == space
#define IS_WHITE(x) (x==9 || x==10 || x==11 || x==13 || x==32)



// Notes:
// We're going to have to make a couple of
// assumptions here.

// The first is, A MODEL IS CONSISTENT
// across its groups.
// That means a model __does not__ use
// v/t/n format for some vertices, and
// only v/t format for others, while
// v//n format for yet others.

// Because of this assumption
// we'll slam the entire model
// into a single vertex buffer
// and draw it with a single call.

// So, a model might use multiple materials
// throughout.  All this means is,
// we need to adjust the DIFFUSE color
// component as we traverse the model.

// We can have a fmt like this:

// v  (5.5,3.7,1.5)
// t  (1.0,2.0)
// n  (2.1,5.2,1.1)
// cA (255,255,0)
// cD (255,128,0)
// cS (100,130,10)

// So this results in a lot of repetition
// of the "cA", "cD" and "cS" values
// (because its basically the MATERIAL)...

// So we'll go with the repeated data approach.

#include "D3DVertex.h"

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

  D3DMATERIAL9 d3dmaterial ;
  
public:
  static D3DMATERIAL9 defaultMaterial ;

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
  D3DCOLORVALUE getKa(){return d3dmaterial.Ambient;}
  D3DCOLORVALUE getKd(){return d3dmaterial.Diffuse;}
  D3DCOLORVALUE getKs(){return d3dmaterial.Specular;}
  D3DCOLORVALUE getKe(){return d3dmaterial.Emissive;}
  float getNs(){ return d3dmaterial.Power ; }

  #pragma region sets
  /// You set Ka from a string
  /// that you read from a file
  /// @param str The string to parse the
  /// 3 values of Ka from
  /// @return True if the parse was successful,
  /// false if it failed (3 params not extracted)
  bool setKa( char* str )
  {
    int res = sscanf( str, "Ka %f %f %f",
      &d3dmaterial.Ambient.r,
      &d3dmaterial.Ambient.g,
      &d3dmaterial.Ambient.b ) ;
    return argCheck( "setKa", str, res, 3 ) ;
  }

  bool setKd( char* str )
  {
    int res = sscanf( str, "Kd %f %f %f",
      &d3dmaterial.Diffuse.r,
      &d3dmaterial.Diffuse.g,
      &d3dmaterial.Diffuse.b ) ;
    return argCheck( "setKd", str, res, 3 ) ;
  }
  
  bool setKs( char* str )
  {
    int res = sscanf( str, "Ks %f %f %f",
      &d3dmaterial.Specular.r,
      &d3dmaterial.Specular.g,
      &d3dmaterial.Specular.b ) ;
    return argCheck( "setKs", str, res, 3 ) ;
  }

  bool setKe( char* str )
  {
    int res = sscanf( str, "Kd %f %f %f",
      &d3dmaterial.Emissive.r,
      &d3dmaterial.Emissive.g,
      &d3dmaterial.Emissive.b ) ;
    return argCheck( "setKd", str, res, 3 ) ;
  }

  bool setNs( char *str )
  {
    int res = sscanf( str, "Ns %f", &d3dmaterial.Power ) ;
    return argCheck( "setNs", str, res, 1 ) ;
  }
  #pragma endregion

  void setSpriteId( int iSpriteId )
  {
    spriteId = iSpriteId ;
  }

  int getSpriteId()
  {
    return spriteId ;
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

  D3DMATERIAL9* getD3DMaterial()
  {
    return &d3dmaterial ;
  }

  ~Material()
  {
    cstrfree( name ) ;
    cstrfree( textureFilename ) ;
  }
} ;

class ObjFile ;

enum FaceSpecMode
{
  Vertices  = 1 << 0,
  Normals   = 1 << 1,
  Texcoords = 1 << 2
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
  
  /// How faces are specified in this file.
  /// It can be any combination of these 3
  FaceSpecMode faceSpecMode ;
  // FaceSpecMode used to be an attribute of
  // the entire model file, until model files
  // were discovered that had v//n format for
  // some groups but v/t/n format for other groups,
  // in the same file.

  /// Tells you if this group is
  /// in a specific FaceSpecMode.
  bool isFaceSpecMode( int queryFSM )
  {
    // its just straight equals.
    return faceSpecMode == queryFSM ;
  }
  
  /// The actual vertex declaration being used by
  /// THIS obj model instance
  D3DWindow::VertexType vType ;

  /// The parent file, which
  /// contains the actual vertices
  /// referred to by facesVertexIndices,
  /// facesNormalIndices and facesTextureIndices
  ObjFile *parentFile ;

  #pragma region temporary construction variables
  // These 3 faces* arrays are
  // temporary because when we
  // draw we'll need to have
  // pre-assembled the faces
  // into a single array.

  /// The faces array is an index buffer
  /// indexing the vertices and normals
  /// arrays
  vector<int> facesVertexIndices ;

  /// The tie-together for normals is
  /// completely independent of the
  /// tie-together for vertices.. hence
  /// why the #normals != #vertices necessarily.
  vector<int> facesNormalIndices ;

  /// Texture coordinate lookup
  vector<int> facesTextureIndices ;


  /// indexCount*:  A running tally of the total number
  /// of indices that will be in 
  /// each of facesVertexIndices, facesNormalIndices, facesTextureIndices
  /// when the parse is complete.
  int indexCountPass1 ;
  int indexCountPass2 ;

  /// vertCount*:  A running tally of the total number
  /// of vertices that will be in
  /// (combinedVerticesV | combinedVerticesVN..)
  /// when the parse is complete
  int vertCountPass1 ;
  int vertCountPass2 ;
  #pragma endregion


  /// The final array of vertex coords.
  /// Only ONE of these 4 can possibly
  /// be used, depends on the vertex format
  /// of the file.  Generally .OBJ files
  /// don't change format in the middle,
  /// instead the modeller will export like
  /// 3 different .OBJ files, all with different
  /// vertex formats.
  vector<Vertex> combinedVerticesV ;
  vector<VertexT> combinedVerticesVT ;
  vector<VertexN> combinedVerticesVN ;
  vector<VertexTN> combinedVerticesVTN ;


  /// Each group uses a material,
  /// It is a good assumption that
  /// each group uses only ONE material.
  Material *material ;

  Group( ObjFile *parent )
  {
    name = NULL ;
    parentFile = parent ;
    material = NULL ;

    indexCountPass1 = 0 ;
    indexCountPass2 = 0 ;
    vertCountPass1 = 0 ;
    vertCountPass2 = 0 ;

    // Initialize face spec mode to nothing
    faceSpecMode = (FaceSpecMode)0 ;
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

  D3DMATERIAL9* getD3DMaterial()
  {
    if( !material )
    {
      // use a default white material instead.
      return &Material::defaultMaterial ;
    }
    else
    {
      return material->getD3DMaterial() ;
    }
  }

  Material* getMaterial()
  {
    return material ;
  }

  /// The number of faces is
  /// equal to the number of
  /// indices divided by 3,
  /// since every 3 indices together
  /// produce a face
  int getNumFaces()
  {
    return this->facesVertexIndices.size() / 3 ;
  }

  ~Group()
  {
    cstrfree( name ) ;
  }
} ;


#pragma region defs
// Define the dictionary words
// and their lengths
// In case you're wondering
// why I define *_LEN for each
// its to avoid hard-coding numbers
// like 2, or 6, and also to avoid
// unnecessary calls to strlen (efficiency)
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
#define OBJ_CHANGE_MATERIAL_LEN 6

/// A group is created by "g"
#define OBJ_GROUP_NAME      "g"

typedef map<string, Material*> /* as simply */ MatMap ;
typedef map<string, Material*>::iterator /* as simply */ MatMapIter ;

typedef map<string, Group*> /* as simply */ GroupMap ;
typedef map<string, Group*>::iterator /* as simply */ GroupMapIter ;

typedef map<string, int> /* as simply */ MapStringInt ;
typedef map<string, int>::iterator /* as simply */ MapStringIntIter ;
#pragma endregion

class ObjFile
{

public:
  /// Call this once to initialize the class.
  static void init( GamePak *window )
  {
    // Set up the default material
    setColor( &Material::defaultMaterial.Ambient,  1.f,  1.f, 1.f, 1.f ) ;
    setColor( &Material::defaultMaterial.Diffuse,  1.f,  1.f, 1.f, 1.f ) ;
    setColor( &Material::defaultMaterial.Specular, 1.f,  1.f, 1.f, 1.f ) ;
    setColor( &Material::defaultMaterial.Emissive, 0.f,  0.f, 0.f, 0.f ) ;
  }

private:

  GamePak * window ;

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


  /// This is the total # triangular faces
  /// in the model, after triangulation.
  /// 3*this number is the size of the
  /// ACTIVE combinedVertices? array.
  int numTriangleFacesTOTAL ;

  /// The string format we expect
  /// faces to be in, depends on
  /// FaceSpecMode.
  /// The general format is
  /// f v/vt/vn v/vt/vn v/vt/vn v/vt/vn
  
  /// For Vertices only, we use something like
  /// f v v v
  /// f 3 5 1
  
  /// For Vertices|Normals (no texture coords), we use
  /// f v//vn v//vn v//vn
  /// f 3//2 1//5 6//1

  /// For Vertices|Texcoords, we use
  /// f v/vt v/vt v/vt
  /// f 4/1 3/5 2/2

  char *faceStringFormat ;

public:

  // Ok, a bit of a kludge here.
  // For this object to load
  // texture objects seemlessly,
  // I actually need your GamePak
  // instance.
  // If GamePak were a true singleton
  // then accessing the one and only instance
  // of it shouldn't be so tough..
  // EVEN IF an extern declaration of it
  // is in GamePak.h, and it is assumed
  // to exist, that would be fine as well..

  ObjFile( GamePak* gameWindow, char* objFilename )
  {
    // Save off the reference
    window = gameWindow ;

    if( !objFilename )
    {
      error( "I can't open a NULL pointer filename!" ) ;
      return ;
    }

    info( "*** PARSING %s ***", objFilename ) ;
    originalObjFilename = objFilename ;

    // Try and open the file
    FILE * objFile = fopen( objFilename, "r" ) ;
    if( !objFile )
    {
      error( "ObjFile: %s not found", objFilename ) ;
      return ;
    }

    // PASS #1
    firstPass( objFile ) ;

    rewind( objFile ) ; // eventually people won't
    // understand this function when there are
    // no more cassette or VHS tapes.

    // PASS #2
    secondPass( objFile ) ;
  }

  
private:
  void firstPass( FILE* f )
  {
    
    // Get the material library filename,
    // open and load all materials
    int numVerts=0, numNormals=0, numTexCoords=0 ;

    // numFacesStock is just the number of "f 3 4 5" statements
    // in the file,
    int numFacesStock = 0 ;
    
    // numTriangleFacesTOTAL is total # faces, after triangulation
    numTriangleFacesTOTAL = 0 ;   // This will be tallied up
    // on this pass, and verified on the second pass.
    

    string currentGroupName = ""; // we must know what
    // group we're on at all times.
    Group *currentGroup = NULL ;

    char buf[ 300 ] ;

    #pragma region parse file pass_1
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

      // If the newline gets left on at the end,
      // we actually want to cut it off.
      cstrnulllastnl( buf ) ;

      if( buf[0] == '#' )
      {
        // Its a comment, pass it thru
        //info( buf ) ;
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
        if( !currentGroup )
        {
          warning( "Obj file parse pass #1:  face specified before any group declarations. "
            "Creating and adding a \"default\" group" ) ;

          // create the group and add it to the groups collection
          currentGroup = new Group( this ) ;
          currentGroup->setName( "__DEFAULT_GROUP__" ) ;

          groups.insert( make_pair(
            "__DEFAULT_GROUP__", currentGroup
          ) ) ;
        }
        
        // Now currentGroup is always defined here.


        // ALWAYS detect faceSpecMode to see if
        // there are any anomolies.
        FaceSpecMode fsm = (FaceSpecMode)getFaceSpecMode( buf ) ;

        if( fsm != currentGroup->faceSpecMode )
        {
          if( !currentGroup->faceSpecMode )
          {
            info( "FaceSpecMode is `%s` for group `%s`", getFaceSpecModeString( fsm ), currentGroup->getName() ) ;
          }
          else
          {
            warning( "Anomaly detected, facespecmode changed from %s to %s while in group %s",
               getFaceSpecModeString( currentGroup->faceSpecMode ),
               getFaceSpecModeString( fsm ),
               currentGroup->getName() ) ;
          }
          
          currentGroup->faceSpecMode = fsm ;
        }

        // face. just keep track of total, this
        // count is a sort of parity check
        // (total #faces counted across GROUPS == total #faces counted)
        // The numbers wouldn't add up only if there
        // was a programmer error in the parse
        numFacesStock++ ;

        // We save ONLY TRIANGLES, not other types
        // of polygons.  This means the NUMBER OF FACES
        // that results from a line
        // f 3 6 9 1
        // is __2__, not just one.

        // if the faces are specified
        // as QUADS, or pentagons, then we need to break this
        // into (VERTS-2) faces.
        int verticesInFace = countVerts( buf ) ;
        int facesFromVerts = verticesInFace - 2 ;
        int numVertsAdded = facesFromVerts*3 ;

        numTriangleFacesTOTAL += facesFromVerts ;

        // Increase the numVerts count
        // FOR THE CURRENT GROUP

        // Increment the total number
        // that should be in group->facesVertexIndices
        currentGroup->indexCountPass1 += verticesInFace ;

        // Increment the total number
        // that should be in group->combinedVerticesV
        currentGroup->vertCountPass1 += numVertsAdded ;
      }
      else if( buf[0] == 'g' && IS_WHITE(buf[1]) )
      {
        // A group.  Create it now, unless it already exists

        char *groupName = buf+2 ; // pass "g ", name goes to end of line

        // If there's a long name with spaces just cut off the extra names
        // this puts it in the same group as the first name.
        cstrnullnextsp( groupName ) ;  // reason for this is
        // when a file has "more than one name" in the group name string.
        // I'm not sure whta this means either, but it
        // creates a "group" that has a unique name
        // FIXED by cutoff.


        currentGroupName = groupName ;

        // Sometimes files contain an EMPTY "g",
        // usually at the very top of the file.
        //
        // I'm not sure what this means but its
        // an empty group.. and the "g" statements
        // aren't usually followed by a material name either.
        
        currentGroup = getGroup( groupName ) ;
        if( currentGroup )
        {
          // The group exists
          // so skip.
        }
        else
        {
          // create the group and add it to the groups collection
          currentGroup = new Group( this ) ;
          currentGroup->setName( groupName ) ;

          groups.insert( make_pair(
            groupName, currentGroup
          ) ) ;

          // Vert counts will be init to 0 in ctor.
        }
      }
      else if( !strncmp( buf, MTL_LIBRARY, MTL_LIBRARY_LEN ) )
      {
        // Its the material library filename!  grab it!
        
        // Now we may as well parse this up here,
        // on the first pass.
        loadMtlFile( buf+(MTL_LIBRARY_LEN+1) ) ;
      }
    } // END OF FILE parse
    #pragma endregion

    info( "verts=%d, normals=%d, texCoords=%d, "
          "stock faces=%d triangulated faces=%d, "
          "groups=%d",
           numVerts, numNormals, numTexCoords,
           numFacesStock, numTriangleFacesTOTAL,
           groups.size() ) ;
    info( "First pass on %s complete", originalObjFilename.c_str() ) ;

    // Allocate space in the vertices, normals, texCoords arrays
    // +1 because index 0 IS NOT USED.
    vertices.resize( numVerts+1 ) ;
    normals.resize( numNormals+1 ) ;
    texCoords.resize( numTexCoords+1 ) ;

    // faces are divided up into the different groups,
    // so we can't really say right now faces.resize()..
    foreach( GroupMapIter, groupIter, groups )
    {
      Group *g = groupIter->second ;

      // Allocate enough space in each group object
      // to hold this many vertex indexes
      // g->getNumFaces() WILL READ 0 HERE

      // g->vertCountPass1 is used because
      // it is a count of the total # vertices
      // in the model AFTER triangulation.

      if( g->faceSpecMode & Vertices )  // it MUST be!
        g->facesVertexIndices.resize( g->vertCountPass1 ) ;
      else
        warning( "Anomoly:  no vertices" ) ;

      if( g->faceSpecMode & Normals )
        groupIter->second->facesNormalIndices.resize( g->vertCountPass1 ) ;
      if( g->faceSpecMode & Texcoords )
        groupIter->second->facesTextureIndices.resize( g->vertCountPass1 ) ;

      info( "Group `%s` has %d vertices, %d faces", g->getName(), g->vertCountPass1, g->getNumFaces() ) ;


      // Determine what vertex declaration to use
      // based on face mode
      if( g->isFaceSpecMode(Vertices|Texcoords|Normals) ) // v/t/n
      {
        g->vType = D3DWindow::PositionTextureNormal ;
        info( "Group %s using PositionTextureNormal vertex", g->getName() ) ;
      }
      else if( g->isFaceSpecMode(Vertices|Texcoords) ) // v/t
      {
        g->vType = D3DWindow::PositionTexture ;
        info( "Group %s using PositionTexture vertex", g->getName() ) ;
      }
      else if( g->isFaceSpecMode(Vertices|Normals) )   // v//n
      {
        g->vType = D3DWindow::PositionNormal ;
        info( "Group %s using PositionNormal vertex", g->getName() ) ;
      }
      else   // v
      {
        g->vType = D3DWindow::Position ;
        info( "Group %s using Position vertex", g->getName() ) ;
      }
    }
  }

  int countVerts( char *buf )
  {
    int count = 0 ;
    char *line = buf+2 ;

    // advance until the first nonwhitespace character
    // past the "f "
    while( isspace( *line ) )  line++ ;

    int len = strlen( line ) ;
    bool onWord = true ;
    
    for( int i = 0 ; i < len; i++ )
    {
      if( isspace( line[i] ) )
      {
        // INcrement count only when
        // stepping OFF word to whitespace
        if( onWord )
        {
          count++ ;
          onWord = false ;
        }
      }
      else
      {
        onWord = true ;
      }
    }

    // If you ended while still on word
    // then increment count here to count
    // the "last" word
    if( onWord )
      count++ ;

    return count ;
  }

  #pragma region face spec mode stuff
  // These functions stay in ObjFile class
  // because they have to do with PARSING
  // the obj file, and aren't really related
  // to Group, (altough FaceSpecMode is defined
  // in Group)
  char* getFaceSpecModeString( FaceSpecMode fsm )
  {
    //if( (fsm & Vertices) && (fsm & Normals) && (fsm & Texcoords) )
    if( fsm == (Vertices|Normals|Texcoords) )
      return "v/t/n" ;
    else if( fsm == (Vertices|Texcoords) )
      return "v/t";
    else if( fsm == (Vertices|Normals) )
      return "v//n";
    else if( fsm == Vertices )
      return "v" ;
    else
      return "INVALID FaceSpecMode" ;
  }

  int getFaceSpecMode( char *str )
  {
    char *pos = str+2 ; // skip "f "
    
    while( !isspace(*pos) )
    {
      if( *pos == '/' )  // Hit first slash.
      {
        // Check if we have a slash right next to it,
        ++pos ;
        if( *pos == '/' )
          return Vertices | Normals ;  // "v//n"

        // If we didn't have a slash right next to it,
        // it'll either be DIGITS then white space (v/t)
        // or there'll be ANOTHER white slash before
        // the next white space.

        while( !isspace(*pos) )
        {
          if( *pos == '/' )
            return Vertices | Texcoords | Normals ; // "v/t/n"
          ++pos ;
        }

        // if it WAS a space next (before
        // hitting a /) then we have v/t
        return Vertices | Texcoords ; //"v/t"
      }

      // next char.
      ++pos;
    }
    
    // If didn't return above, means hit space
    // before hitting slash at all
    return Vertices ; // "v"
  }
  #pragma endregion

  #pragma region parse vertex and extract faces
  bool parseVertex( char *buf, D3DXVECTOR3 *v )
  {
    int res = sscanf( buf, "v %f %f %f", &v->x, &v->y, &v->z ) ;
    return argCheck( "parseVertex", buf, res, 3 ) ;
  }

  bool parseNormal( char *buf, D3DXVECTOR3 *v )
  {
    int res = sscanf( buf, "vn %f %f %f", &v->x, &v->y, &v->z ) ;
    return argCheck( "parseNormal", buf, res, 3 ) ;
  }

  bool parseTexcoord( char *buf, D3DXVECTOR2 *v )
  {
    int res = sscanf( buf, "vt %f %f", &v->x, &v->y ) ;
    return argCheck( "parseTexcoord", buf, res, 2 ) ;
  }

  void extractFaces( char *buf, int numVerts, Group *group )
  {
    int index = group->vertCountPass2 ;

    // We know how many vertices to expect,
    // so we're going to save each in a temporary buffer
    // The reason for the temporary buffer
    // is we need to triangulate
    vector<int> verts( numVerts ) ;
    vector<int> normals( numVerts ) ;
    vector<int> texcoords( numVerts ) ;

    // Extract numVerts from buf
    // f 2 4 1 5
    // f 8/5 2/7 1/3 7/7 1/9 3/7
    // f 2//6 3//5 1//8 8//3 9//7
    // f 9/2/5 8/6/7 9/1/3

    // Assume we start here.  So we know everything about the line.  

    FaceSpecMode ff = (FaceSpecMode)getFaceSpecMode( buf ) ; // E.g. "v/t" ;

    char *line = buf + 2 ;
    while( isspace( *line ) ) ++line ; // skip past any whitespace in front

    // now, based on the line format, use the correct format string
    if( (ff & Texcoords) && (ff & Normals) )
    {
      // v/t/n
      int v,t,n;
      char *pos = line ;
      int vReadCount = 0 ;

      while( pos ) // while we haven't reached the null terminator yet
      {
        int res = sscanf( pos, "%d/%d/%d", &v, &t, &n ) ;
        
        // Make a vertex.
        verts[ vReadCount ] = v ;
        texcoords[ vReadCount ] = t ;
        normals[ vReadCount ] = n ;

        vReadCount++ ;

        cstrNextWord( pos ) ;
      }

      #pragma region triangulation
      int i ;
      i = 0 ;
      for( int c = 1 ; c < (numVerts-1) ; c++ )
      {
        // always start with 0th vertex.
        group->facesVertexIndices[ index + i ] = verts[ 0 ] ;
        group->facesTextureIndices[ index + i ] = texcoords[ 0 ] ;
        group->facesNormalIndices[ index + i ] = normals[ 0 ] ;

        // tie with c'th vertex
        group->facesVertexIndices[ index + i+1 ] = verts[ c ] ;
        group->facesTextureIndices[ index + i+1 ] = texcoords[ c ] ;
        group->facesNormalIndices[ index + i+1 ] = normals[ c ] ;

        // and c'th+1 vertex
        group->facesVertexIndices[ index + i+2 ] = verts[ c+1 ] ;
        group->facesTextureIndices[ index + i+2 ] = texcoords[ c+1 ] ;
        group->facesNormalIndices[ index + i+2 ] = normals[ c+1 ] ;

        // Advance by 3 faces, we just added 3 coords
        i += 3 ;
      }
      #pragma endregion
    }
    else if( ff & Normals )
    {
      // v//n
      int v,n;

      char *pos = line ;
      int vReadCount = 0 ;

      while( pos )
      {
        int res = sscanf( pos, "%d//%d", &v, &n ) ;
        
        verts[ vReadCount ] = v ;
        normals[ vReadCount ] = n ;

        vReadCount++ ;

        // Advance to the next "word"
        cstrNextWord( pos ) ;
      }

      #pragma region triangulation

      // NOW we have numVerts verts.
      // So now, if this number is larger than 3,
      // we need to triangulate.

      // The first face is just the first 3,
      // as they were
      int i ;
      i = 0 ;
      for( int c = 1 ; c < (numVerts-1) ; c++ )
      {
        // always start with 0th vertex.
        group->facesVertexIndices[ index + i ] = verts[ 0 ] ;
        group->facesNormalIndices[ index + i ] = normals[ 0 ] ;

        // tie with c'th vertex
        group->facesVertexIndices[ index + i+1 ] = verts[ c ] ;
        group->facesNormalIndices[ index + i+1 ] = normals[ c ] ;

        // and c'th+1 vertex
        group->facesVertexIndices[ index + i+2 ] = verts[ c+1 ] ;
        group->facesNormalIndices[ index + i+2 ] = normals[ c+1 ] ;

        // Advance by 3 faces, we just added 3 coords
        i += 3 ;
      }
      #pragma endregion
    }
    else if( ff & Texcoords )
    {
      // v/t
      int v,t ;

      char *pos = line ;
      int vReadCount = 0 ;

      while( pos )
      {
        int res = sscanf( pos, "%d/%d", &v, &t ) ;
        
        verts[ vReadCount ] = v ;
        texcoords[ vReadCount ] = t ;

        vReadCount++ ;
        cstrNextWord( pos ) ;
      }

      #pragma region triangulation

      int i ;
      i = 0 ;
      for( int c = 1 ; c < (numVerts-1) ; c++ )
      {
        // always start with 0th vertex.
        group->facesVertexIndices[ index + i ] = verts[ 0 ] ;
        group->facesTextureIndices[ index + i ] = texcoords[ 0 ] ;

        // tie with c'th vertex
        group->facesVertexIndices[ index + i+1 ] = verts[ c ] ;
        group->facesTextureIndices[ index + i+1 ] = texcoords[ c ] ;

        // and c'th+1 vertex
        group->facesVertexIndices[ index + i+2 ] = verts[ c+1 ] ;
        group->facesTextureIndices[ index + i+2 ] = texcoords[ c+1 ] ;

        // Advance by 3 faces, we just added 3 coords
        i += 3 ;
      }
      #pragma endregion
    }
    else
    {
      // v
      int v;

      char *pos = line ;
      int vReadCount = 0 ;

      while( pos )
      {
        sscanf( pos, "%d", &v ) ;
        verts[ vReadCount ] = v ;
        
        vReadCount++ ;
        cstrNextWord( pos ) ;
      }

      #pragma region triangulation

      int i ;
      i = 0 ;
      for( int c = 1 ; c < (numVerts-1) ; c++ )
      {
        // always start with 0th vertex.
        group->facesVertexIndices[ index + i ] = verts[ 0 ] ;

        // tie with c'th vertex
        group->facesVertexIndices[ index + i+1 ] = verts[ c ] ;

        // and c'th+1 vertex
        group->facesVertexIndices[ index + i+2 ] = verts[ c+1 ] ;

        //info( "%s - wound face %d %d %d", group->getName(), verts[0], verts[c], verts[c+1] ) ;

        // Advance by 3 faces, we just added 3 coords
        i += 3 ;
      }

      /*
      // DEBUG
      for( int i = 0 ; i < group->facesVertexIndices.size() ; i+=3 )
      {
        info( "FACE: %d %d %d", group->facesVertexIndices[i],
          group->facesVertexIndices[i+1], group->facesVertexIndices[i+2] ) ;
      }
      */
      #pragma endregion
    }
  }


  #pragma endregion
  void secondPass( FILE* f )
  {
    // In the second pass we actually
    // read up the vertices and save them.

    // A lot of this code is similar
    // to the first pass, only we
    // are actually snapping out the data here.

    int numVerts=0, numNormals=0, numTexCoords=0 ;
    int numFacesStock=0 ;

    // DIFFERS from numTriangleFacesTOTAL, this
    // variable is just an OCD count of
    // total faces in second pass, to make sure
    // its the same as numTriangleFacesTOTAL
    int numFacesTOTALSecondPass=0 ;
    
    string currentGroupName = ""; // we must know what
    // group we're on at all times.

    char buf[ 300 ] ;

    #pragma region parse file pass_2
   
    Group *currentGroup = NULL ;

    while( !feof( f ) )
    {
      fgets( buf, 300, f ) ;
      cstrnulllastnl( buf ) ;

      if( buf[0] == '#' )
      {
        // Its a comment, pass it thru
        //info( buf ) ;
      }
      else if( buf[0] == 'v' && buf[1] == 'n' )
      {
        // vertex normal

        // Increment the counter so we know which normal we're on
        
        // This goes BEFORE use of the variable
        // because index 0 is not used.
        numNormals++ ;

        // Actually save it.
        parseNormal( buf, &normals[ numNormals ] ) ;
      }
      else if( buf[0] == 'v' && buf[1] == 't' )
      {
        //texture coordinate
        numTexCoords++ ;  // index 0 not used
        parseTexcoord( buf, &texCoords[ numTexCoords ] ) ;
      }
      else if( buf[0] == 'v' && buf[1] == 'p' )
      {
        // parameter space vertices
        warning( "Sorry, parameter space vertices not supported, ignoring" ) ;
      }
      else if( buf[0] == 'v' && IS_WHITE(buf[1]) )
      {
        numVerts++ ; // idx 0 unused

        // The only other thing that starts 
        // with 'v' is a vertex itself.
        // the buf[1] character will be
        // either a SPACE or a TAB
        parseVertex( buf, &vertices[ numVerts ] ) ;
      }
      else if( buf[0] == 'f' && IS_WHITE(buf[1]) )
      {
        #pragma region face parse
        numFacesStock++ ;
        
        int verticesInFace = countVerts( buf ) ;
        int facesFromVerts = verticesInFace - 2 ;
        int numVertsAdded = facesFromVerts*3 ; // this is all very
        // twisted looking, but here's what's going
        // on in the 3 lines above.
        // 1. verticesInFace is just a count
        //    of the number of vertices "VISITED"
        //    by this face specification.
        //    f 1 3 2    visits 3 vertices
        //    f 4 5 9 1  visits 4 vertices.

        // We need to know this because we're going
        // to triangulate.  That is, f 4 5 9 1 will
        // be broken into TWO triangles, effectively
        // TWO face specifications
        // f 4 5 9
        // f 4 9 1
        // So that's what FACESFROMVERTS is.
        // in the case of f 4 5 9 1,
        // facesFromVerts = 4 - 2   // meaning
        // we'll extract __2__ faces from
        // f 4 5 9 1.
        // From f 9 5 1 2 8 we'd extract.. 3 faces.
        // f 9 5 1
        // f 9 1 2
        // f 9 2 8
        // This is "fanning" out or whatever, it
        // sometimes produces splintery geometry
        // but it works simply.

        // SO NOW, the NUMBERVERTSADDED variable
        // is the ACTUAL count of vertices we're adding
        // based on the number of faces we've decdied to
        // add after triangulation.  for f 1 3 2 we'd
        // only add 3 vertices, but for f 4 5 9 1 
        // we'd add 3*(NUM_FACES)=3*2=6 vertices.
        
        // keep track of the number of faces
        // we're creating here, as a parity check:
        numFacesTOTALSecondPass += facesFromVerts ;

        // Save the face in the current group
        if( !currentGroup )
        {
          // Create
          warning( "Faces added before any groups were defined, "
            "adding faces to the \"default\" group" ) ;
          
          currentGroup = getGroup( "__DEFAULT_GROUP__" ) ;
        }

        // currentGroup will be defined here.
        
        // Now, parse off the face values.
        extractFaces( buf, verticesInFace, currentGroup ) ;

        // HERE WE UPDATE "WHAT FACE WE ARE ON"
        // This IS NOT a count of the number of faces
        // added.. rather its a count of the total # VERTICES added.
        currentGroup->vertCountPass2 += numVertsAdded ;
        // BECAUSE FACES CAN BE SPECIFIED IN ANY ORDER,
        // (like, file can weave in and out of different groups)
        // You really have to be careful about 
        // keeping this number up to date.
        #pragma endregion
      }
      else if( buf[0] == 'g' && IS_WHITE(buf[1]) )
      {
        char *groupName = buf+2 ; // pass "g ", name goes to end of line

        cstrnullnextsp( groupName ) ;

        currentGroupName = groupName ;

        currentGroup = getGroup( groupName ) ;
        if( currentGroup )
        {
          // The group exists
          // We are switching to entry into this group.
          //info( "Switching to group `%s`", groupName ) ;
        }
        else
        {
          // This shouldn't happen, it means we missed
          // creating a group in the first pass
          error( "Obj file parse pass #2:  group `%s` doesn't exist!", groupName ) ;
        }
      }
      else if( !strncmp( buf, OBJ_CHANGE_MATERIAL, OBJ_CHANGE_MATERIAL_LEN ) )
      {
        // the material change affects
        // the properties of the current group only.
        string mtlName = buf+(OBJ_CHANGE_MATERIAL_LEN+1) ;

        // Set the current group's material
        // to the loaded material, and let that be that.
        currentGroup->material = getMaterial( mtlName ) ;
        // Note if currentGroup->material is NULL
        // then at draw time you'll get a default material
        // used (pure white)
      }
      else if( !strncmp( buf, MTL_LIBRARY, MTL_LIBRARY_LEN ) )
      {
        // Pass #1 loads the material lib file
        info( "Pass #2:  pass 1 loads the material lib file, skipping" ) ;
      }
    }
    #pragma endregion


    // PARITY CHECKS
    if( numFacesTOTALSecondPass != numTriangleFacesTOTAL )
    {
      warning( "numFacesTOTALSecondPass=%d, numTriangleFacesTOTAL=%d, "
        "but these numbers should have been the same.",
        numFacesTOTALSecondPass, numTriangleFacesTOTAL ) ;
    }
    else
    {
      info( "OK! numFacesTOTALSecondPass=%d, numTriangleFacesTOTAL=%d",
        numFacesTOTALSecondPass, numTriangleFacesTOTAL ) ;

    }

    setupVertexBuffers() ;
  }

  Material* getMaterial( string materialName )
  {
    MatMapIter matEntry = materials.find( materialName ) ;
    if( matEntry == materials.end() )
    {
      return NULL ;
    }
    else
    {
      return matEntry->second ;
    }
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
      cstrnulllastnl( buf ) ;

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

        }
        // Make a NEW material
        // / reset the material
        currentMaterial = new Material() ;

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
        {
          // illum term not supported by d3d
          //currentMaterial->setIllum( buf ) ;
        }

        // The Ns term is being specified
        else if( !strncmp( buf, MTL_NSPEC, MTL_NSPEC_LEN ) )
          currentMaterial->setNs( buf ) ;

        else if( !strncmp( buf, MTL_D, MTL_D_LEN ) )
        {
          // d term not supported by d3d
          //currentMaterial->setD( buf ) ;
        }

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

  void setupVertexBuffers()
  {
    // We need to set up vertex arrays
    
    // Combine all the groups..

    // This is massively loop unrolled, but
    // it probably doesn't have to be.
    foreach( GroupMapIter, groupIter, groups )
    {
      Group *g = groupIter->second ;

      #pragma region VTN
      if( g->isFaceSpecMode( Vertices|Normals|Texcoords) )
      {
        // v/t/n
        info( "Group %s as v/t/n", g->getName() ) ;

        g->combinedVerticesVTN.resize( g->vertCountPass2 ) ;
        
        for( int i = 0 ; i < g->vertCountPass2 ; i+=3 )
        {
          int vIndex1 = g->facesVertexIndices[ i ] ;
          int vIndex2 = g->facesVertexIndices[ i+1 ] ;
          int vIndex3 = g->facesVertexIndices[ i+2 ] ;

          int nIndex1 = g->facesNormalIndices[ i ] ;
          int nIndex2 = g->facesNormalIndices[ i + 1 ] ;
          int nIndex3 = g->facesNormalIndices[ i + 2 ] ;

          int tIndex1 = g->facesTextureIndices[ i ] ;
          int tIndex2 = g->facesTextureIndices[ i + 1 ] ;
          int tIndex3 = g->facesTextureIndices[ i + 2 ] ;

          // Now each __3__ vertex index
          // forms a triangle, so, perform the lookup.
          VertexTN v1( vertices[ vIndex1 ], texCoords[ tIndex1 ], normals[ nIndex1 ] ) ;
          VertexTN v2( vertices[ vIndex2 ], texCoords[ tIndex2 ], normals[ nIndex2 ] ) ;
          VertexTN v3( vertices[ vIndex3 ], texCoords[ tIndex3 ], normals[ nIndex3 ] ) ;

          g->combinedVerticesVTN[ i ] = v1 ;
          g->combinedVerticesVTN[ i+1 ] = v2 ;
          g->combinedVerticesVTN[ i+2 ] = v3 ;
        }
      }
      #pragma endregion
      #pragma region VT
      else if( g->isFaceSpecMode(Vertices|Texcoords) )
      {
        // v/t
        info( "Group %s as v/t", g->getName() ) ;
        g->combinedVerticesVT.resize( g->vertCountPass2 ) ;

        // Visit each index in the
        // vertices index buffer
        // using numerical indexing..
        for( int i = 0 ; i < g->vertCountPass2 ; i+=3 )
        {
          int vIndex1 = g->facesVertexIndices[ i ] ;
          int vIndex2 = g->facesVertexIndices[ i+1 ] ;
          int vIndex3 = g->facesVertexIndices[ i+2 ] ;

          int tIndex1 = g->facesTextureIndices[ i ] ;
          int tIndex2 = g->facesTextureIndices[ i + 1 ] ;
          int tIndex3 = g->facesTextureIndices[ i + 2 ] ;

          // Now each __3__ vertex index
          // forms a triangle, so, perform the lookup.
          VertexT v1( vertices[ vIndex1 ], texCoords[ tIndex1 ] ) ;
          VertexT v2( vertices[ vIndex2 ], texCoords[ tIndex2 ] ) ;
          VertexT v3( vertices[ vIndex3 ], texCoords[ tIndex3 ] ) ;

          g->combinedVerticesVT[ i ] = v1 ;
          g->combinedVerticesVT[ i+1 ] = v2 ;
          g->combinedVerticesVT[ i+2 ] = v3 ;
        }
      }
      #pragma endregion
      #pragma region VN
      else if( g->isFaceSpecMode(Vertices|Normals) )
      {
        // v//n
        info( "Group %s as v//n", g->getName() ) ;
        
        Group *g = groupIter->second ;
        g->combinedVerticesVN.resize( g->vertCountPass2 ) ;

        // Visit each index in the
        // vertices index buffer
        // using numerical indexing..
        for( int i = 0 ; i < g->vertCountPass2 ; i+=3 )
        {
          
          int vIndex1 = g->facesVertexIndices[ i ] ;
          int vIndex2 = g->facesVertexIndices[ i+1 ] ;
          int vIndex3 = g->facesVertexIndices[ i+2 ] ;

          int nIndex1 = g->facesNormalIndices[ i ] ;
          int nIndex2 = g->facesNormalIndices[ i + 1 ] ;
          int nIndex3 = g->facesNormalIndices[ i + 2 ] ;

          // Now each __3__ vertex index
          // forms a triangle, so, perform the lookup.
          VertexN v1( vertices[ vIndex1 ], normals[ nIndex1 ] ) ;
          VertexN v2( vertices[ vIndex2 ], normals[ nIndex2 ] ) ;
          VertexN v3( vertices[ vIndex3 ], normals[ nIndex3 ] ) ;

          g->combinedVerticesVN[ i ] = v1 ;
          g->combinedVerticesVN[ i+1 ] = v2 ;
          g->combinedVerticesVN[ i+2 ] = v3 ;
        }
      }
      #pragma endregion
      #pragma region V
      else
      {
        // v
        info( "Group %s as v", g->getName() ) ;
        
        // groupIter is <name,Group*>
        Group *g = groupIter->second ;
        g->combinedVerticesV.resize( g->vertCountPass2 ) ;

        for( int i = 0 ; i < g->vertCountPass2 ; i+=3 )
        {
          int vIndex1 = g->facesVertexIndices[ i ] ;
          int vIndex2 = g->facesVertexIndices[ i+1 ] ;
          int vIndex3 = g->facesVertexIndices[ i+2 ] ;

          // Now each __3__ vertex index
          // forms a triangle, so, perform the lookup.
          Vertex v1 = vertices[ vIndex1 ] ;
          Vertex v2 = vertices[ vIndex2 ] ;
          Vertex v3 = vertices[ vIndex3 ] ;

          //!! Hopefully the double copy will
          // be optimized out by the compiler,
          // but you could always eliminate v1,v2, and v3
          g->combinedVerticesV[ i ] = v1 ;
          g->combinedVerticesV[ i+1 ] = v2 ;
          g->combinedVerticesV[ i+2 ] = v3 ;
        }
      }
      #pragma endregion

    } // END FOREACH GROUP
  }
  
public:
  void computeNormals()
  {
    // Computes normals on loaded vertex set
    warning( "!! computeNormals() not implemented yet" ) ;
    // Should also take into acct (s) smoothing groups
    // in the obj file.
  }

  void draw()
  {
    // first let's draw without vb's
    IDirect3DDevice9 *gpu = window->getGpu() ;

    // Set up all the render states.
    DX_CHECK( gpu->SetRenderState( D3DRS_COLORVERTEX, TRUE ), "ColorVertex enable" ) ;
    DX_CHECK( gpu->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_XRGB(0,0,0) ), "Turn off ambient color" ) ;
    DX_CHECK( gpu->SetRenderState( D3DRS_SPECULARENABLE, TRUE ), "Specular enable" ) ;
    
    // For materials to work lighting must be on
    window->setLighting( TRUE ) ;

    // Set up source of ambient, diffuse, specular and
    // emissive vertex colorings to all come from
    // the MATERIAL.
    DX_CHECK( gpu->SetRenderState( D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL ), "set ambient d3dmcs_material" ) ;
    DX_CHECK( gpu->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL ), "diff d3dmcs_material" );
    DX_CHECK( gpu->SetRenderState( D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL ), "spec d3dmcs_material" ) ;
    DX_CHECK( gpu->SetRenderState( D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL ), "emissive d3dmcs_material" ) ;
    
    // v/t/n
    foreach( GroupMapIter, groupIter, groups )
    {
      Group *g = groupIter->second ;

      window->setVertexDeclaration( g->vType ) ;
      window->setMaterial( g->getD3DMaterial() ) ;

      // Texcoords on, turn on the texture
      if( g->faceSpecMode & Texcoords )
      {
        Material *mat = g->getMaterial() ;
        // set the texture if it's been set up.
        if( mat && mat->getSpriteId() ) // order matters!
          window->setActiveTexture( mat->getSpriteId() ) ;  
      }

      // ACTUALLY DRAW
      if( g->isFaceSpecMode(Vertices|Texcoords|Normals) )
      {
        // v/t/n
        gpu->DrawPrimitiveUP(
          D3DPT_TRIANGLELIST,
          g->getNumFaces(),
          &g->combinedVerticesVTN[0],
          sizeof(VertexTN) ) ;
      }
      else if( g->isFaceSpecMode(Vertices|Texcoords) )
      {
        // v/t
        gpu->DrawPrimitiveUP(
          D3DPT_TRIANGLELIST,
          g->getNumFaces(),
          &g->combinedVerticesVT[0],
          sizeof(VertexT) ) ;
      }
      else if( g->isFaceSpecMode(Vertices|Normals) )
      {
        // v//n
        gpu->DrawPrimitiveUP(
          D3DPT_TRIANGLELIST,
          g->getNumFaces(),
          &g->combinedVerticesVN[0],
          sizeof(VertexN) ) ;
      }
      else // v
      {
        gpu->DrawPrimitiveUP(
          D3DPT_TRIANGLELIST,
          g->getNumFaces(),
          &g->combinedVerticesV[0],
          sizeof(Vertex) ) ;
      }
    }
  }
} ;



#endif // OBJ_FILE_PARSE_H