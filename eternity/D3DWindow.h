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

  // These are COPIES of the pointers
  // that are in SpriteMan.
  vector<ID3DXFont*>   registeredFonts ;   // there could be many of these.
  vector<ID3DXSprite*> registeredSprites ; // there really
  // should only be one sprite drawing object.
  // BUT, it still is defined in a derived class of
  // this class.  So for the derived class
  // to "communicate" with this base class,
  // instead of trying to educate the base class
  // about the derived class (which is really very bad),
  // provide a "receiving point" in the base class
  // that the derived class should just know to call.

  // With inheritance the communication goes the opposite way.

  D3DCOLOR clearColor ;

public:
  D3DWindow( HINSTANCE hInst, TCHAR* windowTitleBar, int windowXPos, int windowYPos, int windowWidth, int windowHeight ) ;
  ~D3DWindow() ;

private:
  bool initD3D() ;

private:
  void d3dLoseDevice() ;
  bool d3dResetDevice( D3DPRESENT_PARAMETERS & pps ) ;
  void d3dDeviceCheck() ;
  void d3dShutdown() ;
  bool d3dSupportsNonPowerOf2Textures() ;

public:

  // Things the d3dwindow needs to do every frame
  // (check and clean-up state of d3d device really)
  void d3dWindowStep() ;

  bool beginDrawing() ; // Call once per frame before drawing.
  // Returns TRUE when beginDrawing succeeded.
  // Don't try and draw if this function fails (it means the D3D device
  // is temporarily "lost" (i.e. some other app has control of it))
  void endDrawing() ;   // "flips the buffer" over so the user can see
  // what we just drew this frame

  void setBackgroundColor( D3DCOLOR color ) ;
  
  int getWidth() ;
  int getHeight() ;

  /// You can override this function if you need to
  /// do something to your game when the window size changes
  virtual bool setSize( int width, int height, bool fullScreen ) override ;
  bool fullScreenInMaxResolution() ; // go fullscreen, in max resolution

  void registerFont( ID3DXFont* font ) ;
  void registerSpriteRenderer( ID3DXSprite* spriteRenderer ) ;
} ;

#endif