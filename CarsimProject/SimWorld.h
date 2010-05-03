#ifndef SIMWORLD_H
#define SIMWORLD_H

#include "cs-main.h"
#include "Car.h"
#include "Track.h"

/// Simulated world class
class SimWorld
{
public:
  Car * car ;
  Track * track ;
  D3DLIGHT9 light0, light1 ;

  // Freecam is flying cam, not free
  // means follow
  enum CamMode
  {
    FreeCam,  // free to move using WASD
    FollowCam // follow the car
  } ;
  
  CamMode camMode ;

  SimWorld() ;
  ~SimWorld() ;

  void LoadCarSimFile( char* filename ) ;

  void Draw() ;

} ;

extern SimWorld * simWorld ;

#endif