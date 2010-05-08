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

/// Drawing modes available.  Yes,
/// these are backwards due to
/// C++ variable naming rules (cannot
/// name a mode "3D", and ThreeD is too long.)
/// Read these as, "Dimensions: 3"
enum DrawingMode
{
  D2 = 2,
  D3 = 3
  // Thought of just having the
  // sole function that uses this enum
  // use straight integers instead (indeed it can
  // due to set up here), but I eventually may
  // create .. some other modes than just
  // straight 2d and 3d
} ;

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
  // BUT, it still is defined in "SpriteMan",
  // and SpriteMan is accessible to only
  // GameWindow (derived class of this class.)
  // So for the derived class
  // to "communicate" with this base class,
  // provide a "receiving point" in the base class
  // that the derived class should just know to call.

  vector<ID3DXLine*> registeredLines ; // just to follow the pattern..
  // there's only one line as of now, but, there could be more in the future
  
  // Think about the direction the inheritance arrow goes in..
  
  // We won't try to tell the base class
  // about the derived class (would be really very bad)
  

  D3DCOLOR clearColor ;

  
  /// Eye, look, up vectors
  /// really only matters for 3d drawing modes
  /// These should be moved into Camera3D,
  /// or Camera3D should be integrated
  /// into this class.
  D3DXVECTOR3 eye, look, up ;

  /// The current drawing mode,
  /// will be either 2d or 3d.
  DrawingMode drawingMode ;


public:
  enum VertexType
  {
    Position,
    PositionColor,
    PositionTexture,
    PositionTextureColor,
    PositionNormal,
    PositionTextureNormal
  } ;
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

private:
  void project2D() ;
  void project3D() ;

public:
  /// Let's you set the 3d camera
  /// to different angles etc
  /// Note setting the camera doesn't
  /// __change the mode__ -- to draw
  /// in 3d you must call setDrawingMode( D3 ) ;
  void setCamera( D3DXVECTOR3 &newEye, D3DXVECTOR3 &newLook, D3DXVECTOR3 &newUp ) ;


  /// Let's you specify either
  /// 2d or 3d drawing.  can be
  /// called any # times, but its
  /// best to batch out your 3d calls first,
  /// then do all your 2d drawing needs
  void setDrawingMode( DrawingMode dm ) ;

  DrawingMode getDrawingMode() ;

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
  void registerLine( ID3DXLine* line) ;

} ;

#endif