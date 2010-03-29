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

void GameObject::intersects( Tile *tile ) 
{
  // default is to do nothing
}

void GameObject::intersects( GameObject *other ) 
{
  // default is to do nothing
}
