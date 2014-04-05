#include "Console.h"

namespace Console
{

  void initConsole()
  {
    // Attach a console
    AllocConsole() ;
    AttachConsole( GetCurrentProcessId() ) ;
    freopen( "CON", "w", stdout ) ;

    hwnd = GetConsoleWindow() ;  // grab its "handle"..
    handle = GetStdHandle( STD_OUTPUT_HANDLE ) ;
  }

  void setRowsAndCols( int rows, int cols )
  {
    char cmd[ 60 ] ;
    sprintf( cmd, "mode %d, %d", cols, rows ) ;
    system( cmd ) ;
  }

  // Sets the console's size in PIXELS.
  // This function has no effect on # rows
  // and cols the console has.  Use setRowsAndCols
  // for that.
  void setSizeInPixels( int pixelsWidth, int pixelsHeight )
  {
    SetWindowPos( hwnd, HWND_TOPMOST, 0, 0, pixelsWidth, pixelsHeight, SWP_NOMOVE | SWP_NOZORDER ) ;
  }

  // moves the console to x, y location,
  // x measured from LEFT EDGE of screen
  // y measured from TOP EDGE of screen
  void moveTo( int x, int y )
  {
    SetWindowPos( hwnd, HWND_TOPMOST, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER ) ;
  }

  // Makes the console the topmost window
  void makeTopmostWindow()
  {
    SetWindowPos( hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE ) ;
  }

  void setColor( WORD color )
  {
    // Change the current color
    SetConsoleTextAttribute( handle, color ) ;
  
    // set up for NEXT call.

    // Now save off the color used 2 calls ago
    // as the color that was used last call.
    colorUsed2CallsAgo = colorUsedLastCall ;
  
    // Now update the color used on THIS call,
    // which will be the color used on the LAST call
    // at the next call.
    colorUsedLastCall = color ;
  
  }

  void revertToLastColor()
  {
    // Change the current color.  This DOESN'T affect
    // the colorUsedLastCall variable etc.
    SetConsoleTextAttribute( handle, colorUsed2CallsAgo ) ;
  }

  void black()
  {
    // White background, black text
    setColor( BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY ) ;
  }

  void white()
  {
    setColor( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY ) ;
  }

  void lightGray()
  {
    setColor( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE ) ;
  }

  void darkGray()
  {
    setColor( FOREGROUND_INTENSITY ) ;
  }

  void brightRed()
  {
    setColor( FOREGROUND_RED | FOREGROUND_INTENSITY ) ;
  }

  void brightGreen()
  {
    setColor( FOREGROUND_GREEN | FOREGROUND_INTENSITY ) ;
  }

  void brightBlue()
  {
    setColor( FOREGROUND_BLUE | FOREGROUND_INTENSITY ) ;
  }

  void brightYellow()
  {
    setColor( FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY ) ;
  }

  void brightMagenta()
  {
    setColor( FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY ) ;
  }

  void brightCyan()
  {
    setColor( FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY ) ;
  }

  void darkRed()
  {
    setColor( FOREGROUND_RED ) ;
  }

  void darkGreen()
  {
    setColor( FOREGROUND_GREEN ) ;
  }

  void darkBlue()
  {
    setColor( FOREGROUND_BLUE ) ;
  }

  void darkYellow()
  {
    setColor( FOREGROUND_RED | FOREGROUND_GREEN ) ;
  }

  void darkMagenta()
  {
    setColor( FOREGROUND_RED | FOREGROUND_BLUE ) ;
  }

  void darkCyan()
  {
    setColor( FOREGROUND_GREEN | FOREGROUND_BLUE ) ;
  }






  /// LOGGING
  // Functions that do error checking
  // Just print info about errors that
  // just happened.
  void logStartup()
  {
    logFile = fopen( "lastRunLog.txt", "w" ) ;

    logOutputsForConsole = LOG_ERROR | LOG_WARNING | LOG_INFO ;
    logOutputsForFile = LOG_ERROR | LOG_WARNING | LOG_INFO ;
    logOutputsForDebugStream = LOG_ERROR | LOG_WARNING ;

    info( "Startup" ) ;
  }

  void logShutdown()
  {
    fprintf( logFile, "-- end" ) ;
    fclose( logFile ) ;
  }

  tm* getCurrentTime()
  {
    static time_t raw ;
  
    // grab the current time
    time( &raw ) ;

    // Now create that timeinfo struct
    static tm* timeinfo ;
    timeinfo = localtime( &raw ) ;
  
    return timeinfo ;
  }

  void log( int logLevel, const char *fmt, va_list args )
  {
    static char msgBuffer[ 512 ] ;
    vsprintf( msgBuffer, fmt, args ) ;
  
    // write time into timeBuff
    static char timeBuf[ 256 ] ;
    strftime( timeBuf, 255, "%X", getCurrentTime() ) ;
  
    char *errLevel ;
    switch( logLevel )
    {
    case ErrorLevel::Error :
      brightRed() ;
      errLevel = "error" ;
      break;

    case ErrorLevel::Warning :
      brightYellow() ;
      errLevel = "warning" ;
      break;

    case ErrorLevel::Info :
      lightGray() ;
      errLevel = "info" ;
      break;

    default:
      darkGray() ;

    }

    // Put it all together
    static char buf[ 768 ] ;
    sprintf( buf, "[ %s ][ %s ]:  %s\n", errLevel, timeBuf, msgBuffer ) ;

    // If the error's log level qualifies it
    // to be output to the console based on
    // current console flags..
    if( logOutputsForConsole & logLevel )
      printf( buf ) ;
    if( logOutputsForFile & logLevel )
      fprintf( logFile, buf ) ;
  


    // Also put it in the Visual Studio debug window
    // this also appears in DEBUGVIEW by Mark Russinovich, which you can get
    // from http://technet.microsoft.com/en-us/sysinternals/bb896647.aspx
    if( logOutputsForDebugStream & logLevel )
    {
      // Add program name in front as well
      sprintf( buf, "[ eternity ][ %s ][ %s ]:  %s\n", errLevel, timeBuf, msgBuffer ) ;
      OutputDebugStringA( buf ) ;
    }
  }

  void error( const char *fmt, ... )
  {
    va_list lp ;
    va_start( lp, fmt ) ;
  
    log( LOG_ERROR, fmt, lp ) ;
  }

  void warning( const char *fmt, ... )
  {
    va_list lp ;
    va_start( lp, fmt ) ;
  
    log( LOG_WARNING, fmt, lp ) ;
  }

  void info( const char *fmt, ... )
  {
    va_list lp ;
    va_start( lp, fmt ) ;
  
    log( LOG_INFO, fmt, lp ) ;
  }

  void plain( const char *fmt, ... )
  {
    va_list args ;
    va_start( args, fmt ) ;

    static char msgBuffer[ 512 ] ;
    vsprintf( msgBuffer, fmt, args ) ;

    // put out to file and console only
    printf( "%s\n", msgBuffer ) ;
    fprintf( logFile, "%s\n", msgBuffer ) ;
  }

  void bail( char *msg, bool openLog )
  {
    info( "BAIL. %s", msg ) ;
    logShutdown() ; //properly shut down the log

    if( openLog )
      system( "START notepad.exe lastRunLog.txt" ) ;
  
    exit( 1 );
  }

}