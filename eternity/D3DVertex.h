#ifndef D3DVERTEX_H
#define D3DVERTEX_H

// This is used to draw the sprites
// on the screen.  You really should
// use YOUR OWN Vertex structure for
// velocity and position etc., because
// you won't understand what a texture
// coordinate is or anything.
struct D3DVertex
{
  float x,y,z ;
  DWORD color ;
  float u, v ; // texture coords
  
  D3DVertex()
  {
    x=y=z = 0.0f;
    color = D3DCOLOR_XRGB( 0,0,0 ) ;
  }

  D3DVertex( float ix, float iy, float iz )
  {
    x=ix;y=iy;z=iz;
    color = D3DCOLOR_XRGB( 255,255,255 ) ;
  }

  D3DVertex( float ix, float iy, float iu, float iv )
  {
    x=ix;y=iy;z=0;
    u = iu; v = iv ;
    color = D3DCOLOR_XRGB( 255,255,255 ) ;
  }

  D3DVertex( float ix, float iy, float iz,
    float iu, float iv,
    unsigned char ir, unsigned char ig, unsigned char ib )
  {
    x=ix;y=iy;z=iz;
    u=iu;v=iv;
    color = D3DCOLOR_XRGB( ir, ig, ib ) ;
  }

  // Lets you pick alpha
  D3DVertex( float ix, float iy, float iz,
    float iu, float iv,
    unsigned char ir, unsigned char ig, unsigned char ib, unsigned char ALPHA )
  {
    x=ix;y=iy;z=iz;
    u=iu;v=iv;
    color = D3DCOLOR_ARGB( ALPHA, ir, ig, ib ) ;
  }
} ;

#endif // D3DVERTEX_H