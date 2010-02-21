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

// Variables that make it so we can
// only output ERROR and WARNING messages
// to the file, but output ALL types of
// ERROR, WARNING, INFO messages to the console.
extern int logOutputsForConsole ;  // like a "variable prototype..."
extern int logOutputsForFile ;
extern int logOutputsForDebugStream ;

void logStartup() ;

tm* CurrentTime() ;

void log( int logLevel, char *fmt, va_list args ) ;

// Something went wrong in the program.
void error( char *fmt, ... ) ;

// Warnings are things that aren't really bad for the program,
// but they are things to watch out for, things you might not expect.
void warning( char *fmt, ... ) ;

// Just some information about things that are happening in the program.
// Normal, expected behavior should come in info() messages.
void info( char *fmt, ... ) ;

void logShutdown() ;

// Print an error message and quit the program.
// Calls logShutdown() before exiting.
void bail( char *msg ) ;

void FMOD_ErrorCheck( FMOD_RESULT result ) ;


// numerical
float randFloat( float a, float b ) ;
float randFloat() ;

float lerp( float a, float b, float t ) ;

double lerp( double a, double b, double t ) ;

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

#define KEY_DOWN_MASK 0x80
#define KEY_TOGGLE_MASK 0x1

#define KEY_IS_DOWN(KeyStateArray,VK) (KeyStateArray[VK]&KEY_DOWN_MASK)
#define KEY_IS_UP(KeyStateArray,VK) (!KEY_IS_DOWN(KeyStateArray,VK))

#endif // HELPER_FUNCTIONS_H