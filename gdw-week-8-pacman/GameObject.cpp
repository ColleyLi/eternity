#include "GameObject.h"

GameObject::GameObject()
{
  
}

void GameObject::setTimePerFrame( float time )
{
  window->getSprite( this->spriteId )->setTimePerFrame( time ) ;
}

float GameObject::getTimePerFrame()
{
  return window->getSprite( this->spriteId )->getTimePerFrame() ;
}

void GameObject::step( float time )
{
  // no default behavior for step
}

void GameObject::draw()
{
  window->drawSprite( spriteId, pos.x, pos.y ) ;
}

bool GameObject::isIntersectingWith( GameObject *other )
{
  // compute length of Vector between centers
  Vector2 vecBetween = pos - other->pos ;

  float len = vecBetween.mag() ;

  // if the length of this vector is
  // less than the sum of their radii
  // then they intersect
  return len < ( radiusBounding + other->radiusBounding ) ;

  // An alternate (slightly more efficient)
  // way (not done here, for simplicity)
  // to compute this is to use
  // vecBetween.magSquared() and compare that
  // against the square of their radii,
  // because the square root computation is slow.
}

void GameObject::doIntersect( Tile *tile ) 
{
  // default is to do nothing
}

void GameObject::doIntersect( GameObject *other ) 
{
  // default is to do nothing
}
