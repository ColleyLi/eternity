#ifndef SIMWORLD_H
#define SIMWORLD_H

#include "cs-main.h"
#include "Car.h"
#include "Track.h"

/// Simulated world class
class SimWorld
{
  Car * car ;
  Track * track ;

public:
  SimWorld()
  {
    info( "Constructing simworld.." ) ;
    
    car = new Car() ;
    track = new Track() ;
  }
  ~SimWorld()
  {
    delete car ;
  }

  void LoadCarSimFile( char* filename )
  {
    warning( "LoadCarSimFile not implemented yet" ) ;

    // Scan to the point in the file where
    // the car model is


    // Scan to the point in the file where
    // the TRACK is.
    
  }


  void Draw()
  {
    #pragma region Draw the ground plane
    int limits = 500 ;

    // * | *
    //   |
    // --+--> x
    //   |
    // * | * (limits, 0, limits)
    //   v
    //    z
   
    window->drawQuad(

      D3DVertexC( limits, 0,  limits, 128, 128, 128 ),
      D3DVertexC( limits, 0, -limits, 128, 128, 128 ),
      D3DVertexC(-limits, 0, -limits, 128, 128, 128 ),
      D3DVertexC(-limits, 0,  limits,  78,  78,  78 )

    ) ;
    #pragma endregion


    // Draw the track.
    track->draw() ;

    // Draw the vehicle
    car->draw() ;
  }

} ;

extern SimWorld * simWorld ;

#endif