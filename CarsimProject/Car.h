#ifndef CAR_H
#define CAR_H

#include "cs-main.h"

/// Represents a carsim car
class Car
{
public:
  D3DXVECTOR3 pos ;
  double steeringAngle ;
  vector<ObjFile *> models ;
  vector<ObjFile *> tireModels ;

  Car()
  {
    // zero out pos
    pos.x=pos.y=pos.z=0;
    steeringAngle = 0 ;
  }

  ~Car()
  {
    foreach( vector<ObjFile*>::iterator, iter, models )
      delete( *iter ) ;

    foreach( vector<ObjFile*>::iterator, iter, tireModels )
      delete( *iter ) ;
  }

  void loadModel( char *filename )
  {
    // REQUIRES:  already opened file,
    // with srcFile POINTING AT START POINT
    // of vertex set

    models.push_back( new ObjFile( window, filename ) ) ;
  }

  void loadTireModel( char *filename )
  {
    // REQUIRES:  already opened file,
    // with srcFile POINTING AT START POINT
    // of vertex set

    tireModels.push_back( new ObjFile( window, filename ) ) ;
  }


  void update()
  {
    // Update position from Carsim,
    // !!make sound if slippage is occurring etc


    // UPDATE CARSIM
    // Compute what impetus should be
    // on the car gas, brake, and steering wheel
    // and push those updates to carsim
  }

  void draw()
  {
    // WORLD UNITS are "metres"
    float w = 1.85f, h = 1.4f, len = 5.0f ;

    // Draw centered around pos
    D3DCOLOR color = D3DCOLOR_XRGB( 255, 128, 0 ) ;

    D3DXMATRIX matTrans ;
    D3DXMatrixTranslation( &matTrans, pos.x, pos.y, pos.z ) ;
    
    // This rights the vehicle to lay on the
    // x-z plane, it is now facing its nose
    // in +X
    D3DXMATRIX matRotX ;
    D3DXMatrixRotationX( &matRotX, -D3DX_PI/2 ) ;

    // Now rotate it about Y-axis to
    // face +Z axis
    
    // We'll also rotate the vehicle to face
    // in the direction of its steering angle
    
    D3DXMATRIX matRotY ;
    D3DXMatrixRotationY( &matRotY, -D3DX_PI/2   +   steeringAngle ) ;
    
    D3DXMATRIX concat = matRotX * matRotY * matTrans ;

    // World matrix translate
    window->setWorld( &concat ) ;

    // Draw the whole model.
    foreach( vector<ObjFile*>::iterator, iter, models )
    {
      (*iter)->draw() ;
    }


    // The tires are translated further out
    // and there are 4 of them
    
    
    float wheelbase = 2.75f ;
    float track = 0.75f ;

    // Left front
    concat._41 += track ;  // +x translation entry.
    window->setWorld( &concat ) ;
    foreach( vector<ObjFile*>::iterator, iter, tireModels )
      (*iter)->draw() ;

    // Left rear
    concat._43 -= wheelbase ;  // +z translation entry.
    window->setWorld( &concat ) ;
    foreach( vector<ObjFile*>::iterator, iter, tireModels )
      (*iter)->draw() ;

    // right rear
    concat._41 -= 2*track ;  // +x translation entry.
    window->setWorld( &concat ) ;
    foreach( vector<ObjFile*>::iterator, iter, tireModels )
      (*iter)->draw() ;

    // right front
    concat._43 += wheelbase ;  // +z translation entry.
    window->setWorld( &concat ) ;
    foreach( vector<ObjFile*>::iterator, iter, tireModels )
      (*iter)->draw() ;

    
    
  }
} ;

#endif