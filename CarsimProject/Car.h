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
#define CARSIM(PROPERTY) (*this->csV.##PROPERTY)

/// Represents a carsim car
class Car
{
public:
  #pragma region sim helper values

  /// Collections of OBJ files to use
  /// when rendering the car
  VectorObj models ;

  /// Collection of obj files to use
  /// when rendering the 4 tires.  This
  /// same collection is rendered 4 times
  /// each frame - once for each of the tires
  VectorObj tireModels ;
  #pragma endregion

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

           /// floating point values for
           /// "how much" on gas, brake.
           /// an analog control stick
           /// would lets us control this
           /// with precision without hvaing
           /// a steering wheel
           *theirThrottle,
           *throttle,      // "normalized" (between 0 and 1)
           *brake,
           *brakeStat,
           *brakePedalForce,
           *brakeBoosterForce,
           *IMP_MYBK_L1,
           *IMP_MYBK_L2,
           *IMP_MYBK_R1,
           *IMP_MYBK_R2,

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

} ;

#endif