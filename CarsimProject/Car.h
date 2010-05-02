#ifndef CAR_H
#define CAR_H

#include "cs-main.h"

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

  
  float motorForce ; // engine power
  float mass ; // vehicle mass
  float speed ; // a lot of thigns use the magnitude of
  // the velocity, so cache the value here every frame
  
  float frontWheelAngle ; // angle front wheels are rotated at wrt their axes of rotation

  vector<ObjFile *> models ;
  vector<ObjFile *> tireModels ;

  Car()
  {
    // zero out pos
    ZeroVector( pos ) ;
    ZeroVector( fwd ) ;
    ZeroVector( up ) ;
    ZeroVector( vel ) ;
    ZeroVector( accelForce ) ;

    // up is +y  
    up.y = 1 ;

    // when the models load,
    // fwd is +x.
    fwd.x = 1 ;

    pos.x = -5 ; // start it "back" 5m
    vel.x = 0.2f ;  // velocity slowly rolling fwd

    motorForce = 120 ;
    mass = 1000 ; // 1000 kg mass


    frontWheelAngle = 0 ;
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

  // The right vector is simply the cross product
  // of the current FWD and UP vectors
  D3DXVECTOR3 getRight()
  {
    D3DXVECTOR3 right ;
    D3DXVec3Cross( &right, &fwd, &up ) ; // RH
    return right ;
  }

  void userControlUpdate()
  {
    if( window->keyIsPressed( VK_UP ) )
      vel.x += 0.05f ;
    if( window->keyIsPressed( VK_DOWN ) )
      vel.x -= 0.05f ;
   

    if( window->keyIsPressed( VK_RIGHT ) )
      frontWheelAngle -= 0.01f ;
    else if( window->keyIsPressed( VK_LEFT ) )
      frontWheelAngle += 0.01f ;
    else
    {
      // bring the wheel towards 0
      // this rate could be made 
      // proportional to velocity but we didn't here
      frontWheelAngle *= 0.75f ;
    }
  }

  void update( double timeSinceLastFrame )
  {
    speed = D3DXVec3Length( &vel ) ;

    // User control
    userControlUpdate() ;
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

  void draw( double timeSinceLastFrame )
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

    float steeringAngle = 0 ;

    // compute the facing angle
    // using the look at formula
    //D3DXVECTOR3 e( -10, 0, 0 ), look( 0, 0, 0 ), u( 1, 0, 0 ) ;

    //D3DXMATRIX fMat ;
    
    //D3DXMatrixLookAtRH( &fMat, &pos, &look, &u ) ;
    //fMat = lookAt( e, look, up ) ;
    //D3DXMATRIX concat = fMat ;

    
    D3DXMATRIX rotZ ;
    D3DXMatrixRotationZ( &rotZ, steeringAngle ) ;
    
    // Matrix mults applied in order they appear left to right.
    // ROTATE first, then translate
    //D3DXMATRIX concat = rotZ * trans ;

    // Try setting right, up

    D3DXMATRIX concat ;
    D3DXMatrixIdentity( &concat ) ;

    fwd = D3DXVECTOR3( 1, 1, 0 ) ;
    D3DXVec3Normalize( &fwd, &fwd ) ;

    up = D3DXVECTOR3( 0, 0, 1 ) ;
    D3DXVec3Normalize( &up, &up ) ;

    D3DXVECTOR3 right = getRight() ;
    D3DXVec3Normalize( &right, &right ) ;
    
    

    /*
    D3DXVECTOR3 r(right), u(up), f(fwd) ;
    float d = D3DXVec3Dot( &up, &fwd ) ;
    u = up - d*fwd ;
    D3DXVec3Normalize( &u, &u ) ;

    D3DXVec3Cross( &r, &u, &f ) ;

    // An article lists this matrix as being

    // right
    // up
    // fwd

    // but that didn't actually work correctly.
    D3DXMatrixSetRow( &concat, 0, &f ) ;
    D3DXMatrixSetRow( &concat, 1, &r ) ;
    D3DXMatrixSetRow( &concat, 2, &u ) ;
    D3DXMatrixSetRow( &concat, 3, &pos ) ; // bottom row is translation
    */
    D3DXMatrixPointTowards( &concat, &fwd, &up, &pos ) ;

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

    // Front wheel rotation is an additional
    // rotation about up vector really
    D3DXMATRIX frontWheelRot, wheelSpeedRot ;
    D3DXMatrixRotationZ( &frontWheelRot, frontWheelAngle ) ;

    // The change to the vehicle wheel will be
    // the rotational distance travelled
    //   - assuming 17" tires, in metres this is 0.43 m.
    //   radsRot = arcRot / r
    
    // arcLen travelled on tires is proportional to vel
    D3DXVECTOR3 velStep = vel*timeSinceLastFrame ;
    float arcLenTravelled = D3DXVec3Length( &velStep ) ;
    float radsRot = arcLenTravelled / 0.43f ; // just keep it @ 0.43

    static float wheelSpeedAngle = 0.0f ;
    wheelSpeedAngle += radsRot ;
    D3DXMatrixRotationY( &wheelSpeedRot, wheelSpeedAngle ) ;

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

    // 1.  rotate in +y for forward velocity
    // 2.  (if front wheel) rotate to take into acct steering direction
    // 3.  translate fromCar
    // 4.  rotate to get with vehicle direction
    // 5.  translate by eye
    
    concat = wheelSpeedRot* frontWheelRot* fromCar *rotZ* trans ;
    window->setWorld( &concat ) ;

    // Draw a tire.
    foreach( vector<ObjFile*>::iterator, iter, tireModels )
      (*iter)->draw() ;

    ////////
    // LEFT REAR
    // from front left, move back (which is -x)
    fromCar._41 -= wheelbase ;  // -x translation entry.
    
    concat = wheelSpeedRot* fromCar *rotZ* trans ;
    window->setWorld( &concat ) ;
    foreach( vector<ObjFile*>::iterator, iter, tireModels )
      (*iter)->draw() ;

    ///////
    // RIGHT REAR
    // from left back, move over -y 2*track
    fromCar._42 -= 2*track ;  // -y translation entry.
    
    concat = wheelSpeedRot* fromCar *rotZ* trans ;
    window->setWorld( &concat ) ;
    foreach( vector<ObjFile*>::iterator, iter, tireModels )
      (*iter)->draw() ;


    ///////
    // RIGHT FRONT
    // from right rear, move forward (+x) wheelbase
    fromCar._41 += wheelbase ;  // +x translation entry.
    
    concat = wheelSpeedRot* frontWheelRot* fromCar *rotZ* trans ;
    window->setWorld( &concat ) ;
    foreach( vector<ObjFile*>::iterator, iter, tireModels )
      (*iter)->draw() ;
    
    
  }
} ;

#endif