#include "Sound.h"
#include "SoundMan.h"
#include "AssetMan.h"

SoundMan* Sound::soundMan = 0;
AssetMan* Sound::assetMan = 0;
Sound::Sound()
{
}

Sound::Sound( int id, char *filename, int options )
{
  load( id, filename, options ) ;
}

Sound::~Sound()
{
}

void Sound::load( int id, char * filename, int options )
{
  assetMan->loadSound( soundMan, id, filename, options );
}
