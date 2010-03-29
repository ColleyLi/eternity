#ifndef HIGHPERFTIMER_H
#define HIGHPERFTIMER_H

#include <windows.h>

class StopWatch
{
  LARGE_INTEGER freq, startTime, endTime ;
  double fFreq ;

public:
  double total_time ; 

  StopWatch()
  {
    QueryPerformanceFrequency( &freq ) ;
    fFreq = (double)freq.QuadPart ;
    total_time = 0 ;
  }

  void start()
  {
    QueryPerformanceCounter( &startTime ) ;
    total_time = 0.0 ;  // start counter at 0 seconds
  }

  void stop()
  {
    QueryPerformanceCounter( &endTime ) ;
    total_time = ( endTime.QuadPart - startTime.QuadPart ) / fFreq ;
  }

} ;

#endif //HIGHPERFTIMER_H