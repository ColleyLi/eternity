#include "Vector.h"

int Vector::print( char *buf ) 
{
  return sprintf( buf, "(%.2f %.2f %.2f)", x,y,z ) ;
}

#pragma region outside operators
Vector operator+( const Vector & a, const Vector & b )
{
  return Vector( a.x + b.x, a.y + b.y, a.z + b.z ) ;
}

Vector operator-( const Vector & a, const Vector & b )
{
  return Vector( a.x - b.x, a.y - b.y, a.z - b.z ) ;
}

Vector operator-( const Vector & a )
{
  return Vector( -a.x, -a.y, -a.z ) ;
}

Vector operator*( const Vector & a, const Vector & b )
{
  // cross product
  return Vector(
    a.y*b.z - b.y*a.z,
    b.x*a.z - a.x*b.z,
    a.x*b.y - b.x*a.y
  ) ;
}

Vector operator*( const Vector & a, const real b )
{
  return Vector( a.x*b, a.y*b, a.z*b ) ;
}

Vector operator*( const real b, const Vector & a )
{
  return Vector( a.x*b, a.y*b, a.z*b ) ;
}

/// Thing is, we're going to treat it as
/// (Vector, 1)
Vector operator*( const Matrix & a, const Vector& v )
{
  Vector result(

    a._11*v.x + a._12*v.y + a._13*v.z + a._14,
    a._21*v.x + a._22*v.y + a._23*v.z + a._24,
    a._31*v.x + a._32*v.y + a._33*v.z + a._34

  ) ;

  real w = a._41*v.x + a._42*v.y + a._43*v.z + a._44 ;
  result /= w ;

  return result ;
}

Vector operator/( const Vector & a, const real b )
{
  return Vector( a.x/b, a.y/b, a.z/b ) ;
}

#pragma endregion
