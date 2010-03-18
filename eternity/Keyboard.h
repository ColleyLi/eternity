#ifndef KEYBOARD_H
#define KEYBOARD_H

class Keyboard
{
public:
  BYTE keyPreviousStates[ 256 ] ; // holds which keys were down last frame
  BYTE keyCurrentStates[ 256 ] ; // holds which keys are down right now

  Keyboard()
  {
    // clear keystates
    memset( keyCurrentStates, 0, 256 ) ;
    memset( keyPreviousStates, 0, 256 ) ;
  }

  void step()
  {
    // Copy over "current" states to "previous" states
    //!! Should MOVE into Keyboard class
    memcpy( keyPreviousStates, keyCurrentStates, 256 ) ;

    // Grab all keystates, to know what the user is currently pushing down.
    if( !GetKeyboardState( keyCurrentStates ) )
    {
      printWindowsLastError( "GetKeyboardState()" ) ;
    }
  }
} ;


#endif