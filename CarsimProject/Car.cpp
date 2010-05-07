#include "Car.h"

double Car::steeringWheelMaxR = 4.5*D3DX_PI;
double Car::sNominalLookAheadAmount = 1.0 ;
double Car::sNominalLookFarAheadAmount = 1.5 ;

Car::Car()
{
  simStepsFrame = 1 ;

  drawDebugLines = false ;

  drawDebugText = true ;

  hidden = false ;

  manualControl = false ; // start with automatic control on

  lapTime = 0.0 ;

  sLast = sNearest = 0.0 ;

  sprintf( lapTimesString, "First lap" ) ;
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
  const static double incrGas = 0.0005f, incrBrake=10000, incrSteer = 0.002f ;


  if( window->keyIsPressed( VK_UP ) )
  {
    CARSIM(throttle) += incrGas ;
  }
  else
  {
    if( !window->keyIsPressed( VK_CONTROL ) )
      CARSIM(throttle) *= 0.95 ;
  }

  if( window->keyIsPressed( VK_DOWN ) )
  {
    // Brake needs incredible pressure (mPA).
    // This value specifies it in Pa, it seems
    CARSIM(brake) += incrBrake ;
  }
  else
  {
    // these don't do anything it seems
    CARSIM(brake) = 0 ;
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
  

/*
// not working, crashes the simulator
  // REVERSE
  if( window->keyJustPressed( '9' ) )
    CARSIM(transmissionMode) = -1 ;
  // FORWARD
  if( window->keyJustPressed( '0' ) )
    CARSIM(transmissionMode) = 2 ;
*/


  //////////
  // CLAMP.
  // Having more than 1.0 throttle doens't make sense
  clamp( CARSIM(throttle), 0.0, 1.0 ) ;

  // What's a reasonable clamp for this?
  //clamp( CARSIM(brake), 0.0, 1.0 ) ;


  // Must have this else you can
  // make the model become unstable (by turning
  // the wheels around in a full circle
  //clamp( CARSIM(steeringAngle), -45.0, 45.0 ) ;

  // So realisitcally the steering wheel rotates
  // 2 and a quarter turns, so that's
  // 2*(2*pi)+ (pi/2) = 4.5*pi
  clamp( CARSIM(steeringAngle), -steeringWheelMaxR, steeringWheelMaxR ) ;

}

void Car::update( double timeSinceLastFrame )
{
  // User control
  if( manualControl )
    userControlUpdate() ;

  updateAutomaticControlValues() ;

  // Sound
  //
  // update the pitch of the engine
  double rpmMax = 800 ;
  double rpmMin = 75 ;

  // scale the thing from 0.5 at low speed 75 rpm
  // to 2.0 at 800 rpm
  rpmNormalized = CARSIM(tach)/(rpmMax-rpmMin) ;
  double rpmScale = 0.5 + (2.0-0.5)*rpmNormalized ;

  window->setPitchShift( rpmScale ) ;
  window->setSoundPosition( EngineMidIn, (FMOD_VECTOR*)&pos, (FMOD_VECTOR*)&vel ) ;
  

  if( fabs(CARSIM(betaSideSlip)) > 0.5 )
  {
    // car is slipping quite a bit
    if( !window->isPlaying( Sounds::Screech ) )
    {
      window->playSound( Sounds::Screech ) ;
    }
    window->setSoundPosition( Screech, (FMOD_VECTOR*)&pos, (FMOD_VECTOR*)&vel ) ;
  }
  else
  {
    // not slipping, so stop the skid sound
    window->stopSound( Sounds::Screech ) ;
  }
}

void Car::drawOutputString( char *str, D3DCOLOR color, float x, float y )
{
  window->drawString(
    Arial16,
    str,
    color,
    x, y,
    window->getWidth()-x, window->getHeight()-y,
    DT_LEFT | DT_TOP ) ;

}

void Car::drawStats()
{


  char buf[300];
  
  D3DCOLOR color= D3DCOLOR_XRGB(255,255,255) ;

  if( drawDebugText )
  {
    #pragma region text

    #define PVEC(theVec) theVec.x, theVec.y, theVec.z
    
    window->setDrawingMode( D2 ) ; // 2d

    int y = 0 ;

    sprintf( buf, "%.2f", simStepsFrame ) ;
    window->drawString( TimesNewRoman24, buf, Color::White,
      window->getWidth()-60, 20, 60, 20, DT_CENTER|DT_VCENTER ) ;

    sprintf( buf, "%.2f", lapTime ) ;
    window->drawString( Arial16, buf, Color::White,
      window->getWidth()-60, 45, 60, 20, DT_CENTER|DT_VCENTER ) ;

    

    /*
    // Position.  Not very useful for evaluating the controller.
    sprintf( buf, "pos: (%.2f, %.2f, %.2f)",
      CARSIM(x), CARSIM(y), CARSIM(z) ) ;
    window->drawString(
      Arial16, buf, color,
      20, y+=20, 600, 600,
      DT_LEFT | DT_TOP ) ;
    */

    // AUTOMATIC OR MANUAL CONTROL
    drawOutputString(
      manualControl?"MANUAL CONTROL":"AUTOMATIC CONTROL",
      manualControl?D3DCOLOR_XRGB(255,255,0):color,
      20, y+=20 ) ;

    sprintf( buf, "brake: %.2f [Mpa]", CARSIM(brake)/1e6 ) ;
    drawOutputString( buf, Color::AliceBlue,
      20, y+=20 ) ;

    /*
    // Speed is displayed, so components aren't
    // always useful
    sprintf( buf, "vel: %.2f %.2f %.2f",
      CARSIM(vx), CARSIM(vy), CARSIM(vz)  ) ;
    drawOutputString(
      buf, color,
      20, y+=20 ) ;
    */

    /*
    // accelerations aren't always useful either
    sprintf( buf, "acc: %.2f %.2f %.2f",
      CARSIM(ax), CARSIM(ay), CARSIM(az)  ) ;
    drawOutputString(
      buf, color,
      20, y+=20 ) ;
    */

    /*
    // Shown visually
    sprintf( buf, "yaw: %.2f pitch: %.2f roll: %.2f",
      CARSIM(yaw), CARSIM(pitch), CARSIM(roll) ) ;
    drawOutputString(
      buf, color,
      20, y+=20 ) ;
    */

    /*
    sprintf( buf, "power: %.2f tach: %.2f",
      CARSIM( tachAngle ),
      CARSIM( speedoAngle )
    ) ;
    drawOutputString(
      Arial16, buf, color,
      20, y+=20 ) ;
    */

    /*
    sprintf( buf, "slip=%.2f powerAv=%.2f",
      CARSIM( betaSideSlip ),
      CARSIM( PwrEngAv )
    ) ;
    drawOutputString(
      buf, color,
      20, y+=20 ) ;
    */

    sprintf( buf, "tire spin:\n%.2f %.2f\n%.2f %.2f",
      CARSIM( slipLongL1 ),
      CARSIM( slipLongR1 ),
      CARSIM( slipLongL2 ),
      CARSIM( slipLongR2 )
    ) ;
    drawOutputString(
      buf, color,
      20, y+=20 ) ;

    y+=40 ; // because we used two \n above

    sprintf( buf, "lat slip:\n%.2f %.2f\n%.2f %.2f",
      CARSIM( slipLatL1 ),
      CARSIM( slipLatR1 ),
      CARSIM( slipLatL2 ),
      CARSIM( slipLatR2 )
    ) ;
    drawOutputString(
      buf, color,
      20, y+=20 ) ;

    y+=40 ; // because we used two \n above


    /*
    sprintf( buf, "theirThrottle=%.2f theirSteering: %.2f",
      CARSIM(theirThrottle), CARSIM(theirSteeringAngle) ) ;
    drawOutputString(
      buf, color,
      20, y+=20 ) ;
    */

    /*
    // These were relevant earlier
    sprintf( buf, "L %.2f curv %.6f curvA %.6f curvFA %.6f",
      L,
      sCurvature,
      sCurvatureAhead,
      sCurvatureFarAhead
    ) ;
    drawOutputString(
      buf, color,
      20, y+=20 ) ;
    */

    switch( speedCurveState )
    {
    case SpeedCurveState::Chicane:
      sprintf( buf, "Chicane" ) ;
      color = Color::Red ;
      break;

    case SpeedCurveState::CurveIntoStraight:
      sprintf( buf, "CurveIntoStraight" ) ;
      color = Color::Orange ;
      break;

    case SpeedCurveState::IntoSharpCurve:
      sprintf( buf, "IntoSharpCurve" ) ;
      color = Color::Red ;
      break;

    case SpeedCurveState::OnCurve:
      sprintf( buf, "OnCurve" ) ;
      color = Color::Aqua ;
      break;

    case SpeedCurveState::StraightAway:
      sprintf( buf, "StraightAway" ) ;
      color = Color::Green ;
      break;

    case SpeedCurveState::StraightIntoGentleCurve:
      sprintf( buf, "StraightIntoGentleCurve" ) ;
      color = Color::Blue ;
      break;

    case SeeFarAheadCurve:
      sprintf( buf, "SeeFarAheadCurve" ) ;
      color = Color::Orange ;
    }

    drawOutputString(
      buf, color,
      20, y+=20 ) ;

    

    /*
    sprintf( buf, "curv@S=%.6f, curv@SA=%.6f |SA-A|=%.6f SMax=%.6f", // targetH=%.2f targetHL=%.2f",
      sCurvature, sCurvatureAhead,
      fabs(sCurvatureAhead-sCurvature),
      sCurvatureMaxSoFar  //, sTargetHeading, sTargetHeadingL
    ) ;
    drawOutputString(
      buf, color,
      20, y+=20 ) ;
    */

    /*
    sprintf( buf, "alongroad=(%.2f,%.2f,%.2f)",
      PVEC(alongRoad) ) ;
    drawOutputString(
      buf, color,
      20, y+=20 ) ;
    */

    
    drawOutputString(
      courseState==OnCourse?"On course":"Off course",
      courseState==OnCourse?Color::Green:Color::Red,
      20, y+=20 ) ;



    /*
    sprintf( buf, "angle: %.2f posToSNearest (%.2f, %.2f, %.2f)",
        angleBetweenFwdAndSNearest, 
        PVEC( posToSNearest )
      ) ;
    drawOutputString(
      buf, Color::Red,
      20, y+=20 ) ;
    */
    
    
    drawOutputString(
      lapTimesString, Color::Yellow,
      20, y+=20 ) ;

    //double incrBy = speedTarget - CARSIM(speedometer) ;
    sprintf( buf, "target-speed %.2f", speedTarget ) ;
    drawOutputString(
      buf,
      color,
      20, y+=20 ) ;
    #pragma endregion
  }

  #pragma region dials etc

  // gas amt
  float gas = CARSIM(throttle)*255.0f ;
  float WW = window->getWidth() ;
  float WH = window->getHeight() ;
  D3DCOLOR gasBoxColor = D3DCOLOR_ARGB( 180, 255, 35, 0 ) ;
  
  // Fill a box for how much yo'ure donw on the gas pedal
  window->drawBox( gasBoxColor, 20, window->getHeight()-20, 20, -gas ) ;
  window->drawString( Arial16, "trt", gasBoxColor,
    20, WH-20, 20, 20, DT_CENTER | DT_VCENTER ) ;




  ////////////
  // SPEED
  // Print your speed
  window->drawBox( 
    D3DCOLOR_ARGB( 180, 0,0,0 ),
    50, WH-40, 100, 40 ) ;
  sprintf( buf, "speed (%.2f)", getMaxSpeedForCurve( sCurvatureAhead ) ) ;

  window->drawString( Arial16, buf, gasBoxColor,
    50, WH-20, 100, 20, DT_CENTER|DT_VCENTER ) ;

  D3DCOLOR spColor = D3DCOLOR_ARGB( 230, 0, 255, 0 ) ;
  sprintf( buf, "%.0lf", getSpeed() ) ;
  window->drawString( TimesNewRoman24, buf, spColor,
    50, WH-40, 100, 20, DT_CENTER|DT_VCENTER ) ;




  


  /*
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
  drawOutputString( buf,
    color,
    400, y+=20 ) ;
  */



  ///////////////
  // TACH
  // Draw the tach
  float tcLen = 50.0f ;

  D3DXVECTOR2 tcC( WW-50, WH-50 ) ; //tach cneter for needle

  // Draw its background image.
  window->drawSprite( TachBack, tcC.x, tcC.y ) ;

  // draw the needles for both of these things.
  float tcAngle ;
  const float tcStartAngle = 4.0, tcEndAngle = 0.0 ; // start angle is this value.

  tcAngle = tcStartAngle + rpmNormalized*(tcEndAngle-tcStartAngle) ;

  // tachometer terminal needle point
  D3DXVECTOR2 tcT ;

  tcT.x = tcC.x + tcLen * cosf( tcAngle ) ;
  tcT.y = tcC.y - tcLen * sinf( tcAngle ) ;// y is upside down, top left is 0,0

  window->drawLine( tcC, tcT, gasBoxColor, 2.0f ) ;

  // Percent of max steering.
  float percMax = CARSIM(steeringAngle)/steeringWheelMaxR ;
  // +1 to the left, -1 to the right.
  
  BYTE bMax = abs(percMax*255) ;
  
  // Red is left, blue is right
  D3DCOLOR wheelColor = D3DCOLOR_ARGB( 180,
    percMax>0 ? bMax:0,
    0,
    percMax<0 ? bMax:0
  ) ;
  // Draw the steering wheel, right next to the tach
  window->drawSprite( SteeringWheel, tcC.x - 100,
                      tcC.y, 100, 100, -DEGREES( CARSIM(steeringAngle) ),
                      wheelColor ) ;

  #pragma endregion
}

void Car::draw( double timeSinceLastFrame )
{

  // No draw if car is hidden, just draw hidden message
  if( hidden )
  {
    window->setDrawingMode( D2 ) ;
    window->drawString( Fonts::Arial8, "car hidden (-> 'c')", D3DCOLOR_ARGB( 255, 255, 255, 0 ) ) ;
    return ;
  }

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
  */

  // Even though the file lists
  // this as RPM, I think its still
  // rad/s.
  static double wheelSpeedAngleL1 = 0.0f,
    wheelSpeedAngleL2 = 0.0f,
    wheelSpeedAngleR1 = 0.0f,
    wheelSpeedAngleR2 = 0.0f ;

  // Prevent wheelSpeedAngle quantities from getting too large
  if( wheelSpeedAngleL1 > 2*PI )
    wheelSpeedAngleL1 -= 2*PI ;
  if( wheelSpeedAngleL2 > 2*PI )
    wheelSpeedAngleL2 -= 2*PI ;
  if( wheelSpeedAngleR1 > 2*PI )
    wheelSpeedAngleR1 -= 2*PI ;
  if( wheelSpeedAngleR2 > 2*PI )
    wheelSpeedAngleR2 -= 2*PI ;



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

  // Compute wheel spin:
  // Add radians moved to wheel angle.
  wheelSpeedAngleL1 += CARSIM(wheelL1V)*timeSinceLastFrame ; // RH qty is "radiansMoved"
  D3DXMatrixRotationY( &mWheelSpeedRot, wheelSpeedAngleL1 ) ;

  concatWheel = mWheelSpeedRot* mFrontWheelRot* mFromCar *vConcatWorld ;
  window->setWorld( &concatWheel ) ;

  // Draw a tire.
  foreach( VectorObjIter, iter, tireModels )
    (*iter)->draw() ;

  ////////
  // LEFT REAR
  // from front left, move back (which is -x)
  mFromCar._41 -= wheelbase ;  // -x translation entry.

  wheelSpeedAngleL2 += CARSIM(wheelL1V)*timeSinceLastFrame ;
  D3DXMatrixRotationY( &mWheelSpeedRot, wheelSpeedAngleL2 ) ;

  concatWheel = mWheelSpeedRot* mFromCar *vConcatWorld ;
  window->setWorld( &concatWheel ) ;
  foreach( VectorObjIter, iter, tireModels )
    (*iter)->draw() ;

  ///////
  // RIGHT REAR
  // from left back, move over -y 2*track
  mFromCar._42 -= 2*track ;  // -y translation entry.

  wheelSpeedAngleR2 += CARSIM(wheelL1V)*timeSinceLastFrame ;
  D3DXMatrixRotationY( &mWheelSpeedRot, wheelSpeedAngleR2 ) ;

  concatWheel = mRightWheelRot* mWheelSpeedRot* mFromCar *vConcatWorld ;
  window->setWorld( &concatWheel ) ;
  foreach( VectorObjIter, iter, tireModels )
    (*iter)->draw() ;


  ///////
  // RIGHT FRONT
  // from right rear, move forward (+x) wheelbase
  mFromCar._41 += wheelbase ;  // +x translation entry.

  wheelSpeedAngleR1 += CARSIM(wheelL1V)*timeSinceLastFrame ;
  D3DXMatrixRotationY( &mWheelSpeedRot, wheelSpeedAngleR1 ) ;

  concatWheel = mRightWheelRot* mWheelSpeedRot* mFrontWheelRot* mFromCar *vConcatWorld ;
  window->setWorld( &concatWheel ) ;
  foreach( VectorObjIter, iter, tireModels )
    (*iter)->draw() ;


}


D3DXVECTOR3 Car::getPos()
{
  return D3DXVECTOR3( CARSIM(x), CARSIM(y), CARSIM(z) ) ;
}

D3DXVECTOR3 Car::getVel()
{
  return D3DXVECTOR3( CARSIM(vx), CARSIM(vy), CARSIM(vz) ) ;
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



/////////////////////////////////////////////
//////////////// CONTROL ////////////////////

void Car::control()
{
  // Main control loop.

  // +L means left of center line
  // -L means right of center line
 
  // +steering is steering left (CCW)
  // -steering is steering right (CW)
  
  // DETERMINE IF WE ARE ON OR OFF COURSE.
  if( fabs( L ) > 8 ) // Too far from center line to recover.
    courseState = Car::OffCourse ;
  else
    courseState = Car::OnCourse ;
  

  // If on course, we drive more aggressively
  if( courseState == Car::OnCourse )
  {
    // Speed:  Compute max speed to travel at
    driveAtMaxSpeedForCurvSAndCurvAS( 1.0 ) ;
    // 1.0=good

    // Direction:  Drive towards the NEXT
    // station's (x, y, z)
    driveTo( sAheadX, sAheadY, sAheadZ, 12.0 ) ;
    // 6.2=good, 10.2=a bit of oversteer
    // a bit of oversteer is good
    // when pushing the limits
    // of the vehicle

  } // END ON COURSE


  else  // OFF COURSE
  {
    // When off course, goal is to get
    // back on course without error.

    // Just say you're always on curve here.
    // could create separate state, but not really necessary.
    // behavior is as if OnCurve
    speedCurveState = SpeedCurveState::OnCurve ;

    // Speed:  Take it slow when recovering.
    driveAt( 20, 0.75, 0.25, 5.0 ) ;
    // 0.75=good.

    // Probably could make proportional
    // to (L), but we'll leave it at 20

    // Direction:  point the car directly
    // at the nearest station in road
    driveTo( sNearestX, sNearestY, sNearestZ, 4.2 ) ;
    

  } // END OFF COURSE

  controlSlippage() ;

}

void Car::controlSlippage()
{
  //////////////////
  // AFTER SETTING the throttle amount by
  // driveAt, we might need to cut it back
  // a bit.

  ///!!!!! USE ALPHA_L1 for lateral slip
  /// KAPPA_L1 for longitudinal slip.
  /// If KAPPA_L2 >> KAPPA_L1 then 
  /// rwd vehicle is in danger of going
  /// into oversteer.

  /// Really "good" control of the vehicle
  /// is when wheel slipping is minimized.
  /// I wanted to write a controller that
  /// could use slippage to its advantage,
  /// but that is more difficult,
  //!!CHECK FOR WHEEL SLIPPAGE
  // If the wheels are slipping,
  // back off the accelerator a bit
  double fslip = fabs( CARSIM(betaSideSlip) ) ;
  if( fslip > 0.05 )
  {
    // Modify throttle by
    // an amount varying with side slip.
    // this is like "punishment" for slipping,
    // you have to reduce throttle.
    CARSIM(throttle) -= 10*fslip ;
    // 10 means if slippage is 0.1,
    // you will have no throttle.
    // 0.1 is pretty much out of control (expt),
    // so this is good.  When slipping we need
    // the grip for steering, and shouldn't
    // waste it on throttle generally.
    //warning( "Controller detects slip, reducing throttle" ) ;

    CARSIM(throttle)= 0.1;
  }
}

void Car::clampCarSimInputValues()
{
  ///////
  // CLAMPS
  // Steering angle must be clamped of course
  // to the physical limits.  Its too bad CarSim
  // doesn't enforce this, but hey.
  clamp( CARSIM(steeringAngle), -Car::steeringWheelMaxR, Car::steeringWheelMaxR ) ;

  // Throttle must be clamped as well
  clamp( CARSIM(throttle), 0.0, 1.0 ) ;

  // NONE OF THE INPUTS MAY BE NAN!!
  // If you send CarSim NAN the whole
  // simulation gets corrupted (all values go to NAN after that.
  // NAN is toxic.)
  if( IsNaN( CARSIM(steeringAngle) ) )
  {
    warning("Steering angle was NaN!" ) ;
    CARSIM(steeringAngle) = 0 ;
  }
  if( IsNaN( CARSIM(throttle) ) )
  {
    warning("Throttle WAS NAN!" ) ;
    CARSIM(throttle) = 0 ;
  }
  if( IsNaN( CARSIM(brake) ) )
  {
    warning("Brake WAS NAN!" ) ;
    CARSIM(brake) = 0 ;
  }
}

void Car::setNormSteering( double val )
{
  if( val > 1 )
  {
    warning( "SetNormSteering wants value between -1 and +1!" ) ;
    val = 1 ;
  }
  else if( val < -1 )
  {
    warning( "SetNormSteering wants value between -1 and +1!" ) ;
    val = -1 ;
  }

  CARSIM(steeringAngle) = val*steeringWheelMaxR ;
}



////////
// DRIVE

void Car::updateAutomaticControlValues()
{
  pos = getPos() ;
  vel = getVel() ;
  vecSNearest = D3DXVECTOR3( sNearestX, sNearestY, sNearestZ ) ;
  vecSAhead   = D3DXVECTOR3( sAheadX, sAheadY, sAheadZ ) ;
  vecSFarAhead   = D3DXVECTOR3( sFarAheadX, sFarAheadY, sFarAheadZ ) ;

  double sDifference = sLast - sNearest ;

  // If difference is near (at least 90% of sEnd) for whole track
  // then its a lap
  
  // To prevent cheating (by crossing back and
  // forth over the finish line) we'd keep
  // (negative count) crosses backwards over the finish line.
  static int negativeCrosses = 0 ;
  if( sDifference < csV.sEnd * -0.9 )
  {
    negativeCrosses++ ;
    info( "Cheater." ) ;
  }
  if( sDifference > csV.sEnd * 0.9 )
  {
    if( negativeCrosses )
    {
      negativeCrosses--;
    }
    else
    {
      // He made a lap
      double now = window->getTimeElapsedSinceGameStart() ;

      lapTimes.push_back( lapTime ) ;
      lapTime = 0 ;

      // Redo the lapTimes string
      int cw = 0 ;
      cw += sprintf( lapTimesString+cw, "Laps " ) ;
      for( int i = 0 ; i < getNumLaps() && cw < 800 ; i++ )
        cw += sprintf( lapTimesString+cw, "%d=%.2f ",
        (i+1), lapTimes[i] ) ;
    }

  }
  sLast = sNearest ;

  alongRoad = vecSAhead - vecSNearest ;

  // Now, secondly compute the
  // vector to your destination.
  // This is the "error" in our
  // position.
  posToSNearest = vecSNearest - pos ; // find the 'error' in position

  // The vector pointing towards our goal
  // is toDest.

  // Compute the angle between
  // our forward vector (that points
  // straight ahead for us) and
  // the vector pointing towards our goal.
  // The difference in angle tells us
  // how to steer.
  fwd = getFwd() ;


  // to find the steering directoin
  // we can work in 2D.
  D3DXVECTOR2 d2Fwd( fwd.x, fwd.y ) ;
  D3DXVECTOR2 d2PosToSNearest( posToSNearest.x, posToSNearest.y ) ;

  angleBetweenFwdAndSNearest = D3DXVec2AngleBetween( &d2Fwd, &d2PosToSNearest ) ;





  /////
  // DRAW SOME VECTORS, debug lines
  if( drawDebugLines )
  {
    // Draw the pos to fwd vector:
    window->draw3DLine( pos, Color::Red, pos + 5*fwd, Color::Red ) ;

    // draw the pos to sNearest vector:
    window->draw3DLine( pos, Color::Blue, vecSNearest, Color::Blue ) ;

    // draw vector alongRoad:
    window->draw3DLine( vecSNearest, Color::Magenta,
      vecSAhead, Color::Magenta ) ;

    //window->draw3DLine( vecSNearest, Color::Yellow,
    //  vecSFarAhead, Color::Yellow ) ;

    // a line from origin to where the car is
    window->draw3DLine(
      D3DXVECTOR3(0,0,0), Color::Yellow,
      pos, Color::Yellow ) ;
  }
}

double Car::getMaxSpeedForCurve( double s )
{
  // compute the speed we should be
  // going as approximately 2 / CURV.
  // The formula 2 / CURV was derived
  // by experiment.  It varies for
  // different vehicles, esp RWD vs FWD.

  double maxSpeed = 400.0 ;

  if( s == 0.0 )
    return maxSpeed ; // just means GUN IT.  like, as fast as you can go. 400kph seems
  // reasonable for a target speed on a straight, if you get get there.
  else
    maxSpeed = fabs( 1.1 / s ) ; // ! s can be negative.
   
  clamp( maxSpeed, 3.0, 400.0 ) ; // never go slower than 3,
  // or faster than 400.0
  return maxSpeed ;
}

double Car::getSpeed()
{
  return 3.6*CARSIM(speedometer) ;
}

void Car::driveAtMaxSpeedForCurvSAndCurvAS( double kAggression )
{
  // If current curvature is high, that limits
  // speed.

  // If current curvature is low, and ahead is low,
  // then you can gun it safely
  const static double eps = 0.0001 ;

  double absCurv = fabs( sCurvature ) ;
  if( absCurv > sCurvatureMaxSoFar )
    sCurvatureMaxSoFar = absCurv ; // just keep track of this

  double absCurvAhead = fabs( sCurvatureAhead ) ;

  // If the curve is near 0, gun it basically.
  if( absCurv < eps &&
      absCurvAhead < eps )
  {
    // Near 0 curvature.  Gun it.
    //CARSIM( throttle ) = 1.0 ;
    driveAt(
      400,  // aim for max speed 400 kph
      10.0, // aggressive acceleration
      1.0,  // allow full throttle
      0.10  // shouldn't brake
    ) ; 

    speedCurveState = StraightAway ;
  }

  // curv now near 0, but ahead, not.
  // So, we CAN brake now easily
  else if( absCurv < eps   &&    // we're ON straight now
           absCurvAhead > 0.01 ) // but curve ahead
  {
    // 0.01 says there's a significant curve ahead
    // Start to drive at the speed that curve dictates.

    // Here is where we can very very aggressively slow down
    // if necessary, because we're not on a curve now.
    driveAt(
      getMaxSpeedForCurve(sCurvatureAhead), // get max speed by curvature
      0.25,  // not too aggressive on the throttle
      0.25,  // quarter throttle allowed
      1.0   // high braking
    ) ;

    speedCurveState = StraightIntoGentleCurve ;
  }


  else
  {
    // We're in a curve. try and travel
    // at the target speed, but don't make
    // any "sudden moves" in terms of
    // gas or brake.

    driveAt(
      getMaxSpeedForCurve(sCurvatureAhead),
      0.5,
      0.75,//75% throttle MAX
      1.0
    ) ; 
    // Accelerating in a curve is always
    // dangerous.  You could lose control.
    // So we have a very gentle aggression
    // on kAggressionThrottle.
    // Braking while on a curve is
    // dangerous as well, because
    // you need the traction for steering.
    // However if going into the curve
    // that's when we take the speed down
    // (see below)    

    speedCurveState = OnCurve ;
  }

  ////////////
  // CURVATURE AHEAD
  double diffCurve = fabs( sCurvature - sCurvatureAhead ) ;

  // If curvature ahead is much different
  // from curvature now, REDUCE SPEED
  if( diffCurve > 0.05 )
  {
    // if the sign is different, then its a chicane
    if( !SameSign( sCurvature, sCurvatureAhead ) )
      speedCurveState = Chicane ;  // twists
    else
      speedCurveState = IntoSharpCurve ;

    // reduce throttle.  Use the curve ahead
    // instead of the current curve to determine
    // our throttle.  This COULD MEAN
    // we're going to gun it coming OUT of curves.
    CARSIM(throttle) = 0.0 ;

    // apply brakes
    // !!????????????????? well you know, this could
    // be the secret sauce to handling huge curves coming up.
    CARSIM(brake) = kAggression*(diffCurve*100)*10e6 ;

    //info( "Sharp curve detected, slowing down" ) ;
  }




  // at high speed..
  if( getSpeed() > 120 )
  {
    // this only applies at high speeds
    double diffFarAhead = fabs( diffCurve - fabs(sCurvatureFarAhead) );

    speedCurveState = SeeFarAheadCurve ;

    // drive at the speed for that curve
    driveAt( 
      getMaxSpeedForCurve(sCurvatureFarAhead),
      1.0,
      1.0,
      0.25
    ) ;
  }



  controlSlippage() ;






  /*
  // CONSIDER:  
  // How far is L?  Vary throttle with size of L.
  // This doesn't always make sense to do however,
  // just because you're not center-line doesn't mean
  // driving isn't going fine
  */
}

void Car::driveTo( double x, double y, double z, double kSteeringAggression )
{
  pos = getPos() ;
  D3DXVECTOR3 dest( x,y,z ) ;

  D3DXVECTOR3 posToDest = dest - pos ;

  D3DXVECTOR2 d2PosToDest( posToDest.x, posToDest.y ) ;
  D3DXVECTOR2 d2Fwd( fwd.x, fwd.y ) ;

  // Try and align "fwd" with posToDest.
  float angleBetween = D3DXVec2AngleBetween( &d2Fwd, &d2PosToDest ) ;

  CARSIM( steeringAngle ) = kSteeringAggression*angleBetween ;
}

/// Drives the car at a specific speed.
/// @param newTargetSpeed Target speed km/h.
/// @param kAggressionThrottle How aggressively we
/// push the throttle to get to our target speed.
/// @param kThrottleClamp If you wish to clamp
/// the maximum throttle, use a value less than 1 here.
/// This is especially important when trying to control
/// acceleration into curves, you can't do it on "km/h
/// I should be driving at here" alone, .....!!!!
////// OR CAN YOU?? Perhaps you can if
////// you control slippage better.
/// @param kAggressionBrake How aggressively we
/// push the brake to get to our target speed.
void Car::driveAt( double newTargetSpeed,
                   double kAggressionThrottle,
                   double kThrottleClamp,
                   double kAggressionBrake )
{
  // We use km/h so CONVERT IT HERE,
  // and save new target speed in
  // the speedTarget state variable.
  speedTarget = newTargetSpeed ;// 3.6 ;

  if( speedTarget < 0 )
  {
    warning( "Speedtarget = %f", speedTarget )  ;
    speedTarget = 3;
  }

  // First zero out brakes
  CARSIM(brake) = 0 ;

  double incrBy = speedTarget - getSpeed() ;

  if( incrBy > 0 )
  {
    // need to speed up
    CARSIM(throttle) = kAggressionThrottle *incrBy ;

    clamp( CARSIM(throttle), 0, kThrottleClamp ) ;
  }
  else if( incrBy < (-1) ) // should slow down by more than a few units
  {
    // Brake a bit.
    // This part of code gets hit
    // when we need to slow down going into a curve,
    // mostly (that's when target speed decreases 
    // most often).
    CARSIM(throttle) = 0.0 ;
    CARSIM(brake) = kAggressionBrake * (fabs(incrBy)*1e6) ;
  }
}

double Car::getLapStartTime()
{
  if( lapTimes.empty() )
  {
    return 0.0 ; // first lap
  }
  else
  {
    // the finish time of the last lap
    return( *(lapTimes.end() - 1) ) ;
  }
}
int Car::getNumLaps()
{
  return lapTimes.size() ;
}