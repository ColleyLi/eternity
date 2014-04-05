#ifndef D3DVERTEX_H
#define D3DVERTEX_H

#include <d3dx9.h>

// v
struct Vertex
{
  D3DXVECTOR3 pos ;

  Vertex():
    pos(0.0f,0.0f,0.0f)
  {
    
  }

  Vertex( float x, float y, float z ):
    pos(x,y,z)
  {
    
  }

  // Avoid copy construction
  // by these ctor's
  Vertex( const D3DXVECTOR3 &iPos ):
    pos( iPos )
  {
    
  }
} ;

// I thought of having VertexC : public Vertex
// but then I realized this creates potential
// confusion for people to do with assumptions
// about it being "ok" to pass in a VertexC when
// a Vertex is called for... because in inheritance,
// you can do that.  Having separate,
// concrete, fully defined objects here
// makes the most sense.  These really are
// distinct, non-interchangeable objects,
// as far as the GPU is concerned.




struct VertexC
{
  D3DXVECTOR3 pos ;
  DWORD color ;

  VertexC():
    pos(0.0f,0.0f,0.0f), color( -1 )
  {
   
  }

  VertexC( const D3DXVECTOR3 &v, D3DCOLOR iColor ) :
    pos( v ), color( iColor )
  {
  }

  VertexC( float x, float y, float z, D3DCOLOR iColor ) :
    pos( x,y,z ), color( iColor )
  {
  }

  VertexC( float x, float y, float z, BYTE r, BYTE g, BYTE b ):
    pos( x,y,z )
  {
    color = D3DCOLOR_XRGB( r, g, b ) ;
  }
} ;

// v/t
struct VertexT
{
  D3DXVECTOR3 pos ;
  D3DXVECTOR2 tex ;

  VertexT()
  {
    pos.x=pos.y=pos.z=0.0f;
    tex.x=tex.y=0.0f;
  }

  VertexT( const D3DXVECTOR3 &iPos, const D3DXVECTOR2 &iTex )
  {
    pos = iPos ;
    tex = iTex ;
  }
} ;

struct VertexTC
{
  D3DXVECTOR3 pos ;
  D3DXVECTOR2 tex ;
  DWORD color ;

  VertexTC() :
    pos( 0.0f, 0.0f, 0.0f ),
    tex( 0.0f, 0.0f ),
    color( -1 )
  {

  }

    
  VertexTC( float x, float y, float z,
    float u, float v ) :

    pos( x, y, z ),
    tex( u, v ),
    color( -1 ) // defaults to WHITE
  {
    
  }

  VertexTC( float x, float y, float z, D3DCOLOR iColor ) :
    pos( x, y, z ),
    tex( 0.0f, 0.0f ),
    color( iColor )
  {

  }

  VertexTC( float x, float y, float z,
    float u, float v, 
    BYTE r, BYTE g, BYTE b ) :

    pos( x, y, z ),
    tex( u, v )
  {
    color = D3DCOLOR_XRGB( r, g, b ) ;
  }
} ;


// v//n
struct VertexN
{
  D3DXVECTOR3 pos ;
  D3DXVECTOR3 norm ;

  VertexN() :
    pos( 0.0f, 0.0f, 0.0f ),
    norm( 0.0f, 0.0f, 0.0f )
  {
    
  }

  VertexN( float x, float y, float z, float nx, float ny, float nz ) :
    pos(x,y,z),
    norm(nx,ny,nz)
  {
  }

  VertexN( const D3DXVECTOR3 &iPos, const D3DXVECTOR3 &iNorm ) :
    pos( iPos ),
    norm( iNorm )
  {

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

// Adds color information to
// a basic vertex

#endif // D3DVERTEX_H