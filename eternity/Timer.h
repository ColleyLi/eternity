#ifndef TIMER_H
#define TIMER_H

#include <windows.h>
#include <stdio.h>
#include "helperFunctions.h"

// Locks game to running at 60 fps
class Timer
{
  LARGE_INTEGER freq, thisTime, lastTime ;
  double fFreq ;

public:
  double frames_per_second ;
  double time_since_last_frame ;
  Timer()
  {
    frames_per_second = time_since_last_frame = 0 ;

    QueryPerformanceFrequency( &freq ) ;
    fFreq = (double)freq.QuadPart ;

    info( "perf counter resolution=%lld ticks/second", freq ) ;
  
    QueryPerformanceCounter( &thisTime ) ;
    QueryPerformanceCounter( &lastTime ) ;
  
    info( "Program start @ time=%lld", thisTime.QuadPart ) ;
  }


  void update()
  {
    lastTime = thisTime ;
    QueryPerformanceCounter( &thisTime ) ;
    time_since_last_frame = ( thisTime.QuadPart - lastTime.QuadPart ) / fFreq ;
    frames_per_second = 1 / time_since_last_frame ;
  }

  // Locks framerate at (frameRateDesired) frames per second.
  // Updates both timeSinceLastFrame and fps public variables
  void lock( float frameRateDesired )
  {
    lastTime = thisTime ;
    
    time_since_last_frame = 0 ;  // start by assuming NO time has
    // passed since the last frame. We'll be TRAPPED in the loop
    // below until ENOUGH TIME has passed since the last frame
    // for the game to be running at exactly 'frameRateDesired'
    // frames per second.
    do
    {
      QueryPerformanceCounter( &thisTime ) ; // update what time it is NOW
      time_since_last_frame = ( thisTime.QuadPart - lastTime.QuadPart ) / fFreq ; // get time difference between NOW and last frame
      frames_per_second = 1 / time_since_last_frame ;  // frames_per_second = ( 1 / time_since_last_frame )
    } while( frames_per_second > frameRateDesired ) ;  // stay in above loop so long as the game's fps is higher than frameRateDesired

  }
} ;

#endif // TIMER_H