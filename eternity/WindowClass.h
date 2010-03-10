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

#include "GDIPlusTexture.h"
#include "Timer.h"
#include "helperFunctions.h"
#include "D3DVertex.h"
#include "Sprite.h"
#include "Mouse.h"

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

// Map type for FSOUND
typedef map<int, FMOD_SOUND*> /* as simply */ SoundMap ;
typedef map<int, FMOD_SOUND*>::iterator /* as simply */ SoundMapIter ;

// Map type for "textures" (which are "images" in direct3d)
typedef map<int, Sprite*> /* as simply */ SpriteMap ;
typedef map<int, Sprite*>::iterator /* as simply */ SpriteMapIter ;

typedef multimap<int, FMOD_CHANNEL*> /* as simply */ ChannelMultimap ;
typedef multimap<int, FMOD_CHANNEL*>::iterator /* as simply */ ChannelMultimapIter ;


#define FMOD_LOOP_FOREVER -1 /* Use this constant in loopSound to have the
sound loop forever */

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
  

  // Game
  char *gameName ;
  bool paused ;  // game paused or unpaused.
  Timer timer ;  // frame limiter


  // FMOD sound objects
  map<int, FMOD_SOUND*> sounds ; 

  // This has to be static to account for the fact
  // that FMOD is a C-API.  The channelEndCallback
  // has to be a static function (or a c-style function)
  // so for the channelEndCallback function to access
  // fmodChannels, fmodChannels must also be static.
  static multimap<int, FMOD_CHANNEL*> fmodChannels ; // keep a channel
  // for every playing sound, remembering what
  // the id of that sound is (to support stopSound(id) function)

  FMOD_SYSTEM *fmodSys ;

  const static int fmodMaxChannels = 32 ;  // max # sounds that
  // can be playing at a time on the sound card.


  FMOD_SOUND* defaultSound ;  // hard-coded default sound
  // not from a file.

  // <Direct3D objects>:  All must be
  // released on shut down and reset
  // when the device gets reset
  IDirect3D9 *d3d ;
  IDirect3DDevice9 *gpu ;
  ID3DXSprite *id3dxSpriteRenderer ;
  ID3DXFont *id3dxDefaultFont ;
  // </Direct3D objects>

  D3DCOLOR clearColor ;
  D3DPRESENT_PARAMETERS d3dpps ;
  D3DCAPS9 caps ;  // represents the capabilities of the gpu device.
  bool isDeviceLost ; // remember whether the device was lost
  // last frame or not.

  SpriteMap sprites ;
  Sprite *defaultSprite ; 
  Sprite *whitePixel ;    // used for drawing solid colored boxes.
  // a failsafe default sprite
  // who doesn't get loaded from a file, rather its
  // a hard-coded sprite in the code here that gets
  // generated at runtime.

  BYTE keyPreviousStates[ 256 ] ; // holds which keys were down last frame
  BYTE keyCurrentStates[ 256 ] ; // holds which keys are down right now

  Mouse mouse ;
  
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
  void initDefaultSprite() ;
  void initWhitePixel() ;

public:
  // Creates a surface you can draw to
  // using GDI+
  GDIPlusTexture* createGDISurface( int width, int height ) ;
  void addSpriteFromGDIPlusTexture( int id, GDIPlusTexture* tex ) ;
  
public:
  bool beginDrawing() ; // Call once per frame before drawing.
  // Returns TRUE when beginDrawing succeeded.
  // Don't try and draw if this function fails (it means the D3D device
  // is temporarily "lost" (i.e. some other app has control of it))
  void endDrawing() ;   // "flips the buffer" over so the user can see
  // what we just drew this frame

  void setBackgroundColor( D3DCOLOR color ) ;
  
  void drawMouseCursor( int id ) ;
  void drawFrameCounter() ;
  
  void drawBox( D3DCOLOR color, int x, int y, int width, int height ) ;
  void drawBox( D3DCOLOR color, RECT &r ) ;
  void drawBoxCentered( D3DCOLOR color, int xCenter, int yCenter, int width, int height ) ;
  void getBoxDimensions( char *str, RECT &r ) ;

  void drawString( char *str, D3DCOLOR color, RECT &r ) ;
  void drawString( char *str, D3DCOLOR color, RECT &r, DWORD formatOptions ) ;

  void drawString( char *str, D3DCOLOR color, float x, float y, float width, float height ) ;
  void drawString( char *str, D3DCOLOR color, float x, float y, float width, float height, DWORD formatOptions ) ;

  // Draws a sprite centered @ (x,y)
  void drawSprite( int id, float x, float y ) ;

  // draws a sprite centered @ (x,y) modulated by color specified
  // You can use this fucntion to make your sprite appear "ghostly"
  // (by using a translucent whitish color like ARGB(128,255,255,255))
  void drawSprite( int id, float x, float y, D3DCOLOR modulatingColor ) ;

  // draws a sprite centered @ (x,y) of width and height specified
  void drawSprite( int id, float x, float y, float width, float height ) ;

  // draws a sprite centered @ (x,y) of width and height specified, using modulating color
  void drawSprite( int id, float x, float y, float width, float height, float angle ) ;

  void drawSprite( int id, float x, float y, float width, float height, float angle, D3DCOLOR modulatingColor ) ;

  

  /* Creates boxed text as a sprite
  Uses the default font (Arial 18) to draw the text
  in the textColor you specify */
  void boxedTextSprite( int spriteId, char *str, D3DCOLOR textColor ) ;

  /* Puts text against a square
  fitted background. */
  void boxedTextSprite( int spriteId, char *str, D3DCOLOR textColor, D3DCOLOR backgroundColor ) ;

  /* The "padding" parameter is how much space to pad out around the sides. */
  void boxedTextSprite( int spriteId, char *str, D3DCOLOR textColor, D3DCOLOR backgroundColor, int padding ) ;

  /* the RECT for padding specifies left, top, right, bottom padding. */
  void boxedTextSprite( int spriteId, char *str, D3DCOLOR textColor, D3DCOLOR backgroundColor, RECT padding ) ;
  
  /* Boxed text has fontName, size, and boldness you specify.
  Boldness:  Should a number between 0 and 1000.  400 is considered "normal", while 700 is considered "bold". */
  void boxedTextSprite( int spriteId, char *str, D3DCOLOR textColor, D3DCOLOR backgroundColor, RECT padding, char *fontName, float size, int boldness, bool italics ) ;

private:
  void boxedTextSprite( int spriteId, char *str, D3DCOLOR textColor, D3DCOLOR backgroundColor, RECT padding, ID3DXFont *font ) ;

public:
  void addSprite( int id, Sprite *sprite ) ;

  void loadSprite( int id, char *filename,
    D3DCOLOR backgroundColor = D3DCOLOR_ARGB( 0,0,0,0 ),
    int singleSpriteWidth = SPRITE_READ_FROM_FILE,
    int singleSpriteHeight = SPRITE_READ_FROM_FILE,
    int numFrames = SPRITE_READ_FROM_FILE,
    float timeBetweenFrames = 0.5f ) ;



  // Gets you a random sprite id from the map.
  // Useful for testing, not much else
  int randomSpriteId() ;
  int randomSpriteId( int below ) ;

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

  /// Loads a sound, .wav, .mp3, etc.,
  /// and assigns an ID to it.
  /// Note if you want to be able to
  /// LOOP the sound you have to use either
  /// FMOD_SOFTWARE or FMOD_CREATESTREAM
  /// as the options value.  Use FMOD_SOFTWARE
  /// for short, looping sounds (like a 2 second
  /// torch/fire crackle) and FMOD_CREATESTREAM
  /// for longer sounds (like a 5 minute background
  /// music song).
  void loadSound( int id, char * filename, int options = 0 ) ;

  /// Plays a previously loaded sound by
  /// the same ID you passed to loadSound
  void playSound( int id ) ;

  /// Stops playback of ALL sounds
  /// by id.
  /// Really most useful for stopping
  /// one kind of background music
  /// and starting another.
  void stopSound( int id ) ;

  /// Loop a sound `loopCount` times,
  /// set `loopCount` to FMOD_LOOP_FOREVER
  /// to loop a sound infinitely
  void loopSound( int id, int loopCount = FMOD_LOOP_FOREVER ) ;

  /// Channel callback
  static FMOD_RESULT F_CALLBACK channelEndCallback(
    FMOD_CHANNEL* channel,
    FMOD_CHANNEL_CALLBACKTYPE type,
    void* data1, void* data2 )
  {
    if( type == FMOD_CHANNEL_CALLBACKTYPE_END )
    {
      //info( "Sound %p has finished playing", channel ) ;

      // Now we need to remove the multimap entry
      // where this FMOD_CHANNEL* is the __value__

      ChannelMultimapIter channelIter = fmodChannels.begin() ;
      while( channelIter != fmodChannels.end() )
      {
        if( channelIter->second == channel )
        {
          // Delete it from the multimap,
          // as it has finished playing.

          // !! Bug::"map/set iterator not incrementable"
          // These are cleaned up in Window::stopSound anyway..
          // but this should be fixed.
          //fmodChannels.erase( channelIter ) ;
          
          return FMOD_OK ;
        }

        channelIter++;
      }
    }


    warning( "I could not find sound in channel '%p' "
      "in the fmodChannels multimap.  Have you cleared "
      "the multimap?", channel ) ;
    
    return FMOD_OK ;

  }


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

private:

public:
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
  bool keyJustPressed( int vkCode ) ;

  // Tells you if a key is BEING HELD DOWN
  bool keyIsPressed( int vkCode ) ;

  // Returns true if a key was JUST let go of.
  // A complimentary function to justPressed()
  bool keyJustReleased( int vkCode ) ;
  
  // Mouse.
  void mouseUpdateInput( RAWINPUT *raw ) ;
  bool mouseJustPressed( Mouse::Button button ) ;
  bool mouseIsPressed( Mouse::Button button ) ;
  bool mouseJustReleased( Mouse::Button button ) ;


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