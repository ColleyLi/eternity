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

// v
struct Vertex
{
  D3DXVECTOR3 pos ;

  Vertex() { pos.x=pos.y=pos.z=0.0f; }

  // Avoid copy construction
  // by these ctor's
  Vertex( const D3DXVECTOR3 &iPos )
  {
    pos = iPos ;
  }
} ;

// v/t
struct VertexT
{
  D3DXVECTOR3 pos ;
  D3DXVECTOR2 tex ;

  VertexT(){ pos.x=pos.y=pos.z=tex.x=tex.y=0.0f; }

  VertexT( const D3DXVECTOR3 &iPos, const D3DXVECTOR2 &iTex )
  {
    pos = iPos ;
    tex = iTex ;
  }
} ;

// v//n
struct VertexN
{
  D3DXVECTOR3 pos ;
  D3DXVECTOR3 norm ;

  VertexN() { pos.x=pos.y=pos.z=norm.x=norm.y=norm.z=0.0f; }

  VertexN( const D3DXVECTOR3 &iPos, const D3DXVECTOR3 &iNorm )
  {
    pos = iPos ;
    norm = iNorm ;
  }
} ;

// v/t/n
struct VertexTN
{
  D3DXVECTOR3 pos ;
  D3DXVECTOR2 tex ;
  D3DXVECTOR3 norm ;

  // Preferred this format over memsets, they are hard to read :
  VertexTN() { pos.x=pos.y=pos.z=tex.x=tex.y=norm.x=norm.y=norm.z=0.0f; }

  VertexTN( const D3DXVECTOR3 &iPos, const D3DXVECTOR2 &iTex, const D3DXVECTOR3 &iNorm )
  {
    pos = iPos ;
    tex = iTex ;
    norm = iNorm ;
  }
} ;

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

  D3DMATERIAL9 getMaterial()
  {
    return d3dmaterial ;
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

  D3DMATERIAL9 getMaterial()
  {
    if( !material )
    {
      // use a default white material instead.
      return Material::defaultMaterial ;
    }
    else
    {
      return material->getMaterial() ;
    }
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
  /// We need to save vertex declarations
  static IDirect3DVertexDeclaration9 *v, *vt, *vtn, *vn ;

  /// The actual vertex declaration being used by
  /// THIS obj model instance
  IDirect3DVertexDeclaration9 *vDecl ;

public:
  /// Call this once to initialize the class.
  static void init( GameWindow *window )
  {
    // Set up the default material
    setColor( &Material::defaultMaterial.Ambient,  1.f,  1.f, 1.f, 1.f ) ;
    setColor( &Material::defaultMaterial.Diffuse,  1.f,  1.f, 1.f, 1.f ) ;
    setColor( &Material::defaultMaterial.Specular, 1.f,  1.f, 1.f, 1.f ) ;
    setColor( &Material::defaultMaterial.Emissive, 0.f,  0.f, 0.f, 0.f ) ;

    IDirect3DDevice9 *gpu = window->getGpu();

    int cOffset = 0 ;

    D3DVERTEXELEMENT9 pos ;
    pos.Usage = D3DDECLUSAGE_POSITION ;
    pos.UsageIndex = 0 ;
    pos.Stream = 0 ;
    pos.Type = D3DDECLTYPE_FLOAT3 ;
    pos.Offset = 0 ;
    pos.Method = D3DDECLMETHOD_DEFAULT ;
    cOffset += 3*sizeof(float);

    D3DVERTEXELEMENT9 tex ;
    tex.Usage = D3DDECLUSAGE_TEXCOORD ;
    tex.UsageIndex = 0 ;
    tex.Stream = 0 ;
    tex.Type = D3DDECLTYPE_FLOAT2 ;
    tex.Offset = cOffset ;
    tex.Method = D3DDECLMETHOD_DEFAULT ;
    cOffset += 2*sizeof(float);

    D3DVERTEXELEMENT9 norm ;
    norm.Usage = D3DDECLUSAGE_NORMAL ;
    norm.UsageIndex = 0 ;
    norm.Stream = 0 ;
    norm.Type = D3DDECLTYPE_FLOAT3 ;
    norm.Offset = cOffset ;
    norm.Method = D3DDECLMETHOD_DEFAULT ; 

    
    D3DVERTEXELEMENT9 end = D3DDECL_END() ;
    
    // Vertices only.  Uses a material for color.
    D3DVERTEXELEMENT9 *vertexElementsV = new D3DVERTEXELEMENT9[2] ;
    vertexElementsV[ 0 ] = pos ;
    vertexElementsV[ 1 ] = end ;
    // CREATE THE DECLARATION
    DX_CHECK( gpu->CreateVertexDeclaration( vertexElementsV, &v ), "CreateVertexDeclaration FAILED v!" ) ;

    // VT
    // Vertices and textures.
    D3DVERTEXELEMENT9 *vertexElementsVT = new D3DVERTEXELEMENT9[3] ;
    vertexElementsVT[ 0 ] = pos ;
    vertexElementsVT[ 1 ] = tex ;
    vertexElementsVT[ 2 ] = end ;
    DX_CHECK( gpu->CreateVertexDeclaration( vertexElementsVT, &vt ), "CreateVertexDeclaration FAILED vt!" ) ;

    // VTN
    D3DVERTEXELEMENT9 *vertexElementsVTN = new D3DVERTEXELEMENT9[4] ;
    vertexElementsVTN[ 0 ] = pos ;
    vertexElementsVTN[ 1 ] = tex ;
    vertexElementsVTN[ 2 ] = norm ;
    vertexElementsVTN[ 3 ] = end ;
    DX_CHECK( gpu->CreateVertexDeclaration( vertexElementsVTN, &vtn ), "CreateVertexDeclaration FAILED vtn!" ) ;

    // VN
    D3DVERTEXELEMENT9 *vertexElementsVN = new D3DVERTEXELEMENT9[3] ;
    vertexElementsVN[ 0 ] = pos ;
    norm.Offset = 3*sizeof(float);  // This needed to be updated
    vertexElementsVN[ 1 ] = norm ;
    vertexElementsVN[ 2 ] = end ;
    DX_CHECK( gpu->CreateVertexDeclaration( vertexElementsVN, &vn ), "CreateVertexDeclaration FAILED vn!" ) ;

  }

private:

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

  // Count up the number of faces
  // stored in each group.
  MapStringInt numFacesByGroup ;

  /// How faces are specified in this file.
  /// It can be any combination of these 3
  enum FaceSpecMode
  {
    Vertices  = 1 << 0,
    Normals   = 1 << 1,
    Texcoords = 1 << 2
  } ;
  FaceSpecMode faceSpecMode ;

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

    // Initialize face spec mode to nothing
    faceSpecMode = (FaceSpecMode)0 ;

    if( !objFilename )
    {
      error( "I can't open a NULL pointer filename!" ) ;
      return ;
    }

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
        // Actually, ALWAYS detect to see if
        // there are any anomolies.
        FaceSpecMode fsm = (FaceSpecMode)getFaceSpecMode( buf ) ;
        if( fsm != faceSpecMode )
        {
          if( !faceSpecMode )
          {
            info( "FaceSpecMode is `%s`", getFaceSpecModeString( fsm ) ) ;
          }
          else
          {
            warning( "Anomaly detected, facespecmode changed from %s to %s",
               getFaceSpecModeString( faceSpecMode ),
               getFaceSpecModeString( fsm ) ) ;
          }
          
          faceSpecMode = fsm ;
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

        numTriangleFacesTOTAL += facesFromVerts ;

        // Increase the numFaces count
        // FOR THE CURRENT GROUP

        // if currentGroup has a value
        // then it will already have been
        // inserted into the `numFacesByGroup`
        // map
        MapStringIntIter iter = numFacesByGroup.find( currentGroupName ) ;
        if( iter == numFacesByGroup.end() )
        {
          error( "Obj file parse pass #1:  face specified "
            "to join a group that doesn't exist `%s`", currentGroupName.c_str() ) ;
        }
        else
        {
          // The group name exists already, so increment face count
          iter->second = iter->second + facesFromVerts ;
        }
      }
      else if( buf[0] == 'g' && IS_WHITE(buf[1]) )
      {
        // Count DISTINCT groups, which'll just be groups.size() when
        // this is all done

        // A group.  Create it now, unless it already exists

        // Get the name
        char *groupName = buf+2 ; // pass "g ", name goes to end of line

        // If there's a long name with spaces just cut off the extra names
        // this puts it in the same group as the first name.
        cstrnullnextsp( groupName ) ;

        currentGroupName = groupName ;

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
        // (in this implemenation!).  FIXED by cutoff.

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
          numFacesByGroup.insert( make_pair( currentGroupName, 0 ) ) ;
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
    foreach( MapStringIntIter, iter, numFacesByGroup )
    {
      int faces = iter->second ;
      info( "Group `%s` has %d faces", iter->first.c_str(), faces ) ;

      // Allocate enough space in each group object
      // to hold that many faces.
      
      // Make sure the group exists
      GroupMapIter groupIter = groups.find( iter->first ) ;
      if( groupIter == groups.end() )
      {
        // NOT FOUND!
        // This shouldn't ever happen if the parse code is correct
        error( "Group `%s` doesn't exist in the `groups` collection", iter->first.c_str() ) ;
      }
      else
      {
        // Every face takes __3__ indices to specify it.
        groupIter->second->facesVertexIndices.resize( 3*faces ) ;
        groupIter->second->facesNormalIndices.resize( 3*faces ) ;
        groupIter->second->facesTextureIndices.resize( 3*faces ) ;
      }
    }


    // Determine what vertex declaration to use
    // based on face mode
    if( (faceSpecMode & FaceSpecMode::Texcoords) && (faceSpecMode & FaceSpecMode::Normals) ) // v/t/n
      vDecl = vtn ;
    else if( faceSpecMode & FaceSpecMode::Texcoords ) // v/t
      vDecl = vt ; 
    else if( faceSpecMode & FaceSpecMode::Normals )   // v//n
      vDecl = vn ;
    else   // v
      vDecl = v ;

    //!! IF THERE WAS NO MATERIAL FILENAME
    // BY EOF, THEN CREATE A DEFAULT MATERIAL
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

  char* getFaceSpecModeString( FaceSpecMode fsm )
  {
    if( (fsm & Vertices) && (fsm & Normals) && (fsm & Texcoords) )
      return "v/t/n/" ;
    else if( (fsm & Vertices) && (fsm & Texcoords) )
      return "v/t";
    else if( (fsm & Vertices) && (fsm & Normals) )
      return "v//n";
    else if( fsm & Vertices )
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
          return FaceSpecMode::Vertices | FaceSpecMode::Normals ;  // "v//n"

        // If we didn't have a slash right next to it,
        // it'll either be DIGITS then white space (v/t)
        // or there'll be ANOTHER white slash before
        // the next white space.

        while( !isspace(*pos) )
        {
          if( *pos == '/' )
            return FaceSpecMode::Vertices | FaceSpecMode::Texcoords | FaceSpecMode::Normals ; // "v/t/n"
          ++pos ;
        }

        // if it WAS a space next (before
        // hitting a /) then we have v/t
        return FaceSpecMode::Vertices | FaceSpecMode::Texcoords ; //"v/t"
      }

      // next char.
      ++pos;
    }
    
    // If didn't return above, means hit space
    // before hitting slash at all
    return FaceSpecMode::Vertices ; // "v"
  }

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

  void extractFaces( char *buf, int numVerts, Group *group, int index )
  {
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

      while( *pos ) // while we haven't reached the null terminator yet
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
        if( index +i+2 >= (int)group->facesVertexIndices.size() )
        {
          //!! this error really shouldn't happen
          // and needs to be fixed
          error("`%s` too big for its britches", group->getName()) ;
          continue;
        }
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
        if( index +i+2 >= (int)group->facesVertexIndices.size() )
        {
          //!! this error really shouldn't happen
          // and needs to be fixed
          error("`%s` too big for its britches", group->getName()) ;
          continue;
        }
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
        if( index +i+2 >= (int)group->facesVertexIndices.size() )
        {
          //!! this error really shouldn't happen
          // and needs to be fixed
          error("`%s` too big for its britches", group->getName()) ;
          continue;
        }
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
        if( index +i+2 >= (int)group->facesVertexIndices.size() ) // 4018 needs to go away.
        {
          //!! this error really shouldn't happen
          // and needs to be fixed
          error("`%s` too big for its britches", group->getName()) ;
          continue;
        }
        // always start with 0th vertex.
        group->facesVertexIndices[ index + i ] = verts[ 0 ] ;

        // tie with c'th vertex
        group->facesVertexIndices[ index + i+1 ] = verts[ c ] ;

        // and c'th+1 vertex
        group->facesVertexIndices[ index + i+2 ] = verts[ c+1 ] ;

        // Advance by 3 faces, we just added 3 coords
        i += 3 ;
      }

      #pragma endregion
    }
  }

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
    

    MapStringInt facesByGroupCount ;  // this is parallel to numFacesByGroup.
    // By the end of the second pass they will match
    // exactly, but facesByGroupCount needs to TALLY
    // up the faces as they come in so we know what
    // entry in the arrays to index as we're adding faces.

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

        numFacesTOTALSecondPass += facesFromVerts ;


        MapStringIntIter iter = facesByGroupCount.find( currentGroupName ) ;
        if( iter == facesByGroupCount.end() )
        {
          error( "Obj file parse pass #2:  face specified "
            "to join a group that doesn't exist `%s`", currentGroupName.c_str() ) ;
        }
        else
        {
          // The group name exists already,
          // which is good as we are second pass.

         
          // Save the face in the current group
          if( !currentGroup )
          {
            error( "I can't save this face because there is no current group defined" ) ;
          }
          else
          {
            // Now, parse off the face values.
            extractFaces( buf, verticesInFace, currentGroup, iter->second ) ;
          }


          // HERE WE UPDATE "WHAT FACE WE ARE ON"
          iter->second = iter->second + facesFromVerts ;

          // BECAUSE FACES CAN BE SPECIFIED IN ANY ORDER,
          // (like, file can weave in and out of different groups)
          // You really have to be careful about 
          // keeping this number up to date.

          /*
          if( iter->second > numFacesByGroup[ iter->first ] )
          {
            error( "Really weird error, face count exceeds what first pass got" ) ;
          }
          */
          
        }

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

        
        // on the second pass we only need
        // to keep `facesByGroupCount` updated
        // If its the first time encountering
        // this group, then create it and init
        // its face count to 0.
        MapStringIntIter iter = facesByGroupCount.find( currentGroupName ) ;
        if( iter == facesByGroupCount.end() )
        {
          // Initialize numFaces @ 0
          facesByGroupCount.insert( make_pair( currentGroupName, 0 ) ) ;
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

  int getNumFacesInGroup( string groupName )
  {
    MapStringIntIter countEntry = numFacesByGroup.find( groupName ) ;
    if( countEntry == numFacesByGroup.end() )
    {
      warning( "getNumFaces: Group %s doesn't exist, you get 0", groupName.c_str() ) ;
      return 0 ;
    }
    else
    {
      return countEntry->second ;
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
    IDirect3DDevice9 *gpu = window->getGpu() ;

    // Combine all the groups..
    #pragma region VTN
    if( (faceSpecMode & Normals) && (faceSpecMode & Texcoords) )
    {
      // v/t/n
      info( "VB as v/t/n" ) ;

      foreach( GroupMapIter, groupIter, groups )
      {
        Group *g = groupIter->second ;

        // the count of faces for this group
        // is in the map numFacesByGroup
        int numFacesInGroup = getNumFacesInGroup( g->getName() ) ;
        if( numFacesInGroup*3 != g->facesVertexIndices.size() )
          warning( "Parity error:  number of faces in group `%s` "
          "not matching up:  numFacesInGroup*3=%d, facesVertexIndices=%d",
          g->getName(), numFacesInGroup*3, g->facesVertexIndices.size() ) ;
        g->combinedVerticesVTN.resize( numFacesInGroup ) ;
        
        for( int i = 0 ; i < groupIter->second->facesVertexIndices.size() ; i+=3 )
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
    }
    #pragma endregion
    #pragma region VT
    else if( faceSpecMode & Texcoords )
    {
      // v/t
      info( "VB as v/t" ) ;
      
      foreach( GroupMapIter, groupIter, groups )
      {
        Group *g = groupIter->second ;

        // the count of faces for this group
        // is in the map numFacesByGroup
        int numFacesInGroup = getNumFacesInGroup( g->getName() ) ;
        if( numFacesInGroup*3 != g->facesVertexIndices.size() )
          warning( "Parity error:  number of faces in group `%s` "
          "not matching up:  numFacesInGroup*3=%d, facesVertexIndices=%d",
          g->getName(), numFacesInGroup*3, g->facesVertexIndices.size() ) ;
        g->combinedVerticesVT.resize( numFacesInGroup ) ;

        // Visit each index in the
        // vertices index buffer
        // using numerical indexing..
        for( int i = 0 ; i < groupIter->second->facesVertexIndices.size() ; i+=3 )
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
    }
    #pragma endregion
    #pragma region VN
    else if( faceSpecMode & Normals )
    {
      // v//n
      info( "VB as v//n" ) ;
      
      foreach( GroupMapIter, groupIter, groups )
      {
        Group *g = groupIter->second ;

        int numFacesInGroup = getNumFacesInGroup( g->getName() ) ;
        if( numFacesInGroup*3 != g->facesVertexIndices.size() )
          warning( "Parity error:  number of faces in group `%s` "
          "not matching up:  numFacesInGroup*3=%d, facesVertexIndices=%d",
          g->getName(), numFacesInGroup*3, g->facesVertexIndices.size() ) ;
        g->combinedVerticesVN.resize( numFacesInGroup*3 ) ;

        // Visit each index in the
        // vertices index buffer
        // using numerical indexing..
        for( int i = 0 ; i < numFacesInGroup ; i+=3 )
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
    }
    #pragma endregion
    #pragma region V
    else
    {
      // v
      info( "VB as v" ) ;
      
      // groupIter is <name,Group*>
      foreach( GroupMapIter, groupIter, groups )
      {
        Group *g = groupIter->second ;
        
        int numFacesInGroup = getNumFacesInGroup( g->getName() ) ;
        if( numFacesInGroup*3 != g->facesVertexIndices.size() )
          warning( "Parity error:  number of faces in group `%s` "
          "not matching up:  numFacesInGroup*3=%d, facesVertexIndices=%d",
          g->getName(), numFacesInGroup*3, g->facesVertexIndices.size() ) ;
        g->combinedVerticesV.resize( numFacesInGroup ) ;

        for( int i = 0 ; i < groupIter->second->facesVertexIndices.size() ; i+=3 )
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
    }
    #pragma endregion
  }
  
public:
  void computeNormals()
  {
    // Computes normals on loaded vertex set
  }

  void draw()
  {
    // first let's draw without vb's
    IDirect3DDevice9 *gpu = window->getGpu() ;

    //printf( "There are %d verts to draw\n", this->vertices.size() ) ;
  
    // Set up all the render states.
    DX_CHECK( gpu->SetRenderState( D3DRS_COLORVERTEX, TRUE ), "ColorVertex enable" ) ;
    DX_CHECK( gpu->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_XRGB(0,0,0) ), "Turn off ambient color" ) ;
    DX_CHECK( gpu->SetRenderState( D3DRS_SPECULARENABLE, TRUE ), "Specular enable" ) ;
    
    // For materials to work lighting must be on
    DX_CHECK( gpu->SetRenderState( D3DRS_LIGHTING, TRUE ), "Lighting on" ) ;

    // Set up source of ambient, diffuse, specular and
    // emissive vertex colorings to all come from
    // the MATERIAL.
    DX_CHECK( gpu->SetRenderState( D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL ), "set ambient d3dmcs_material" ) ;
    DX_CHECK( gpu->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL ), "diff d3dmcs_material" );
    DX_CHECK( gpu->SetRenderState( D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL ), "spec d3dmcs_material" ) ;
    DX_CHECK( gpu->SetRenderState( D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL ), "emissive d3dmcs_material" ) ;

    DX_CHECK( gpu->SetVertexDeclaration( vDecl ), "SetVertexDeclaration FAILED!" ) ;
    

    // It depends on
    // vertex mode..
    if( (faceSpecMode & Texcoords) && (faceSpecMode & Normals) )
    {
      // v/t/n
      /*
      foreach( GroupMapIter, groupIter, groups )
      {
        DX_CHECK( gpu->SetMaterial( &groupIter->second->getMaterial() ), "Set material" ) ;
        gpu->DrawPrimitiveUP(
          D3DPT_TRIANGLELIST, 
          numTriangleFacesTOTAL,
          &combinedVerticesVTN[0],
          sizeof(VertexTN) ) ;
      }
      */
    }
    else if(faceSpecMode & Texcoords)
    {
      // v/t
    }
    else if(faceSpecMode & Normals)
    {
      // v//n
    }
    else
    {
      // v
    }

    // groupIter is <name,Group*>
    foreach( GroupMapIter, groupIter, groups )
    {
      DX_CHECK( gpu->SetMaterial( &groupIter->second->getMaterial() ), "Set material" ) ;

      

      /*
      gpu->DrawPrimitiveUP(
        D3DPT_TRIANGLELIST,
        0,

        // We only USE facesVertexIndices here, but
        // we're drawing out of the COMBINED
        // vertex array.

        //!! THIS MUST BE CHANGED TO COMBINED.
        groupIter->second->facesVertexIndices.size(),
        groupIter->second->facesVertexIndices.size()/3,
        &groupIter->second->facesVertexIndices[0],

        D3DFMT_INDEX32,
        &combinedVertices[0],
        sizeof( D3DXVECTOR3 )
      ) ;
      */
    }
    
  }

  
} ;



#endif // OBJ_FILE_PARSE_H