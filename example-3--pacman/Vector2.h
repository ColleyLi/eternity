#ifndef VECTOR2_H
#define VECTOR2_H

#include <math.h>
#include "pacman-main.h"

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

class Vector2
{
public:
  float x,y;
  Vector2() ;
  Vector2(float ix, float iy) ;
  Vector2(const Vector2& other) ;

  // Almost equal
  bool near( const Vector2& other ) ;

  float mag() ;

  float magSquared() ;

  /// Makes this vector
  /// a unit vector, while
  /// retaining its direction
  Vector2& normalize() ;

  Vector2& setMag( float newMagnitude ) ;

  Vector2& print() ;
} ;

// Operator overloading outside
bool operator==(const Vector2& a, const Vector2& b ) ;

Vector2 operator+(const Vector2& a, const Vector2& b) ;
Vector2 operator-(const Vector2& a, const Vector2& b) ;

Vector2 operator*(const Vector2& a, float c) ;

// and support for (scalar*vector) [2.5f*v1]
Vector2 operator*(float c, const Vector2& a) ;

float dot(const Vector2& a, const Vector2& b) ;

Vector2& operator+=( Vector2& a, Vector2& b ) ;
Vector2& operator*=( Vector2& a, float c ) ;
Vector2& operator/=( Vector2& a, float c ) ;

#endif  // VECTOR2_H