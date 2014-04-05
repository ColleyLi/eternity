#ifndef D3DWINDOW_H
#define D3DWINDOW_H

/*
    ___ _____   ___ 
   /   \___ /  /   \
  / /\ / |_ \ / /\ /
 / /_// ___) / /_// 
/___,' |____/___,'  
                  
*/

#include "GraphicsWindow.h"

class D3DWindow : public GraphicsWindow
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
  // BUT, it still is defined in "SpriteMan",
  // and SpriteMan is accessible to only
  // GamePak (derived class of this class.)
  // So for the derived class
  // to "communicate" with this base class,
  // provide a "receiving point" in the base class
  // that the derived class should just know to call.

  vector<ID3DXLine*> registeredLines ; // just to follow the pattern..
  // there's only one line as of now, but, there could be more in the future
  
  D3DCOLOR clearColor ;

private:
  /// v:   position only
  /// vc:  position and color
  /// vt:  position texture
  /// vn:  position normal
  /// vtn: position texture normal
  /// all of these are in the order they appear
  IDirect3DVertexDeclaration9 *v, *vc, *vt, *vtc, *vn, *vtn ;

public:
  D3DWindow( HINSTANCE hInst, TCHAR* windowTitleBar ) ;
  D3DWindow( HINSTANCE hInst, TCHAR* windowTitleBar, int windowXPos, int windowYPos, int windowWidth, int windowHeight ) ;
  ~D3DWindow() ;
  IDirect3DDevice9* getGpu(){ return gpu; }

private:
  bool setupGPU() ;
  bool initD3D() ;
  bool initD3D( int width, int height ) ;

public:
  void setDefaultRenderStateOptions() ;

private:
  void initVertexDeclarations() ;

public:
  void screenshot() ;


  void setLight( int index, D3DLIGHT9* light ) ;
  void setLighting( BOOL on ) ;
  void setMaterial( D3DMATERIAL9* material ) ;
  void setVertexDeclaration( VertexType vType ) ;

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

public:
  bool beginDrawing() ; // Call once per frame before drawing.
  // Returns TRUE when beginDrawing succeeded.
  // Don't try and draw if this function fails (it means the D3D device
  // is temporarily "lost" (i.e. some other app has control of it))
  bool endDrawing() ;   // "flips the buffer" over so the user can see
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
  void registerLine( ID3DXLine* line) ;

} ;

#endif