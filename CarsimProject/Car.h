#ifndef CAR_H
#define CAR_H

#include "cs-main.h"

typedef vector<ObjFile*> /* as simply */ VectorObj ;
typedef vector<ObjFile*>::iterator /* as simply */ VectorObjIter ;

/// Represents a carsim car
class Car
{
public:
  D3DXVECTOR3 pos ; // eye
  D3DXVECTOR3 fwd ; // the forward vector to the vehicle
  D3DXVECTOR3 up ;  // the vehicle's up vector may change
  // as it goes up inclines, etc.

  D3DXVECTOR3 vel ; // actual vehicle motion velocity.  not necessarily aligned with fwd.

  D3DXVECTOR3 accelForce ; // the accelerating force
  // on the vehicle.  It is ideally in the direction the tires
  // are facing, with a force from the motor.

  // two signals to say how much he's on the gas and brake
  // each between 0 and 1
  float onGas, onBrake ;

  // the analog control stick lets us control this

  // engine power, braking power, steering power from wheels due to friction
  float forceMotor, forceBrake, forceSteer ; 
  float mass ; // vehicle mass
  float speed ; // a lot of thigns use the magnitude of
  // the velocity, so cache the value here every frame

  float frontWheelAngle ; // angle front wheels are rotated at wrt their axes of rotation

  VectorObj models ;
  VectorObj tireModels ;

  Car() ;

  ~Car() ;

  void loadModel( char *filename ) ;
  void loadTireModel( char *filename ) ;

  // The right vector is simply the cross product
  // of the current FWD and UP vectors
  D3DXVECTOR3 getRight() ;

  void userControlUpdate() ;

  // my test sim function
  void mySim( double timeSinceLastFrame ) ;

  void update( double timeSinceLastFrame ) ;

  void draw( double timeSinceLastFrame ) ;
} ;

#endif