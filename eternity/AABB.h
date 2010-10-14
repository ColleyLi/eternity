#ifndef AABB_H
#define AABB_H

#include <d3dx9.h>

//!! this is really general AABB intersect code.
class AABB2
{
protected:
  /// top left corner of this UIObject
  D3DXVECTOR2 topLeft ;

  /// Width is x, size is y.
  /// That's pretty intuitive, actually
  D3DXVECTOR2 size ;

public:
  AABB2( float x, float y, float width, float height ) ;

  inline float left() { return topLeft.x ; }

  inline float right() { return topLeft.x + size.x ; }

  inline float top() { return topLeft.y ; }

  inline float bottom() { return topLeft.y + size.y ; }

  inline float width() { return size.x ; }

  inline float height() { return size.y ; }

  bool intersects( float x, float y ) ;
  
  bool intersects( AABB2 other ) ;
} ;

#endif