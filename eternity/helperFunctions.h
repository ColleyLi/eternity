#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

// Functions that do error checking
// Just print info about errors that
// just happened.

#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>

#include <fmod.h>
#include <fmod_errors.h>

#include <d3d9.h>      // core direct3d
#include <d3dx9.h>     // aux libs

#include <dxerr.h>    // detailed error messages

#include "ConsoleColors.h"

#define LOG_ERROR   ( 1 << 0 )
#define LOG_WARNING ( 1 << 1 )
#define LOG_INFO    ( 1 << 2 )

extern FILE *logFile ;

// Variables that make it so we can
// only output ERROR and WARNING messages
// to the file, but output ALL types of
// ERROR, WARNING, INFO messages to the console.
extern int logOutputsForConsole ;  // like a "variable prototype..."
extern int logOutputsForFile ;
extern int logOutputsForDebugStream ;

void logStartup() ;

tm* CurrentTime() ;

void log( int logLevel, const char *fmt, va_list args ) ;

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

void logShutdown() ;

/// Print an error message and quit the program.
/// Calls logShutdown() before exiting.
void bail( char *msg, bool openLog=false ) ;

bool FMOD_ErrorCheck( FMOD_RESULT result ) ;

int XINPUT_Check( int errCode ) ;

bool argCheck( char *fnName, char* str, int numArgsGot, int numArgsExpected ) ;

// numerical.  A few of these functions
// can be templated.

/// Gets you a random float between
/// 'a' and 'b'
float randFloat( float a, float b ) ;

/// Gets you a random float
/// between 0.0 and 1.0
float randFloat() ;

/// Gets you a random int
/// on [low,high)
int randInt( int low, int high ) ;

/// Linearly interpolates between
/// a and b by 't'.
float lerp( float a, float b, float t ) ;

/// Linearly interpolates between
/// a and b by 't'.
double lerp( double a, double b, double t ) ;

/// Clamps num between low and high
/// modifies num
void clamp( int &num, int low, int high ) ;

/// Clamps num between low and high
/// modifies num
void clamp( float &num, float low, float high ) ;

/// Clamp for
/// doubles
void clamp( double &num, double low, double high ) ;

/// Clamps an integer value
/// num between low and high
/// without modifying num
int clampCopy( const int num, int low, int high ) ;

/// Clamps a floating point value
/// num between low and high
/// without modifying num
float clampCopy( const float num, float low, float high ) ;

/// Templated versions of lerp and clamp
/*
template<typename T>
T lerp( T a, T b, T t )
{
  return a + ( b - a )*t ;
}

template<typename T>
void clamp( T &num, T low, T high )
{
  if( num < low )
    num = low ;
  if( num > high )
    num = high ;
}
*/



/// Rounds x to
/// nearest integer
int round( double x ) ;

/// Matrix: Set a row with a vector
/// (leaves 4th entry as was)
D3DXMATRIX* D3DXMatrixSetRow( D3DXMATRIX* matrix, int row, D3DXVECTOR3* vec );

/// Matrix: Set a column with a vector
/// (leaves 4th entry as was)
D3DXMATRIX* D3DXMatrixSetCol( D3DXMATRIX* matrix, int col, D3DXVECTOR3* vec );

// ASSUMES YOUR VECTORS ARE ALREADY NORMALIZED
D3DXMATRIX* D3DXMatrixPointTowards( D3DXMATRIX *mat, D3DXVECTOR3 *fwd, D3DXVECTOR3 *up, D3DXVECTOR3 *eye ) ;

D3DXVECTOR3* D3DXVec3SetMagnitude( D3DXVECTOR3 *vec, float mag ) ;

D3DXVECTOR3* D3DXVec3Truncate( D3DXVECTOR3 *vec, float minMag, float maxMag ) ;

float D3DXVec3AngleBetweenAlreadyNormal( D3DXVECTOR3 *u, D3DXVECTOR3 *v ) ;

float D3DXVec3AngleBetween( D3DXVECTOR3 *u, D3DXVECTOR3 *v ) ;

inline void ZeroVector(D3DXVECTOR3 &vec)
{
  vec.x=vec.y=vec.z=0.0f;
}

inline void printMat( D3DXMATRIX mat )
{
  plain( "%.2f %.2f %.2f %.2f\n"
    "%.2f %.2f %.2f %.2f\n"
    "%.2f %.2f %.2f %.2f\n"
    "%.2f %.2f %.2f %.2f\n",
    mat._11, mat._12, mat._13, mat._14,
    mat._21, mat._22, mat._23, mat._24,
    mat._31, mat._32, mat._33, mat._34,
    mat._41, mat._42, mat._43, mat._44 ) ;
}

D3DXMATRIX lookAt( D3DXVECTOR3 eye, D3DXVECTOR3 look, D3DXVECTOR3 up ) ;

void setColor( D3DCOLORVALUE *color, float a, float r, float g, float b ) ;

void addSinewave( short *data, int durationInSamples,
                  int offset, int frequency,
                  short amplitude, bool distortion ) ;

void addSquarewave( short *data, int durationInSamples, int offset,
                    int fundamentalFrequency, short amplitude ) ;

void addSlidingSinewave( short *data, int durationInSamples,
                         int offset,
                         int frequency1, int frequency2,
                         short amplitude1, short amplitude2,
                         bool distortion ) ;

void addSlidingSquarewave( short *data, int durationInSamples, int offset,
                           int fundamentalFrequency1, int fundamentalFrequency2,
                           short amplitude1, short amplitude2 ) ;

/// Gets you a unicode copy of the string you pass it.
/// YOU ARE RESPONSIBLE FOR CALLING delete[] on the string returned!
wchar_t* getUnicode( char* ascii ) ;

/// b must exist.
#define cstrcpy(a,b) if(b){a=(char*)malloc( strlen(b)+1 ) ; strcpy(a,b) ;}
#define cstrfree(str) if(str){free(str);str=0;}

/// Advance your str pointer to the next word
/// in the string.  If there is no next word,
/// you will get a NULL
#define cstrNextWord(str) {str=strchr(str, ' '); if(str){while( (*str) && isspace(*str) ){++str;} if(!(*str)){str=0;}}}
//                         1. find next ' '      2.  if there was a ' ', adv past all whitespace   3. if nothing but white space null out str

// R S T 0
// 0 1 2 3
// len is 3
// str[len] = 0 does nothing
// str[len-1] sets last char to 0
/// nulls the newline at the end of a string.
/// we have to use strrchr to make sure its safe
#define cstrnulllastnl(str) {char* nl=strrchr(str,'\n'); if(nl){*nl=0;}}
#define cstrnullnextsp(str) {char* nl=strchr(str,' '); if(nl){*nl=0;}}

char* sprintNow( char* buf ) ;

void printRawKeyboard( RAWINPUT * raw ) ;
void printRawMouse( RAWINPUT * raw ) ;

void setRectangle( DWORD* arrayPtr, RECT section, int w, int h, DWORD *value ) ;

void printWindowsLastError( char *msg ) ;

bool DX_CHECK( HRESULT hr, char * msg ) ;  // checks for errors on the HR passed.

// Macros.
#define foreach( listPtrType, listPtr, listInstance ) for( listPtrType listPtr = listInstance.begin(); listPtr != listInstance.end(); ++listPtr )
#define SAFE_RELEASE(ptr) if(ptr) { ptr->Release(); ptr = NULL; }
#define CAST_AS_DWORD(x) *((DWORD*)&x)
#define PI 3.1415926535897932384626433832795
#define DEGTORADCONST 0.017453292519943295769236907684886
#define RADTODEGCONST 57.295779513082320876798154814105
#define RADIANS(degreeMeasure) (degreeMeasure*DEGTORADCONST)
#define DEGREES(radianMeasure) (radianMeasure*RADTODEGCONST)

#define KEY_DOWN_MASK 0x80
#define KEY_TOGGLE_MASK 0x1

#define KEY_IS_DOWN(KeyStateArray,VK) (KeyStateArray[VK]&KEY_DOWN_MASK)
#define KEY_IS_UP(KeyStateArray,VK) (!KEY_IS_DOWN(KeyStateArray,VK))

#define containsFlag( val, flag ) ( val & flag )
#define notContainsFlag( val, flag ) ( !(val & flag) )

#define DESTROY(OBJ) if(OBJ){delete (OBJ); (OBJ)=0;}

#endif // HELPER_FUNCTIONS_H