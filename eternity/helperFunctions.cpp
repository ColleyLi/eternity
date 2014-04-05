#include "helperFunctions.h"


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

void clamp( double &num, double low, double high )
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

D3DXVECTOR3* D3DXVec3SetMagnitude( D3DXVECTOR3 *vec, float mag )
{
  D3DXVec3Normalize( vec, vec ) ;
  *vec = (*vec)*mag ;
  return vec ;
}

D3DXVECTOR3* D3DXVec3Truncate( D3DXVECTOR3 *vec, float minMag, float maxMag )
{
  float mag2 = D3DXVec3LengthSq( vec ) ;

  if( mag2 > maxMag*maxMag )
  {
    // Needs correction
    return D3DXVec3SetMagnitude( vec, maxMag ) ;
  }
  else if( mag2 < minMag*minMag )
  {
    return D3DXVec3SetMagnitude( vec, minMag ) ;
  }

  return vec ;
}

// same as D3DXVec3AngleBetween, only assumes
// vectors you pass it are already normalized
float D3DXVec3AngleBetweenAlreadyNormal( D3DXVECTOR3 *u, D3DXVECTOR3 *v )
{
  D3DXVECTOR3 cross ;
  D3DXVec3Cross( &cross, u, v ) ;

  float magCross = D3DXVec3Length( &cross ) ;

  float angle = asinf( magCross ) ;

  return angle ;
}

// used cross product to get signed result
// u X v = |u| |v| sin(t)
float D3DXVec3AngleBetween( D3DXVECTOR3 *u, D3DXVECTOR3 *v )
{
  D3DXVECTOR3 uC, vC ;
  D3DXVec3Normalize( &uC, u ) ;
  D3DXVec3Normalize( &vC, v ) ;

  return D3DXVec3AngleBetweenAlreadyNormal( &uC, &vC ) ;
}

float D3DXVec2AngleBetweenAlreadyNormal( D3DXVECTOR2 *u, D3DXVECTOR2 *v )
{
  float cross = D3DXVec2CCW( u, v ) ;
  return asinf( cross ) ;
}

float D3DXVec2AngleBetween( D3DXVECTOR2 *u, D3DXVECTOR2 *v )
{
  D3DXVECTOR2 uC, vC ;
  D3DXVec2Normalize( &uC, u ) ;
  D3DXVec2Normalize( &vC, v ) ;

  return D3DXVec2AngleBetweenAlreadyNormal( &uC, &vC ) ;
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


wchar_t* getUnicode( const char* ascii )
{
  int len = strlen( ascii ) ;
  WCHAR * wstr = new WCHAR[ len+1 ] ;

  MultiByteToWideChar( CP_ACP, 0, ascii, len, wstr, len ) ;
  wstr[ len ] = 0 ; // add null terminator

  return wstr ;
}

// Prints now into your buffer
char* sprintNow( char* buf )
{
  time_t now ;
  tm * timeinfo ;

  time( &now ) ;
  timeinfo = localtime( &now ) ;

  strftime( buf, MAX_PATH, "%Y_%m_%d_%A_%H-%M-%S.png", timeinfo );

  return buf ;
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

