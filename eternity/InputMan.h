#ifndef INPUTMAN_H
#define INPUTMAN_H

#include <windows.h>
#include "helperFunctions.h"
#include "Mouse.h"
#include "Keyboard.h"

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
  Keyboard keyboard ;
  Mouse mouse ;

protected:
  void initInputMan( HWND hwnd, int windowWidth, int windowHeight );
  void inputManStep() ;

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

  void inputManSetClipZone( RECT & clipZone ) ;
} ;

#endif