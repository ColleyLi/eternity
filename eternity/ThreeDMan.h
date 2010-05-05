#ifndef THREE_D_MAN_H
#define THREE_D_MAN_H

// Handles caching and drawing 3d geometry
#include <windows.h>
#include <d3d9.h>      // core direct3d
#include <d3dx9.h>     // aux libs
#include <dxerr.h>     // detailed error messages

#include <map>
#include <vector>
using namespace std ;

#include "D3DVertex.h"
#include "helperFunctions.h"
#include "Camera3D.h"

typedef map< int, vector<VertexTC> > /* as simply */ MapIntVertexTC ;
typedef map< int, vector<VertexTC> >::iterator /* as simply */ MapIntVertexTCIter ;

class ThreeDMan
{
private:
  IDirect3DDevice9 * lgpu ;

protected:
  Camera3D camera ;

  /// Maintain a vector of vertices
  /// to draw out in 3d
  vector<VertexC> verticesC ;
  map< int, vector<VertexTC> > mapVerticesTC ;

  vector<VertexC> linesC ;
  
  void initThreeDMan( IDirect3DDevice9 *theGpu ) ;

public:

  // Camera.
  Camera3D* getCamera() { return &camera ; }

  /// Sets the D3D VIEW matrix by
  /// whatever the camera's doing.
  /// You have to call this every frame,
  /// else we assume you don't want that.
  //void setViewByCamera() ;

  /// Set the world transformation matrix
  void setWorld( D3DXMATRIX* world ) ;

  // This series of create* functions can be
  // replaced by templates, but later
  // for now i only need vcolor, and vtexturecolor
  /// the triangle described by 3 vertices
  /// is abc, assuming CCW winding order
  void createTriVC( VertexC &a, VertexC &b, VertexC &c ) ;

  /// the rectangle described by 4 vertices
  /// is abc, cda, assuming CCW winding order
  /// b    a
  /// 
  /// c    d
  void createQuadVC( VertexC &a, VertexC &b, VertexC &c, VertexC &d ) ;

  /// Start creating a set of quads or tris
  /// that use the same texture.
private:
  void startVTCSet( int spriteId ) ;

public:
  void createTriVTC( int spriteId, VertexTC &a, VertexTC &b, VertexTC &c ) ;
  void createQuadVTC( int spriteId, VertexTC &a, VertexTC &b, VertexTC &c, VertexTC &d ) ;



  void draw3DLine( D3DXVECTOR3 startPoint, D3DCOLOR startColor,
    D3DXVECTOR3 endPoint, D3DCOLOR endColor ) ;

} ;

#endif // THREE_D_MAN_H