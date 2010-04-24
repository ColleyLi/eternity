#ifndef D3DVERTEX_H
#define D3DVERTEX_H

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

// Adds color information to
// a basic vertex
struct VertexC
{
  D3DXVECTOR3 pos ;
  DWORD color ;

  VertexC(){ pos.x=pos.y=pos.z=0.0f; color = 0 ; }

  VertexC( float x, float y, float z, D3DCOLOR iColor )
  {
    pos.x = x ;
    pos.y = y ;
    pos.z = z ;
    color = iColor ;
  }

  VertexC( float x, float y, float z, BYTE r, BYTE g, BYTE b )
  {
    pos.x = x ;  pos.y = y ;  pos.z = z ;
    color = D3DCOLOR_XRGB( r, g, b ) ;
  }
} ;

#endif // D3DVERTEX_H