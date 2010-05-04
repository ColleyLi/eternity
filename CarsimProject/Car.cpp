#include "Car.h"

Car::Car()
{
  
}

Car::~Car()
{
  foreach( VectorObjIter, iter, models )
    delete( *iter ) ;

  foreach( VectorObjIter, iter, tireModels )
    delete( *iter ) ;
}

void Car::loadCarModel( char *filename )
{
  models.push_back( new ObjFile( window, filename ) ) ;
}

void Car::loadTireModel( char *filename )
{
  tireModels.push_back( new ObjFile( window, filename ) ) ;
}

void Car::userControlUpdate()
{
  const static double incrGas = 0.0005f, incrBrake=1.0f, incrSteer = 0.002f ;


  if( window->keyIsPressed( VK_UP ) )
  {
    CARSIM(throttle) += incrGas ;
  }
  else
  {
    CARSIM(throttle) *= 0.95 ;
  }

  if( window->keyIsPressed( VK_DOWN ) )
  {
    // These do'nt do antyhign it seems
    //CARSIM(brake) += incrBrake ;
    //CARSIM(brakeStat) += incrBrake ;
    //CARSIM(brakePedalForce) += incrBrake ;
    //CARSIM(brakeBoosterForce) += incrBrake ;
    
    // These are in N*m of torque they exert.
    CARSIM(IMP_MYBK_L1) += incrBrake ;
    CARSIM(IMP_MYBK_L2) += incrBrake ;
    CARSIM(IMP_MYBK_R1) += incrBrake ;
    CARSIM(IMP_MYBK_R2) += incrBrake ;
  }
  else
  {
    // these don't do anything it seems
    //CARSIM(brake) = 0 ;
    //CARSIM(brakeStat) = 0 ;
    //CARSIM(brakePedalForce) = 0 ;
    //CARSIM(brakeBoosterForce) = 0 ;


    CARSIM(IMP_MYBK_L1) = 0 ;
    CARSIM(IMP_MYBK_L2) = 0 ;
    CARSIM(IMP_MYBK_R1) = 0 ;
    CARSIM(IMP_MYBK_R2) = 0 ;
  }



  // Steer right is - angle.  left is + angle
  if( window->keyIsPressed( VK_RIGHT ) || window->keyIsPressed( VK_LEFT ) )
  {
    if( window->keyIsPressed( VK_RIGHT ) )
    {
      CARSIM(steeringAngle) -= incrSteer ;
    }
    else
    {
      CARSIM(steeringAngle) += incrSteer ;
    }
  }
  else //not moving the wheel, so bring to center
    CARSIM(steeringAngle) *= 0.999 ; // bring the wheel towards 0
  // this rate could be made 
  // proportional to velocity but we didn't here


  //////////////////
  /////// JOYSTICK CONTROL
  if( window->gamepadIsPresent( Gamepad::One ) )
  {
    
  }


  //////////
  // CLAMP.
  // Having more than 1.0 throttle doens't make sense
  clamp( CARSIM(throttle), 0.0, 1.0 ) ;


  // Must have this else you can
  // make the model become unstable (by turning
  // the wheels around in a full circle
  //clamp( CARSIM(steeringAngle), -45.0, 45.0 ) ;

  // So realisitcally the steering wheel rotates
  // 2 and a quarter turns, so that's
  // 2*(2*pi)+ (pi/2) = 4.5*pi
  clamp( CARSIM(steeringAngle), -4.5*D3DX_PI, 4.5*D3DX_PI ) ;

}

void Car::update( double timeSinceLastFrame )
{
  // User control
  userControlUpdate() ;


  // update the pitch of the engine
  double rpmMax = 800 ;
  double rpmMin = 75 ;

  // scale the thing from 0.5 at low speed 75 rpm
  // to 2.0 at 800 rpm

  double normalizedRPM = CARSIM(tachAngle)/(rpmMax-rpmMin) ;
  double rpmScale = 0.5 + (2.0-0.5)*normalizedRPM ;

  window->setPitchShift( rpmScale ) ;

  if( fabs(CARSIM(betaSideSlip)) > 0.5 )
  {
    // car is slipping.
    if( !window->isPlaying( Sounds::Screech ) )
    {
      window->playSound( Sounds::Screech ) ;
    }
  }
  else
  {
    // not slipping, so stop the skid sound
    window->stopSound( Sounds::Screech ) ;
  }
  


  
  // COMPUTE NEXT STATE UPDATE TO CARSIM.

}

void Car::drawStats()
{
  window->setDrawingMode( D2 ) ; // 2d
  char buf[300];
  
  D3DCOLOR color= D3DCOLOR_XRGB(255,255,255) ;

  int y = 20 ;
  sprintf( buf, "pos: %.2f %.2f %.2f",
    CARSIM(x), CARSIM(y), CARSIM(z) ) ;
  window->drawString(
    Arial16, buf, color,
    20, y+=20, 600, 600,
    DT_LEFT | DT_TOP ) ;

  sprintf( buf, "vel: %.2f %.2f %.2f",
    CARSIM(vx), CARSIM(vy), CARSIM(vz)  ) ;
  window->drawString(
    Arial16, buf, color,
    20, y+=20, 200, 200,
    DT_LEFT | DT_TOP ) ;

  /*
  sprintf( buf, "acc: %.2f %.2f %.2f",
    CARSIM(ax), CARSIM(ay), CARSIM(az)  ) ;
  window->drawString(
    Arial16, buf, color,
    20, y+=20, 200, 200,
    DT_LEFT | DT_TOP ) ;
  */

  sprintf( buf, "yaw: %.2f pitch: %.2f roll: %.2f",
    CARSIM(yaw), CARSIM(pitch), CARSIM(roll) ) ;
  window->drawString(
    Arial16, buf, color,
    20, y+=20, 600, 600,
    DT_LEFT | DT_TOP ) ;

  sprintf( buf, "power: %.2f tach: %.2f speedo: %.2f side-slip: %.2f",
    CARSIM(PwrEngAv),
    CARSIM( tachAngle ),
    CARSIM( speedoAngle ),
    CARSIM( betaSideSlip )
  ) ;
  window->drawString(
    Arial16, buf, color,
    20, y+=20, 600, 600,
    DT_LEFT | DT_TOP ) ;



  
  sprintf( buf, "throttle: %.2f brake: %.2f steer: %.2f",
    CARSIM(throttle), 
    CARSIM(brake), CARSIM(steeringAngle) ) ;
  window->drawString(
    Arial16, buf, color,
    20, y+=20, 600, 600,
    DT_LEFT | DT_TOP ) ;

  sprintf( buf, "theirThrottle=%.2f theirSteering: %.2f",
    CARSIM(theirThrottle), CARSIM(theirSteeringAngle) ) ;
  window->drawString(
    Arial16, buf, color,
    20, y+=20, 600, 600,
    DT_LEFT | DT_TOP ) ;

  /*
  // these don't do naything
  sprintf( buf, "brake %2.f brakeStat %2.f brakePedal %.2f boost %.2f",
    CARSIM(brake),
    CARSIM(brakeStat),
    CARSIM(brakePedalForce),
    CARSIM(brakeBoosterForce)
  ) ;
  window->drawString(
    Arial16, buf, color,
    20, y+=20, 600, 600,
    DT_LEFT | DT_TOP ) ;
  */






  sprintf( buf, "brake l1: %2.f l2: %2.f r1: %.2f r2: %.2f",
    CARSIM(IMP_MYBK_L1),
    CARSIM(IMP_MYBK_L2),
    CARSIM(IMP_MYBK_R1),
    CARSIM(IMP_MYBK_R2)
  ) ;
  window->drawString(
    Arial16, buf, color,
    20, y+=20, 600, 600,
    DT_LEFT | DT_TOP ) ;






  // Draw the tach and speedometer
  


  // gas amt



  y = 20 ;

  D3DXVECTOR3 look = window->getCamera()->getLook() ;
  D3DXVECTOR3 right = window->getCamera()->getRight() ;
  D3DXVECTOR3 up = window->getCamera()->getUp() ;




  // Draw the eye of the camera
  // let's see what the vectors are
  sprintf( buf, "cam look=(%.2f, %.2f, %.2f)\n"
    "right=(%.2f, %.2f, %.2f)\n"
    "up=(%.2f, %.2f, %.2f)",
    
    look.x, look.y, look.z,
    right.x, right.y, right.z,
    up.x, up.y, up.z ) ;
    

  window->drawString( Fonts::Arial16, buf,
    color,
    400, y+=20, 200, 600, DT_TOP | DT_LEFT ) ;




  
}

void Car::draw( double timeSinceLastFrame )
{

  window->setDrawingMode( D3 ) ;
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
  // to where they really are.
  D3DXMATRIX mTrans ;
  D3DXMatrixTranslation( &mTrans,
    CARSIM(x), CARSIM(y), CARSIM(z) ) ;

  // Rotate the vehicle 
  // yaw pitch roll from CarSim.
  D3DXMATRIX mRpy ;
  
  /*
D3D thinks:
Yaw [in]
Yaw around the y-axis, in radians.

Pitch [in]
Pitch around the x-axis, in radians.

Roll [in]
Roll around the z-axis, in radians.
  */
  D3DXMatrixRotationYawPitchRoll(
    
    &mRpy,
    
    // Carsim values are DEGREES
    // so get radians out of them
    CARSIM(pitch), // pitch on z axis
    CARSIM(roll), // roll about the x axis
    CARSIM(yaw) // this is our yaw..
    // the z-axis is 
  ) ;

  // Vehicle's concatenated world
  // transformation matrix.
  D3DXMATRIX vConcatWorld ;

  // Rotate first, then translate.
  vConcatWorld = mRpy * mTrans ;

  // Use it
  window->setWorld( &vConcatWorld ) ;



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






  D3DXMATRIX concatWheel ;
  D3DXMatrixIdentity( &concatWheel ) ;

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
  D3DXMATRIX mRightWheelRot, mFrontWheelRot, mWheelSpeedRot ;

  // We have to rotate the right-side wheels 180 degrees
  // this is still done
  // in the Z-axis because the vehicle hasn't been
  // pitched yet.  the vConcat matrix will take care of all of that.
  D3DXMatrixRotationZ( &mRightWheelRot, D3DX_PI ) ;

  // rotate the front wheels according
  // to the steering angle.
  // 
  D3DXMatrixRotationZ(
    &mFrontWheelRot, 
    CARSIM (wheelL1Steer)  // they are both the same
  ) ;


  // The change to the vehicle wheel will be
  // the rotational distance travelled
  //   - assuming 17" tires, in metres this is 0.43 m.
  //     radsRot = arcRot / r

  // arcLen travelled on tires (outer circle) is proportional to vel
  /*
  D3DXVECTOR3 distStep = vel*timeSinceLastFrame ; // distance wheel travelled on outer circle
  float arcLenTravelled = D3DXVec3Length( &distStep ) ;
  float radsRot = arcLenTravelled / 0.43f ; // just keep it @ 0.43

  static float wheelSpeedAngle = 0.0f ;
  wheelSpeedAngle += radsRot ;
  
  D3DXMatrixRotationY( &wheelSpeedRot, wheelSpeedAngle ) ;
  */

  D3DXMatrixIdentity( &mWheelSpeedRot ) ;

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

  D3DXMATRIX mFromCar ;
  D3DXMatrixIdentity( &mFromCar ) ; // init with identity

  //////////
  // LEFT FRONT
  // Only needs to move left, which is +y for stock model.
  mFromCar._42 += track ;  // +y translation entry.

  concatWheel = mWheelSpeedRot* mFrontWheelRot* mFromCar *vConcatWorld ;
  window->setWorld( &concatWheel ) ;

  // Draw a tire.
  foreach( VectorObjIter, iter, tireModels )
    (*iter)->draw() ;

  ////////
  // LEFT REAR
  // from front left, move back (which is -x)
  mFromCar._41 -= wheelbase ;  // -x translation entry.

  concatWheel = mWheelSpeedRot* mFromCar *vConcatWorld ;
  window->setWorld( &concatWheel ) ;
  foreach( VectorObjIter, iter, tireModels )
    (*iter)->draw() ;

  ///////
  // RIGHT REAR
  // from left back, move over -y 2*track
  mFromCar._42 -= 2*track ;  // -y translation entry.

  concatWheel = mRightWheelRot* mWheelSpeedRot* mFromCar *vConcatWorld ;
  window->setWorld( &concatWheel ) ;
  foreach( VectorObjIter, iter, tireModels )
    (*iter)->draw() ;


  ///////
  // RIGHT FRONT
  // from right rear, move forward (+x) wheelbase
  mFromCar._41 += wheelbase ;  // +x translation entry.

  concatWheel = mRightWheelRot* mWheelSpeedRot* mFrontWheelRot* mFromCar *vConcatWorld ;
  window->setWorld( &concatWheel ) ;
  foreach( VectorObjIter, iter, tireModels )
    (*iter)->draw() ;


}


D3DXVECTOR3 Car::getPos()
{
  return D3DXVECTOR3( CARSIM(x), CARSIM(y), CARSIM(z) ) ;
}



D3DXVECTOR3 Car::getFwd()
{
  // Compute the forward vector as YAW rotation
  // of (1,0,0) about Z axis

  // Actually same op as for orienting
  // the vehicle itself.

  D3DXMATRIX mRpy ;

  D3DXMatrixRotationYawPitchRoll(
    
    &mRpy,
    
    // Carsim values are DEGREES
    // so get radians out of them
    CARSIM(pitch), // pitch on z axis
    CARSIM(roll), // roll about the x axis
    CARSIM(yaw) // this is our yaw..
    // the z-axis is 
  ) ;


  D3DXVECTOR3 r(1,0,0) ;
  
  D3DXVec3TransformNormal( &r, &r, &mRpy ) ;

  return r ;
}