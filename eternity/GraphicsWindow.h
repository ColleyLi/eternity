#ifndef GRAPHICSWINDOW_H
#define GRAPHICSWINDOW_H

#include "WindowClass.h"

// Interface that D3DWindow and OpenGL window
// must implement to provide basic hardware
// accelerated rendering caps
// to the program
class GraphicsWindow : public Window
{
public:
  /// Set the size of the GraphicsWindow.
  /// Propagates down into base classes
  /// so D3D knows about the change as well.
  virtual bool setSize( int width, int height, bool fullScreen ) override ;

} ;

#endif