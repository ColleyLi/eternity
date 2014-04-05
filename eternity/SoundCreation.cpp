#include "SoundCreation.h"
#include "helperFunctions.h"

void addSinewave( short *data, int durationInSamples,
                  int offset, int frequency,
                  short amplitude, bool distortion ) 
{
  static double NTs = 1.0 / 44100.0 ;  // assumes this sampling rate

  // "distortion" is just adding 1 to the
  // sinewave so that it will be all positive, so it will totally 
  // get crazy with distortion when multiplied by amp,
  // if amp is high enough
  if( distortion )
    for( int i = offset ; i < offset + durationInSamples ; i++ )
      data[ i ] += (short)(amplitude*( 1 + sin( 2*PI*frequency*i*NTs ) ) ) ;
  else
    for( int i = offset ; i < offset + durationInSamples ; i++ )
      data[ i ] += (short)(amplitude*( sin( 2*PI*frequency*i*NTs ) ) ) ;
}

void addSquarewave( short *data, int durationInSamples, int offset,
                    int fundamentalFrequency, short amplitude )
{
  static double NTs = 1.0 / 44100.0 ;  // assumes this sampling rate

  // we'll just do a signum on the fundamental
  for( int i = offset ; i < offset + durationInSamples ; i++ )
  {
    // we're only interested in the sign of this
    int up = ( sin( 2*PI*fundamentalFrequency*i*NTs ) > 0 ) ?
      +1 :  // if the sin() is bigger than 0, then +1
      -1 ;  // if the sin() is <= 0, then -1
    
    data[ i ] += up*amplitude ;
  }
}

void addSlidingSinewave( short *data, int durationInSamples,
                         int offset,
                         int frequency1, int frequency2,
                         short amplitude1, short amplitude2,
                         bool distortion )
{
  static double NTs = 1.0 / 44100.0 ;  // assumes this sampling rate

  if( distortion )
    for( int i = offset ; i < offset + durationInSamples ; i++ )
    {
      double percentageAlong = (double)i/durationInSamples ;
      double freq = lerp( frequency1, frequency2, percentageAlong ) ;
      double amp = lerp( amplitude1, amplitude2, percentageAlong ) ;
      data[ i ] += (short)(amp*( 1 + sin( 2*PI*freq*i*NTs ) ) ) ;
    }
  else
    for( int i = offset ; i < offset + durationInSamples ; i++ )
    {
      double percentageAlong = (double)i/durationInSamples ;
      double freq = lerp( frequency1, frequency2, percentageAlong ) ;
      double amp = lerp( amplitude1, amplitude2, percentageAlong ) ;
      data[ i ] += (short)(amp*( sin( 2*PI*freq*i*NTs ) ) ) ;
    }
}

void addSlidingSquarewave( short *data, int durationInSamples, int offset,
                           int fundamentalFrequency1, int fundamentalFrequency2,
                           short amplitude1, short amplitude2 )
{
  static double NTs = 1.0 / 44100.0 ;  // assumes this sampling rate

  // As we work along our way, the % of freq1 or freq2 is dictated
  double percentageAlong = 0.0 ;

  // we'll just do a signum on the fundamental
  for( int i = offset ; i < offset + durationInSamples ; i++ )
  {
    percentageAlong = (double)i/durationInSamples ;

    // lerp between two freqs
    double freq = lerp( fundamentalFrequency1, fundamentalFrequency2, percentageAlong ) ;
    double amp  = lerp( amplitude1, amplitude2, percentageAlong ) ;

    // we're only interested in the sign of this
    int up = ( sin( 2*PI*freq*i*NTs ) > 0 ) ?
      +1 :  // if the sin() is bigger than 0, then +1
      -1 ;  // if the sin() is <= 0, then -1
    
    data[ i ] += (short)(up*amp) ;
  }
}
