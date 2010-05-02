#include "helperFunctions.h"

FILE *logFile ;

int logOutputsForConsole ;  // like a "variable declaration..."
int logOutputsForFile ;
int logOutputsForDebugStream ;

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

tm* CurrentTime()
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
  strftime( timeBuf, 255, "%X", CurrentTime() ) ;
  
  char *errLevel ;
  switch( logLevel )
  {
  case LOG_ERROR :
    consoleBrightRed() ;
    errLevel = "error" ;
    break;

  case LOG_WARNING :
    consoleBrightYellow() ;
    errLevel = "warning" ;
    break;

  case LOG_INFO :
    consoleLightGray() ;
    errLevel = "info" ;
    break;
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

void logShutdown()
{
  fprintf( logFile, "-- end" ) ;
  fclose( logFile ) ;
}

void bail( char *msg, bool openLog )
{
  info( "BAIL. %s", msg ) ;
  logShutdown() ; //properly shut down the log

  if( openLog )
    system( "START notepad.exe lastRunLog.txt" ) ;
  
  exit( 1 );
}

bool FMOD_ErrorCheck( FMOD_RESULT result )
{
  if( result != FMOD_OK )
  {
    error( "FMOD error! (%d) %s", result, FMOD_ErrorString(result) );
    return false ;
  }

  return true ;
}

bool argCheck( char *fnName, char* str, int numArgsGot, int numArgsExpected )
{
  if( numArgsGot != numArgsExpected )
  {
    warning( "%s failed to parse %d args from `%s`, parsed %d", fnName, numArgsExpected, str, numArgsGot ) ;
    return false ;
  }
  else
    return true ;
}

int XINPUT_Check( int errCode )
{
  if( errCode != ERROR_SUCCESS ) // what genius..
  {
    error( "XInput error: (%d) - is the controller connected?", errCode ) ;
  }

  return errCode ;
}

float randFloat( float a, float b )
{
  return lerp( a, b, randFloat() ) ;
}

// Random float between 0 and 1
float randFloat()
{
  return (float)rand() / RAND_MAX ;
}

int randInt( int low, int high ) 
{
  return low + rand() % ( high-low ) ;
}

float lerp( float a, float b, float t )
{
  return a + ( b - a )*t ;
}

double lerp( double a, double b, double t )
{
  return a + ( b - a )*t ;
}

void clamp( int &num, int low, int high )
{
  if( num < low )
    num = low ;
  if( num > high )
    num = high ;
}

void clamp( float &num, float low, float high )
{
  if( num < low )
    num = low ;
  if( num > high )
    num = high ;
}

int clampCopy( const int num, int low, int high )
{
  // Follow same behavior as clamp(),
  // so high first
  if( num > high )
    return high ;
  else if( num < low )
    return low ; // 
  else           // otherwise if it falls between low and high
    return num ; // you get the same number back
}

float clampCopy( const float num, float low, float high )
{
  if( num > high )
    return high ;
  else if( num < low )
    return low ; // 
  else           // otherwise if it falls between low and high
    return num ; // you get the same number back
}

int round( double x )
{
  return (int)( 0.5 + x ) ;
}

D3DXMATRIX* D3DXMatrixSetRow( D3DXMATRIX* matrix, int row, D3DXVECTOR3* vec )
{
  // We'll do this using a memcpy.
  //memcpy( &(matrix->m[row][0]), vec, sizeof(D3DXVECTOR3) ) ;
  matrix->m[row][0] = vec->x ; 
  matrix->m[row][1] = vec->y ; 
  matrix->m[row][2] = vec->z ;

  return matrix ;
}

D3DXMATRIX* D3DXMatrixSetCol( D3DXMATRIX* matrix, int col, D3DXVECTOR3* vec )
{
  matrix->m[0][col] = vec->x ; 
  matrix->m[1][col] = vec->y ; 
  matrix->m[2][col] = vec->z ;

  return matrix ;
}

D3DXMATRIX* D3DXMatrixPointTowards( D3DXMATRIX *mat, D3DXVECTOR3 *fwd, D3DXVECTOR3 *up, D3DXVECTOR3 *eye )
{
  D3DXMatrixIdentity( mat ) ;

  float d = D3DXVec3Dot( up, fwd ) ;
  D3DXVECTOR3 u2 = *up - d*(*fwd) ;
  D3DXVec3Normalize( &u2, &u2 ) ;

  D3DXVECTOR3 right ;
  D3DXVec3Cross( &right, &u2, fwd ) ;

  // An article lists this matrix as being

  // but that didn't actually work correctly.
  D3DXMatrixSetRow( mat, 0, fwd ) ;
  D3DXMatrixSetRow( mat, 1, &right ) ;
  D3DXMatrixSetRow( mat, 2, &u2 ) ;
  D3DXMatrixSetRow( mat, 3, eye ) ; // bottom row is translation

  return mat ;
}

/*
D3DXMATRIX lookAt( D3DXVECTOR3 eye, D3DXVECTOR3 look, D3DXVECTOR3 up )
{
  D3DXVECTOR3 xaxis, yaxis, zaxis ;

  zaxis = eye - look ;
  D3DXVec3Normalize( &zaxis, &zaxis ) ;

  D3DXVec3Cross( &xaxis, &up, &zaxis ) ;
  D3DXVec3Normalize( &xaxis, &xaxis ) ;

  D3DXVec3Cross( &yaxis, &zaxis, &xaxis ) ;

  D3DXVECTOR3 dots ;
  dots.x = - D3DXVec3Dot( &xaxis, &eye ) ;
  dots.y = - D3DXVec3Dot( &yaxis, &eye ) ;
  dots.z = - D3DXVec3Dot( &zaxis, &eye ) ;

  D3DXMATRIX ret(

    zaxis.x,   yaxis.x,  -xaxis.x,  0,
    zaxis.y,   yaxis.y,  -xaxis.y,  0,
    zaxis.z,   yaxis.z,  -xaxis.z,  0,
   
    
    -dots.z,    -dots.y,   -dots.x,    1 ) ;

  return ret ;
}
*/

void setColor( D3DCOLORVALUE *color, float a, float r, float g, float b )
{
  color->a = a ;
  color->r = r ;
  color->g = g ;
  color->b = b ;
}

void addSinewave( short *data, int durationInSamples,
                  int offset, int frequency,
                  short amplitude, bool distortion ) 
{
  static double NTs = 1.0 / 44100.0 ;  // assumes this sampling rate

  // "distortion" is just adding 1 to the
  // sinewave so that it will be all positive, so it will totally 
  // get crazy with distortion when multiplied by amp,
  // if amp is high enough
  if( distortion )
    for( int i = offset ; i < offset + durationInSamples ; i++ )
      data[ i ] += (short)(amplitude*( 1 + sin( 2*PI*frequency*i*NTs ) ) ) ;
  else
    for( int i = offset ; i < offset + durationInSamples ; i++ )
      data[ i ] += (short)(amplitude*( sin( 2*PI*frequency*i*NTs ) ) ) ;
}

void addSquarewave( short *data, int durationInSamples, int offset,
                    int fundamentalFrequency, short amplitude )
{
  static double NTs = 1.0 / 44100.0 ;  // assumes this sampling rate

  // we'll just do a signum on the fundamental
  for( int i = offset ; i < offset + durationInSamples ; i++ )
  {
    // we're only interested in the sign of this
    int up = ( sin( 2*PI*fundamentalFrequency*i*NTs ) > 0 ) ?
      +1 :  // if the sin() is bigger than 0, then +1
      -1 ;  // if the sin() is <= 0, then -1
    
    data[ i ] += up*amplitude ;
  }
}

void addSlidingSinewave( short *data, int durationInSamples,
                         int offset,
                         int frequency1, int frequency2,
                         short amplitude1, short amplitude2,
                         bool distortion )
{
  static double NTs = 1.0 / 44100.0 ;  // assumes this sampling rate

  if( distortion )
    for( int i = offset ; i < offset + durationInSamples ; i++ )
    {
      double percentageAlong = (double)i/durationInSamples ;
      double freq = lerp( frequency1, frequency2, percentageAlong ) ;
      double amp = lerp( amplitude1, amplitude2, percentageAlong ) ;
      data[ i ] += (short)(amp*( 1 + sin( 2*PI*freq*i*NTs ) ) ) ;
    }
  else
    for( int i = offset ; i < offset + durationInSamples ; i++ )
    {
      double percentageAlong = (double)i/durationInSamples ;
      double freq = lerp( frequency1, frequency2, percentageAlong ) ;
      double amp = lerp( amplitude1, amplitude2, percentageAlong ) ;
      data[ i ] += (short)(amp*( sin( 2*PI*freq*i*NTs ) ) ) ;
    }
}

void addSlidingSquarewave( short *data, int durationInSamples, int offset,
                           int fundamentalFrequency1, int fundamentalFrequency2,
                           short amplitude1, short amplitude2 )
{
  static double NTs = 1.0 / 44100.0 ;  // assumes this sampling rate

  // As we work along our way, the % of freq1 or freq2 is dictated
  double percentageAlong = 0.0 ;

  // we'll just do a signum on the fundamental
  for( int i = offset ; i < offset + durationInSamples ; i++ )
  {
    percentageAlong = (double)i/durationInSamples ;

    // lerp between two freqs
    double freq = lerp( fundamentalFrequency1, fundamentalFrequency2, percentageAlong ) ;
    double amp  = lerp( amplitude1, amplitude2, percentageAlong ) ;

    // we're only interested in the sign of this
    int up = ( sin( 2*PI*freq*i*NTs ) > 0 ) ?
      +1 :  // if the sin() is bigger than 0, then +1
      -1 ;  // if the sin() is <= 0, then -1
    
    data[ i ] += (short)(up*amp) ;
  }
}

wchar_t* getUnicode( char* ascii )
{
  int len = strlen( ascii ) ;
  WCHAR * wstr = new WCHAR[ len+1 ] ;

  MultiByteToWideChar( CP_ACP, 0, ascii, len, wstr, len ) ;
  wstr[ len ] = 0 ; // add null terminator

  return wstr ;
}

void printRawKeyboard( RAWINPUT * raw )
{
  printf("Kbd: "
        "VK=%04x key=%c "
        "MakeCode=%04x (key dp) "
        "Flags:%04x "
        "%s "
        "Reserved:%04x "
        "ExtraInformation:%08x "
        "msg=%04x "
        "WM_KEYDOWN=%d "
        "WM_KEYUP=%d\n", 
        raw->data.keyboard.VKey, raw->data.keyboard.VKey,

        raw->data.keyboard.MakeCode,  // key depression
        raw->data.keyboard.Flags,     // down or up
        raw->data.keyboard.Flags ? "up" : "down",
        raw->data.keyboard.Reserved,
        raw->data.keyboard.ExtraInformation,
        raw->data.keyboard.Message,
        raw->data.keyboard.Message == WM_KEYDOWN,
        raw->data.keyboard.Message == WM_KEYUP
      ) ;
}

void printRawMouse( RAWINPUT * raw )
{
  printf("Mouse: "
    "usFlags=%04x "
    "ulButtons=%04x "
    "usButtonFlags=%04x "
    "usButtonData=%d "
    "ulRawButtons=%d "
    "lLastX=%d "
    "lLastY=%d \n",
    raw->data.mouse.usFlags, 
    raw->data.mouse.ulButtons, 
    raw->data.mouse.usButtonFlags, 
    raw->data.mouse.usButtonData, 
    raw->data.mouse.ulRawButtons, 
    raw->data.mouse.lLastX, 
    raw->data.mouse.lLastY
  );
}


// TEmplates would solve this problem.
void setRectangle( DWORD* arrayPtr, RECT section, int w, int h, DWORD *value )
{
  for( int col = section.left ; col < section.right ; col++ )
  {
    for( int row = section.top ; row < section.bottom ; row++ )
    {
      int idx = col + w*row ;
      arrayPtr[ idx ] = *value ;
    }
  }
}

void printWindowsLastError( char *msg )
{
  LPSTR errorString = NULL ;

  int result = FormatMessageA( FORMAT_MESSAGE_ALLOCATE_BUFFER |
                 FORMAT_MESSAGE_FROM_SYSTEM,
                 0,
                 GetLastError(),
                 0,
                 (LPSTR)&errorString,
                 0,
                 0 );

  error( "%s %s", msg, errorString ) ;
  
  LocalFree( errorString ) ;
}

bool DX_CHECK( HRESULT hr, char * msg )
{
  if( FAILED( hr ) )
  {
    error( "%s. %s:  %s",
            msg, DXGetErrorStringA( hr ), DXGetErrorDescriptionA( hr ) ) ;

    return false ;
  }

  else
    return true ;

}

