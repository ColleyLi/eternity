#include "ObjFileParse.h"
#include "GamePak.h"

D3DMATERIAL9 Material::defaultMaterial ;

void ObjFile::firstPass( FILE* f )
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
      loadMtlFile( buf+(MTL_LIBRARY_LEN+1), window->spriteMan ) ;
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
      g->vType = PositionTextureNormal ;
      info( "Group %s using PositionTextureNormal vertex", g->getName() ) ;
    }
    else if( g->isFaceSpecMode(Vertices|Texcoords) ) // v/t
    {
      g->vType = PositionTexture ;
      info( "Group %s using PositionTexture vertex", g->getName() ) ;
    }
    else if( g->isFaceSpecMode(Vertices|Normals) )   // v//n
    {
      g->vType = PositionNormal ;
      info( "Group %s using PositionNormal vertex", g->getName() ) ;
    }
    else   // v
    {
      g->vType = Position ;
      info( "Group %s using Position vertex", g->getName() ) ;
    }
  }
}

void ObjFile::loadMtlFile( char *mtllibName, SpriteMan *spriteMan )
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
        int spriteId = spriteMan->getNextSpriteId() ;

        new Sprite( spriteId, spriteFile ) ;
          
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

void ObjFile::draw()
{
  // first let's draw without vb's
  IDirect3DDevice9 *gpu = window->window->getGpu() ;

  // Set up all the render states.
  DX_CHECK( gpu->SetRenderState( D3DRS_COLORVERTEX, TRUE ), "ColorVertex enable" ) ;
  DX_CHECK( gpu->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_XRGB(0,0,0) ), "Turn off ambient color" ) ;
  DX_CHECK( gpu->SetRenderState( D3DRS_SPECULARENABLE, TRUE ), "Specular enable" ) ;
    
  // For materials to work lighting must be on
  window->window->setLighting( TRUE ) ;

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

    window->window->setVertexDeclaration( g->vType ) ;
    window->window->setMaterial( g->getD3DMaterial() ) ;

    // Texcoords on, turn on the texture
    if( g->faceSpecMode & Texcoords )
    {
      Material *mat = g->getMaterial() ;
      // set the texture if it's been set up.
      if( mat && mat->getSpriteId() ) // order matters!
        window->spriteMan->setActiveTexture( mat->getSpriteId() ) ;  
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
