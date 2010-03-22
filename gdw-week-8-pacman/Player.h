#ifndef PLAYER_H
#define PLAYER_H

#include "pacman-main.h"

#include "FourDirectionMovingObject.h"
#include "Ghost.h"
#include "EnumDefinitions.h"

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
  int score ;
  
  // Weapons he has, and ammo for them
  bool hasHandgun, hasFlamethrower, hasUzi ;
  int ammoHandgun, ammoFlamethrower, ammoUzi ;
  #pragma endregion

public:
  Player() ;

  int getLives() ;
  int getScore() ;

  /// "Override" the step function in GameObject
  virtual void step( float time ) override ;

  /// Pacman is intersected by a tile
  virtual void intersects( Tile *tile ) override ;

  /// What to do when Pacman
  /// is intersected by another GameObject
  virtual void intersects( GameObject *other ) override ;

  /// What to do when Pacman
  /// is intersected by another FourDirectionMovingObject
  virtual void intersects( FourDirectionMovingObject *other ) override ;

  /// What to do when Pacman
  /// is intersected by a Ghost
  virtual void intersects( Ghost *ghost ) ;

} ;

#endif