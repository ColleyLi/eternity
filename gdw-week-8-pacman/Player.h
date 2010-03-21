#ifndef PLAYER_H
#define PLAYER_H

#include "pacman-main.h"

#include "FourDirectionMovingObject.h"
//#include "GameWorld.h"

// contains player state data
class Player : public FourDirectionMovingObject
{
  #pragma region Pacman stats
  // Firing is _not_ going to be considered
  // as a distinct "state" or mode of operation,
  // because the guy can be firing going any direction
  bool firing ;

  // Timeout to wait (in seconds) before being
  // allowed to fire again.  Timeout varies
  // between weapons fired
  float firingTimeout ;
  int health ;
  int lives ;
  
  // Weapons he has, and ammo for them
  bool hasHandgun, hasFlamethrower, hasUzi ;
  int ammoHandgun, ammoFlamethrower, ammoUzi ;
  #pragma endregion

public:
  Player() ;

  int getLives() ;

  /// "Override" the step function in GameObject
  virtual void step( float time ) override ;

  /*
  virtual void draw() override
  {
    switch( motionState )
    {
    case MotionState::MovingRight:
      // The sprite is 
      break ;
    }
  }
  */
} ;

#endif