#ifndef CONSOLE_H
#define CONSOLE_H

#include <windows.h>
#include <stdio.h>
#include <time.h>


// Use a namespace as a compromise
// between functions in global namespace
// and a static class.
namespace Console
{
  static HWND hwnd ;
  static HANDLE handle ;

  /// Last color used by console so
  /// it can be reverted back one step
  static WORD colorUsedLastCall, colorUsed2CallsAgo ;

  
  /// Logging.
  enum ErrorLevel
  {
    Error   = 1 << 0,
    Warning = 1 << 1,
    Info    = 1 << 2
  } ;

  static FILE *logFile ;

  // Variables that make it so we can
  // only output ERROR and WARNING messages
  // to the file, but output ALL types of
  // ERROR, WARNING, INFO messages to the console.
  static int logOutputsForConsole ;
  static int logOutputsForFile ;
  static int logOutputsForDebugStream ;

  /// This class should be a singleton.
  /// An application can have only 1 console.
  void initConsole() ;

  void setRowsAndCols( int rows, int cols ) ;

  // Sets the console's size in PIXELS.
  // This function has no effect on # rows
  // and cols the console has.  Use setRowsAndCols
  // for that.
  void setSizeInPixels( int pixelsWidth, int pixelsHeight ) ;

  // moves the console to x, y location,
  // x measured from LEFT EDGE of screen
  // y measured from TOP EDGE of screen
  void moveTo( int x, int y ) ;

  // Makes the console the topmost window
  void makeTopmostWindow() ;

  void setColor( WORD color );

  void revertToLastColor() ;

  void black();

  void white() ;

  void lightGray() ;

  void darkGray() ;

  void brightRed() ;

  void brightGreen() ;

  void brightBlue() ;

  void brightYellow() ;

  void brightMagenta() ;

  void brightCyan() ;

  void darkRed() ;

  void darkGreen() ;

  void darkBlue() ;

  void darkYellow() ;

  void darkMagenta() ;

  void darkCyan() ;

  
  #define LOG_ERROR   ( 1 << 0 )
  #define LOG_WARNING ( 1 << 1 )
  #define LOG_INFO    ( 1 << 2 )


  void logStartup() ;
  void logShutdown() ;
  void log( int logLevel, const char *fmt, va_list args ) ;
  tm* getCurrentTime() ;




  /// Something went wrong
  /// in the program.
  void error( const char *fmt, ... ) ;

  /// Warnings are things that aren't really bad for the program,
  /// but they are things to watch out for, things you might not expect.
  void warning( const char *fmt, ... ) ;

  /// Just some information about things that are happening in the program.
  /// Normal, expected behavior should come in info() messages.
  void info( const char *fmt, ... ) ;

  /// Logs a debug message but with no
  /// timestamp
  void plain( const char *fmt, ... ) ;

  /// Print an error message and quit the program.
  /// Calls logShutdown() before exiting.
  void bail( char *msg, bool openLog=false ) ;

  /// Turn on or off log outputs for
  /// file, console and debugstream
  void setLogOutputsForConsole( bool isOn ) ;
  void setLogOutputsForFile( bool isOn ) ;
  void setLogOutputsForDebugStream( bool isOn ) ;

}

#endif