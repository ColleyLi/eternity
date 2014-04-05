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

#include "Console.h"
using namespace Console ;

#pragma region Error checking

void printWindowsLastError( char *msg ) ;
bool DX_CHECK( HRESULT hr, char * msg ) ;  // checks for errors on the HR passed.
int XINPUT_Check( int errCode ) ;

bool FMOD_ErrorCheck( FMOD_RESULT result ) ;

void printRawKeyboard( RAWINPUT * raw ) ;
void printRawMouse( RAWINPUT * raw ) ;

bool argCheck( char *fnName, char* str, int numArgsGot, int numArgsExpected ) ;
#pragma endregion

#pragma region math
// numerical.  A few of these functions
// can be templated.

#define IsNaN(x) (x!=x)

#define IsNear(x,y,EPS) (fabs((x)-(y))<EPS)

#define SameSign(x,y) ( (x >= 0 && y >= 0) || (x < 0 && y < 0) )

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

template<typename T>
bool contains( const std::vector<T> & container, const T & item )
{
  for( int i = 0 ; i < container.size() ; i++ )
    if(container[i]==item)
      return true ;

  return false ;
}

template<typename T>
bool removeItem( std::vector<T> & container, const T & item )
{
  for( int k = 0 ; k < container.size() ; k++ )
  {
    if( container[k] == item )
    {
      container.erase( container.begin() + k ) ;
      return true ;
    }
  }

  return false ;
}
*/

template<typename T>
inline void swap( T &a, T &b )
{
  T temp = a ;
  a = b ;
  b = temp ;
}

/// Rounds x to
/// nearest integer
int round( double x ) ;

#pragma endregion


#ifdef DIRECT3D_VERSION
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

float D3DXVec2AngleBetweenAlreadyNormal( D3DXVECTOR2 *u, D3DXVECTOR2 *v ) ;

float D3DXVec2AngleBetween( D3DXVECTOR2 *u, D3DXVECTOR2 *v ) ;

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
#endif






#pragma region string and character
/// Gets you a unicode copy of the string you pass it.
/// YOU ARE RESPONSIBLE FOR CALLING delete[] on the string returned!
wchar_t* getUnicode( const char* ascii ) ;

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
#pragma endregion


void setRectangle( DWORD* arrayPtr, RECT section, int w, int h, DWORD *value ) ;


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

// If you accidently delete the wrong thing (something that is null)
// instead of the object you intended to delete, you may never find
// the resulting memory leak (since there is no error on DESTROY(NULL))
// That's why in _DEBUG mode, you get a warning.
#if _DEBUG
#define DESTROY_ARRAY(ARRAY) if(ARRAY){delete[] (ARRAY); (ARRAY)=0;} else { warning( "Delete on NULL array, line %d of %s", __LINE__, __FILE__ ) ; }
#define DESTROY(OBJ) if(OBJ){delete (OBJ); (OBJ)=0;} else { warning( "Delete on NULL object, line %d of %s", __LINE__, __FILE__ ) ; }
#else
#define DESTROY_ARRAY(ARRAY) if(ARRAY){delete[] (ARRAY); (ARRAY)=0;}
#define DESTROY(OBJ) if(OBJ){delete (OBJ); (OBJ)=0;}
#endif


// Define "real" value as float
// in the Math files (Matrix and Vector)
#define real float

#endif // HELPER_FUNCTIONS_H