#ifndef WINDOW_CLASS_H
#define WINDOW_CLASS_H

#pragma region includes and library link
// std c
#include <stdio.h>
#include <math.h>

// stl
#include <vector>
#include <map>
#include <stack>
using namespace std ;

// windows
#include <windows.h>
#include <tchar.h>
#include <gdiplus.h>   // for rendering primitives to textures
#pragma comment( lib, "gdiplus.lib" )
using namespace Gdiplus ;

#include "helperFunctions.h"

// D3D
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")  // aux libs
#pragma comment(lib, "dxerr.lib")
#ifdef _DEBUG
  #pragma comment(lib,"d3dx9d.lib")
#else
  #pragma comment(lib,"d3dx9.lib")
#endif

#pragma endregion


LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam ) ;


/*
          _           _                   
__      _(_)_ __   __| | _____      _____ 
\ \ /\ / / | '_ \ / _` |/ _ \ \ /\ / / __|
 \ V  V /| | | | | (_| | (_) \ V  V /\__ \
  \_/\_/ |_|_| |_|\__,_|\___/ \_/\_/ |___/

*/

// A class that "abstracts away" the process
// of getting a window up on the screen
// (and initializing directx!)
class Window
{ 
protected:
  // Windows
  HINSTANCE hInstance ;
  HWND hwnd ;
  
  stack<char*> directories ;

public:
  Window( HINSTANCE hInst, TCHAR* windowTitleBar, int windowXPos, int windowYPos, int windowWidth, int windowHeight ) ;
  ~Window() ;

  virtual bool setSize( int width, int height, bool fullScreen ) ;
  //!! Provide virtual functions getWidth(), getHeight() which
  // get the client area width and height.


  /// Takes you back to the directory you were
  /// in previously (equivalent to "back button"
  /// in windows explorer.)
  bool cdPop();

  /// Switches you into a working directory
  void cd( char *path );
} ;





#endif // WINDOW_CLASS_H