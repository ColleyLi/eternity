#ifndef CAR_H
#define CAR_H

#include "cs-main.h"

/// Represents a carsim car
class Car
{
public:
  D3DXVECTOR3 pos ;
  D3DXVECTOR3 vel ; // actual vehicle velocity
  D3DXVECTOR3
  D3DXVECTOR3 fwdForce ; // the accelerating force
  // on the vehicle.  It is ideally in the direction the tires
  // are facing, with a force from the motor

  D3DXVECTOR3 up ; // the vehicle's up vector may change
  // as it goes up inclines, etc.

  float motorForce ; // engine power
  float mass ; // vehicle mass
  
  
  double steeringAngle ; // angle front wheels are facing
  vector<ObjFile *> models ;
  vector<ObjFile *> tireModels ;

  Car()
  {
    // zero out pos
    pos.x=pos.y=pos.z=0;
    vel.x=vel.y=vel.z=0;
    fwdForce.x=fwdForce.y=fwdForce.z=0 ;
    up.x=up.z= 0;
    up.y = 1; // up is +y

    // velocity can only have
    // xz component, velocity is
    // actually due to tire angle.
    // and speed prevoius frame.

    // steering ultimately modifies velocity,
    // but only so much as the road surface and tire grade allows
    pos.x = -5 ; // start it "back 5m"
    vel.x = 0.2 ;  // velocity slowly rolling fwd

    motorForce = 120 ;
    mass = 1000 ; // 1000 kg mass


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


  void update( double timeSinceLastFrame )
  {
    // Update position from Carsim,

    // apply fwdForce to change velocity

    // F = ma
    // v2 = v1 + a*t*t

    // !!make sound if slippage is occurring etc
    pos += vel*timeSinceLastFrame ;

    // UPDATE CARSIM
    // Compute what impetus should be
    // on the car gas, brake, and steering wheel
    // and push those updates to carsim
  }

  void draw()
  {

    // CARSIM COORDINATES
    //
    // The stock models all come
    // oriented sitting in the XY plane, nose
    // of vehicle pointing in +X, and UP is +Z.
    //
    // To avoid fighting CarSim, we use
    // CarSim's conventions, even though 
    // in most applications we've been
    // used to +Y as being up.
    //


    // WORLD UNITS are "metres"




    // Translate the car (and wheels)
    // to where they are.
    D3DXMATRIX trans ;
    D3DXMatrixTranslation( &trans, pos.x, pos.y, pos.z ) ;
    
    
    // Rotate the vehicle to face
    // in the direction of its .. forward angle.
    // !!A vehicle can slip, so this needs to be fixed
    
    D3DXMATRIX rotZ ;
    D3DXMatrixRotationZ( &rotZ, steeringAngle ) ;
    
    // Matrix mults applied in order they appear left to right.
    // ROTATE first, then translate
    D3DXMATRIX concat = rotZ * trans ;


    // Use it
    window->setWorld( &concat ) ;

    // Draw the whole model with this matrix set
    foreach( vector<ObjFile*>::iterator, iter, models )
    {
      (*iter)->draw() ;
    }


    // The tires are translated further out
    // and there are 4 of them
    float wheelbase = 2.75f ;
    float track = 0.75f ;
    // These values were derived by experiment
    // working with "european_sedan" carsim model
    // The values probably can be read from somewhere,
    // but we are not sure where at this point.
    // So we derived them experimentally.

    D3DXMatrixIdentity( &concat ) ;
    //D3DXMatrixIdentity( &rotZ ) ;  // rotZ remains the same to start.
    //D3DXMatrixIdentity( &trans ) ; // trans remains the same to start.

    /// Remmeber, vehicle points in +x
    //   ^ +y
    //   |
    //  o| o
    // --|-->+x
    //  o| o
    //
    // http://msdn.microsoft.com/en-us/library/bb206269(VS.85).aspx
    // D3D uses 
    // [ 1  0  0  0 ]
    // [ 0  1  0  0 ]
    // [ 0  0  1  0 ]
    // [ tx ty tz 1 ]
    //
    // The tires actually start
    // in a position where if you
    // translate them laterally,
    // they will line up with the
    // wheel well perfectly.

    D3DXMATRIX fromCar ;
    D3DXMatrixIdentity( &fromCar ) ; // init with identity

    // Left front.  Only needs to move left, which is +y for stock model.
    fromCar._42 += track ;  // +y translation entry.

    // 1.  translate fromCar
    // 2.  rotate
    // 3.  translate by eye
    concat = fromCar *rotZ* trans ;
    window->setWorld( &concat ) ;

    // Draw a tire.
    foreach( vector<ObjFile*>::iterator, iter, tireModels )
      (*iter)->draw() ;

    // Left rear
    // from front left, move back (which is -x)
    fromCar._41 -= wheelbase ;  // -x translation entry.
    
    concat = fromCar *rotZ* trans ;
    window->setWorld( &concat ) ;
    foreach( vector<ObjFile*>::iterator, iter, tireModels )
      (*iter)->draw() ;

    // right rear
    // from left back, move over -y 2*track
    fromCar._42 -= 2*track ;  // -y translation entry.
    
    concat = fromCar *rotZ* trans ;
    window->setWorld( &concat ) ;
    foreach( vector<ObjFile*>::iterator, iter, tireModels )
      (*iter)->draw() ;

    // right front
    // from right rear, move forward (+x) wheelbase
    fromCar._41 += wheelbase ;  // +x translation entry.
    
    concat = fromCar *rotZ* trans ;
    window->setWorld( &concat ) ;
    foreach( vector<ObjFile*>::iterator, iter, tireModels )
      (*iter)->draw() ;
    
    
  }
} ;

#endif