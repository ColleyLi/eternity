#include "Player.h"
#include "GameWorld.h"

Player::Player()
{
  // init health and lives
  health = 100 ;
  lives  = 3 ;
  score = 0 ;

  speed = 2.0f ;
  
  // start him going right
  requestedMotionState = motionState = MotionState::MovingRight ;

  // init with no weapons
  hasHandgun = hasFlamethrower = hasUzi = false ;
  ammoHandgun = ammoFlamethrower = ammoUzi = 0 ;

  // Position and velocity are initialized
  // in the level-load code (because his position
  // is the placement of the 'P' in lvl1.txt)
}

Player::~Player()
{

}

int Player::getLives()
{
  return lives ;
}

int Player::getHealth()
{
  return health ;
}

int Player::getScore()
{
  return score ;
}

void Player::step( float time ) // override
{
  // Update the direction he's facing
  // based on what key was just pressed
  if( window->keyIsPressed( VK_RIGHT ) ||
      window->gamepadIsPressed( Gamepad::One, Gamepad::DPadRight ) )
  {
    reqMotionState( MotionState::MovingRight ) ;
  }
  if( window->keyIsPressed( VK_LEFT ) ||
      window->gamepadIsPressed( Gamepad::One, Gamepad::DPadLeft ) )
  {
    reqMotionState( MotionState::MovingLeft ) ;
  }
  if( window->keyIsPressed( VK_UP ) ||
      window->gamepadIsPressed( Gamepad::One, Gamepad::DPadUp ) )
  {
    reqMotionState( MotionState::MovingUp ) ;
  }
  if( window->keyIsPressed( VK_DOWN ) ||
      window->gamepadIsPressed( Gamepad::One, Gamepad::DPadDown ) )
  {
    reqMotionState( MotionState::MovingDown ) ;
  }


  // Intersect Pacman with the tile he's on
  doIntersect( game->getTileAt( pos ) ) ;

  // Call the FourDirectionMovingObject
  // base class function to ACTUALLY MOVE
  // this object (so all object moving
  // using the same BASE CODE!)
  FourDirectionMovingObject::step( time ) ;

  
}

/// Pacman is intersected by a tile
void Player::doIntersect( Tile *tile )
{
  switch( tile->getTile() ) 
  {
  case Tiles::Pellet:

    // Pick up pellet:
    // reset tile to being a
    // blank tile now
    tile->setTile( ' ' ) ;
    tile->setSpriteId( Sprites::Empty ) ;

    window->playSound( Sounds::PacmanPellet1 ) ;

    // increase player score
    score += 10 ;
    break ;

  case Tiles::PowerPellet:
    // reset tile to being a
    // blank tile now
    tile->setTile( ' ' ) ;
    tile->setSpriteId( Sprites::Empty ) ;

    window->playSound( Sounds::PacmanPellet2 ) ;

    // increase player score
    score += 30 ;
    break ;

  case Tiles::Bonus:
    // give pacman a score and speed boost
    speed = 4.0f ;
    score += 300 ;
    window->playSound( Sounds::PacmanPellet3 ) ;

    // erase it
    tile->setTile( ' ' ) ;
    tile->setSpriteId( Sprites::Empty ) ;
  }
}

/// What to do when Pacman
/// is intersected by another GameObject
void Player::doIntersect( GameObject *other )
{
  
}

/// What to do when Pacman
/// is intersected by another FourDirectionMovingObject
void Player::doIntersect( FourDirectionMovingObject *other )
{
}

/// What to do when Pacman
/// is intersected by a Ghost
void Player::doIntersect( Ghost *ghost )
{
  window->playSound( Sounds::PacmanHurt ) ;

  health--;
  clamp( health, 0, 100 ) ;
}

