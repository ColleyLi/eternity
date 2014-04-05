#ifndef SPRITE_H
#define SPRITE_H

#include <d3d9.h>      // core direct3d
#include <d3dx9.h>     // aux libs

#define SPRITE_READ_FROM_FILE -5925499 /* read the property from the file */
// Why is it -5925499?  It was too likely
// someone would WANT TO SCALE by -1.
// So we chose a large, large value
// You won't ever want to draw something
// -5925499 pixels would you?
// -5925499 also happens to be
// a telephone number .. :).
#define SPRITE_INFINITY_LONG -1 /* don't advance the frame rate */

#include "helperFunctions.h"
#include "GDIPlusTexture.h"

class SpriteMan;
class AssetMan;

class Sprite
{
  friend class AssetMan;
private:
  static IDirect3DDevice9* gpu ;

private:
  IDirect3DTexture9 *spritesheet ; // this really should be stored
  // in AssetMan.

  D3DXIMAGE_INFO imageInfo ;
  int sheetWidth, sheetHeight ; 

  const char *originalFilename ;
  int spriteWidth ;
  int spriteHeight ;

  // Animation and frame counting
  int n ; /*!< (n)umber frame you're on */

  int numFrames ; /*!< the number of frames
  this sprite is known to have.  A sheet
  can have some empty cells at the end
  that are not used. */

  float internalClock ;     // the internalClock that
  // keeps track of "what time it is".  This is used
  // to change what sprite to draw with when enough
  // time has passed, based on the "secondsPerFrame" variable

  float secondsPerFrame ;   // Amount of time to spend on each frame before advancing
  // debating whether to name this either
  // "secondsPerFrame" or "timeBetweenFrames"

  RECT *rects ;  // CACHED set of source rectangles
  // to draw at each frame.  A mild optimization
  // to avoid recomputing the source rects each frame.
  // If you have hundreds of sprites, this could add up

public:
  // Load a static sprite with only 1 frame (not animated)
  // Loads width and height values from file
  // Consider "backgroundColor" as
  // the transparent color (doesn't have to be
  // photoshop "transparent")
  static Sprite* staticSprite( int id, const char *filename, D3DCOLOR backgroundColor )
  {
    return new Sprite( id, filename, backgroundColor );
  }


  // Load an animated sprite from a file,
  // singleFrameWidth and singleFrameHeight
  // are the width and height of a single frame,
  // numFrames is the number of frames you've got
  // in the sprite.
  static Sprite* animatedSprite( int id, const char *filename,
    D3DCOLOR backgroundColor, int singleFrameWidth, int singleFrameHeight,
    int numFrames, float timeBetweenFrames )
  {
    return new Sprite( id, filename, backgroundColor, singleFrameWidth, singleFrameHeight,
      numFrames, timeBetweenFrames );
  }

  
  
  // Provide a ctor that accepts a passed-in texture
  Sprite( int id, int w, int h, IDirect3DTexture9 *tex );

  // This function assumes we're loading a
  // spritesheet for an animated sprite.

  // singleFrameWidth and singleFrameHeight
  // are the width and height for A SINGLE FRAME
  // of the animated sprite, not the SPRITESHEET itself.

  // backgroundColor GETS ERASED TO TRANSPARENT.
  // If you're using PNG images, then you don't
  // need a background color (because PNG supports
  // transparency inside the file itself)
  Sprite(
    int id,
    const char *filename,
  
    //!! Avoid so many default parameters

    D3DCOLOR backgroundColor = D3DCOLOR_ARGB( 0,0,0,0 ), // defaults to transparent black (i.e. no effect)
    
    // If you are loading in
    // a SPRITE SHEET, then singleFrameWidth
    // and singleFrameHeight should be
    // the width of a SINGLE IMAGE on that sheet.
    // SPRITE_READ_FROM_FILE for these two parameters
    // means we will assume
    // the entire sheet is meant to be a single sprite
    int singleFrameWidth = SPRITE_READ_FROM_FILE,
    int singleFrameHeight = SPRITE_READ_FROM_FILE,
    
    int numFramesToUse = SPRITE_READ_FROM_FILE,
    // If you specify SPRITE_READ_FROM_FILE,
    // then the number of frames it will count
    // are based on singleFrameWidth
    // and singleFrameHeight,
    // but it will assume every cell is used
    float timeBetweenFrames = 0.5f
  );

  ~Sprite()
  {
    SAFE_RELEASE( spritesheet ) ;
  }
  
public:
  float getTimePerFrame()
  {
    return secondsPerFrame ;
  }

  /// How long to stay at frame before advancing to the next frame.
  /// Set to any negative value (e.g. SPRITE_INFINITY_LONG)
  /// to prevent the sprite from animating
  void setTimePerFrame( float secondsToSpendOnEachFrame )
  {
    secondsPerFrame = secondsToSpendOnEachFrame ;
  }

  void setFrame( int frameNumber )
  {
    // no negative frame numbers!
    if( frameNumber < 0 )
    {
      warning( "Attempt to set negative frame # %d on sprite=%s, "
        "setting to frame=0", frameNumber, originalFilename ) ;
      frameNumber = 0 ;
    }

    n = frameNumber ; // set the frame number
    if( n >= numFrames )
      n = 0 ;
  }
  
  // advance this sprite by this much TIME
  void advance( float timeElapsed )
  {
    // negative value for "secondsPerFrame" means
    // to NOT animate.
    if( secondsPerFrame < 0 ) // SPRITE_INFINITY_LONG is -1,
      return ; // we're not going to increment the frame.

    internalClock += timeElapsed ;

    // if we exceeded the amount of time to spend
    // on each frame ..
    if( internalClock > secondsPerFrame )
    {
      n++ ; // .. then advance the frame number
      if( n >= numFrames )
        n = 0 ;

      internalClock = 0 ;  // .. and reset the internalClock to 0
    }
    
  }
  
  // Gets you the rectangle for a specific frame
public:  //!! MAKE THIS PRIVATE so user must use getCurrentRectangle()
  RECT getRectForFrame( int frameIndex )
  {
    if( frameIndex >= numFrames )
    {
      warning( "Frame index out of bounds (frame=%d).  Giving you the last frame instead.", frameIndex ) ;
      frameIndex = numFrames - 1 ; // because frames are
      // indexed from 0, so the LAST frame is numFrames - 1
      // (because FIRST frame is frame 0, second frame = frame 1..
      //  so last frame = numFrames - 1)
    }

    // Now, how many sprites will there be
    // across the sheet?
    int numAcross = getSheetWidth()  / spriteWidth ;
    int numDown   = getSheetHeight() / spriteHeight ;
    
    // Get the row and column this frameIndex
    // falls in the sheet
    int row = frameIndex / numAcross ;
    int col = frameIndex % numAcross ;
    
    #pragma region Explaining how to get row and col
    /*
         0   1   2   <- col
       _____________
       |0  |1  |2  |
     0 | x | x | x |
       |___|___|___|
       |3  |4  |5  |
     1 | x | x | x |
       |___|___|___|
     ^
     |
    row

    columns across
    rows go down
    frame numbers are in the boxes
    (the x is supposed to be a picture of the sprite in that frame)

    so, say I want frame 4.  well that's
    int row = 4 / 3 = 1   [ INTEGER DIVISION, cuts off the decimal ]
    int col = 4 % 3 = 1   [ MODULUS.. 4 % 3 means "the remainder of when 4 is divided by 3".  __Try it on paper__ to see. ]

    which means frame 4 sits at (1, 1).  Which is correct
    when you look at the picture.

    In case you didn't know about INTEGER DIVISION
    its just a rule in C++ that if you divide
    two integers together the result is always
    __ROUNDED DOWN__ to the nearest integer.
    The decimal is basically LOST.



    Now say I want frame 1.  Well, that's
    int row = 1 / 3 = 0
    int col = 1 % 3 = 1

    so frame 1 sits at (0, 1).
    
    ( In this notation here (0, 1) its (row,col), row goes first.)
    
    */
    #pragma endregion

    // Compute the pixel location on the sprite sheet itself
    int pixelLocationX = col * spriteWidth ;
    int pixelLocationY = row * spriteHeight ;

    // Now create the bounds rectangle struct
    RECT rect ;

    rect.left = pixelLocationX ;
    rect.right = pixelLocationX + spriteWidth ;

    rect.top = pixelLocationY ;
    rect.bottom = pixelLocationY + spriteHeight ;

    // Let's assert to make sure the sprite
    // isn't out of bounds.
    // ( CAN assume spriteWidth, spriteHeight will be +
    // (checked in CTOR). )
    if( rect.left < 0 ||
        rect.top < 0  ||
        rect.right > getSheetWidth() ||
        rect.bottom > getSheetHeight() )
    {
      error( "Internal error:  sprite index out of bounds: "
        "l=%d t=%d r=%d b=%d, but sprite "
        "width=%d height=%d",
        rect.left, rect.top, rect.right, rect.bottom,
        getSheetWidth(), getSheetHeight() ) ;
    }

    return rect ;
  }

public:
  static SpriteMan *spriteMan;
  static AssetMan *assetMan;

  // gets you the current rectangle for the sprite on this frame
  RECT getRect()
  {
    return getRectForFrame( n ) ;
  }


  // This really should return an
  // "instruction" type of object
  // which is a subrectangle to draw
  // of the texture for the current frame
  IDirect3DTexture9* getTexture()
  {
    return spritesheet ;
  }

  int getSpriteWidth()
  {
    return spriteWidth ;
  }

  int getSpriteHeight()
  {
    return spriteHeight ;
  }

  int getSheetWidth()
  {
    return imageInfo.Width ;
  }

  int getSheetHeight()
  {
    return imageInfo.Height ;
  }

  int getCenterX()
  {
    return getSpriteWidth() / 2 ;
  }

  int getCenterY()
  {
    return getSpriteHeight() / 2 ;
  }

  float getScaleXFor( float desiredWidth )
  {
    return desiredWidth / getSpriteWidth() ;
  }

  float getScaleYFor( float desiredHeight )
  {
    return desiredHeight / getSpriteHeight() ;
  }

  const char* getOriginalFilename()
  {
    return originalFilename ;
  }
} ;



#endif // SPRITE_H