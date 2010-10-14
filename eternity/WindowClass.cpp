////////////////////////////////////////////////////
// Windows "plumbing".

#include "WindowClass.h"

// A class that "abstracts away" the process
// of getting a window up on the screen.
Window::Window( HINSTANCE hInst, TCHAR* windowTitleBar, int windowXPos, int windowYPos, int windowWidth, int windowHeight )
{
  // Save off these parameters in private instance variables.
  hInstance = hInst ;

  // Create a window.
  WNDCLASSEX window = { 0 } ;
  window.cbSize			= sizeof( WNDCLASSEX ) ;
  window.hbrBackground	= (HBRUSH)GetStockObject( WHITE_BRUSH );
  window.hCursor = LoadCursor( NULL, IDC_ARROW ) ;
  window.hIcon = LoadIcon( NULL, IDI_APPLICATION ) ;
  window.hIconSm = LoadIcon( NULL, IDI_APPLICATION ) ;
  window.hInstance = hInstance ;
  window.lpfnWndProc = WndProc ;
  window.lpszClassName = TEXT( "myWindow" ) ;  // XXXX MATCH ME XXXX
  window.lpszMenuName = NULL;
  window.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC ;

  if(!RegisterClassEx( &window ))
  {
    bail( "Something's wrong with the WNDCLASSEX structure you defined.. quitting" ) ;
    return ;
  }

  RECT wndSize ;
  wndSize.left = windowXPos ;
  wndSize.right = windowXPos + windowWidth ;
  wndSize.top = windowYPos ;
  wndSize.bottom = windowYPos + windowHeight ;

  AdjustWindowRectEx( &wndSize, WS_OVERLAPPEDWINDOW, NULL, 0 ) ;

  // Create the main window
  hwnd = CreateWindowEx(

    0 /*WS_EX_TOPMOST*/,  // extended window style.. if set to WS_EX_TOPMOST, for example,
    // then your window will be the topmost window all the time.  Setting it to 0
    // will make your window just another regular everyday window (not topmost or anything).

    TEXT( "myWindow" ),  // window class name.. must match XXXX MATCH ME XXXX above, exactly

    windowTitleBar,      // text in title bar of your window

    WS_OVERLAPPEDWINDOW, // window style.  Try using WS_POPUP, for example.

    wndSize.left, wndSize.top,
    wndSize.right - wndSize.left, wndSize.bottom - wndSize.top,

    // Don't worry about the next 4, not important for now.
    NULL, NULL,
    hInstance, NULL
  ) ;
  ShowWindow( hwnd, SW_SHOWNORMAL ) ;
  UpdateWindow( hwnd ) ;

  info( "Turning off the mouse cursor..." ) ;
  ShowCursor( FALSE ) ;


}

Window::~Window()
{
  // ... clean up and shut down ... 

  
  while( !directories.empty() )
  {
    // free all strings stored in the "directories" stack
    free( directories.top() ) ;
    directories.pop() ;
  }
}


bool Window::setSize( int width, int height, bool fullScreen )
{
  info( "WindowClass resizing window to %d %d", width, height ) ;

  RECT wndSize ;
  wndSize.left = 0 ;
  wndSize.right = width ;
  wndSize.top =  0 ;
  wndSize.bottom = height ;

  // tell the mouse about the resize, before you
  // adjust the rect

  // We have to AdjustWindowRectEx() so the client area
  // is exactly the right size
  AdjustWindowRectEx( &wndSize, WS_OVERLAPPEDWINDOW, NULL, 0 ) ;

  SetWindowPos( hwnd, HWND_TOP, 0, 0, wndSize.right - wndSize.left, wndSize.bottom - wndSize.top, 
    SWP_NOMOVE | SWP_NOZORDER ) ;

  return true ;
}

/// Takes you back to the directory you were
/// in previously (equivalent to "back button"
/// in windows explorer.)
bool Window::cdPop()
{
  if( directories.empty() )
  {
    error( "You are already at the top level directory" ) ;
    return false ;
  }
  
  if( !SetCurrentDirectoryA( directories.top() ) )
  {
    error( "Couldn't switch directory to %s", directories.top() ) ;
  }

  free( directories.top() ) ;
  directories.pop() ;
  return true ;
}

/// Switches you into a working directory
void Window::cd( char *path )
{
  if( !path )
  {
    error( "You can't change directory to NULL, nothing done" ) ;
    return ;
  }

  // Save the current directory to the stack
  char *cwd = (char*)malloc( MAX_PATH ) ;
  GetCurrentDirectoryA( MAX_PATH, cwd ) ;
  directories.push( cwd ) ;
  
  if( !SetCurrentDirectoryA( path ) )
  {
    error( "Couldn't switch directory to %s", path ) ;
  }
  else
  {
    // This verifies the cd command worked
    char nowDir[MAX_PATH];
    GetCurrentDirectoryA( MAX_PATH, nowDir ) ;
    info( "Current working directory is '%s'", nowDir ) ;
  }
}