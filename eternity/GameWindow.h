#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "helperFunctions.h"
#include "D3DWindow.h"
#include "SoundMan.h"
#include "SpriteMan.h"
#include "InputMan.h"

/*
  __ _  __ _ _ __ ___   ___ 
 / _` |/ _` | '_ ` _ \ / _ \
| (_| | (_| | | | | | |  __/
 \__, |\__,_|_| |_| |_|\___|
 |___/                      

*/

class GameWindow : public D3DWindow, public SoundMan, public SpriteMan, public InputMan
{
protected:
  // Game
  bool paused ;  // game paused or unpaused.
  Timer timer ;  // frame limiter

public:
  GameWindow( HINSTANCE hInst, TCHAR* windowTitleBar, int windowXPos, int windowYPos, int windowWidth, int windowHeight ) ;
  ~GameWindow() ;

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



  // Draws the mouse cursor with the sprite you pass
  // If you say "showCursorCoordinates=true" then
  // it will print the cursor coordinate values
  // at the bottom right corner of the mouse
  void drawMouseCursor( int spriteId, bool showCursorCoordinates=false ) ;
  void drawFrameCounter() ;
  




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



  virtual bool setSize( int width, int height, bool fullScreen ) override ;

} ;
#endif