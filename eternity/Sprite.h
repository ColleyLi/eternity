#ifndef SPRITE_H
#define SPRITE_H

#include <assert.h>

#include <d3d9.h>      // core direct3d
#include <d3dx9.h>     // aux libs

#define SPRITE_READ_FROM_FILE -1 /* read the property from the file */
#define SPRITE_INFINITY_LONG -1 /* don't advance the frame rate */

class Sprite
{
private:
  IDirect3DTexture9 *spritesheet ;
  D3DXIMAGE_INFO imageInfo ;

  int spriteWidth ;
  int spriteHeight ;

  // Animation and frame counting
  int n ; // (n)umber frame you're on
  int numFrames ; // the number of frames
  // this sprite is known to have.  A sheet
  // can have some empty cells at the end
  // that are not used.

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
  //!!DELETE ME.  
  // provide a ctor that accepts an HDC
  Sprite( int w, int h, IDirect3DTexture9 *tex )
  {
    spriteWidth = w ;
    spriteHeight = h ;

    //!! haxx
    imageInfo.Width = w ;
    imageInfo.Height = h ;
    
    spritesheet = tex ;

    n = 0 ; 
    numFrames = 1 ;

    internalClock = 0 ;
    secondsPerFrame = 1 ;
    
    info( "Artifically created sprite w=%d h=%d", w, h ) ;
  }

  // This function assumes we're loading a
  // spritesheet for an animated sprite.

  // singleSpriteWidth and singleSpriteHeight
  // are the width and height for A SINGLE FRAME
  // of the animated sprite, not the SPRITESHEET itself.

  // backgroundColor GETS ERASED TO TRANSPARENT.
  // If you're using PNG images, then you don't
  // need a background color (because PNG supports
  // transparency inside the file itself)
  Sprite( IDirect3DDevice9 *gpu, char *filename,
    D3DCOLOR backgroundColor = D3DCOLOR_ARGB( 0,0,0,0 ), // defaults to transparent black (i.e. no effect)
    
    // If you are loading in
    // a SPRITE SHEET, then singleSpriteWidth
    // and singleSpriteHeight should be
    // the width of a SINGLE IMAGE on that sheet.
    // SPRITE_READ_FROM_FILE for these two parameters
    // means we will assume
    // the entire sheet is meant to be a single sprite
    int singleSpriteWidth = SPRITE_READ_FROM_FILE,
    int singleSpriteHeight = SPRITE_READ_FROM_FILE,
    
    int numFramesToUse = SPRITE_READ_FROM_FILE,
    // If you specify SPRITE_READ_FROM_FILE,
    // then the number of frames it will count
    // are based on singleSpriteWidth
    // and singleSpriteHeight,
    // but it will assume every cell is used
    float timeBetweenFrames = 0.51f
  )
  {
    // initialize internal clock and animation parameters
    n = 0 ;
    internalClock = 0 ;
    secondsPerFrame = timeBetweenFrames ;

    // save these off
    spriteWidth = singleSpriteWidth ;
    spriteHeight = singleSpriteHeight ;
    numFrames = numFramesToUse ;

    #pragma region param value error_check
    if( !spriteWidth )
      warning( "Sprite width was 0.  Not changing anything, but are you sure that's what you want?" ) ;
    if( !spriteHeight )
      warning( "Sprite height was 0.  Not changing anything, but are you sure that's what you want?" ) ;
    if( !numFrames )
      warning( "numFrames was 0.  Not changing anything, but are you sure that's what you want?" ) ;

    if( spriteWidth < -1 )
    {
      warning( "singleSpriteWidth can't be negative (sent %d).  Reversing the sign.", singleSpriteWidth ) ;
      spriteWidth = -spriteWidth ;
    }
    if( spriteHeight < -1 )
    {
      warning( "singleSpriteHeight can't be negative (sent %d).  Reversing the sign.", singleSpriteHeight ) ;
      spriteHeight = -spriteHeight ;
    }
    if( numFrames < -1 )
    {
      warning( "numFrames can't be negative (sent %d).  Reversing the sign.", numFrames ) ;
      numFrames = -numFrames ;
    }
    #pragma endregion




    HRESULT hr ;

    // If the filename was NULL, its a signal to generate
    // the default texture
    if( !filename )
    {
      hr = D3DXCreateTexture( gpu, 16, 16, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &spritesheet ) ;
      DX_CHECK( hr, "create texture from scratch" ) ;

      // Query the info out
      D3DSURFACE_DESC desc ;
      spritesheet->GetLevelDesc( 0, &desc ) ;
      imageInfo.Height = desc.Height ;
      imageInfo.Width = desc.Width ;
      
      imageInfo.Depth = 32 ;
      imageInfo.Format = desc.Format ;
      imageInfo.MipLevels = spritesheet->GetLevelCount() ;
      


      D3DLOCKED_RECT lockedRect ;
      if( DX_CHECK( spritesheet->LockRect( 0, &lockedRect, NULL, 0 ), "Lock rect, texture gen" ) )
      {
        this->numFrames = 4 ;
        this->secondsPerFrame = 0.5f ;
        this->n = 0 ;
        this->internalClock = 0 ;
        this->spriteHeight = 8 ;
        this->spriteWidth = 8 ;
        
        unsigned int *data = new unsigned int[ 16*16 ] ;
        int squareNo = 0 ;

        // Cover the first 8x8 square in white
        for( int i = squareNo*8*8 ; i < (squareNo+1)*8*8 ; i++ )
          data[ i ] = D3DCOLOR_ARGB( 255, 255, 255, 255 ) ;
        squareNo++ ;

        // Cover the next 8x8 square in blue
        for( int i = squareNo*8*8 ; i < (squareNo+1)*8*8 ; i++ )
          data[ i ] = D3DCOLOR_ARGB( 255, 0, 0, 255 ) ;
        squareNo++ ;

        // Cover the next 8x8 square in white
        for( int i = squareNo*8*8 ; i < (squareNo+1)*8*8 ; i++ )
          data[ i ] = D3DCOLOR_ARGB( 255, 0, 0, 255 ) ;
        squareNo++ ;

        // Cover the next 8x8 square in black
        for( int i = squareNo*8*8 ; i < (squareNo+1)*8*8 ; i++ )
          data[ i ] = D3DCOLOR_ARGB( 255, 0, 0, 0 ) ;

        lockedRect.pBits = data ;

        DX_CHECK( spritesheet->UnlockRect( 0 ), "Unlock rect, texture gen" ) ;

        free( data ) ;

        info( "Texture generated successfully" ) ;
      }
      else
      {
        error( "'Default' texture generation failed" ) ;
      }
      

      // skip the rest of the function
      return ;
    }
    
    
    
    // Here, we're loading from a file like normal
    hr = loadTexturePow2( gpu, filename, backgroundColor ) ;

    /////!!!!!!!!!!!!!!!!

    //!!! TO AVOID THE PROBLEM USE D3DXGetImageInfoFromFileA()!!!!!
    
    if( DX_CHECK( hr, "Texture load" ) )
    {
      // If these are still SPRITE_READ_FROM_FILE,
      // then the user intended to have them
      // set by the file's properties
      if( spriteWidth == SPRITE_READ_FROM_FILE )
        spriteWidth = imageInfo.Width ;
      
      if( spriteHeight == SPRITE_READ_FROM_FILE )
        spriteHeight = imageInfo.Height ;

      // Compute maximum # frames allowable
      // based on the spritesheet texture
      // width and height and spriteWidth and
      // spriteHeight
      int numAcross = imageInfo.Width / spriteWidth ; // could be 1
      int numDown = imageInfo.Height / spriteHeight ; // could be 1 also

      int maxFramesAllowable = numAcross * numDown ; // could be 1 to.. whatever

      // Make sure the number of frames is logical/possible
      if( numFrames > maxFramesAllowable )
      {
        warning( "%d sprites (%d x %d) won't fit on your "
          "(%d x %d) sheet, defaulting to %d frames.",
          numFrames,
          spriteWidth, spriteHeight,
          imageInfo.Width, imageInfo.Height,
          maxFramesAllowable ) ;

        numFrames = maxFramesAllowable ;
      }

      if( numFrames == SPRITE_READ_FROM_FILE )
      {
        numFrames = maxFramesAllowable ;

        info( "Defaulted to %d frames.  Note I'm not "
          "checking for empty frames.", maxFramesAllowable ) ;
      }

      info( "'%s' loaded, width=%d, height=%d, numFrames=%d", 
        filename, 
        imageInfo.Width, imageInfo.Height,
        numFrames ) ;


      //!! Now, we should "cache up" the RECTs
      // to use on each frame.  This makes
      // for a bit more memory usage, and
      // a bit of a hit up front, but
      // better runtime performance

      
    }
    else
    {
      error( "Your texture %s has FAILED TO LOAD, does the file exist?  "
        "Supported file formats: .bmp, .dds, .dib, .hdr, .jpg, .pfm, .png, .ppm, and .tga. "
        "Notice .GIF IS NOT SUPPORTED. "
        "Don't blame me, blame Microsoft. "
        "Loading placeholder texture instead, for now.", filename ) ;

      // !! load it through GDI+
      // GIF2PNG (http://gnuwin32.sourceforge.net/packages/pngutils.htm)
      // for easy batch conversion of .gif to .png, even extracts
      // frames of an animated gif
  
      bail( "A texture failed to load" ) ;
    }
  }

  ~Sprite()
  {
    SAFE_RELEASE( spritesheet ) ;
  }
private:
  HRESULT loadTextureNonPow2( IDirect3DDevice9 *gpu, char *filename, D3DCOLOR backgroundColor )
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
      D3DX_DEFAULT,    // filter image
      D3DX_DEFAULT,    // filter mip
      
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

  HRESULT loadTexturePow2( IDirect3DDevice9 *gpu, char *filename, D3DCOLOR backgroundColor )
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
      D3DX_DEFAULT,    // filter image
      D3DX_DEFAULT,    // filter mip
      
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

  public:

  // How long to stay at frame before advancing to the next frame
  void setTimePerFrame( float secondsToSpendOnEachFrame )
  {
    secondsPerFrame = secondsToSpendOnEachFrame ;
  }
  
  // advance this sprite by this much TIME
  void advance( float timeElapsed )
  {
    if( secondsPerFrame == SPRITE_INFINITY_LONG )
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

} ;



#endif // SPRITE_H