#ifndef GHOST_H
#define GHOST_H

#include "pacman-main.h"

#include "FourDirectionMovingObject.h"

class Ghost : public FourDirectionMovingObject
{
public:
  // (i)nky, (b)linky, (p)inky, (s)ue,
  // as named in lvl1.txt
  char name ;
  
  
} ;

#endif