#ifndef VECTOR2_H
#define VECTOR2_H
#include <math.h>

#ifdef near
#undef near
// Conflict between WinDef.h archaic
// "near" and our Vector2.near()
// If two vectors v1 and v2
// are within EPS of each other
// in both x and y,
// then v1.near(v2)
// is true
#define EPS 0.0001f

class Vector2
{
public:
  float x,y;
  Vector2():x(0.f),y(0.f){}
  Vector2(float ix, float iy):x(ix),y(iy){}
  Vector2(const Vector2& other){
    x=(other.x),y=(other.y);
  }

  // Almost equal
  bool near( const Vector2& other )
  {
    if( fabs( x - other.x ) < EPS &&
        fabs( y - other.y ) < EPS )
      return true ;
    else
      return false ;
  }

  float mag()
  {
    return sqrtf( x*x + y*y ) ;
  }

  float magSquared()
  {
    return x*x+y*y ;
  }

  void print()
  {
    printf( "(%.2f, %.2f)\n", x,y ) ;
  }
} ;

// Operator overloading outside
bool operator==(const Vector2& a, const Vector2& b )
{
  return a.x==b.x && a.y==b.y ;
}

Vector2 operator+(const Vector2& a, const Vector2& b)
{
  return Vector2( a.x+b.x, a.y+b.y ) ;
}

Vector2 operator-(const Vector2& a, const Vector2& b)
{
  return Vector2( a.x-b.x, a.y-b.y ) ;
}

Vector2 operator*(const Vector2& a, float c)
{
  return Vector2(a.x*c, a.y*c);
}

// and support for (scalar*vector) [2.5f*v1]
Vector2 operator*(float c, const Vector2& a)
{
  return Vector2(a.x*c, a.y*c);
}

float dot(const Vector2& a, const Vector2& b)
{
  return a.x*b.x + a.y*b.y ;
}

#endif
#endif