#include "GameObject.h"

GameObject::GameObject()
{

}

void GameObject::step( float time )
{
  // no default behavior for step
}

void GameObject::draw()
{
  window->drawSprite( spriteId, pos.x, pos.y ) ;
}