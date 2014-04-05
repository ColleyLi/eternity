#ifndef SOUND_H
#define SOUND_H

#include "helperFunctions.h"

class SoundMan;
class AssetMan;

class Sound
{
private:
  /// Right now we are grounded to an FMOD implementation.
  /// Later a Sound object may be provided by DirectSound
  /// instead of FMOD.
  FMOD_SOUND* fmodSound ;

public:
  static SoundMan* soundMan;
  static AssetMan* assetMan;

 Sound() ;

  /// Calls load() immediately
  Sound( int id, char *filename, int options = 0 ) ;

  ~Sound() ;

  /// Loads a sound, .wav, .mp3, etc.,
  /// and assigns an ID to it.
  /// Note if you want to be able to
  /// LOOP the sound you have to use either
  /// FMOD_SOFTWARE or FMOD_CREATESTREAM
  /// as the options value.  Use FMOD_SOFTWARE
  /// for short, looping sounds (like a 2 second
  /// torch/fire crackle) and FMOD_CREATESTREAM
  /// for longer sounds (like a 5 minute background
  /// music song).
  void load( int id, char *filename, int options = 0 ) ;

} ;

#endif