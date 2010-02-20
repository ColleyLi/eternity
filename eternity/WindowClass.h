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

// Graphics
#include <d3d9.h>      // core direct3d
#include <d3dx9.h>     // aux libs

#include <dxerr.h>     // detailed error messages

// Sound
#include <fmod.h>      // not working?  Did you install fmod!
#include <fmod_errors.h>

#include "Timer.h"
#include "helperFunctions.h"
#include "D3DVertex.h"
#include "Sprite.h"

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

// The game has 3 possible states.
// At the Menu, Paused, and Running.
enum GameState
{
  Menu,
  Paused,
  Running
} ;

// Map type for FSOUND
typedef map<int, FMOD_SOUND*> /* as simply */ SoundMap ;
typedef map<int, FMOD_SOUND*>::iterator /* as simply */ SoundMapIter ;

// Map type for "textures" (which are "images" in direct3d)
typedef map<int, Sprite*> /* as simply */ SpriteMap ;
typedef map<int, Sprite*>::iterator /* as simply */ SpriteMapIter ;

// A class that "abstracts away" the process
// of getting a window up on the screen
// (and initializing directx!)
LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam ) ;

class Window
{ 
private:
  // Windows
  HINSTANCE hInstance ;
  HWND hwnd ;
  char *gameName ;

  bool paused ;  // game paused or unpaused.
  
  Timer timer ;  // frame limiter

  // FMOD sound objects
  map<int, FMOD_SOUND*> sounds ; 
  FMOD_SYSTEM *fmodSys ;
  FMOD_CHANNEL *fmodChannel ;
  FMOD_SOUND* defaultSound ;  // hard-coded default sound
  // not from a file.

  // <Direct3D objects>:  All must be
  // released on shut down and reset
  // when the device gets reset
  IDirect3D9 *d3d ;
  IDirect3DDevice9 *gpu ;
  ID3DXSprite *id3dxSpriteRenderer ;
  ID3DXFont *id3dxFontArial12 ;
  // </Direct3D objects>

  D3DPRESENT_PARAMETERS d3dpps ;
  D3DCAPS9 caps ;  // represents the capabilities of the gpu device.
  bool isDeviceLost ; // remember whether the device was lost
  // last frame or not.

  SpriteMap sprites ;
  Sprite *defaultSprite ; // a failsafe default sprite
  // who doesn't get loaded from a file, rather its
  // a hard-coded sprite in the code here that gets
  // generated at runtime.

  BYTE keyPreviousStates[ 256 ] ; // holds which keys were down last frame
  BYTE keyCurrentStates[ 256 ] ; // holds which keys are down right now
  int mouseX, mouseY ;  // x and y position of the mouse in the window.
  
public:
  Window( HINSTANCE hInst, TCHAR* windowTitleBar, int windowXPos, int windowYPos, int windowWidth, int windowHeight ) ;

  ~Window() ;


  /*
      ___ _____   ___ 
     /   \___ /  /   \
    / /\ / |_ \ / /\ /
   / /_// ___) / /_// 
  /___,' |____/___,'  
                      
  */
private:
  bool initD3D() ;
  void d3dLoseDevice() ;
  bool d3dResetDevice( D3DPRESENT_PARAMETERS & pps ) ;
  void d3dDeviceCheck() ;
  void d3dShutdown() ;
  bool d3dSupportsNonPowerOf2Textures() ;


  Graphics* createPrimitiveSurface() ;

  
public:
  bool beginDrawing() ; // Call once per frame before drawing.
  // Returns TRUE when beginDrawing succeeded.
  // Don't try and draw if this function fails (it means the D3D device
  // is temporarily "lost" (i.e. some other app has control of it))
  void endDrawing() ;   // "flips the buffer" over so the user can see
  // what we just drew this frame

  
  void drawMouseCursor( int id ) ;
  void drawFrameCounter() ;
  void drawString( char *str, float x, float y, float width, float height, D3DCOLOR color, DWORD formatOptions ) ;
  void drawSprite( int id, float x, float y, float width = SPRITE_READ_FROM_FILE, float height = SPRITE_READ_FROM_FILE ) ;

  void loadSprite( int id, char *filename,
    D3DCOLOR backgroundColor = D3DCOLOR_ARGB( 0,0,0,0 ),
    int singleSpriteWidth = SPRITE_READ_FROM_FILE,
    int singleSpriteHeight = SPRITE_READ_FROM_FILE,
    int numFrames = SPRITE_READ_FROM_FILE,
    float timeBetweenFrames = 0.5f ) ;

  // Gets you a random sprite id from the map.
  // Useful for testing, not much else
  int randomSpriteId() ;

  void drawAxes() ;

  /*
    __                     _ 
   / _|_ __ ___   ___   __| |
  | |_| '_ ` _ \ / _ \ / _` |
  |  _| | | | | | (_) | (_| |
  |_| |_| |_| |_|\___/ \__,_|

  */
private:
  // initialize FMOD sound system
  void initFMOD() ;

public:
  void loadSound( int id, char * filename, int options = 0 ) ;
  void playSound( int id ) ;

  /*
            _           _                   
  __      _(_)_ __   __| | _____      _____ 
  \ \ /\ / / | '_ \ / _` |/ _ \ \ /\ / / __|
   \ V  V /| | | | | (_| | (_) \ V  V /\__ \
    \_/\_/ |_|_| |_|\__,_|\___/ \_/\_/ |___/

  */
  int getWidth() ;
  int getHeight() ;
  bool setSize( int width, int height, bool fullScreen ) ;
  bool fullScreenInMaxResolution() ; // go fullscreen, in max resolution

  /*
   _                   _   
  (_)_ __  _ __  _   _| |_ 
  | | '_ \| '_ \| | | | __|
  | | | | | |_) | |_| | |_ 
  |_|_| |_| .__/ \__,_|\__|
          |_|              
  */
private:
  void startupRawInput();

public:
  void moveMouseX( int diffX ) ;
  void moveMouseY( int diffY ) ;
  int getMouseX() ;
  int getMouseY() ;

  // Tells you if "key" was JUST PRESSED DOWN.
  // 
  // JUST PRESSED DOWN means the key was
  // UP in previous frame, but is DOWN in
  // THIS frame.
  // 
  // If you press and hold a key, for no matter
  // how many seconds you hold it down for,
  // you'll only have JustPressed return true ONCE.
  bool justPressed( int vkCode ) ;

  // Tells you if a key is BEING HELD DOWN
  bool isPressed( int vkCode ) ;

  // Returns true if a key was JUST let go of.
  // A complimentary function to justPressed()
  bool justReleased( int vkCode ) ;
  

  /*
    __ _  __ _ _ __ ___   ___ 
   / _` |/ _` | '_ ` _ \ / _ \
  | (_| | (_| | | | | | |  __/
   \__, |\__,_|_| |_| |_|\___|
   |___/                      

  */
  void step() ;

  void pause() ;
  void unpause() ;
  bool isPaused() ;

  // Returns true if the game is having trouble
  // keeping up at 60fps
  bool isSlow() ;

  // Gives you the amount of time that passed since the last frame
  // Keep in mind this value will "jitter" depending on how the
  // game is currently running.. you have access to it, but you
  // might just want to use a VIRTUAL_SECOND ("fake second") of
  // fixed length (0.016666 seconds.. or 60 fps) instead.

  // In the event of game slowdown, if you use real time,
  // then things will start to jump around the screen,
  // somewhat unpredictably.
  float getTimeElapsedSinceLastFrame() ;




  

} ;

#endif // WINDOW_CLASS_H