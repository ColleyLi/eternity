#ifndef ASSETMAN_H
#define ASSETMAN_H


#include "Sprite.h"


// SoundMan becomes part of AssetMan via inheritance,
// providing AssetMan with Sound loading capabilities.
// At the same time SoundMan remains orthogonal to
// everything else, with its sound-related concerns
// relegated to a single file.

#include "SoundMan.h"

// Eventually ObjFileParse.h should migrate into
// this file.




class AssetMan : public SoundMan
{
public:
  /// Collections of loaded objects
  HRESULT loadTextureNonPow2( IDirect3DDevice9 *gpu, Sprite* sprite, char *filename, D3DCOLOR backgroundColor );
  HRESULT loadTexturePow2( IDirect3DDevice9 *gpu, Sprite* sprite, const char *filename, D3DCOLOR backgroundColor );

  bool loadSound( SoundMan* soundMan, int soundId, char * filename, int options = 0 ) ;
} ;

#endif