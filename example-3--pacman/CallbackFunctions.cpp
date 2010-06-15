#include "CallbackFunctions.h"

void transitionToState( GameWorld::GameState newState )
{
  game->setState( newState ) ;
}
