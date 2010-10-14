#include "AABB.h"

//!! this is really general AABB intersect code.
AABB2::AABB2( float x, float y, float width, float height ) :
  topLeft( x, y ), size( width, height )
{

}

bool AABB2::intersects( float x, float y )
{
  /*

  +-------+
  | this  |
  |      *|
  +-------+

  */
  // exclusive test.
  if( x > left() && x < right() && 
      y > top() && y < bottom() )
    return true ;
  else
    return false ;
}

bool AABB2::intersects( AABB2 other )
{
  /*

  +-------+
  | this  |
  |    +------+
  +----|--+   |
        | other|
        +------+

  */
    
  if( other.top() > this->bottom() ||
      this->top() > other.bottom() ||
        
      other.left() > this->right() ||
      this->left() > other.right() )
  {
    // They cannot intersect
    return false ;
  }
  else
    return true ; // they must intersect
      
}
