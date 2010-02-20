#include "ConsoleColors.h"

void consoleCreate()
{
  // Attach a console
  AllocConsole() ;
  AttachConsole( GetCurrentProcessId() ) ;
  freopen( "CON", "w", stdout ) ;
}

void consoleRowsAndCols( int rows, int cols )
{
  char cmd[ 60 ] ;
  sprintf( cmd, "mode %d, %d", cols, rows ) ;
  system( cmd ) ;
}

// Sets the console's size in PIXELS.
// This function has no effect on # rows
// and cols the console has.  Use consoleRowsAndCols
// for that.
void consoleSizePixels( int width, int height )
{
  HWND hwndConsole = GetConsoleWindow() ;  // grab its "handle"..
  SetWindowPos( hwndConsole, HWND_TOPMOST, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER ) ;
}

// moves the console to x, y location,
// x measured from LEFT EDGE of screen
// y measured from TOP EDGE of screen
void consoleMove( int x, int y )
{
  HWND hwndConsole = GetConsoleWindow() ;  // grab its "handle"..
  SetWindowPos( hwndConsole, HWND_TOPMOST, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER ) ;
}

// Makes the console the topmost window
void consoleMakeTopmostWindow()
{
  HWND hwndConsole = GetConsoleWindow() ;  // grab its "handle"..
  SetWindowPos( hwndConsole, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE ) ;
}

void consoleSetColor( WORD color )
{
  static HANDLE h = 0 ;
  if( h == 0 )  h = GetStdHandle( STD_OUTPUT_HANDLE ) ;

  SetConsoleTextAttribute( h, color ) ;
}

void consoleBlack()
{
  // White background, black text
  consoleSetColor( BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE |
    BACKGROUND_INTENSITY ) ;
}

void consoleBrightRed()
{
  consoleSetColor( FOREGROUND_RED | FOREGROUND_INTENSITY ) ;
}

void consoleBrightGreen()
{
  consoleSetColor( FOREGROUND_GREEN | FOREGROUND_INTENSITY ) ;
}

void consoleBrightBlue()
{
  consoleSetColor( FOREGROUND_BLUE | FOREGROUND_INTENSITY ) ;
}

void consoleBrightYellow()
{
  consoleSetColor( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY ) ;
}

void consoleBrightMagenta()
{
  consoleSetColor( FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY ) ;
}

void consoleBrightCyan()
{
  consoleSetColor( FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY ) ;
}

void consoleLightGray()
{
  consoleSetColor( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE ) ;
}

void consoleDarkGray()
{
  consoleSetColor( FOREGROUND_INTENSITY ) ;
}

void consoleWhite()
{
  consoleSetColor( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE |
    FOREGROUND_INTENSITY ) ;
}

void consoleDarkRed()
{
  consoleSetColor( FOREGROUND_RED ) ;
}

void consoleDarkGreen()
{
  consoleSetColor( FOREGROUND_GREEN ) ;
}

void consoleDarkBlue()
{
  consoleSetColor( FOREGROUND_BLUE ) ;
}

void consoleDarkYellow()
{
  consoleSetColor( FOREGROUND_RED | FOREGROUND_GREEN ) ;
}

void consoleDarkMagenta()
{
  consoleSetColor( FOREGROUND_RED | FOREGROUND_BLUE ) ;
}

void consoleDarkCyan()
{
  consoleSetColor( FOREGROUND_GREEN | FOREGROUND_BLUE ) ;
}


