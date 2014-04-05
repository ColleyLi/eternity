#include "AssetMan.h"
#include "SpriteMan.h"

HRESULT AssetMan::loadTextureNonPow2( IDirect3DDevice9 *gpu, Sprite* sprite, char *filename, D3DCOLOR backgroundColor )
{
  HRESULT hr = D3DXCreateTextureFromFileExA(
    gpu,
    filename,
                                 
    // D3DX_DEFAULT will round up to nearest power of 2, or
    // D3DX_DEFAULT_NONPOW2 for actual size.
    // !!NOTE:  you probably should check if the
    // gpu supports NONPOW2 sprites before
    // going ahead with this.
    D3DX_DEFAULT_NONPOW2,
    D3DX_DEFAULT_NONPOW2,
      
    D3DX_FROM_FILE,  // D3DX_FROM_FILE means no mips.
    // D3DX_DEFAULT will create a complete mipmap chain.

    0, // Usage:  you could make the tex a render target with D3DUSAGE_RENDERTARGET but we don't want to
    D3DFMT_UNKNOWN,  // take the format from the file.
    D3DPOOL_MANAGED, // So we don't have to re-load textures if the device is lost
    D3DX_FILTER_NONE,    // filter image  !! CHANGES THE SIZE OF THE IMAGE!
    D3DX_FILTER_NONE,    // filter mip    !! CHANGES THE SIZE OF THE IMAGE!
      
    backgroundColor, // The background color we should
    // consider transparent.  This gets replaced by
    // a completely CLEAR color.

    &sprite->imageInfo,
    NULL,              // no palette
    &sprite->spritesheet // the texture member variable to store
    // the entire spritesheet in
  ) ;

  return hr ;
}

HRESULT AssetMan::loadTexturePow2( IDirect3DDevice9 *gpu, Sprite* sprite, const char *filename, D3DCOLOR backgroundColor )
{
  // D3DXCreateTextureFromFile()
  // http://msdn.microsoft.com/en-us/library/ee417125%28VS.85%29.aspx

  HRESULT hr = D3DXCreateTextureFromFileExA(
    gpu,
    filename,
                                 
    // D3DX_DEFAULT will round up to nearest power of 2, or
    // D3DX_DEFAULT_NONPOW2 for actual size.
    D3DX_DEFAULT,
    D3DX_DEFAULT,
      
    D3DX_FROM_FILE,  // D3DX_FROM_FILE means no mips.
    // D3DX_DEFAULT will create a complete mipmap chain.

    0, // Usage:  you could make the tex a render target with D3DUSAGE_RENDERTARGET but we don't want to
    D3DFMT_UNKNOWN,  // take the format from the file.
    D3DPOOL_MANAGED, // So we don't have to re-load textures if the device is lost
    D3DX_FILTER_NONE,    // filter image !! CHANGES THE SIZE OF THE IMAGE!
    D3DX_FILTER_NONE,    // filter mip   !! CHANGES THE SIZE OF THE IMAGE!
      
    backgroundColor, // The background color we should
    // consider transparent.  This gets replaced by
    // a completely CLEAR color.

    &sprite->imageInfo,
    NULL,              // no palette
    &sprite->spritesheet // the texture member variable to store
    // the entire spritesheet in
  ) ;

  return hr ;
}

bool AssetMan::loadSound( SoundMan* soundMan, int soundId, char * filename, int options )
{
  FMOD_SOUND *newSound ;
  
  // Load the sound
  bool success = FMOD_ErrorCheck(
    FMOD_System_CreateSound(
    // !! Too often effects don't work on
    // target pc's with crummy sound cards.
    // We should check the card's caps
    // before setting.
    soundMan->fmodSys, filename, FMOD_SOFTWARE | options, 0, &newSound ) ) ;

  if( !success )
  {
    error( "Could not load %s, check file exists", filename ) ;
    return false;
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
  return true;
}






