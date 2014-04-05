#ifndef GRAPHICSWINDOW_H
#define GRAPHICSWINDOW_H

#include "WindowClass.h"

enum VertexType
{
  Position,
  PositionColor,
  PositionTexture,
  PositionTextureColor,
  PositionNormal,
  PositionTextureNormal
} ;

// Interface that D3DWindow and OpenGL window
// must implement to provide basic hardware
// accelerated rendering caps
// to the program
class GraphicsWindow : public Window
{
public:
  enum UnderlyingRenderingAPI
  {
    DirectX,
    OpenGL
  }; 

  GraphicsWindow( HINSTANCE hInst, TCHAR* windowTitleBar, 
    int windowXPos, int windowYPos, int windowWidth, int windowHeight ) ;

  /// Set the size of the GraphicsWindow.
  /// Propagates down into base classes
  /// so D3D knows about the change as well.
  virtual bool setSize( int width, int height, bool fullScreen ) { return true;}

} ;

#endif