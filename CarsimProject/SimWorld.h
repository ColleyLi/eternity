#ifndef SIMWORLD_H
#define SIMWORLD_H

#include "cs-main.h"
#include "Car.h"

/// Simulated world class
class SimWorld
{
public:
  Car * car ;
  D3DLIGHT9 light0, light1 ;

  // Freecam is flying cam, not free
  // means follow
  enum CamMode
  {
    CockpitCam, // like you're in the seat of the vehicle.
    FollowCam, // follow the car
    FreeCam   // free to move using WASD
  } ;
  
  CamMode camMode ;

  SimWorld() ;
  ~SimWorld() ;

  void LoadCarSimFile( char* filename ) ;

  void Draw() ;

} ;

extern SimWorld * simWorld ;

#endif