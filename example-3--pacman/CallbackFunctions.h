#ifndef CALLBACK_FUNCTIONS_H
#define CALLBACK_FUNCTIONS_H

#include "pacman-main.h"
#include "GameWorld.h"

// A file full of c-style callback functions
// to be used with window->addCallback()

// We have to use C-Style functions
// because the eternity callback
// mechanism doesn't support object
// member functions as callbacks.

void transitionToState( GameWorld::GameState newState ) ;

#endif