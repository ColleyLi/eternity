#include "Vector.h"

/// Defines a line for drawing
struct Line
{
  Vector start, end ;
  D3DCOLOR color ;

  float thickness ;

  Line()
  {
    start = end = Vector() ;
    color = 0 ;
    thickness = 1;
  }

  Line( Vector iStart, Vector iEnd, D3DCOLOR iColor, float iThickness )
  {
    start = iStart ;
    end = iEnd ;
    color = iColor ;
    thickness = iThickness ;

    // just waiting until there's variables pod and pad..
  }
} ;
