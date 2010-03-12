#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

// Let's define a few states..
enum GameObjectState
{
  MovingLeft,
  MovingRight,
  MovingUp,
  MovingDown
} ;

// A GameObject is a movable,
// intersectable entity..
class GameObject
{
public:
  float x,y ; // x, y position.
  // Using public members

  // Retain 4 id's:  One for
  // moving left, one for moving
  // right, etc.
  int spriteIdMovingLeft,
      spriteIdMovingRight,
      spriteIdMovingUp,
      spriteIdMovingDown;

  // The state of the object is
  // described by this variable.
  // Everything here has "modes"..
  GameObjectState state ;

  // Depending on his state..
  void step( float timeElapsed )
  {
    float stepSize = 0.01f ;
    switch( state )
    {
    case GameObjectState::MovingLeft:
      x -= stepSize ;
      break;
    case GameObjectState::MovingRight:
      x += stepSize ;
      break;
    case GameObjectState::MovingUp:
      // y _decreases_ going up
      y -= stepSize ;
      break;
    case GameObjectState::MovingDown:
      y += stepSize ;
      break;

    }
  }

  // Depending on his state..
  void draw()
  {
    switch( state )
    {
    case GameObjectState::MovingLeft:
      window->drawSprite( spriteIdMovingLeft, x, y ) ;
      break;
    case GameObjectState::MovingRight:
      window->drawSprite( spriteIdMovingRight, x, y ) ;
      break;
    case GameObjectState::MovingUp:
      window->drawSprite( spriteIdMovingUp, x, y ) ;
      break;
    case GameObjectState::MovingDown:
      window->drawSprite( spriteIdMovingDown, x, y ) ;
      break;

    }
  }
} ;

#endif