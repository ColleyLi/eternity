#ifndef WINDOW_CLASS_H
#define WINDOW_CLASS_H

#pragma region includes and library link
// windows
#include <windows.h>
#include <gdiplus.h>   // for rendering primitives to textures
#include <tchar.h>

// std c
#include <stdio.h>
#include <math.h>

// stl
#include <vector>
#include <map>
#include <stack>

// Graphics
#include <d3d9.h>      // core direct3d
#include <d3dx9.h>     // aux libs

#include <dxerr.h>     // detailed error messages

#include "GDIPlusTexture.h"
#include "Timer.h"
#include "helperFunctions.h"
#include "D3DVertex.h"
#include "Sprite.h"
#include "Mouse.h"

#include "SoundMan.h"

using namespace Gdiplus ;
using namespace std ;

#pragma comment( lib, "gdiplus.lib" )
#pragma comment( lib, "fmodex_vc.lib" )

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


// A class that "abstracts away" the process
// of getting a window up on the screen
// (and initializing directx!)
LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam ) ;


/*
          _           _                   
__      _(_)_ __   __| | _____      _____ 
\ \ /\ / / | '_ \ / _` |/ _ \ \ /\ / / __|
 \ V  V /| | | | | (_| | (_) \ V  V /\__ \
  \_/\_/ |_|_| |_|\__,_|\___/ \_/\_/ |___/

*/
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