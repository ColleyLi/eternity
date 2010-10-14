#ifndef VECTOR4_H
#define VECTOR4_H

#include <math.h>
#include <d3dx9.h>

#include "helperFunctions.h"
#include "Matrix.h"

// What it means to be "near"
const real EPS = 1e-3f ;

union Matrix ;


/// *** ON Vector.
/// Here I haven't included 'w' in
/// any of the cross product or anything
/// computations.  'w' is ONLY used
/// in perspective projection.  For
/// all other intents and purposes this
/// is a Vector3.. class.. hence why it is
/// named simply "Vector" ..

union Vector
{
  #pragma region members
  // UT
  real components[ 4 ] ;

  // UT
  struct { // msvc extension.  http://msdn.microsoft.com/en-us/library/z2cx9y4f.aspx
    real x,y,z,w ;
  } ;
  // see also http://stackoverflow.com/questions/2253878/why-does-c-disallow-unnamed-structs-and-unions
  // to get around on linux, try
  // #define x components[0]
  // #define y components[1]
  #pragma endregion

  #pragma region <CTOR>
  /// UT base ctor
  Vector():x(0.0f),y(0.0f),z(0.0f),w(0.0f) { }

  /// UT value ctor
  Vector( real ix, real iy, real iz ):x(ix),y(iy),z(iz),w(1.0f) { }

  /// UT copy ctor
  Vector( const Vector & b ):x(b.x),y(b.y),z(b.z),w(b.w) { }

  static Vector fromD3DVector( const D3DXVECTOR3 & vec ) {
    return Vector( vec.x, vec.y, vec.z ) ;
  }

  /// Gives you a vector that is spatially between
  /// the AABB formed by min, max.
  static Vector randomWithin( const Vector & min, const Vector & max ) {
    return Vector( randFloat( min.x, max.x ), 
      randFloat( min.y, max.y ), 
      randFloat( min.z, max.z )
    ) ;
  }

  static Vector random() {
    return Vector( randFloat(), randFloat(), randFloat() ) ;
  }
  #pragma endregion </CTOR>
  
  #pragma region <CONST>
  // UT
  // Well, I'm NOT including w in this comparison.
  inline bool operator==( const Vector & b ) const {
    return x==b.x && y==b.y && z==b.z ; // w not considered
  }

  // UT
  inline bool operator!=( const Vector & b ) const {
    return x!=b.x || y!=b.y || z!=b.z ; // w not considered
  }

  // UT
  inline bool Near( const Vector & b ) const {
    return fabs( x - b.x ) < EPS &&
           fabs( y - b.y ) < EPS &&
           fabs( z - b.z ) < EPS ;
  }

  // UT
  inline real Dot( const Vector & b ) const {
    return x*b.x + y*b.y + z*b.z ;
  }

  // UT
  inline real len() const {
    return sqrtf( x*x+y*y+z*z );  // w not considered
  }

  // UT
  inline real len2() const {
    return x*x + y*y + z*z ;  // w not considered
  }

  // for when using a vector as barycentric..
  // UT
  inline bool isNonnegative() const {
    return x >= 0 && y >= 0 && z >= 0 ;
  }

  // UT
  inline real angleWith( const Vector& b ) const {
    return acosf( (this->Dot( b ))/(len()*b.len()) ) ;
  }

  int print( char *buf ) ;

  // UT
  inline char getDominantAxis() const {
    if( fabs(x) > fabs(y) && fabs(x) > fabs(z) )  return 'x' ;
    else if( fabs(y) > fabs(x) && fabs(y) > fabs(z) )  return 'y' ;
    else if( fabs(z) > fabs(x) && fabs(z) > fabs(y) )  return 'z' ;
    else return 'n' ; // (none -- at least two axes equal.
    // this happens for (0,0,0), (1,1,0) etc.
  }


  // UT
  /// Gets you a perpendicular vector.  Tries to
  /// use the dominant axis so ( 0, 0, 1 ) will
  /// return ( -1, 0, 0 ) (a correct perpendicular)
  /// not ( -0, 0, 1 ) (which is useless, same vector)
  inline Vector getPerpendicular() const {
    // in the below switch, we
    // detect the dominant axis, and make sure
    // to use that component in the swap.
    // also we AVOID using a zero component for
    // the swap:  (-0.7, 0, 0.6) => (-0, -0.7, 0.6) is NOT a perpendicular
    // (but it would have been if the y component was non-zero)

    // (0,0,0) returns a vector (0, -0, 0)
    switch( getDominantAxis() )
    {
    case 'x': // x should be involved.  (1, 0, 0) --> ( -0, 1, 0 )
      if( y == 0.0 ) // avoid y
        return Vector( z, 0, -x ) ; // swap x, z. apply negation to x
      else
        return Vector( y, -x, 0 ) ; // swap x, y
      break ;
    case 'y': // y should be involved:  (-1, 1, -1) --> (-1,-1, -1)
      // avoid swapping a zero component
      if( x == 0.0 ) // avoid x
        return Vector( 0, z, -y ) ; // y,z
      else
        return Vector( -y, x, 0 ) ; // x,y
    case 'z': // z should be involved
    case 'n': // No matter which we switch.
    default:
      if( x == 0.0 ) // avoid x
        return Vector( 0, -z, y ) ; // y,z [(0,0,0) case]
      else
        return Vector( -z, 0, x ) ; // swap -z,x
    }
  }

  // Only compile if d3d is available
  #ifdef DIRECT3D_VERSION
  inline D3DXVECTOR3 getD3DVector() const {
    return D3DXVECTOR3( x, y, z ) ;
  }
  #endif

  #pragma endregion </CONST>

  #pragma region <NON-CONST>
  // UT
  inline real& operator[]( int index ) {
    return components[ index ] ;
  }

  // UT
  inline Vector& negate(){
    x = -x ;   y = -y ;   z = -z ;
    return *this ;
  }
  
  // UT
  inline Vector& normalize(){
    real length = len2() ;
    if( length == 0 ) return *this ; // avoid the sqrt op if 0 len
    length = sqrt( length ) ;
    x /= length ;   y /= length ;   z /= length ;
    return (*this) ;
  }

  inline Vector& setMag( real newMag ){
    this->normalize() ;
    (*this) *= newMag ;

    return *this ;
  }

  inline Vector& rotateZ( real radians ) {
    x = x*cos(radians) - y*sin(radians) ;
    y = x*sin(radians) + y*cos(radians) ;
    //z=z;
    return *this ;
  }

  /// Adds another vector to this one,
  /// mutating _this_.
  // UT
  inline Vector& operator+=( const Vector & b ) {
    x += b.x ;   y += b.y ;   z += b.z ;
    return *this ;
  }

  /// Subtracts another vector from
  /// this one.
  // UT
  inline Vector& operator-=( const Vector & b ) {
    x -= b.x ;   y -= b.y ;   z -= b.z ;
    return *this ;
  }

  // UT
  inline Vector& operator*=( const real b ) {
    x *= b ;     y *= b ;     z *= b ;
    return *this ;
  }

  // UT
  inline Vector& operator/=( const real b ) {
    x /= b ;     y /= b ;     z /= b ;
    return *this ;
  }

  inline Vector& clamp(
    real lx, real hx,
    real ly, real hy,
    real lz, real hz )
  {
    ::clamp( x, lx, hx ) ;
    ::clamp( y, ly, hy ) ;
    ::clamp( z, lz, hz ) ;

    return *this ;
  }
  #pragma endregion </NON-CONST>

} ;



#pragma region outside operators
/// CONST
/// Operators outside.  We don't want to define
/// a.plus( b ) ; rather its more like add( a, b ) here
// UT
Vector operator+( const Vector & a, const Vector & b ) ;

// UT
Vector operator-( const Vector & a, const Vector & b ) ;

// unary minus
// UT
Vector operator-( const Vector & a ) ;

// multiplication by a vector is going to be the cross product
// UT
Vector operator*( const Vector & a, const Vector & b ) ;

// UT
Vector operator*( const Vector & a, const real b ) ;

// UT
Vector operator*( const real b, const Vector & a ) ;

/// Post multiply matrix by vector
Vector operator*( const Matrix & a, const Vector& v ) ;

/// Premultiply matrix by vector.
// We chose to forbid this operation to force
// correct and consistent matrix multiplies.
//Vector operator*( const Vector& v, const Matrix & matrix ) ;

// UT
Vector operator/( const Vector & a, const real b ) ;

#pragma endregion outside operators





#endif