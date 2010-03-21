#include "Vector2.h"

#ifdef near
#undef near
#endif  // near

// Conflict between WinDef.h archaic
// "near" and our Vector2.near()
// If two vectors v1 and v2
// are within EPS of each other
// in both x and y,
// then v1.near(v2)
// is true
#define EPS 0.0001f

Vector2::Vector2():x(0.f),y(0.f){}
Vector2::Vector2(float ix, float iy):x(ix),y(iy){}
Vector2::Vector2(const Vector2& other){
  x=(other.x),y=(other.y);
}

// Almost equal
bool Vector2::near( const Vector2& other )
{
  if( fabs( x - other.x ) < EPS &&
      fabs( y - other.y ) < EPS )
    return true ;
  else
    return false ;
}

float Vector2::mag()
{
  return sqrtf( x*x + y*y ) ;
}

float Vector2::magSquared()
{
  return x*x+y*y ;
}

/// Makes this vector
/// a unit vector, while
/// retaining its direction
Vector2& Vector2::normalize()
{
  float m = mag() ;
  x /= m ;
  y /= m ;
  return *this ;
}

Vector2& Vector2::setMag( float newMagnitude )
{
  float m = newMagnitude / mag() ;
  x *= m ;
  y *= m ;
  return *this ;
}

Vector2& Vector2::print()
{
  info( "(%.2f, %.2f)\n", x,y ) ;
  return *this ;
}

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

Vector2& operator+=( Vector2& a, Vector2& b )
{
  a.x += b.x ;
  a.y += b.y ;

  return a ;
}

Vector2& operator*=( Vector2& a, float c )
{
  a.x *= c ;
  a.y *= c ;

  return a ;
}

Vector2& operator/=( Vector2& a, float c )
{
  // Instead of dividing twice,
  // we'll just do it once here
  c = 1.0f / c ;
  
  a.x *= c ;
  a.y *= c ;

  return a ;
}
