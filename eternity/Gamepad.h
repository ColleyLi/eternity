#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <XInput.h>
#include <limits.h>
#include "helperFunctions.h"

#pragma comment( lib, "xinput.lib" )






// xbox 360 gamepad controller
class Gamepad
{
  XINPUT_STATE gpPreviousState[ XUSER_MAX_COUNT ] ;
  XINPUT_STATE gpCurrentState[ XUSER_MAX_COUNT ] ;

public:
  enum PlayerIndex
  {
    One = 0,
    Two = 1,
    Three = 2,
    Four = 3
  } ;

  enum Button
  {
    DPad_Up    = XINPUT_GAMEPAD_DPAD_UP,
    DPad_Down  = XINPUT_GAMEPAD_DPAD_DOWN,
    DPad_Left  = XINPUT_GAMEPAD_DPAD_LEFT,
    DPad_Right = XINPUT_GAMEPAD_DPAD_RIGHT,
    Start      = XINPUT_GAMEPAD_START,
    Back       = XINPUT_GAMEPAD_BACK,
    ThumbLeft  = XINPUT_GAMEPAD_LEFT_THUMB,
    ThumbRight = XINPUT_GAMEPAD_RIGHT_THUMB,
    ShoulderLeft = XINPUT_GAMEPAD_LEFT_SHOULDER,
    ShoulderRight = XINPUT_GAMEPAD_RIGHT_SHOULDER,
    A = XINPUT_GAMEPAD_A,
    B = XINPUT_GAMEPAD_B,
    X = XINPUT_GAMEPAD_X,
    Y = XINPUT_GAMEPAD_Y             
  } ;

  PlayerIndex checkPlayerIndex( PlayerIndex playerIndex )
  {
    if( playerIndex < 0 || playerIndex > XUSER_MAX_COUNT )
    {
      warning( "XInput gamepad: %d invalid player index, using index 0 instead", playerIndex ) ;
      playerIndex = One ;
    }

    return playerIndex ;
  }
  
  // pressed now
  bool isPressed( PlayerIndex playerIndex, int vkCode )
  {
    playerIndex = checkPlayerIndex(playerIndex) ;

    return gpCurrentState[ playerIndex ].Gamepad.wButtons & vkCode ;
  }

  // Pressed now, but up previous frame
  bool justPressed( PlayerIndex playerIndex, int vkCode )
  {
    playerIndex = checkPlayerIndex(playerIndex) ;

    return (gpCurrentState[ playerIndex ].Gamepad.wButtons & vkCode)&&
          !(gpPreviousState[ playerIndex ].Gamepad.wButtons & vkCode) ;
  }

  // Released now, but down previous frame
  bool justReleased( PlayerIndex playerIndex, int vkCode )
  {
    playerIndex = checkPlayerIndex(playerIndex) ;

    return !(gpCurrentState[ playerIndex ].Gamepad.wButtons & vkCode) &&
            (gpPreviousState[ playerIndex ].Gamepad.wButtons & vkCode) ;
  }

  float leftThumbX( PlayerIndex playerIndex )
  {
    playerIndex = checkPlayerIndex(playerIndex) ;
    short lx = gpCurrentState[ playerIndex ].Gamepad.sThumbLX ;
    if( lx < 0 )
      return -(float)lx / SHRT_MIN ;
    else
      return (float)lx / SHRT_MAX ;
  }

  float leftThumbY( PlayerIndex playerIndex )
  {
    playerIndex = checkPlayerIndex(playerIndex) ;
    short ly = gpCurrentState[ playerIndex ].Gamepad.sThumbLY ;
    if( ly < 0 )
      return -(float)ly / SHRT_MIN ;
    else
      return (float)ly / SHRT_MAX ;
  }

  float rightThumbX( PlayerIndex playerIndex )
  {
    playerIndex = checkPlayerIndex(playerIndex) ;
    short rx = gpCurrentState[ playerIndex ].Gamepad.sThumbRX ;
    if( rx < 0 )
      return -(float)rx / SHRT_MIN ;
    else
      return (float)rx / SHRT_MAX ;
  }

  float rightThumbY( PlayerIndex playerIndex )
  {
    playerIndex = checkPlayerIndex(playerIndex) ;
    short ry = gpCurrentState[ playerIndex ].Gamepad.sThumbRY ;
    if( ry < 0 )
      return -(float)ry / SHRT_MIN ;
    else
      return (float)ry / SHRT_MAX ;
  }

  float leftTrigger( PlayerIndex playerIndex )
  {
    playerIndex = checkPlayerIndex(playerIndex) ;
    unsigned char lt = gpCurrentState[ playerIndex ].Gamepad.bLeftTrigger ;
    return (float)lt/255.f ;
  }

  float rightTrigger( PlayerIndex playerIndex )
  {
    playerIndex = checkPlayerIndex(playerIndex) ;
    unsigned char rt = gpCurrentState[ playerIndex ].Gamepad.bRightTrigger ;
    return (float)rt/255.f ;
  }

  // The right side of the game pad "buzzes"
  // the controller (vibrates "softly"),
  // while the left side
  // rumbles it.  The left side will
  // cause it to walk across the table or something.
  // Vibration values can be between 0.0f (no rumble)
  // and 1.0f (max rumble).
  // The gamepad will continue to vibrate
  // until you call stopVibrate()
  void setVibrate( PlayerIndex playerIndex, float leftRumble, float rightBuzz )
  {
    playerIndex = checkPlayerIndex(playerIndex) ;

    XINPUT_VIBRATION vibration ;

    clamp( leftRumble, 0.0f, 1.0f ) ;
    clamp( rightBuzz, 0.0f, 1.0f ) ;

    // Convert float values to being between
    // 0 and 65536.
    vibration.wLeftMotorSpeed = leftRumble * USHRT_MAX ;
    vibration.wRightMotorSpeed = rightBuzz * USHRT_MAX ;

    int errCode = XINPUT_Check( XInputSetState( playerIndex, &vibration ) ) ;
    if( errCode == ERROR_NOT_CONNECTED )
    {
      warning( "XBox360 gamepad controller %d not connected", playerIndex ) ;
    }
  }

  // Stops the controller from vibrating.
  void stopVibrate( PlayerIndex playerIndex )
  {
    playerIndex = checkPlayerIndex(playerIndex) ;

    XINPUT_VIBRATION vibration = { 0 } ;
    int errCode = XINPUT_Check( XInputSetState( playerIndex, &vibration ) ) ;

    if( errCode == ERROR_NOT_CONNECTED )
    {
      warning( "XBox360 gamepad controller %d not connected", playerIndex ) ;
    }
  }

  void step()
  {
    for( int i = 0 ; i < XUSER_MAX_COUNT ; i++ )
    {
      // Save off current state as new previous state..
      gpPreviousState[ i ] = gpCurrentState[ i ] ;
      
      // snatch in the new state
      XInputGetState( i, &gpCurrentState[ i ] ) ;
      // If the controller isn't connected,
      // this should just fail silently (hence
      // why we don't error check it).
    }
  }

  // There is a XInputGetKeystroke function 
  // that also gives you input events
  // but we haven't used it here

} ;
#endif