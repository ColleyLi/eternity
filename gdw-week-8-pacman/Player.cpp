#include "Player.h"

Player::Player()
{
  // init health and lives
  health = 100 ;
  lives  = 3 ;

  speed = 2.0f ;

  // init with no weapons
  hasHandgun = hasFlamethrower = hasUzi = false ;
  ammoHandgun = ammoFlamethrower = ammoUzi = 0 ;

  // Position and velocity are initialized
  // in the level-load code (because his position
  // is the placement of the 'P' in lvl1.txt)
}

int Player::getLives()
{
  return lives ;
}

void Player::step( float time ) // override
{
  // Update the direction he's facing
  // based on what key was just pressed
  if( window->keyJustPressed( VK_RIGHT ) ||
      window->gamepadJustPressed( Gamepad::One, Gamepad::DPadRight ) )
  {
    motionState = MotionState::MovingRight ;
  }
  if( window->keyJustPressed( VK_LEFT ) ||
      window->gamepadJustPressed( Gamepad::One, Gamepad::DPadLeft ) )
  {
    motionState = MotionState::MovingLeft ;
  }
  if( window->keyJustPressed( VK_UP ) ||
      window->gamepadJustPressed( Gamepad::One, Gamepad::DPadUp ) )
  {
    motionState = MotionState::MovingUp ;
  }
  if( window->keyJustPressed( VK_DOWN ) ||
      window->gamepadJustPressed( Gamepad::One, Gamepad::DPadDown ) )
  {
    motionState = MotionState::MovingDown ;
  }
 
  // Call the FourDirectionMovingObject
  // base class function to ACTUALLY MOVE
  // this object (so all object moving
  // using the same BASE CODE!)
  FourDirectionMovingObject::step( time ) ;

  
}


