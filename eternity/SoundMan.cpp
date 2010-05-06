#include "SoundMan.h"

multimap<int, FMOD_CHANNEL*> SoundMan::fmodChannels ;

SoundMan::SoundMan()
{
}

void SoundMan::createDefaultSound()
{
  int numSamples = 8000 ;
  int fs = 44100 ;
  float NTs  = 1.0f/fs ; // sampling interval
  short *data = new short[ numSamples ] ;

/*
    Frequencies for equal tempered tuning
    1    2    3     4     5    6     7     8     (( OCTAVE ))
C   32   65   131   262   523  1047  2093  4186
C#  35   69   139   277   554  1109  2218  4435
D   37   73   147   294   587  1175  2349  4699
D#  39   78   156   311   622  1245  2489  4978
E   41   82   165   330   659  1319  2637  5274
F   44   87   175   349   698  1397  2794  5588
F#  46   93   185   370   740  1480  2960  5920
G   49   98   196   392   784  1568  3136  6272
G#  52   104  208   415   831  1661  3322  6645
A   55   110  220   440   880  1760  3520  7040
A#  58   117  233   466   932  1865  3729  7459
B   61   123  247   494   988  1976  3951  7902
*/

  //addSinewave( data, numSamples, 0, 65, 5000, false ) ;
  //addSinewave( data, 4000, 1000, 131, 5000, false ) ;
  //addSinewave( data, 3000, 2000, 262, 5000, false ) ;
  //addSinewave( data, 4000, 3000, 523, 5000, false ) ;

  // A footstep
  //for( int i = 60 ; i < 18000 ; i++ )
  //  addSinewave( data, 8000, 0, i, 5, false ) ;

  // A heeled footstep
  //for( int i = 60 ; i < 18000 ; i+=10 )
  //  addSinewave( data, 8000, 0, i, 10, false ) ;

  // This is that awfully annoying "spark" sound
  // that the guys in the topman stage make in megaman 3
  //for( int i = 60 ; i < 18000 ; i+=i )
  //  addSinewave( data, 8000, 0, i, i, false ) ;

  // Some kind of error
  //addSlidingSinewave( data, 8000, 0, 330, 294, 6000, 6000, false ) ;
  //addSlidingSinewave( data, 8000, 0, 587, 659, 6000, 6000, false ) ;

  /*
  // a "warf" kind of sound
  int noteLen = 500 ;
  int pos = -noteLen ;
  short amp = 12000 ;
  addSinewave( data, noteLen, pos += noteLen, 330, amp, false ) ;
  addSinewave( data, noteLen, pos += noteLen, 349, amp, false ) ;
  addSinewave( data, noteLen, pos += noteLen, 370, amp, false ) ;
  addSinewave( data, noteLen, pos += noteLen, 392, amp, false ) ;
  addSinewave( data, noteLen, pos += noteLen, 415, amp, false ) ;
  addSinewave( data, noteLen, pos += noteLen, 440, amp, false ) ;
  addSinewave( data, noteLen, pos += noteLen, 466, amp, false ) ;
  addSinewave( data, noteLen, pos += noteLen, 494, amp, false ) ;
  */

  // a "warf" kind of sound, c scale
  /*
  int noteLen = 500 ;
  int pos = -noteLen ;
  short amp = 12000 ;
  addSinewave( data, noteLen, pos += noteLen, 262, amp, false ) ; // C
  addSinewave( data, noteLen, pos += noteLen, 294, amp, false ) ; // D
  addSinewave( data, noteLen, pos += noteLen, 330, amp, false ) ; // E
  addSinewave( data, noteLen, pos += noteLen, 349, amp, false ) ; // F
  addSinewave( data, noteLen, pos += noteLen, 392, amp, false ) ; // G
  addSinewave( data, noteLen, pos += noteLen, 440, amp, false ) ; // A
  addSinewave( data, noteLen, pos += noteLen, 494, amp, false ) ; // B
  addSinewave( data, noteLen, pos += noteLen, 523, amp, false ) ; // C
  */

  int noteLen = 4000 ;
  int pos = -noteLen ;
  short amp = 12000 ;
  addSinewave( data, noteLen, pos += noteLen, 392, amp, false ) ; // G
  addSinewave( data, noteLen, pos += noteLen, 262, amp, false ) ; // C
  
  
  
  
  
  // This sounds like a kick drum
  //addSlidingSinewave( data, 8000, 0, 131, 62, 16000, 16000, false ) ;

  // This sounds like bumping into a solid wall in mario bros 1
  //addSlidingSquarewave( data, 8000, 0, 131, 62, 12000, 12000 ) ;
  
  // A really squeaky objection.
  //addSlidingSquarewave( data, 8000, 0, 2000, 100, 12000, 12000 ) ;

  // laser fire
  //addSlidingSquarewave( data, 8000, 0, 4000, 2000, 6000, 6000 ) ;
  //addSlidingSquarewave( data, 8000, 0, 16000, 19000, 6000, 6000 ) ;
  
  
  //addSquarewave( data, 5000, 2000, 1047, 1000 ) ;
  //addSquarewave( data, 5000, 2000, 523, 666, 1000 ) ;

  FMOD_CREATESOUNDEXINFO soundInfo = {0};
  soundInfo.cbsize = sizeof( FMOD_CREATESOUNDEXINFO ) ;
  soundInfo.length = numSamples * sizeof( short ) ;
  soundInfo.defaultfrequency = fs ;
  soundInfo.numchannels = 1 ;
  soundInfo.userdata = data ;
  soundInfo.format = FMOD_SOUND_FORMAT::FMOD_SOUND_FORMAT_PCM16 ;

  // Generate the default sound
  FMOD_ErrorCheck(
    FMOD_System_CreateSound(
      fmodSys,
      (char*)data,
      FMOD_SOFTWARE | FMOD_CREATESAMPLE | FMOD_OPENUSER,
      &soundInfo, &defaultSound
    )
  ) ;

  void *ptr1, *ptr2 ;
  unsigned int bytesLocked1, bytesLocked2 ;
  FMOD_ErrorCheck(
    FMOD_Sound_Lock( defaultSound, 0, numSamples * sizeof( short ), &ptr1, &ptr2, &bytesLocked1, &bytesLocked2 ) 
  ) ;

  info( "locked %d, %d bytes", bytesLocked1, bytesLocked2 ) ;
  memcpy( ptr1, data, bytesLocked1 ) ;
  
  FMOD_ErrorCheck(
    FMOD_Sound_Unlock( defaultSound, ptr1, ptr2, bytesLocked1, bytesLocked2 ) 
  ) ;

  // erase the local copy of the generated data
  //free( data ) ;
}

void SoundMan::initSoundMan()
{
  FMOD_ErrorCheck( FMOD_System_Create( &fmodSys ) ) ;

  UINT version ;
  FMOD_ErrorCheck( FMOD_System_GetVersion( fmodSys, &version ) ) ;

  if( version < FMOD_VERSION ) 
  {
    // if the version being used to run
    // this program is different from the
    // version the program was compiled with,
    // print an error
    error("FMOD Error!  You are using an old version of FMOD %08x.  This program requires %08x", version, FMOD_VERSION);
  }

  info( "Initializing fmod with %d channels . . .", fmodMaxChannels ) ;
  FMOD_ErrorCheck( FMOD_System_Init(
    fmodSys,
    fmodMaxChannels,
    FMOD_INIT_NORMAL, //| FMOD_INIT_3D_RIGHTHANDED,
    NULL )
  ) ;


  FMOD_ErrorCheck( 
    FMOD_System_Set3DSettings( fmodSys, 1.0f, 1.0f, 1.0f ) 
  ) ;


  /*
  //!! erase me
  FMOD_DSP* dspPITCHSHIFT ;

  FMOD_RESULT result ;
  result = FMOD_System_CreateDSPByType(fmodSys, FMOD_DSP_TYPE_PITCHSHIFT, &dspPITCHSHIFT);
  FMOD_ErrorCheck(result);

  result = FMOD_System_AddDSP(fmodSys, dspPITCHSHIFT, 0);
  FMOD_ErrorCheck(result);

  result = FMOD_DSP_SetParameter(dspPITCHSHIFT, FMOD_DSP_PITCHSHIFT_FFTSIZE, 1024);
  FMOD_ErrorCheck(result);

  result = FMOD_DSP_SetParameter(dspPITCHSHIFT, FMOD_DSP_PITCHSHIFT_PITCH, 1.2f);
  FMOD_ErrorCheck(result);

  FMOD_System_Update( fmodSys ) ;
  Sleep(20) ;
  int dspPITCHSHIFT_ACTIVE;

  FMOD_DSP_GetActive(dspPITCHSHIFT, &dspPITCHSHIFT_ACTIVE);

  if( !dspPITCHSHIFT_ACTIVE )
  {
    puts( "buyt why?" ) ;
    system("pause" ) ;
  }
  */


  createDefaultSound() ;
}


void SoundMan::soundStep()
{
  // call this once per "tick"
  FMOD_ErrorCheck( FMOD_System_Update( fmodSys ) ) ;
}

void SoundMan::setSoundPosition( int soundId, FMOD_VECTOR *pos, FMOD_VECTOR *vel )
{
  FMOD_CHANNEL* channel = getFmodChannel( soundId ) ;

  if( channel )
    FMOD_Channel_Set3DAttributes( channel, pos, vel ) ;
  else
    warning( "Can't set position: Sound `%d` is not playing right now", soundId ) ;
}

void SoundMan::setListener(
  FMOD_VECTOR *pos,
  FMOD_VECTOR *vel,
  FMOD_VECTOR *fwd,
  FMOD_VECTOR *up )
{
  FMOD_System_Set3DListenerAttributes( fmodSys, 0, pos, vel, fwd, up ) ;
}

void SoundMan::soundPause()
{
  foreach( ChannelMultimapIter, channelIter, fmodChannels )
  {
    FMOD_Channel_SetPaused( channelIter->second, TRUE ) ;
  }
}

void SoundMan::soundUnpause()
{
  // unpause all channels..
  foreach( ChannelMultimapIter, channelIter, fmodChannels )
  {
    FMOD_Channel_SetPaused( channelIter->second, FALSE ) ;
  }
}
void SoundMan::loadSound( int soundId, char * filename, int options )
{
  FMOD_SOUND *newSound ;
  
  // Load the sound
  bool success = FMOD_ErrorCheck(
    FMOD_System_CreateSound(
    // !! Too often effects don't work on
    // target pc's with crummy sound cards.
    // We should check the card's caps
    // before setting.
    fmodSys, filename, FMOD_SOFTWARE | options, 0, &newSound ) ) ;

  if( !success )
  {
    error( "Could not load %s, check file exists", filename ) ;
    return ;
  }

  // File loaded, ok, now
  // check if a sound by that ID already exists.
  // If it does, unload and destroy it
  SoundMapIter soundEntry = sounds.find( soundId ) ;
  if( soundEntry != sounds.end() )
  {
    warning( "Sound id=%d already existed, destroying and overwriting with %s.", soundId, filename ) ;
    FMOD_SOUND *oldSound = NULL ;
    oldSound = soundEntry->second ;
    FMOD_Sound_Release( oldSound ) ;

    // remove from the map
    sounds.erase( soundEntry ) ;
  }

  // Now save it by its id in the map
  sounds.insert( make_pair( soundId, newSound ) ) ;
}

FMOD_SOUND* SoundMan::getSound( int soundId )
{
  FMOD_SOUND *sound = NULL ;
  SoundMapIter soundEntry = sounds.find( soundId ) ;
  
  if( soundEntry != sounds.end() )
    sound = soundEntry->second ;
  else
    warning( "Sound id=%d doesn't exist, you get NULL", soundId ) ;

  return sound ;
}

/// !!TEMPORARYAR FUNCTION
FMOD_CHANNEL* SoundMan::getFmodChannel( int soundId )
{
  ChannelMultimapIter channelIter = fmodChannels.begin();

  while( channelIter != fmodChannels.end() )
  {
    if( channelIter->first == soundId )
    {
      //!! due to ANOTHER bug which I 
      // could'nt root out earlier just because
      // a sound is prsent in this map doesn't/
      // mean its playing.. also check isPlaying
      // prpoerty of the channel
      FMOD_BOOL playing ;
      FMOD_Channel_IsPlaying( channelIter->second, &playing ) ;
      if( playing )
      {
        // this is the channel you wanted 
        return channelIter->second ;
      }
    }

    ++channelIter ;
  }
  
  // no channel
  return NULL ;
}

bool SoundMan::isPlaying( int soundId )
{
  ChannelMultimapIter channelIter = fmodChannels.begin();

  while( channelIter != fmodChannels.end() )
  {
    if( channelIter->first == soundId )
    {
      //!! due to ANOTHER bug which I 
      // could'nt root out earlier just because
      // a sound is prsent in this map doesn't/
      // mean its playing.. also check isPlaying
      // prpoerty of the channel
      FMOD_BOOL playing ;
      FMOD_Channel_IsPlaying( channelIter->second, &playing ) ;
      if( playing )
      {
        return true ; // that's all we needed to know
      }
    }

    ++channelIter ;
  }
  
  return false ;
}

void SoundMan::playSound( int soundId )
{
  FMOD_SOUND *sound = defaultSound ;
  SoundMapIter soundEntry = sounds.find( soundId ) ;
  
  if( soundEntry != sounds.end() )
    sound = soundEntry->second ;
  else
    warning( "Sound id=%d doesn't exist", soundId ) ;

  FMOD_CHANNEL* channel ;

  // Play this sound on any available free channel
  FMOD_ErrorCheck(
    FMOD_System_PlaySound( fmodSys, FMOD_CHANNEL_FREE, sound, false, &channel )
  ) ;

  // Register a callback on the channel
  //FMOD_CHANNEL_CALLBACK( channel, FMOD_CHANNEL_CALLBACKTYPE_END, 0, 0 ) ;
  FMOD_Channel_SetCallback( channel, channelEndCallback ) ;
  
  fmodChannels.insert( make_pair( soundId, channel ) ) ;
  
  //int numChannelsPlaying ;
  //FMOD_System_GetChannelsPlaying( fmodSys, &numChannelsPlaying ) ;
  //info( "There are %d channels playing right now", numChannelsPlaying ) ;
}

void SoundMan::playSoundWithDSP( int soundId, FMOD_DSP* dsp )
{
  FMOD_SOUND *sound = getSound( soundId ) ;
  if( !sound )
  {
    warning( "Sound id=%d doesn't exist", soundId ) ;
    sound = defaultSound ;
  }
  
  FMOD_CHANNEL* channel ;

  // Play this sound on any available free channel
  FMOD_ErrorCheck(
    FMOD_System_PlaySound( fmodSys, FMOD_CHANNEL_FREE, sound, false, &channel )
  ) ;


  // Set the dsp effect on the channel
  //!!! TEST:  using the pitch shift dsp always for now
  FMOD_Channel_AddDSP( channel, dspPITCHSHIFT, 0 ) ;

  // Register a callback on the channel
  //FMOD_CHANNEL_CALLBACK( channel, FMOD_CHANNEL_CALLBACKTYPE_END, 0, 0 ) ;
  FMOD_Channel_SetCallback( channel, channelEndCallback ) ;
  
  fmodChannels.insert( make_pair( soundId, channel ) ) ;
  
  //int numChannelsPlaying ;
  //FMOD_System_GetChannelsPlaying( fmodSys, &numChannelsPlaying ) ;
}

void SoundMan::loopSoundWithDSP( int soundId, int loopCount, FMOD_DSP* dsp )
{
  FMOD_SOUND *sound = getSound( soundId ) ;
  if( !sound )
  {
    warning( "Sound id=%d doesn't exist", soundId ) ;
    return ;
  }

  FMOD_ErrorCheck( FMOD_Sound_SetLoopCount( sound, loopCount ) ) ;

  FMOD_ErrorCheck( FMOD_Sound_SetMode( sound, FMOD_LOOP_NORMAL ) ) ;

  FMOD_CHANNEL* channel ;
  FMOD_ErrorCheck(
    FMOD_System_PlaySound( fmodSys, FMOD_CHANNEL_FREE, sound, false, &channel )
  ) ;

  // Set the dsp effect on the channel
  //!!! TEST:  using the pitch shift dsp always for now
  FMOD_Channel_AddDSP( channel, dspPITCHSHIFT, 0 ) ;

  // Register a callback on the channel
  //FMOD_CHANNEL_CALLBACK( channel, FMOD_CHANNEL_CALLBACKTYPE_END, 0, 0 ) ;
  FMOD_Channel_SetCallback( channel, channelEndCallback ) ;
  
  fmodChannels.insert( make_pair( soundId, channel ) ) ;
  
  //int numChannelsPlaying ;
  //FMOD_System_GetChannelsPlaying( fmodSys, &numChannelsPlaying ) ;
}

void SoundMan::stopSound( int soundId )
{
  // Retrieve all channels playing
  // this sound id.
  ChannelMultimapIter channelIter = fmodChannels.begin();

  while( channelIter != fmodChannels.end() )
  {
    if( channelIter->first == soundId )
    {
      //info( "Stopping %d", soundId ) ;
      FMOD_Channel_Stop( channelIter->second ) ;

      ChannelMultimapIter toErase = channelIter ;

      // Move on to next one..
      ++channelIter ;

      // and remove it from the collection
      fmodChannels.erase( toErase ) ;
    }
    else
    {
      ++channelIter ; // increment by one
    }
  }
}

//!! NOTE!!  From FMOD docs:  FMOD_Sound_setLoopCount:
//  "This function does not affect
//   FMOD_HARDWARE based sounds that are not streamable."
// So if you want to loop a sound less than INFINITY
// times you must create it with the FMOD_CREATESTREAM
// or FMOD_SOFTWARE flags, e.g.
//
// loadSound( HumanMusic, "sounds/loopingSoundEffect.mp3", FMOD_SOFTWARE ) ;
//
// OR
//
// loadSound( HumanMusic, "sounds/loopingSoundEffect.mp3", FMOD_CREATESTREAM ) ;
//
// otherwise it will loop forever
void SoundMan::loopSound( int soundId, int loopCount )
{
  FMOD_SOUND *sound = getSound( soundId ) ;
  
  if( !sound )
  {
    warning( "Sound id=%d doesn't exist, not looping anything", soundId ) ;
    
    // We're returning here, because there's no use
    // in looping the error sound
    return ;
  }

  if( loopCount > 0 )
   loopCount-- ; // by default fmod wants to REPEAT the sound
  // loopCount times .. i.e. plays it loopCount+1 times

  FMOD_MODE fmodSoundMode ;
  FMOD_Sound_GetMode( sound, &fmodSoundMode ) ;

  if( notContainsFlag( fmodSoundMode, (FMOD_SOFTWARE|FMOD_CREATESTREAM) ) )
  {
    warning( "Sound %d was not created with FMOD_SOFTWARE "
      "or FMOD_CREATESTREAM, this sound will loop forever.", soundId  ) ;
  }

  FMOD_ErrorCheck( FMOD_Sound_SetLoopCount( sound, loopCount ) ) ;

  FMOD_ErrorCheck( FMOD_Sound_SetMode( sound, FMOD_LOOP_NORMAL ) ) ;

  FMOD_CHANNEL* channel ;
  FMOD_ErrorCheck(
    FMOD_System_PlaySound( fmodSys, FMOD_CHANNEL_FREE, sound, false, &channel )
  ) ;

  fmodChannels.insert( make_pair( soundId, channel ) ) ;

  // Now unset the loop, so that
  // the sound is unaffected for future plays
  //FMOD_ErrorCheck( FMOD_Sound_SetMode( sound, FMOD_LOOP_OFF ) ) ;
  //FMOD_ErrorCheck( FMOD_Sound_SetLoopCount( sound, 0 ) ) ;
  // ^^This causes a problem for FMOD_CREATESTREAM loaded
  // sounds

}


//!! TEMPORARY FUNCTION
void SoundMan::createPitchShiftToChannel( FMOD_CHANNEL *channel )
{
  //!! INCOMPLETE
  /// must add map of DSP effects for each sound
  // so each isn't doubled-up

  FMOD_RESULT fr ;

  // set up piutch shift dsp
  fr = FMOD_System_CreateDSPByType( fmodSys, FMOD_DSP_TYPE_PITCHSHIFT, &dspPITCHSHIFT ) ;
  FMOD_ErrorCheck( fr ) ;
  
  //fr = FMOD_System_AddDSP(fmodSys, dspPITCHSHIFT, 0) ;
  //FMOD_ErrorCheck( fr ) ;


  fr = FMOD_Channel_AddDSP( channel, dspPITCHSHIFT, NULL ) ;
  FMOD_ErrorCheck( fr ) ;

  // set fft quality of pitch shift
  fr = FMOD_DSP_SetParameter( dspPITCHSHIFT, FMOD_DSP_PITCHSHIFT_FFTSIZE, 2048 ) ;
  FMOD_ErrorCheck( fr ) ;

  fr = FMOD_DSP_SetParameter( dspPITCHSHIFT, FMOD_DSP_PITCHSHIFT_PITCH, 1.0 ) ;
  FMOD_ErrorCheck( fr ) ;

}

void SoundMan::setPitchShift( float amount )
{
  FMOD_ErrorCheck( FMOD_DSP_SetParameter( dspPITCHSHIFT, FMOD_DSP_PITCHSHIFT_PITCH, amount ) ) ;
}


SoundMan::~SoundMan()
{
  // FMOD
  SoundMapIter iter ;
  foreach( SoundMapIter, iter, sounds )
  {
    FMOD_ErrorCheck( FMOD_Sound_Release( iter->second ) ) ;
  }
  FMOD_ErrorCheck( FMOD_System_Release( fmodSys ) ) ;
}
