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
