#ifndef MATRIX_H
#define MATRIX_H

#include "helperFunctions.h"
#include "Vector.h"

union Vector ;

// Modelled after d3dx package
union Matrix
{
  struct
  {
    real _11, _12, _13, _14,
         _21, _22, _23, _24,
         _31, _32, _33, _34,
         _41, _42, _43, _44 ;
  };
  real m[4][4] ;

  #pragma region ctors

  // UT
  Matrix() ;

  // UT
  Matrix( real m11, real m12, real m13, real m14,
          real m21, real m22, real m23, real m24,
          real m31, real m32, real m33, real m34,
          real m41, real m42, real m43, real m44 ) ;

  // UT
  Matrix( const Matrix & a ) ;
  #pragma endregion

  #pragma region <STATIC CONSTRUCTION ROUTINES>
  // You can't declare members of a union as static.
  // Well!  Who woulda guessed.  You can however 
  // declare the class as a struct with an anonymous
  // union in it.
  // static Matrix identity ;
  // static Matrix GetIdentity() ; // NO POINT -- just
  // call Matrix() base ctor to get an identity matrix.

  // UT
  static Matrix RotationX( real angle ) ;

  // UT
  static Matrix RotationY( real angle ) ;

  // UT
  static Matrix RotationZ( real angle ) ;

  // UT
  /// Arbitrary axis angle rotation
  static Matrix Rotation( const Vector & u, real angle ) ;
  
  // UT
  static Matrix RotationYawPitchRoll( real yaw, real pitch, real roll ) ;

  // UT
  static Matrix Translate( const Vector & v ) ;

  // UT
  static Matrix Translate( real x, real y, real z ) ;

  // UT
  static Matrix Scale( real x, real y, real z ) ;

  // UT
  static Matrix Scale( const Vector& v ) ;

  // UT
  static Matrix LookAt( const Vector & eye, const Vector & look, const Vector & up ) ;

  // UT
  // Two projection matrices
  static Matrix ProjOrtho( real l, real r, real b, real t, real n, real f ) ;

  // UT
  /// Perspective projection with an
  /// infinite far clipping plane.
  /// The fact that this projection matrix
  /// converts an assumed right handed coordinate
  /// system to an LH system is encoded in
  /// the function's name
  static Matrix ProjPerspectiveRHToLH( real fieldOfView, real aspectRatio, real n ) ;

  // UT
  /// Must specify the far clipping plane
  /// with this version.
  static Matrix ProjPerspectiveRHToLH( real fieldOfView, real aspectRatio, real n, real f ) ;

  // UT
  static Matrix Face( const Vector & forward, const Vector & right, const Vector & up ) ;

#ifdef WIN32
  // UT
  // Define d3d stuff
  static Matrix FromD3DXMatrix( D3DXMATRIX *mat ) ;
#endif
  #pragma endregion </STATIC CONSTRUCTION ROUTINES>

  #pragma region <CONST>
  // UT
  /// Checks for EXACT equality.  Use with
  /// caution, most of the time you should
  /// be using Near, and not this operator.
  bool operator==( const Matrix& b ) const ;

  // UT
  /// Checks for ANY difference between corresponding
  /// elements of this and b.  Usually you should
  /// use !Near(b) instead.
  bool operator!=( const Matrix& b ) const ;

  // UT
  /// Checks that all entries in this are
  /// Near (within 1e-6) of the corresponding
  /// entries in b.
  bool Near( const Matrix& b ) const ;

  // UT
  /// You specify epsilon, instead of
  /// using the built in value.
  bool Near( const Matrix& b, real eps ) const ;

  // UT
  /// Checks if the 3x3 submatrix this
  /// is Near the 3x3 submatrix in b
  bool Near3x3( const Matrix& b ) const ;

  // UT
  /// Gets you the determinant of
  /// this matrix, defined on the
  /// _11 .. _33 3x3 submatrix
  real det() const ;
  
  /// Invert
  Matrix operator!() const ;
  #pragma endregion </CONST>

  #pragma region <NON-CONST>
  // UT
  /// Makes the matrix as UPPER TRIANGULAR
  /// as it can, by only doing row swap
  /// operations and nothing else
  Matrix& triangularize() ;

  // UT
  Matrix& swapRows( int row1, int row2 ) ;
  
  // UT
  Matrix& rowAdd( int srcRow, int dstRow, real multiplier ) ;

  // UT
  Matrix& rowMult( int row, real mult ) ;
  
  // UT
  Matrix& invert() ;

  // UT
  /// Rounds the entries of this matrix
  /// down to exactly 0.0 where they are
  /// close enough to 0.0
  Matrix& roundOff() ;
  
  // UT
  /// Transposes the matrix.
  Matrix& transpose() ;

  // UT
  Matrix& operator+=( const Matrix& b ) ;

  // UT
  Matrix& operator+=( real scalar ) ;
  
  // UT
  Matrix& operator-=( const Matrix& b ) ;

  // UT
  Matrix& operator-=( real scalar ) ;
  
  // UT
  Matrix& operator*=( const Matrix& b ) ;

  // UT
  Matrix& operator*=( real scalar ) ;

  // UT
  Matrix& operator/=( real scalar ) ;
  #pragma endregion </NON-CONST>
  
} ;

#pragma region outside operators

// UT
Matrix operator+( const Matrix & a, const Matrix & b ) ;

// UT
Matrix operator+( const Matrix & a, real scalar ) ;

// UT
Matrix operator+( real scalar, const Matrix & a ) ;

// UT
Matrix operator-( const Matrix & a, const Matrix & b ) ;
// UT
Matrix operator-( const Matrix & a, real scalar ) ;
// UT
Matrix operator-( real scalar, const Matrix & a ) ;

// UT
Matrix operator*( const Matrix & a, const Matrix & b ) ;
// UT
Matrix operator*( const Matrix & a, real scalar ) ;
// UT
Matrix operator*( real scalar, const Matrix & a ) ;

#pragma endregion

#endif
