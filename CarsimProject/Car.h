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
  #pragma region renderer
  /// Collections of OBJ files to use
  /// when rendering the car
  VectorObj models ;

  /// Collection of obj files to use
  /// when rendering the 4 tires.  This
  /// same collection is rendered 4 times
  /// each frame - once for each of the tires
  VectorObj tireModels ;

  /// The car is hidden
  /// from view used for debug
  bool hidden ;

  /// Whether to draw the angle / force
  /// steering debug lines or not
  bool drawDebugLines ;
  #pragma endregion

  /// Make simulation run faster
  /// by increasing this value
  double simStepsFrame ;

  /// A control to override automatic
  /// or manual vehicle control
  bool manualControl ;

  #pragma region sim helper values

  /// running count of lapTime.
  /// If increase simStepsFrame,
  /// this value is multiplied as well.
  double lapTime ;

  /// Count of seconds it took
  /// to complete each lap around the track.
  vector<double> lapTimes ;

  /// Cached string with laptimes
  /// saved in it
  char lapTimesString[ 1024 ] ;

  /// Cached value.  More than one thing uses this,
  /// computed once per step in update()
  double rpmNormalized ;

  /// _L_ateral distance from
  /// road centerline by vs_road_l().  Used for control.
  /// LEFT of centerline is + value,
  /// RIGHT of centerline is - value.
  double L ;

  D3DXVECTOR3 pos, vel ;

  /// The target speed we should be travelling at now,
  /// usually based on value returned by "getMaxSpeedForCurve"
  double speedTarget ;

  /// Value of s for last frame.
  /// used for detecting if you made a lap or not.
  double sLast ;

  /// Nearest station to us
  /// given our X, Y, by vs_road_s()
  double sNearest ;

  /// Computed value of sNearest + (some lookahead,
  /// usually 5).  sAhead to use should depend on SPEED!!
  double sAhead ;
  
  /// A secondary lookahead that is
  /// twice as far as sAhead.  It is
  /// used to detect curves coming up.
  double sFarAhead ;

  /// The nearest x,y,z location
  /// at the nearest station
  double sNearestX, sNearestY, sNearestZ ;
  D3DXVECTOR3 vecSNearest ;

  /// The AHEAD x,y,z on the road
  /// that we should be aiming the
  /// car towards.
  double sAheadX, sAheadY, sAheadZ, sFarAheadX, sFarAheadY, sFarAheadZ ;
  D3DXVECTOR3 vecSAhead, vecSFarAhead ;

  /// Measurement of road curvature.
  /// Relatively small values,
  /// curvature right is -
  /// curvature left is +
  double sCurvature ;

  /// Keep track of this for
  /// observation purposes
  double sCurvatureMaxSoFar ;

  /// Measure of road curv AHEAD.
  /// Large difference between curve now
  /// and curve ahead means SLOW DOWN,
  /// esp sign variation
  double sCurvatureAhead ;

  /// Looking farther ahead
  /// for curvature than sCurvatureAhead
  double sCurvatureFarAhead ;

  /// Heading angle at the current
  /// station
  double sTargetHeading ;

  /// "lateral position of target path",..
  double sTargetHeadingL ;

  /// A vector that points ALONG the road.
  /// this is the instantaneous direction
  /// that we're SUPPOSED to be pointing in

  /// Saved as state vars
  /// so we can draw them
  D3DXVECTOR3 fwd, alongRoad, posToSNearest, posToSAhead ;

  float angleBetweenFwdAndSNearest, 
        angleBetweenFwdAndSAhead ;

  enum VehicleCourseState
  {
    OnCourse,
    OffCourse
  } ;
  /// Set to true when
  /// vehicle has diverted from course
  VehicleCourseState courseState ;

  enum SpeedCurveState
  {
    /// On a straight
    StraightAway,

    /// Going on a straight, curve coming up
    StraightIntoGentleCurve,

    /// Going into a very tight curve
    IntoSharpCurve,

    /// Twisty-turvy.  Section of road that winds 
    /// back and forth, left and right
    Chicane,

    /// Driving on a curve
    OnCurve,

    /// Leaving a curve for a straight-away
    CurveIntoStraight,

    /// I see a far ahead curve
    SeeFarAheadCurve
  } ;

  /// All this affects the maximum
  /// speed we can travel at
  SpeedCurveState speedCurveState ;

  /// The maximum rotation the steering wheel
  /// will go.  We clamp the steering wheel
  /// input angle (by this val and -this val)
  static double steeringWheelMaxR ;

  /// The amount of 's' to lookahead
  /// for finding the direction vector
  /// that points from our nearest
  /// station ALONG THE ROAD
  static double sNominalLookAheadAmount ;

  static double sNominalLookFarAheadAmount ;

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

         

           *tach,
           *speedometer,

           *PwrEngAv,

           *betaSideSlip,

           /// lateral slip angles
           *slipLatL1,
           *slipLatR1,
           *slipLatL2,
           *slipLatR2,

           /// wheel-spin
           *slipLongL1,
           *slipLongR1,
           *slipLongL2,
           *slipLongR2,

           *timeStep,
           

           sStart,
           sEnd,
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

  /// quickly draws an output string
  /// in a default font, top left corner
  void drawOutputString( char *str, D3DCOLOR color, float x, float y ) ;

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

  D3DXVECTOR3 getVel() ;

  /// Gets you the vector that points forward
  /// to the vehicle now.
  D3DXVECTOR3 getFwd() ;











  //////////////
  // CONTROL FUNCTIONS

  void control() ;

  /// Reduces set throttle if
  /// car is slipping, ESPECIALLY
  /// if front wheels are spinning
  void controlSlippage() ;

  /// Clamp values and make
  /// sure none of them is NAN.
  void clampCarSimInputValues() ;

  /// Set max steer left +1,
  /// max steer right -1.
  void setNormSteering( double val ) ;

  /// Update the values used
  /// by the automatic controller
  void updateAutomaticControlValues() ;



  ////////
  // DRIVE

  /// Gets you the max speed this vehicle
  /// should travel at based on curvature.
  double getMaxSpeedForCurve( double s ) ;

  /// Gets you current speed in KPH
  double getSpeed() ;

  /// Drives at the max speed
  /// estimated possible given
  /// curvature@S and curvature@AS
  void driveAtMaxSpeedForCurvSAndCurvAS( double kAggression ) ;

  /// Alters STEERING ANGLE to drive the car to
  /// a specific location.
  /// @param kSteeringAggression is how aggressively we
  /// correct error -- in other words, the
  /// gain.  You must clamp the steering wheel
  /// to realistic limits after you run this, 
  /// so the wheel doesn't go.. "all the way around"
  void driveTo( double x, double y, double z, double kSteeringAggression ) ;
 
  /// Drives the car at
  /// a specific speed.
  void driveAt( double newTargetSpeed,
                double kAggressionThrottle,
                double kThrottleClamp,
                double kAggressionBrake ) ;


  double getLapStartTime() ;
  int getNumLaps() ;
} ;

#endif