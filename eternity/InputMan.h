#ifndef INPUTMAN_H
#define INPUTMAN_H

#include <windows.h>
#include "helperFunctions.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Gamepad.h"

/*
 _                   _   
(_)_ __  _ __  _   _| |_ 
| | '_ \| '_ \| | | | __|
| | | | | |_) | |_| | |_ 
|_|_| |_| .__/ \__,_|\__|
        |_|              
*/

class InputMan
{
private:
  Keyboard keyboard ;  /*!< manages keyboard state */
  Mouse mouse ;        /*!< manages mouse state */
  Gamepad gamepad ;    /*!< manages up to 4 gamepad states */

protected:
  
  /** Fire up the input manager */
  void initInputMan( HWND hwnd, int windowWidth, int windowHeight );

  /** Update all the input devices */
  void inputManStep() ;

public:
  /** Gets you the mouse's x location */
  int getMouseX() ;

  /** Gets you the mouse's current y location */
  int getMouseY() ;

  /// Tells you if "key" was JUST PRESSED DOWN.
  /// 
  /// JUST PRESSED DOWN means the key was
  /// UP in previous frame, but is DOWN in
  /// THIS frame.
  /// 
  /// If you press and hold a key, for no matter
  /// how many seconds you hold it down for,
  /// you'll only have JustPressed return true ONCE.
  /// @param vkCode The virtual keycode.
  bool keyJustPressed( int vkCode ) ;

  /// Tells you if a key is BEING HELD DOWN
  /// @param vkCode The virtual keycode (starts
  /// with VK_*, listing at 
  /// http://msdn.microsoft.com/en-us/library/ms927178.aspx).
  /// Don't use the VK_Mousebutton ones, use
  /// mouseJustPressed for that.
  bool keyIsPressed( int vkCode ) ;

  /// Returns true if a key was JUST let go of.
  /// A complimentary function to justPressed()
  bool keyJustReleased( int vkCode ) ;
  
  /// Send the mouse
  /// updated raw input (sent from
  /// a Windows message)
  /// @param raw The RAWINPUT* pointer from Windows
  void mouseUpdateInput( RAWINPUT *raw ) ;

  /** The mouse was just clicked (once) */
  bool mouseJustPressed( Mouse::Button button ) ;

  /** The mouse button is being held down */
  bool mouseIsPressed( Mouse::Button button ) ;

  /** Mouse-up:  The mouse button was just released */
  bool mouseJustReleased( Mouse::Button button ) ;

  /** Stop the mouse from going out of
      clipZone's bounds.
      @param clipZone A rectangle that
      describes how far left, right, up and down
      the mouse can go in pixels */
  void inputManSetClipZone( RECT & clipZone ) ;

  /** Gamepad button just pressed once */
  bool gamepadJustPressed( Gamepad::PlayerIndex playerIndex, Gamepad::Button button ) ;
  
  /** Gamepad button is being held down */
  bool gamepadIsPressed( Gamepad::PlayerIndex playerIndex, Gamepad::Button button ) ;
  
  /** Gamepad button just let go of */
  bool gamepadJustReleased( Gamepad::PlayerIndex playerIndex, Gamepad::Button button ) ;

  /** Gives you the x-position of the left thumb
      joystick on the gamepad you want.
      @return Value between -1 (extreme left) and +1 (extreme right)
      */
  float gamepadThumbLeftX( Gamepad::PlayerIndex playerIndex ) ;

  /** Gives you the y-position of the left thumb
      joystick on the gamepad you want.
      @return Value between -1 (extreme down) and +1 (extreme up)
      */
  float gamepadThumbLeftY( Gamepad::PlayerIndex playerIndex ) ;

  /** Gives you the x-position of the right thumb
      joystick on the gamepad you want.
      @return Value between -1 (extreme left) and +1 (extreme right)
      */
  float gamepadThumbRightX( Gamepad::PlayerIndex playerIndex ) ;

  /** Gives you the y-position of the right thumb
      joystick on the gamepad you want.
      @return Between -1=(extreme down), +1=(extreme up)
      */
  float gamepadThumbRightY( Gamepad::PlayerIndex playerIndex ) ;

  /** Gives you the left shoulder trigger
      value.
      @return Between 0=(completely released), +1=(completely pressed in)
      */
  float gamepadTriggerLeft( Gamepad::PlayerIndex playerIndex ) ;

  /** Gives you the right shoulder trigger
      value.
      @return Between 0=(completely released), +1=(completely pressed in)
      */
  float gamepadTriggerRight( Gamepad::PlayerIndex playerIndex ) ;

  /// Vibrates a controller.
  /// The gamepad will continue to vibrate
  /// until you call stopVibrate()
  /// @param leftRumble The left side
  /// rumbles it.  The left side will
  /// cause it to walk across the table or something.
  /// 0.0f = (no rumble), 1.0f = (max rumble).
  /// @param rightSide The right side of the game pad "buzzes"
  /// the controller (vibrates "softly")
  /// 0.0f = (no rumble), 1.0f = (max rumble).
  void gamepadVibrate( Gamepad::PlayerIndex playerIndex, float leftRumble, float rightBuzz ) ;

  /** Stops a gamepad from vibrating. */
  void gamepadStopVibrate( Gamepad::PlayerIndex playerIndex ) ;

} ;

#endif