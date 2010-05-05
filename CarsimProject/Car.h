#ifndef CAR_H
#define CAR_H

#include "cs-main.h"

typedef vector<ObjFile*> /* as simply */ VectorObj ;
typedef vector<ObjFile*>::iterator /* as simply */ VectorObjIter ;

// This macro was defined
// instead of naked pointer
// assignment to avoid a potentially bad bug,
// csV.value=0 (zero'ing out the POINTER
// instead of what it points to.)
// REALLY a bunch of GETS and SETS would
// be a bit more elegant, but higher maintenance.
// As the set of carsim properties is in high
// state of flux right now, we chose to use this
// macro instead of the hihg maintenance route
#define CARSIM(PROPERTY) (*this->csV.##PROPERTY)

/// Represents a carsim car
class Car
{
public:
  #pragma region sim helper values

  /// Driving target
  ObjFile* target ;

  /// Collections of OBJ files to use
  /// when rendering the car
  VectorObj models ;

  /// Collection of obj files to use
  /// when rendering the 4 tires.  This
  /// same collection is rendered 4 times
  /// each frame - once for each of the tires
  VectorObj tireModels ;
  #pragma endregion

  /// Cached value.  More than one thing uses this,
  /// computed once per step in update()
  double rpmNormalized ;

  /// _L_ateral distance from
  /// road centerline by vs_road_l().  Used for control.
  /// LEFT of centerline is + value,
  /// RIGHT of centerline is - value.
  double L ;

  /// Nearest station to us
  /// given our X, Y, by vs_road_s()
  double sNearest ;

  /// The car is hidden
  /// from view used for debug
  bool hidden ;

  /// Saved as state vars
  /// so we can draw them
  D3DXVECTOR3 toDest, inFront, fwd ;

  float angleBetweenFwdAndDest ;

  enum VehicleCourseState
  {
    OnCourse,
    OffCourse
  } ;
  /// Set to true when
  /// vehicle has diverted from course
  VehicleCourseState courseState ;

  /// The maximum rotation the steering wheel
  /// will go.  We clamp the steering wheel
  /// input angle (by this val and -this val)
  static double steeringWheelMaxR ;

  /// Structure for containing POINTERS TO
  /// CarSim state variable values.
  /// These variables RESIDE IN CARSIM!!
  /// These are used primarily in
  /// carsim_external.cpp.
  struct CarSimStateVariableValues
  {
    double *x,             // vehicle x pos
           *y,             // vehicle y pos
           *z,             // vehicle z pos

           *vx,            // vehicle x vel
           *vy,            // vehicle y vel
           *vz,            // vehicle z vel

           *yaw,           // vehicle yaw
           *pitch,
           *roll,

           // yaw of wheels due to steering.
           *wheelL1Steer,
           *wheelR1Steer,

           // spin of wheels due to.. well, speed
           *wheelL1V,
           *wheelL2V,
           *wheelR1V,
           *wheelR2V,

           /// floating point values for
           /// "how much" on gas, brake.
           /// an analog control stick
           /// would lets us control this
           /// with precision without hvaing
           /// a steering wheel
           *theirThrottle,
           *throttle,      // "normalized" (between 0 and 1)
           *brake,
           
           *theirSteeringAngle,
           *steeringAngle, // imported steering wheel angle 

         

           *tachAngle,
           *speedoAngle,

           *PwrEngAv,

           *betaSideSlip,

           *timeStep,
           

           roadSStart,
           roadSEnd,
           *startTime      // start time
    ;
//not working, crashes the simulator
   //int *transmissionMode; // -1 =R, 0=N, 2=Drive auto

  } csV ;

  Car() ;
  ~Car() ;

  /// Load an obj file
  /// intended to be used
  /// as part of the car's body
  void loadCarModel( char *filename ) ;

  /// Load an obj file
  /// intended to be used
  /// as part of the car's tire model
  void loadTireModel( char *filename ) ;

  /// poll keyboard and mouse / control stick
  /// for input values and update
  /// the values of CarSim.
  void userControlUpdate() ;

  /// Called from main update loop
  /// once per frame, calls the userControlUpdate
  /// and calculates the next input we will
  /// send to CarSim (via the import variables)
  void update( double timeSinceLastFrame ) ;

  /// Draws angles force vectors
  /// for debugging
  void drawDebug() ;

  /// Draws the stats of
  /// the vehicle to the screen,
  /// including throttle etc
  void drawStats() ;

  /// Draws the actual car,
  /// called from main draw loop
  /// once per frame
  void draw( double timeSinceLastFrame ) ;




  /// Functions to get the position
  /// as a vector
  D3DXVECTOR3 getPos() ;

  /// Gets you the vector that points forward
  /// to the vehicle now.
  D3DXVECTOR3 getFwd() ;











  //////////////
  // CONTROL FUNCTIONS


  void setNormSteering( double val )
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


  /// Drives the car to
  /// a specific location.
  /// @param kAggression is how aggressively we
  /// correct error -- in other words, the
  /// gain.  The steering value is clamped
  /// so the wheel doesn't go.. "all the way around"
  void driveTo( double x, double y, double z, double kAggression )
  {
    // First of all, take it slow when recovering.
    driveAt( 20 / 3.6, 0.75 ) ; // probably make proportional
    // to our actual speed

    D3DXVECTOR3 pos = getPos() ;
    D3DXVECTOR3 dest( x, y, z ) ;

    // Now, secondly compute the
    // vector to your destination.
    // This is the "error" in our
    // position.
    toDest = dest - pos ;

    // The vector pointing towards our goal
    // is toDest.

    // Compute the angle between
    // our forward vector (that points
    // straight ahead for us) and
    // the vector pointing towards our goal.
    // The difference in angle tells us
    // how to steer.
    fwd = getFwd() ;

    // Just in front is a point,
    // 1 unit "in front of" the car
    inFront = pos + fwd ;

    // we could zero outthe z-component now,
    // since there's nothing we cna do about fixng error in THAT


    // Draw the pos to fwd vector:
    window->draw3DLine( pos, Color::Red, inFront, Color::Red ) ;

    // draw the pos to dest vector:  toDest
    window->draw3DLine( pos, Color::Blue, dest, Color::Blue ) ;


    // to find the steering directoin
    // we can work in 2D.
    D3DXVECTOR2 d2Fwd( fwd.x, fwd.y ) ;
    D3DXVECTOR2 d2ToDest( toDest.x, toDest.y ) ;

    D3DXVec2Normalize( &d2Fwd, &d2Fwd ) ;
    D3DXVec2Normalize( &d2ToDest, &d2ToDest ) ;

    // get signed angle between
    // the inFront vector and the toDest vector.
    float det = d2Fwd.x * d2ToDest.y    -   d2Fwd.y * d2ToDest.x ;
    angleBetweenFwdAndDest = D3DXVec2CCW( &d2Fwd, &d2ToDest ) ;

    // sin(t) = (u x v) / (|u||v|)
    //float angle = asinf( det ) ; // we normalized the vectors.

    printf( "ANGLE %.2f\n", angleBetweenFwdAndDest ) ;

    // This is the magnitude of the angle
    // between where we're headed and
    // where we should be steering.

    // !!We need to MAP steering wheel angle
    // to change in output angle.  Or, use
    // a the normalize set procedure (setNormSteering) where we can
    // set the wheel to values between -1 and +1,
    // +1 means all the way left and -1 means all the way right.
    // (because + is ccw rotation)
    CARSIM( steeringAngle ) = kAggression*angleBetweenFwdAndDest ;
 
    
  }

  /// Drives the car at
  /// a specific speed.
  // kAggression: this gain is the "bravery"
  // with which we approach the speed.  If this
  // gets closer to or exceeds 1.0 then we'll gun it
  //towards wherever we have to go    
  void driveAt( double targetSpeed, double kAggression )
  {
    double incrBy = targetSpeed - CARSIM(speedoAngle) ;

    double kBrave = 0.75 ; 

    if( incrBy > 0 )
    {
      // need to speed up
      CARSIM(throttle) = kAggression*incrBy;

      clamp( CARSIM(throttle), 0.0, 1.0 ) ;
    }
    else if( incrBy < (-1) ) // should slow down by more than a few units
    {
      // Brake a bit
      CARSIM(brake) = kAggression*7e6 ;
    }
  }

} ;

#endif