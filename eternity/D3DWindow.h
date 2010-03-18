#ifndef D3DWINDOW_H
#define D3DWINDOW_H

/*
    ___ _____   ___ 
   /   \___ /  /   \
  / /\ / |_ \ / /\ /
 / /_// ___) / /_// 
/___,' |____/___,'  
                  
*/

#include "WindowClass.h"

class D3DWindow : public Window
{
protected:
  IDirect3D9 *d3d ;
  IDirect3DDevice9 *gpu ;

  D3DPRESENT_PARAMETERS d3dpps ;
  D3DCAPS9 caps ;  // represents the capabilities of the gpu device.
  bool isDeviceLost ; // remember whether the device was lost
  // last frame or not.

  //!! left off
  vector<ID3DXFont*>   registeredFonts ;
  vector<ID3DXSprite*> registeredSprites ;

  D3DCOLOR clearColor ;

public:
  D3DWindow( HINSTANCE hInst, TCHAR* windowTitleBar, int windowXPos, int windowYPos, int windowWidth, int windowHeight ) ;
  ~D3DWindow() ;

private:
  bool initD3D() ;

protected:
  // Because you may need to do more with these
  // in case the device gets lost.
  virtual void d3dLoseDevice() ;
  virtual bool d3dResetDevice( D3DPRESENT_PARAMETERS & pps ) ;
  virtual void d3dDeviceCheck() ;

private:
  void d3dShutdown() ;
  bool d3dSupportsNonPowerOf2Textures() ;

public:
  bool beginDrawing() ; // Call once per frame before drawing.
  // Returns TRUE when beginDrawing succeeded.
  // Don't try and draw if this function fails (it means the D3D device
  // is temporarily "lost" (i.e. some other app has control of it))
  void endDrawing() ;   // "flips the buffer" over so the user can see
  // what we just drew this frame

  void setBackgroundColor( D3DCOLOR color ) ;
  
  int getWidth() ;
  int getHeight() ;
  bool setSize( int width, int height, bool fullScreen ) ;
  bool fullScreenInMaxResolution() ; // go fullscreen, in max resolution

} ;

#endif