#ifndef GDIPLUS_TEXTURE
#define GDIPLUS_TEXTURE

#include <windows.h>
#include <gdiplus.h>   // for rendering primitives to textures
#include <tchar.h>

#include <d3d9.h>      // core direct3d
#include <d3dx9.h>     // aux libs

#include "helperFunctions.h"

using namespace Gdiplus ;

class GDIPlusTexture
{
private:
  IDirect3DTexture9 *tex ;
  IDirect3DSurface9 *surface ;
  HDC texDC ;
  IDirect3DDevice9 *gpu ;  // retain own reference obtained during construction
  int width, height ;

public:
  Graphics *g ;  // g is how you draw to the texture

public:
  // Create a new texture that
  // you can draw to using GDI+
  // smoothingMode will "blur the edges"
  // of the texture so it doesn't look so blocky.
  // "smoothingMode" is slower but looks much better.
  GDIPlusTexture( IDirect3DDevice9 *gpu, int width, int height, bool smoothingMode=true ) ;

  // Call this when you're DONE
  // drawing with g and you want
  // the texture back.  You CAN
  // continue to draw after you
  // getTexture()
  IDirect3DTexture9* getTexture() ;

  int getWidth() ;
  int getHeight() ;

  // Simply draws text with a box around it,
  // computed to be the right size
  void boxedText( TCHAR* text, Font *font, RECT padding, Color textColor, Color bkgColor, StringAlignment alignment ) ;

  // Uses the rect you specify to draw the text
  // and the background box.  text may get cut off
  // using this method
  void boxedText( TCHAR* text, Font *font, RECT padding, Color textColor, Color bkgColor, RectF limitingRect, StringAlignment alignment ) ;

  // Supports BMP, GIF, JPEG, PNG, TIFF, and EMF
  // D3DX supports .bmp, .dds, .dib, .hdr, .jpg, .pfm, .png, .ppm, and .tga
  // So you can use this to load
  // .gif, .tiff, and .emf files while
  // the D3DX function cannot load them.
  static IDirect3DTexture9* D3DXCreateTextureFromFileViaGDIPlus(
    IDirect3DDevice9 *gpu, TCHAR *filename
  ) ;

  ~GDIPlusTexture() ;

} ;

#endif