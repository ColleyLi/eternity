#ifndef GAMEPAK_H
#define GAMEPAK_H

#include "helperFunctions.h"
#include "GDIPlusTexture.h"
#include "Timer.h"

#include "D3DWindow.h"
#include "SoundMan.h"
#include "SpriteMan.h"
#include "InputMan.h"
#include "Callback.h"
#include "UIMan.h"
#include "AssetMan.h"

class GamePak
{
protected:
  bool paused ;  /*!< game paused or unpaused. */
  Timer timer ;  /*!< used as timer and frame limiter */

  /// The vector full of callback functions
  /// to execute, sometime in the future.
  vector<Callback*> callbacks ;

public:
  #pragma region managers
  /// Very important for bootstrapping,
  /// loads images, sounds and other assets.
  AssetMan *assetMan ;

  /// The GamePak has access to a WINDOW.
  /// The WINDOW in turn has access to the
  /// system GPU.  GamePak communicates all
  /// desires to the GPU thru GraphicsWindow.
  GraphicsWindow *window ;

  /// Manages sound playing, etc
  SoundMan *soundMan ;

  /// Manages input from mouse, keyboard, and gamepad
  InputMan *inputMan ;

  /// Manages the UI layer
  UIMan *uiMan ;
  #pragma endregion

  /// Creates a GamePak in FULL SCREEN
  /// mode
  GamePak::GamePak( HINSTANCE hInst, TCHAR* windowTitleBar, GraphicsWindow::UnderlyingRenderingAPI api ) ;

  /// Creates a GamePak instance
  /// for you.
  GamePak( HINSTANCE hInst, TCHAR* windowTitleBar, 
    int windowXPos, int windowYPos, int windowWidth, int windowHeight, 
    GraphicsWindow::UnderlyingRenderingAPI api ) ;

  ~GamePak() ;

  void initManagers();

  /// "Steps" the game engine
  /// forward one step.  Must call
  /// this once per Update in your
  /// main game loop!
  void step() ;

  /// Pauses the
  /// game
  void pause() ;

  /// Unpauses
  /// the game
  void unpause() ;

  /// Tells you if the
  /// game is paused or not
  bool isPaused() ;

  /// Returns true if the game is having trouble
  /// keeping up at 60fps
  bool isSlow() ;

  /// Gives you the amount of time that passed since the last frame
  /// Keep in mind this value will "jitter" depending on how the
  /// game is currently running.. you have access to it, but you
  /// might just want to use a VIRTUAL_SECOND ("fake second") of
  /// fixed length (0.016666 seconds.. or 60 fps) instead.
  ///
  /// In the event of game slowdown, if you use real time,
  /// then things will start to jump around the screen,
  /// somewhat unpredictably.
  double getTimeElapsedSinceLastFrame() ;

  /// Gives you the total amount of time
  /// that has passed since the game started.
  /// Useful for setting up CALLBACKS that
  /// should execute in say, 5 seconds from NOW.
  double getTimeElapsedSinceGameStart() ;

  /// Draws the mouse cursor with the sprite you pass
  /// If you say "showCursorCoordinates=true" then
  /// it will print the cursor coordinate values
  /// at the bottom right corner of the mouse
  void drawMouseCursor( int spriteId, bool showCursorCoordinates=false ) ;

  /// Draws a framecounter in the topright corner
  /// of the screen.  Meant for diagnostic purposes
  /// only (so you can see your game framerate!)
  void drawFrameCounter() ;

  /// Draws the global sense of time
  /// that the game keeps track of
  void drawTimer() ;

  // The createFont function has to be HERE
  // so that it can call addFont() in spriteManager
  // and registerFont in D3DWindow.

  /// Makes a font for you.  Remember
  /// the integer ID you give the font!
  /// fontName:  Just the font name as it appears
  /// in C:\WINDOWS\Fonts.  You can use any font
  /// that you have installed on your system.
  /// Size:  The size of the font.
  /// boldness:  Typeface "WEIGHT".  Usually a number between 100 and 900,
  /// the higher the "bolder".  You can use the predefined
  /// FW_* constants, like FW_NORMAL=400, which means "normal" weight
  /// FW_BOLD=700, which means BOLD.  FW_THIN=100 is very thin.
  void createFont( int fontId, char *fontName, float size, int boldness, bool italics ) ;

  /// Register a callback
  /// to execute sometime in
  /// the future.  The "when"
  /// value you set inside your
  /// callback object will be
  /// "how many seconds into the future"
  /// the callback should be executed.
  void addCallback( Callback *callback ) ;

  /// Register a callback
  /// to execute at the timestamp
  /// you specify.  You really should
  /// get the current game time,
  /// every time you do this.
  /// Usually you'll want to just use addCallback().
  void addCallbackAbsolute( Callback *callback ) ;

private:
  /// Gets called by step() once
  /// per time around the gameloop
  /// will call all items in 'callbacks'
  /// whose execution time is up
  void runCallbacks() ;

} ;



/// Part of using the eternity engine means
/// there will be a single superglobal entity
/// called `window'.  window IS, for all intents
/// and purposes, THE ETERNITY ENGINE to you.
// We could have called this `eternity` or
// just simply `e`, but I think `window` is
// more intuitive because all this time you
// are talking about the GamePak.
extern GamePak * $ ;

#endif