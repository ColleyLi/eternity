#include "Car.h"

Car::Car()
{
  // zero out pos
  ZeroVector( pos ) ;
  ZeroVector( fwd ) ;
  ZeroVector( up ) ;
  ZeroVector( vel ) ;
  ZeroVector( accelForce ) ;

  // up is +z
  up.z = 1 ;

  // when the models load,
  // fwd is +x.
  fwd.x = 1 ;

  pos.x = -5 ; // start it "back" 5m
  vel.x = 0.2f ;  // velocity slowly rolling fwd

  forceMotor = 5 ;
  forceBrake = 5 ;
  forceSteer = 8 ;

  mass = 1000 ; // 1000 kg mass

  onGas = onBrake = 0.0f ;
  frontWheelAngle = 0 ;
}

Car::~Car()
{
  foreach( VectorObjIter, iter, models )
    delete( *iter ) ;

  foreach( VectorObjIter, iter, tireModels )
    delete( *iter ) ;
}

void Car::loadModel( char *filename )
{
  // REQUIRES:  already opened file,
  // with srcFile POINTING AT START POINT
  // of vertex set

  models.push_back( new ObjFile( window, filename ) ) ;
}

void Car::loadTireModel( char *filename )
{
  // REQUIRES:  already opened file,
  // with srcFile POINTING AT START POINT
  // of vertex set

  tireModels.push_back( new ObjFile( window, filename ) ) ;
}

// The right vector is simply the cross product
// of the current FWD and UP vectors
D3DXVECTOR3 Car::getRight()
{
  D3DXVECTOR3 right ;
  D3DXVec3Cross( &right, &fwd, &up ) ; // RH
  return right ;
}

void Car::userControlUpdate()
{
  const static float incrGas = 0.05f, incrBrake=0.04f, incrSteer = 0.02f ;

  if( window->keyIsPressed( VK_UP ) )
  {
    // make engine sound, pitch proporitonal to speed
    onGas += incrGas ;
  }
  else
  {
    // make idle sound, pitch proporitonal to speed
    onGas = 0 ;
  }

  if( window->keyIsPressed( VK_DOWN ) )
    onBrake += incrBrake ;
  else
    onBrake = 0 ;

  clamp( onGas, 0.0f, 1.0f ) ;
  clamp( onBrake, 0.0f, 1.0f ) ;

  // Steer right is - angle.  left is + angle
  if( window->keyIsPressed( VK_RIGHT ) || window->keyIsPressed( VK_LEFT ) )
  {
    if( window->keyIsPressed( VK_RIGHT ) )
      frontWheelAngle -= incrSteer ;
    else
      frontWheelAngle += incrSteer ;
  }
  else //not moving the wheel, so bring to center
    frontWheelAngle *= 0.85f ; // bring the wheel towards 0
  // this rate could be made 
  // proportional to velocity but we didn't here

  clamp( frontWheelAngle, -D3DX_PI/4, D3DX_PI/4 ) ;

}

void Car::mySim( double timeSinceLastFrame )
{
  
  // Now, the magnitude of participation of accelSteerForce
  // depends on the sin() of the angle
  //float forceSteerNow = speed * forceSteer * fabsf(sinf( frontWheelAngle ) ); // if angle is 0,
  // then the force is 0.  fabsf because it "turns around" the vector
  // 180 degrees if its steering to the right / left.  + to left, - to right.



  // (onModifier)*(maxForce)*directionvector
  D3DXVECTOR3 accelFwdForce = onGas * forceMotor * fwd ;  // if not on gas, this goes to 0.
  D3DXVECTOR3 accelBrakeForce = onBrake * forceBrake * -fwd ; // if not on brake, this goes to 0
  D3DXVECTOR3 steerDir = fwd ;

  // accelSteerForce is in the direction of
  // -FWD ROTATED ABOUT UP by frontWheelAngle radians.
  D3DXMATRIX rotM ;
  D3DXMatrixIdentity( &rotM ) ;
  D3DXMatrixRotationAxis( &rotM, &up, frontWheelAngle ) ;

  //D3DXVECTOR4 accelSteerForce4 ;
  //D3DXVec3Transform( &accelSteerForce4, &accelSteerForce, &rotM ) ;
  //accelSteerForce = D3DXVECTOR3( accelSteerForce4 ) ;

  // !! DOUBLE CHECK THIS WORKS EXACTLY AS D3DXVec3Transform()
  D3DXVec3TransformNormal( &steerDir, &steerDir, &rotM ) ;

  // Now the force due to steering is perpendicular
  // it is steerDir x UP

  D3DXVECTOR3 accelSteerForce ;
  D3DXVec3Cross( &accelSteerForce, &steerDir, &up ) ;

  // Now, the magnitude of the force then
  // is proportional to sin(steeringAngle).
  // when steering angle is 0, there is no
  // accelSteerForce.
  accelSteerForce *= -(sinf( frontWheelAngle )) * forceSteer ;









  // Compute cumulative acceleration force
  accelForce = accelFwdForce + accelBrakeForce + accelSteerForce ;


  // Sum of acceleration forces exerted by vehicle
  // limited by the capabilities of the vehicles tires

  float maxMag = 5.0f ;

  float mag2 = D3DXVec3LengthSq( &accelForce ) ;

  if( mag2 > maxMag*maxMag )
  {
    // can't exert more than this much force
    // This is that "circle" model that paper was talking about
    // Slippage
    // !!make sound if slippage is occurring. etc
    window->playSound( Sounds::Screech ) ;
    D3DXVec3SetMagnitude( &accelForce, maxMag ) ;
  }
  else
  {
    // stop the screech
    window->stopSound( Sounds::Screech ) ;
  }




  // The accelerating force
  // alters the velocity of the vehicle
  // Change velocity
  vel += accelForce * timeSinceLastFrame ;
  pos += vel*timeSinceLastFrame ;

  // The vehicle has to "try" to
  // make its forward vector
  // line up with its velocity vector.
  fwd += vel ;
  D3DXVec3Normalize( &fwd, &fwd ) ;

  // Update position from Carsim,







}

void Car::update( double timeSinceLastFrame )
{
  speed = D3DXVec3Length( &vel ) ;

  // User control
  //userControlUpdate() ; // turned off
}

void Car::draw( double timeSinceLastFrame )
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

  // compute the facing angle
  // using the look at formula
  D3DXMATRIX cRot ;



  D3DXMatrixPointTowards( &cRot, &fwd, &up, &pos ) ;

  // Use it
  window->setWorld( &cRot ) ;

  // Draw the whole model with this matrix set
  foreach( VectorObjIter, iter, models )
  {
    (*iter)->draw() ;
  }



  // Now for the tires, we'll have to compute
  // a different matrix


  // 1.  rotate in +y for forward velocity
  // 2.  (if front wheel) rotate about vehicle UP vector to take into acct steering direction
  // 3.  translate fromCar

  // THen, the rest: (apply cRot)
  // 4.  rotate to get with vehicle direction
  // 5.  translate by eye


  D3DXMATRIX concat ;
  D3DXMatrixIdentity( &concat ) ;


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
  D3DXMatrixRotationZ( &frontWheelRot, frontWheelAngle ) ;// this is still done
  // in the Z-axis because the vehicle hasn't been
  // pitched yet.  the cRot matrix will take care of all of that.

  // The change to the vehicle wheel will be
  // the rotational distance travelled
  //   - assuming 17" tires, in metres this is 0.43 m.
  //     radsRot = arcRot / r

  // arcLen travelled on tires (outer circle) is proportional to vel
  D3DXVECTOR3 distStep = vel*timeSinceLastFrame ; // distance wheel travelled on outer circle
  float arcLenTravelled = D3DXVec3Length( &distStep ) ;
  float radsRot = arcLenTravelled / 0.43f ; // just keep it @ 0.43

  static float wheelSpeedAngle = 0.0f ;
  wheelSpeedAngle += radsRot ;
  D3DXMatrixRotationY( &wheelSpeedRot, wheelSpeedAngle ) ;

  /// Remember, vehicle points in +x
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

  concat = wheelSpeedRot* frontWheelRot* fromCar *cRot ;
  window->setWorld( &concat ) ;

  // Draw a tire.
  foreach( VectorObjIter, iter, tireModels )
    (*iter)->draw() ;

  ////////
  // LEFT REAR
  // from front left, move back (which is -x)
  fromCar._41 -= wheelbase ;  // -x translation entry.

  concat = wheelSpeedRot* fromCar *cRot ;
  window->setWorld( &concat ) ;
  foreach( VectorObjIter, iter, tireModels )
    (*iter)->draw() ;

  ///////
  // RIGHT REAR
  // from left back, move over -y 2*track
  fromCar._42 -= 2*track ;  // -y translation entry.

  concat = wheelSpeedRot* fromCar *cRot ;
  window->setWorld( &concat ) ;
  foreach( VectorObjIter, iter, tireModels )
    (*iter)->draw() ;


  ///////
  // RIGHT FRONT
  // from right rear, move forward (+x) wheelbase
  fromCar._41 += wheelbase ;  // +x translation entry.

  concat = wheelSpeedRot* frontWheelRot* fromCar *cRot ;
  window->setWorld( &concat ) ;
  foreach( VectorObjIter, iter, tireModels )
    (*iter)->draw() ;


}