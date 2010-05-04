#include "ThreeDMan.h"

void ThreeDMan::initThreeDMan( IDirect3DDevice9 *theGpu )
{
  lgpu = theGpu ;
}

  /*
void ThreeDMan::setViewByCamera()
{

  !!
well, this function used to exist
but it turned out that I kept 3 state
variables in D3DWindow eye look and up.
those need to go away, and this functoin
should be acivated int he future


}
  */

void ThreeDMan::setWorld( D3DXMATRIX *world )
{
  HRESULT hr = lgpu->SetTransform( D3DTS_WORLD, world ) ;
  DX_CHECK( hr, "world transformation" ) ;
}






void ThreeDMan::createTriVC(
  VertexC &a, VertexC &b, VertexC &c
)
{
  verticesC.push_back( a ) ;
  verticesC.push_back( b ) ;
  verticesC.push_back( c ) ;
}

void ThreeDMan::createQuadVC(
  VertexC &a, VertexC &b, VertexC &c, VertexC &d
)
{
  verticesC.push_back( a ) ;
  verticesC.push_back( b ) ;
  verticesC.push_back( c ) ;

  verticesC.push_back( c ) ;
  verticesC.push_back( d ) ;
  verticesC.push_back( a ) ;
}

void ThreeDMan::startVTCSet( int spriteId )
{
  // Create an entry in the map
  MapIntVertexTCIter tcEntry = mapVerticesTC.find( spriteId ) ;
  if( tcEntry == mapVerticesTC.end() )
  {
    // didn't exist, so create it
    vector<VertexTC> entry ;
    mapVerticesTC.insert( make_pair(spriteId, entry) ) ;
  }

  // Otherwise, already existed
}

void ThreeDMan::createTriVTC(
  // The tex to use when rendering
  int spriteId,
  VertexTC &a, VertexTC &b, VertexTC &c
)
{
  startVTCSet( spriteId ) ;

  // assume it exists now, having
  // called startVTCSet()
  mapVerticesTC[ spriteId ].push_back( a ) ;
  mapVerticesTC[ spriteId ].push_back( b ) ;
  mapVerticesTC[ spriteId ].push_back( c ) ;
}

void ThreeDMan::createQuadVTC(
  // The texture to use when rendering
  int spriteId,
  VertexTC &a, VertexTC &b, VertexTC &c, VertexTC &d
)
{
  startVTCSet( spriteId ) ;

  mapVerticesTC[ spriteId ].push_back( a ) ;
  mapVerticesTC[ spriteId ].push_back( b ) ;
  mapVerticesTC[ spriteId ].push_back( c ) ;

  mapVerticesTC[ spriteId ].push_back( c ) ;
  mapVerticesTC[ spriteId ].push_back( d ) ;
  mapVerticesTC[ spriteId ].push_back( a ) ;
}

