#ifndef ASSETMAN_H
#define ASSETMAN_H


#include "Sprite.h"

// Eventually ObjFileParse.h should migrate into
// this file.

class AssetMan
{
  // Load a static sprite with only 1 frame (not animated)
  // Loads width and height values from file
  // Consider "backgroundColor" as
  // the transparent color (doesn't have to be
  // photoshop "transparent")
  void loadStaticSprite( int id, const char *filename, D3DCOLOR backgroundColor ) ;

  // Load an animated sprite from a file,
  // singleFrameWidth and singleFrameHeight
  // are the width and height of a single frame,
  // numFrames is the number of frames you've got
  // in the sprite.
  void loadAnimatedSprite( int id, const char *filename,
    D3DCOLOR backgroundColor,
    int singleFrameWidth,
    int singleFrameHeight,
    int numFrames,
    float timeBetweenFrames ) ;

} ;

#endif