#include "Matrix.h"

#pragma region ctors
Matrix::Matrix():
  // IDENTITY
  _11(1),_12(0),_13(0),_14(0),
  _21(0),_22(1),_23(0),_24(0),
  _31(0),_32(0),_33(1),_34(0),
  _41(0),_42(0),_43(0),_44(1)
{ }

Matrix::Matrix(
  real m11, real m12, real m13, real m14,
  real m21, real m22, real m23, real m24,
  real m31, real m32, real m33, real m34,
  real m41, real m42, real m43, real m44 ):
_11(m11), _12(m12), _13(m13), _14(m14),
_21(m21), _22(m22), _23(m23), _24(m24),
_31(m31), _32(m32), _33(m33), _34(m34),
_41(m41), _42(m42), _43(m43), _44(m44)
{ }

Matrix::Matrix( const Matrix & a ):
  _11(a._11),_12(a._12),_13(a._13),_14(a._14),
  _21(a._21),_22(a._22),_23(a._23),_24(a._24),
  _31(a._31),_32(a._32),_33(a._33),_34(a._34),
  _41(a._41),_42(a._42),_43(a._43),_44(a._44)
{ }

//Matrix::Matrix( MatrixType mType, Vector arg ) {}
#pragma endregion

#pragma region <CONST>
bool Matrix::operator==( const Matrix& b ) const
{
  // unrolled
  return 
    _11 == b._11 &&
    _12 == b._12 &&
    _13 == b._13 &&
    _14 == b._14 &&

    _21 == b._21 &&
    _22 == b._22 &&
    _23 == b._23 &&
    _24 == b._24 &&

    _31 == b._31 &&
    _32 == b._32 &&
    _33 == b._33 &&
    _34 == b._34 &&

    _41 == b._41 &&
    _42 == b._42 &&
    _43 == b._43 &&
    _44 == b._44 ;
}
bool Matrix::operator!=( const Matrix& b ) const
{
  return 
    _11 != b._11 ||
    _12 != b._12 ||
    _13 != b._13 ||
    _14 != b._14 ||

    _21 != b._21 ||
    _22 != b._22 ||
    _23 != b._23 ||
    _24 != b._24 ||

    _31 != b._31 ||
    _32 != b._32 ||
    _33 != b._33 ||
    _34 != b._34 ||

    _41 != b._41 ||
    _42 != b._42 ||
    _43 != b._43 ||
    _44 != b._44 ;
}

bool Matrix::Near( const Matrix& b ) const
{
  return 
    fabs( _11 - b._11 ) < EPS &&
    fabs( _12 - b._12 ) < EPS &&
    fabs( _13 - b._13 ) < EPS &&
    fabs( _14 - b._14 ) < EPS &&

    fabs( _21 - b._21 ) < EPS &&
    fabs( _22 - b._22 ) < EPS &&
    fabs( _23 - b._23 ) < EPS &&
    fabs( _24 - b._24 ) < EPS &&

    fabs( _31 - b._31 ) < EPS &&
    fabs( _32 - b._32 ) < EPS &&
    fabs( _33 - b._33 ) < EPS &&
    fabs( _34 - b._34 ) < EPS &&

    fabs( _41 - b._41 ) < EPS &&
    fabs( _42 - b._42 ) < EPS &&
    fabs( _43 - b._43 ) < EPS &&
    fabs( _44 - b._44 ) < EPS ;
}

// You specify epsilon, instead of
// using the built in value.
bool Matrix::Near( const Matrix& b, real eps ) const
{
  return 
    fabs( _11 - b._11 ) < eps &&
    fabs( _12 - b._12 ) < eps &&
    fabs( _13 - b._13 ) < eps &&
    fabs( _14 - b._14 ) < eps &&

    fabs( _21 - b._21 ) < eps &&
    fabs( _22 - b._22 ) < eps &&
    fabs( _23 - b._23 ) < eps &&
    fabs( _24 - b._24 ) < eps &&

    fabs( _31 - b._31 ) < eps &&
    fabs( _32 - b._32 ) < eps &&
    fabs( _33 - b._33 ) < eps &&
    fabs( _34 - b._34 ) < eps &&

    fabs( _41 - b._41 ) < eps &&
    fabs( _42 - b._42 ) < eps &&
    fabs( _43 - b._43 ) < eps &&
    fabs( _44 - b._44 ) < eps ;
}

bool Matrix::Near3x3( const Matrix& b ) const
{
  return
    fabs( _11 - b._11 ) < EPS &&
    fabs( _12 - b._12 ) < EPS &&
    fabs( _13 - b._13 ) < EPS &&

    fabs( _21 - b._21 ) < EPS &&
    fabs( _22 - b._22 ) < EPS &&
    fabs( _23 - b._23 ) < EPS &&

    fabs( _31 - b._31 ) < EPS &&
    fabs( _32 - b._32 ) < EPS &&
    fabs( _33 - b._33 ) < EPS ;
}

real Matrix::det() const
{
  return _11*( _22*_33 - _32*_23 ) - //cofactor
    _12*( _21*_33 - _31*_23 ) +
    _13*( _21*_32 - _31*_22 ) ;
}

/// Invert
// Bang the matrix to invert it.
Matrix Matrix::operator!() const
{
  if( det() == 0 )
  {
    // NOT INVERTIBLE
    return Matrix() ;
  }

  // you can't invert in place
  Matrix matrix = *this ;
  Matrix inverse ;  // start out as identity

  // you can use the adjoint (9 determinants + 1)
  // or gaussian elimination

  // for a 3x3 matrix, the 9 dets isn't actually all 
  // that bad..
  
  
  // [ 4 8 12 ]
  // | 2 9  1 |
  // [ 1 1  1 ]

  // arrange the matrix such that it is as triangular
  // as can be
  matrix.triangularize() ;
  real divisor ;

  // [ 4  8  12 ]
  // | 2  9   1 |
  // [ 1  1   1 ]
  for( int row = 0 ; row < 3 ; row++ )
  {
    // 1.  make leading row 1 unity
    // [ 4  8  12 ]  /  4
    // | 2  9   1 |
    // [ 1  1   1 ]
    divisor = 1.0 / matrix.m[ row ][ row ] ;
    inverse.rowMult( row, divisor ) ;
    matrix.rowMult( row, divisor ) ;
    // [ 1 2 3 | 1/4  0  0 ]
    // | 2 9 1 |   0  1  0 |
    // [ 1 1 1 |   0  0  1 ]
    

    // 2.  FROM current row to bottom,
    //     clear out to zeros in col below
    // [ 1 2 3 ]
    // | 2 9 1 | - _21*R1
    // [ 1 1 1 ] - _31*R1
    for( int rowBelow = row + 1; rowBelow < 3 ; rowBelow++ )
    {
      divisor = -matrix.m[ rowBelow ][ row ] ;
      inverse.rowAdd( row, rowBelow, divisor ) ;
      matrix.rowAdd( row, rowBelow, divisor ) ;
    }
    // [ 1  2  3 |  1/4  0  0 ]
    // | 0  5 -5 | -1/2  1  0 |
    // [ 0 -1 -2 | -1/4  0  1 ]
    
    // re-triangularize
    
    // you can't call the retriangularize method
    // because you have to perform the exact same
    // operations on the inverse, plus you don't need
    // to re-triangularize rows above current row
    int parked = row +1 ; // start by considering
    // the row you are on as "already parked"

    const int n = 3 ;  // n is a 3x3 matrix
  
    // walk across cols
    for( int col = row + 1 ;
         col < n && parked < (n-1);
         ++col )
    {
      // walk down rows, looking for ANY non-zero entry.
      for( int retriRow = parked ;
           retriRow < n && parked < (n-1);
           ++retriRow )
      {
        if( !IsNear( matrix.m[ retriRow ][ col ], 0.0, 1e-6 ) )
        {
          // this is it!  take this row to (one below) the
          // last parked row.
          matrix.swapRows( retriRow, parked ) ;
          inverse.swapRows( retriRow, parked ) ;

          parked++ ;
          // WE CONTINUE looking for non-zero elements
          // to stack on the top part of the matrix.
          // by the end, all the 0 rows will be at
          // the bottom.
        }
      }
    } // END TRIANGULARIZE

    // Next two iterations will produce:
    // [ 1  2  3 |  1/4  0  0 ]
    // | 0  5 -5 | -1/2  1  0 |  /  5
    // [ 0 -1 -2 | -1/4  0  1 ]

    // [ 1  2  3 |   1/4   0  0 ]
    // | 0  1 -1 | -1/10 1/5  0 |
    // [ 0 -1 -2 |  -1/4   0  1 ]  + R2

    // [ 1  2  3 |   1/4    0  0 ]
    // | 0  1 -1 | -4/40  1/5  0 |
    // [ 0  0 -3 | -14/40 1/5  1 ]  /  -3

    // [ 1  2  3 |   1/4     0     0 ]
    // | 0  1 -1 | -1/10   1/5     0 |
    // [ 0  0  1 |  7/60 -1/15  -1/3 ]

    // DONE.  That is as far as step 1 take it,
    // to row echelon form on the left,
    // and some kind of partial inversion on
    // the right.
  }

  


  /// PART B:  backwards elimination

  // From the last step,
  // [ 1  2  3 |   1/4     0     0 ]
  // | 0  1 -1 | -1/10   1/5     0 |
  // [ 0  0  1 |  7/60 -1/15  -1/3 ]

  // we now go from the bottom row and, (well,
  // the RIGHTMOST COLUMN really, which just
  // so happens to correspond with the bottom
  // row!)
  const int startRow = 1 ;
  
  // At each COLUMN, walking from the
  // right..
  for( int col = 2 ; col >= 1 ; col-- )
  {
    // GO THRU ALL ROWS ABOVE..
    // until the top row..
    for( int row = col - 1; row >= 0 ; row-- )
    {
      // and eliminate the term here
      real mult = -matrix.m[ row ][ col ] ;
      inverse.rowAdd( col, row, mult ) ;
      matrix.rowAdd( col, row, mult ) ;
    }
    // One iteration:
    // [ 1  2  3 |   1/4     0     0 ]
    // | 0  1 -1 | -1/10   1/5     0 |  + R3
    // [ 0  0  1 |  7/60 -1/15  -1/3 ]

    // [ 1  2  3 |   1/4     0     0 ]
    // | 0  1  0 |  1/60  2/15  -1/3 |
    // [ 0  0  1 |  7/60 -1/15  -1/3 ]
  }

  inverse.roundOff() ;
  return inverse ;
}
#pragma endregion </CONST>

#pragma region <NON-CONST>
Matrix& Matrix::triangularize()
{
  // get as triangular as can be
  // collect zero rows at bottom
  // [ 0   1   2 ]
  // | 1   2   3 |
  // [ 0   0   1 ]
  const static int n = 4 ;
  int parked = 0 ;
  
  // walk across cols
  for( int col = 0 ;
       col < n && parked < (n-1);
       ++col )
  {
    // walk down rows, looking for ANY non-zero entry.
    for( int row = parked ;
         row < n && parked < (n-1);
         ++row )
    {
      if( !IsNear( m[ row ][ col ], 0.0, 1e-6 ) )
      {
        // this is it!  take this row to (one below) the
        // last parked row.
        swapRows( row, parked ) ;

        parked++ ;

        // WE CONTINUE looking for non-zero elements
        // to stack on the top part of the matrix.
        // by the end, all the 0 rows will be at
        // the bottom.
      }
    }
  }

  return *this ;
}

Matrix& Matrix::swapRows( int row1, int row2 )
{
  if( row1 != row2 )  // no-op if it is
  {
    // at first i used 3 here,
    // assuming operation on a 3x3 submatrix.
    // but then i changed it to 4
    for( int col = 0 ; col < 4 ; col++ )
      swap<real>( m[ row1 ][ col ], m[ row2 ][ col ] ) ;
  }
  return *this ;
}


// srcMatrix is always "this"
Matrix& Matrix::rowAdd( int srcRow, int dstRow, real multiplier )
{
  for( int col = 0 ; col < 4 ; col++ )
    m[ dstRow ][ col ] += m[ srcRow ][ col ] * multiplier ;

  return *this ;
}

Matrix& Matrix::rowMult( int row, real mult )
{
  // only do 3 cols
  for( int col = 0 ; col < 4 ; col++ )
    m[ row ][ col ] *= mult ;

  return *this ;
}

Matrix& Matrix::invert()
{
  *this = !(*this) ;

  return *this ;
}

Matrix& Matrix::roundOff()
{
  for( int row = 0 ; row < 4 ; row++ )
    for( int col = 0 ; col < 4 ; col++ )
      if( IsNear( m[ row ][ col ], 0.0, EPS ) )
        m[ row ][ col ] = 0.0 ;

  return *this ;
}


/// Transposes the matrix.
Matrix& Matrix::transpose()
{
  // _11 _12 _13 _14
  // _21 _22 _23 _24
  // _31 _32 _33 _34
  // _41 _42 _43 _44

  //6 swaps
  swap<real>( _21, _12 ) ;
  swap<real>( _31, _13 ) ;
  swap<real>( _32, _23 ) ;
  
  swap<real>( _41, _14 ) ;
  swap<real>( _42, _24 ) ;
  swap<real>( _43, _34 ) ;

  return *this ;
}

Matrix& Matrix::operator+=( const Matrix& b )
{
  _11 += b._11 ;   _12 += b._12 ;   _13 += b._13 ;   _14 += b._14 ;
  _21 += b._21 ;   _22 += b._22 ;   _23 += b._23 ;   _24 += b._24 ;
  _31 += b._31 ;   _32 += b._32 ;   _33 += b._33 ;   _34 += b._34 ;
  _41 += b._41 ;   _42 += b._42 ;   _43 += b._43 ;   _44 += b._44 ;

  return *this ;
}

Matrix& Matrix::operator+=( real scalar )
{
  _11 += scalar ;   _12 += scalar ;   _13 += scalar ;   _14 += scalar ;
  _21 += scalar ;   _22 += scalar ;   _23 += scalar ;   _24 += scalar ;
  _31 += scalar ;   _32 += scalar ;   _33 += scalar ;   _34 += scalar ;
  _41 += scalar ;   _42 += scalar ;   _43 += scalar ;   _44 += scalar ;

  return *this ;
}

Matrix& Matrix::operator-=( const Matrix& b )
{
  _11 -= b._11 ;   _12 -= b._12 ;   _13 -= b._13 ;   _14 -= b._14 ;
  _21 -= b._21 ;   _22 -= b._22 ;   _23 -= b._23 ;   _24 -= b._24 ;
  _31 -= b._31 ;   _32 -= b._32 ;   _33 -= b._33 ;   _34 -= b._34 ;
  _41 -= b._41 ;   _42 -= b._42 ;   _43 -= b._43 ;   _44 -= b._44 ;

  return *this ;
}

Matrix& Matrix::operator-=( real scalar )
{
  _11 -= scalar ;   _12 -= scalar ;   _13 -= scalar ;   _14 -= scalar ;
  _21 -= scalar ;   _22 -= scalar ;   _23 -= scalar ;   _24 -= scalar ;
  _31 -= scalar ;   _32 -= scalar ;   _33 -= scalar ;   _34 -= scalar ;
  _41 -= scalar ;   _42 -= scalar ;   _43 -= scalar ;   _44 -= scalar ;

  return *this ;
}

Matrix& Matrix::operator*=( const Matrix& b )
{
  // You cannot update 'this' in place 
  // as you compute the result because the
  // entries get used in more than one line..

  // Treating 'this' as the left hand matrix..
  *this = *this * b ;
  return *this ;
}

Matrix& Matrix::operator*=( real scalar )
{
  _11 *= scalar ;   _12 *= scalar ;   _13 *= scalar ;   _14 *= scalar ;
  _21 *= scalar ;   _22 *= scalar ;   _23 *= scalar ;   _24 *= scalar ;
  _31 *= scalar ;   _32 *= scalar ;   _33 *= scalar ;   _34 *= scalar ;
  _41 *= scalar ;   _42 *= scalar ;   _43 *= scalar ;   _44 *= scalar ;

  return *this ;
}

Matrix& Matrix::operator/=( real scalar )
{
  scalar = 1.0/scalar ;

  return *this *= scalar ;
}
#pragma endregion </NON-CONST>

#pragma region <STATIC CONSTRUCTION ROUTINES>
Matrix Matrix::RotationX( real angle )
{
  return Matrix(
    1,          0,          0, 0,
    0, cos(angle),-sin(angle), 0,
    0, sin(angle), cos(angle), 0,
    0,          0,          0, 1
  ) ;
}

Matrix Matrix::RotationY( real angle )
{
  return Matrix(
    cos(angle), 0, sin(angle), 0,
             0, 1,          0, 0,
   -sin(angle), 0, cos(angle), 0,
             0, 0,          0, 1
  );
}

Matrix Matrix::RotationZ( real angle )
{
  return Matrix(
    cos(angle),-sin(angle), 0, 0,
    sin(angle), cos(angle), 0, 0,
             0,          0, 1, 0,
             0,          0, 0, 1
  ) ;
}

/// Arbitrary axis angle rotation
Matrix Matrix::Rotation( const Vector & u, real angle )
{
  real c = cos( angle ) ;
  real l_c = 1 - c ;

  real s = sin( angle ) ;
    
  return Matrix(
    u.x*u.x + (1 - u.x*u.x)*c,   u.x*u.y*l_c - u.z*s,   u.x*u.z*l_c + u.y*s,   0,
    u.x*u.y*l_c + u.z*s,   u.y*u.y+(1 - u.y*u.y)*c,   u.y*u.z*l_c - u.x*s,     0,
    u.x*u.z*l_c - u.y*s,   u.y*u.z*l_c + u.x*s,   u.z*u.z + (1 - u.z*u.z)*c,   0,
    0, 0, 0, 1
  ) ;
}

Matrix Matrix::RotationYawPitchRoll( real yaw, real pitch, real roll )
{
  // Order: roll, pitch, yaw
  // z-axis, x-axis, y-axis,
  Matrix rollMat = RotationZ( roll ) ;

  Matrix pitchMat = RotationX( pitch ) ;

  Matrix yawMat = RotationY( yaw ) ;

  return yawMat*pitchMat*rollMat ;
}

Matrix Matrix::Translate( const Vector& v )
{
  return Matrix(

    1, 0, 0, v.x,
    0, 1, 0, v.y,
    0, 0, 1, v.z,
    0, 0, 0, 1

  ) ;
}

Matrix Matrix::Translate( real x, real y, real z )
{
  return Matrix(

    1, 0, 0, x,
    0, 1, 0, y,
    0, 0, 1, z,
    0, 0, 0, 1

  ) ;
}

Matrix Matrix::Scale( real x, real y, real z )
{
  return Matrix(
    x, 0, 0, 0,
    0, y, 0, 0,
    0, 0, z, 0,
    0, 0, 0, 1
  ) ;
}

Matrix Matrix::Scale( const Vector& v )
{
  return Matrix(
    v.x,   0,   0, 0,
    0,   v.y,   0, 0,
    0,     0, v.z, 0,
    0,     0,   0, 1
  ) ;
}

Matrix Matrix::LookAt( const Vector & eye, const Vector & look, const Vector & up )
{
  Vector w = eye - look ; //(eye - look).normalize() ; // FORWARD
  w.normalize() ;
  Vector u = (up * w).normalize() ; // RIGHT
  Vector v = (w * u) ; // UP:  already normal

  Matrix rot = Face( w, u, v ) ;
  Matrix trans = Translate( -eye ) ;

  // translate followed by rotation
  return rot * trans ;
}

// Two projection matrices
Matrix Matrix::ProjOrtho( real l, real r, real b, real t, real n, real f )
{
  // The orthographic projection is
  // simply a translate followed by
  // a scale
  Matrix translate(
    1, 0, 0, -(l+r)/2,
    0, 1, 0, -(b+t)/2,
    0, 0, 1, -(n+f)/2,
    0, 0, 0, 1
  ) ;

  Matrix scale(
    2 / (r - l),           0,           0, 0,
              0, 2 / (t - b),           0, 0,
              0,           0, 2 / (n - f), 0,
              0,           0,           0, 1
  ) ;

  return scale*translate ;
  
  /*
  // works
  Matrix translate(
    1, 0, 0, (l + r)/(l - r),
    0, 1, 0, (b + t)/(b - t),
    0, 0, 1, (f + n)/(f - n),
    0, 0, 0,  1
  ) ;

  Matrix scale(
    2 / (r - l),           0,           0, 0,
              0, 2 / (t - b),           0, 0,
              0,           0, 2 / (n - f), 0,
              0,           0,           0, 1
  ) ;

  return translate*scale ;
  */
}

Matrix Matrix::ProjPerspectiveRHToLH( real fieldOfView, real aspectRatio, real n )
{
  real cotVal = 1.0 / tan( fieldOfView / 2.0 ) ;

  real xScale = cotVal / aspectRatio ;
  real yScale = cotVal ;

  return Matrix(
    xScale,      0,  0,    0,
         0, yScale,  0,    0,
         0,      0, -1, -2*n,
         0,      0, -1,    0
  ) ;

  // This matrix 

}

Matrix Matrix::ProjPerspectiveRHToLH( real fieldOfView, real aspectRatio, real n, real f )
{
  real cotVal = 1.0 / tan( fieldOfView / 2.0 ) ;

  real xScale = cotVal / aspectRatio ;
  real yScale = cotVal ;

  real dz = f - n ;

  return Matrix(
    xScale,      0,             0,             0,
         0, yScale,             0,             0,
         0,      0,   -(f + n)/dz, -(2*n*f) / dz,
         0,      0,            -1,             0
  ) ;

  // * The _33 and _43 entries are actually
  // *(-1) inverted -- multiplied by -1.
  // the reason for this is actually to reverse
  // the direction of the z-axis, and make it
  // a LH coordinate system IMMEDIATELY BEFORE
  // the perspective projection.  You know
  // it will be before the persp because the
  // reversal of column 3 is a post-multiply
  // by a scale(1,1,-1) matrix.  we are column
  // major here, so that post mult is BEFORE.

  // n1:  The -1 here flips from a RH coordinate
  // system to an LH coordinate system, by forcing
  // a perspective divide by -z
  // (in the next persp divide step)
  // The reason for this flip is because
  // we want the z values to INCREASE as
  // they get further away.  This means we
  // are now looking down +z, not -z, and

  /*
  // D3D configuration:  canonical view volume
  // is (-1,-1,0)..(1,1,1)
  real yScale = -1.0 / tan( fieldOfView / 2.0 ) ;
  real xScale = yScale / aspectRatio ;

  return Matrix(
    xScale,      0,             0,             0,
         0, yScale,             0,             0,
         0,      0,   f / (n - f), n*f / (n - f),
         0,      0,            -1,    0
  ) ;
  */

  
}

Matrix Matrix::Face( const Vector & forward, const Vector & right, const Vector & up )
{
  // error checking
  if( !IsNear( forward.len2(), 1, EPS ) )
    error( "Matrix::Face:  forward len=%f", forward.len() ) ;
  if( !IsNear( right.len2(), 1, EPS ) )
    error( "Matrix::Face:  forward len=%f", right.len() ) ;
  if( !IsNear( up.len2(), 1, EPS ) )
    error( "Matrix::Face:  forward len=%f", up.len() ) ;

  return Matrix(
    right.x, up.x, -forward.x, 0,
    right.y, up.y, -forward.y, 0,
    right.z, up.z, -forward.z, 0,
          0,    0,          0, 1
  ) ;

  /*
  // this one doesn't work at all
  return Matrix(
  
      right.x,   right.y,   right.z, 0,
         up.x,      up.y,      up.z, 0,
   -forward.x,-forward.y,-forward.z, 0,
            0,         0,         0, 1
    
  ) ;
  */
  /*
  
  // right = +x
  // up = +y
  // fwd = +z
  // RIGHT = UP*FWD (( using an LH cross product ))
  return Matrix(
  
      right.x,   right.y,   right.z, 0,
         up.x,      up.y,      up.z, 0,
    forward.x, forward.y, forward.z, 0,
            0,         0,         0, 1
    
  ) ;
  */
}

#ifdef WIN32
// Define d3d stuff
Matrix Matrix::FromD3DXMatrix( D3DXMATRIX *mat )
{
  return Matrix(

    mat->_11, mat->_12, mat->_13, mat->_14,
    mat->_21, mat->_22, mat->_23, mat->_24,
    mat->_31, mat->_32, mat->_33, mat->_34,
    mat->_41, mat->_42, mat->_43, mat->_44

  ) ;
}
#endif
#pragma endregion </STATIC CONSTRUCTION ROUTINES>


#pragma region <NON-MEMBER FUNCTIONS>
Matrix operator+( const Matrix & a, const Matrix & b )
{
  return Matrix(
    
    a._11 + b._11, a._12 + b._12, a._13 + b._13, a._14 + b._14,
    a._21 + b._21, a._22 + b._22, a._23 + b._23, a._24 + b._24,
    a._31 + b._31, a._32 + b._32, a._33 + b._33, a._34 + b._34,
    a._41 + b._41, a._42 + b._42, a._43 + b._43, a._44 + b._44

  ) ;
}

Matrix operator+( const Matrix & a, real scalar )
{
  return Matrix(
    
    a._11 + scalar, a._12 + scalar, a._13 + scalar, a._14 + scalar,
    a._21 + scalar, a._22 + scalar, a._23 + scalar, a._24 + scalar,
    a._31 + scalar, a._32 + scalar, a._33 + scalar, a._34 + scalar,
    a._41 + scalar, a._42 + scalar, a._43 + scalar, a._44 + scalar

  ) ;
}

Matrix operator+( real scalar, const Matrix & a )
{
  return Matrix(
    
    a._11 + scalar, a._12 + scalar, a._13 + scalar, a._14 + scalar,
    a._21 + scalar, a._22 + scalar, a._23 + scalar, a._24 + scalar,
    a._31 + scalar, a._32 + scalar, a._33 + scalar, a._34 + scalar,
    a._41 + scalar, a._42 + scalar, a._43 + scalar, a._44 + scalar

  ) ;
}

Matrix operator-( const Matrix & a, const Matrix & b )
{
  return Matrix(
    
    a._11 - b._11, a._12 - b._12, a._13 - b._13, a._14 - b._14,
    a._21 - b._21, a._22 - b._22, a._23 - b._23, a._24 - b._24,
    a._31 - b._31, a._32 - b._32, a._33 - b._33, a._34 - b._34,
    a._41 - b._41, a._42 - b._42, a._43 - b._43, a._44 - b._44

  ) ;
}

Matrix operator-( const Matrix & a, real scalar )
{
  return Matrix(
    
    a._11 - scalar, a._12 - scalar, a._13 - scalar, a._14 - scalar,
    a._21 - scalar, a._22 - scalar, a._23 - scalar, a._24 - scalar,
    a._31 - scalar, a._32 - scalar, a._33 - scalar, a._34 - scalar,
    a._41 - scalar, a._42 - scalar, a._43 - scalar, a._44 - scalar

  ) ;
}

Matrix operator-( real scalar, const Matrix & a )
{
  return Matrix(
    
    scalar - a._11, scalar - a._12, scalar - a._13, scalar - a._14,
    scalar - a._21, scalar - a._22, scalar - a._23, scalar - a._24,
    scalar - a._31, scalar - a._32, scalar - a._33, scalar - a._34,
    scalar - a._41, scalar - a._42, scalar - a._43, scalar - a._44

  ) ;
}

Matrix operator*( const Matrix & a, const Matrix & b )
{
  return Matrix(

    a._11*b._11 + a._12*b._21 + a._13*b._31 + a._14*b._41, // _11
    a._11*b._12 + a._12*b._22 + a._13*b._32 + a._14*b._42, // _12
    a._11*b._13 + a._12*b._23 + a._13*b._33 + a._14*b._43, // _13
    a._11*b._14 + a._12*b._24 + a._13*b._34 + a._14*b._44, // _14

    a._21*b._11 + a._22*b._21 + a._23*b._31 + a._24*b._41, // _21
    a._21*b._12 + a._22*b._22 + a._23*b._32 + a._24*b._42, // _22
    a._21*b._13 + a._22*b._23 + a._23*b._33 + a._24*b._43, // _23
    a._21*b._14 + a._22*b._24 + a._23*b._34 + a._24*b._44, // _24

    a._31*b._11 + a._32*b._21 + a._33*b._31 + a._34*b._41, // _31
    a._31*b._12 + a._32*b._22 + a._33*b._32 + a._34*b._42, // _32
    a._31*b._13 + a._32*b._23 + a._33*b._33 + a._34*b._43, // _33
    a._31*b._14 + a._32*b._24 + a._33*b._34 + a._34*b._44, // _34

    a._41*b._11 + a._42*b._21 + a._43*b._31 + a._44*b._41, // _41
    a._41*b._12 + a._42*b._22 + a._43*b._32 + a._44*b._42, // _42
    a._41*b._13 + a._42*b._23 + a._43*b._33 + a._44*b._43, // _43
    a._41*b._14 + a._42*b._24 + a._43*b._34 + a._44*b._44  // _44

  ) ;
}

Matrix operator*( const Matrix & a, real scalar )
{
  return Matrix(

    a._11 * scalar, a._12 * scalar, a._13 * scalar, a._14 * scalar,
    a._21 * scalar, a._22 * scalar, a._23 * scalar, a._24 * scalar,
    a._31 * scalar, a._32 * scalar, a._33 * scalar, a._34 * scalar,
    a._41 * scalar, a._42 * scalar, a._43 * scalar, a._44 * scalar

  ) ;
}

Matrix operator*( real scalar, const Matrix & a )
{
  return Matrix(

    a._11 * scalar, a._12 * scalar, a._13 * scalar, a._14 * scalar,
    a._21 * scalar, a._22 * scalar, a._23 * scalar, a._24 * scalar,
    a._31 * scalar, a._32 * scalar, a._33 * scalar, a._34 * scalar,
    a._41 * scalar, a._42 * scalar, a._43 * scalar, a._44 * scalar

  ) ;

}

#pragma endregion <NON-MEMBER FUNCTIONS>


