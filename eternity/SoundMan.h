#ifndef SOUNDMAN_H
#define SOUNDMAN_H

// SoundMan is short for "SoundManager", yo.
/*
  __                     _ 
 / _|_ __ ___   ___   __| |
| |_| '_ ` _ \ / _ \ / _` |
|  _| | | | | | (_) | (_| |
|_| |_| |_| |_|\___/ \__,_|

*/
#include <map>
using namespace std ;

#include <fmod.h>      // not working?  Did you install fmod!
#include <fmod_errors.h>
#include "helperFunctions.h"


// Map type for FSOUND
typedef map<int, FMOD_SOUND*> /* as simply */ SoundMap ;
typedef map<int, FMOD_SOUND*>::iterator /* as simply */ SoundMapIter ;

typedef multimap<int, FMOD_CHANNEL*> /* as simply */ ChannelMultimap ;
typedef multimap<int, FMOD_CHANNEL*>::iterator /* as simply */ ChannelMultimapIter ;

#define FMOD_LOOP_FOREVER -1 /* Use this constant in loopSound to have the
sound loop forever */

class SoundMan
{
private:
  // FMOD sound objects
  map<int, FMOD_SOUND*> sounds ; 

  // This has to be static to account for the fact
  // that FMOD is a C-API.  The channelEndCallback
  // has to be a static function (or a c-style function)
  // so for the channelEndCallback function to access
  // fmodChannels, fmodChannels must also be static.
  static multimap<int, FMOD_CHANNEL*> fmodChannels ; // keep a channel
  // for every playing sound, remembering what
  // the id of that sound is (to support stopSound(id) function)

  FMOD_SYSTEM *fmodSys ;

  const static int fmodMaxChannels = 32 ;  // max # sounds that
  // can be playing at a time on the sound card.


  FMOD_SOUND* defaultSound ;  // hard-coded default sound
  // not from a file.


  //!! Needs to be wrapped better
  FMOD_DSP* dspPITCHSHIFT ;


protected:

  SoundMan() ;

  // initialize FMOD sound system
  void initSoundMan() ;

  void soundStep() ;
  void soundPause() ;
  void soundUnpause() ;

public:
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
  void loadSound( int soundId, char * filename, int options = 0 ) ;

  /// Gets you the FMOD_SOUND* pointer
  /// in case you want it
  FMOD_SOUND* getSound( int soundId ) ;

  /// Plays a previously loaded sound by
  /// the same ID you passed to loadSound
  void playSound( int soundId ) ;

  /// Plays a sound with desired dsp effect
  /// 
  void playSoundWithDSP( int soundId, FMOD_DSP* dsp ) ;

  void loopSoundWithDSP( int soundId, FMOD_DSP* dsp, int loopCount ) ;

  /// Stops playback of ALL sounds
  /// by id.
  /// Really most useful for stopping
  /// one kind of background music
  /// and starting another.
  void stopSound( int soundId ) ;

  /// Loop a sound `loopCount` times,
  /// set `loopCount` to FMOD_LOOP_FOREVER
  /// to loop a sound infinitely
  void loopSound( int soundId, int loopCount = FMOD_LOOP_FOREVER ) ;

  /// Channel callback
  static FMOD_RESULT F_CALLBACK channelEndCallback(
    FMOD_CHANNEL* channel,
    FMOD_CHANNEL_CALLBACKTYPE type,
    void* data1, void* data2 )
  {
    if( type == FMOD_CHANNEL_CALLBACKTYPE_END )
    {
      //info( "Sound %p has finished playing", channel ) ;

      // Now we need to remove the multimap entry
      // where this FMOD_CHANNEL* is the __value__

      ChannelMultimapIter channelIter = fmodChannels.begin() ;
      while( channelIter != fmodChannels.end() )
      {
        if( channelIter->second == channel )
        {
          // Delete it from the multimap,
          // as it has finished playing.

          // !! Bug::"map/set iterator not incrementable"
          // These are cleaned up in Window::stopSound anyway..
          // but this should be fixed.
          //fmodChannels.erase( channelIter ) ;
          
          return FMOD_OK ;
        }

        channelIter++;
      }
    }


    warning( "I could not find sound in channel '%p' "
      "in the fmodChannels multimap.  Have you cleared "
      "the multimap?", channel ) ;
    
    return FMOD_OK ;

  }

  // !! TEMPORARY FUNCTION
  void createPitchShift() ;

  void setPitchShift( float amount ) ;

  ~SoundMan() ;
} ;

#endif