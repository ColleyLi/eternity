#include "AssetMan.h"

void AssetMan::loadSprite( int spriteId, const char *filename, D3DCOLOR backgroundColor )
{
  Sprite *sprite = new Sprite( filename ) ;
  addSprite( spriteId, sprite ) ;
}

// id is how you will refer to this sprite after its been loaded
// filename is just the filename on the disk drive
void AssetMan::loadAnimatedSprite( int spriteId, const char *filename,
                         D3DCOLOR backgroundColor,
                         int singleFrameWidth, int singleFrameHeight,
                         int numFrames, float timeBetweenFrames )
{
  Sprite *sprite = new Sprite( filename, backgroundColor,
    singleFrameWidth, singleFrameHeight, numFrames, timeBetweenFrames ) ;
  addSprite( spriteId, sprite ) ;
}

HRESULT AssetMan::loadTextureNonPow2( char *filename, D3DCOLOR backgroundColor )
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

    &this->imageInfo,
    NULL,              // no palette
    &this->spritesheet // the texture member variable to store
    // the entire spritesheet in
  ) ;

  return hr ;
}

HRESULT AssetMan::loadTexturePow2( const char *filename, D3DCOLOR backgroundColor )
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

    &this->imageInfo,
    NULL,              // no palette
    &this->spritesheet // the texture member variable to store
    // the entire spritesheet in
  ) ;

  return hr ;
}
