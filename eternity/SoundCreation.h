#ifndef SOUND_CREATION_H
#define SOUND_CREATION_H

void addSinewave( short *data, int durationInSamples,
                int offset, int frequency,
                short amplitude, bool distortion ) ;

void addSquarewave( short *data, int durationInSamples, int offset,
                    int fundamentalFrequency, short amplitude ) ;

void addSlidingSinewave( short *data, int durationInSamples,
                          int offset,
                          int frequency1, int frequency2,
                          short amplitude1, short amplitude2,
                          bool distortion ) ;

void addSlidingSquarewave( short *data, int durationInSamples, int offset,
                            int fundamentalFrequency1, int fundamentalFrequency2,
                            short amplitude1, short amplitude2 ) ;



#endif