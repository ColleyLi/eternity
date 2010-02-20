#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

// A very primitive structure
// for a basic game object
class GameObject
{
public:
  float x, y ;
  float vx, vy ;
  float ax, ay ;
  float friction ;

  int spriteIndex ;

  GameObject()
  {
    x = y = vx = vy = ax = ay = 0.0f ;
    friction = 0.33f ;
    spriteIndex = 0 ;
  }

  void step( float t )
  {
    x += vx*t ;
    y += vy*t ;

    vx += ax*t ;
    vy += ay*t ;

    // wear down acceleration by friction
    ax *= friction ;
    ay *= friction ;
  }

  void wrap( float xmin, float ymin, float xmax, float ymax )
  {
    if( x < xmin ) x = xmin ;
    else if( x > xmax ) x = xmax ;
    if( y < ymin ) y = ymin ;
    else if( y > ymax ) y = ymax ;
  }


} ;

#endif // GAME_OBJECT_H