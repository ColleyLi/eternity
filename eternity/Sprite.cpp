#include "Sprite.h"
#include "GamePak.h"
#include "AssetMan.h"

SpriteMan *Sprite::spriteMan = 0;
AssetMan *Sprite::assetMan = 0;
IDirect3DDevice9 *Sprite::gpu = 0;

Sprite::Sprite( int id, int w, int h, IDirect3DTexture9 *tex )
{
  sheetWidth = spriteWidth = w ;
  sheetHeight= spriteHeight= h ;
    
  spritesheet = tex ;

  n = 0 ; 
  numFrames = 1 ;

  internalClock = 0 ;
  secondsPerFrame = 1 ;
    
  originalFilename = "Generated texture (no file)" ;
  info( "Artifically created sprite w=%d h=%d", w, h ) ;

  spriteMan->addSprite( id, this );
}

Sprite::Sprite( int id, const char *filename, D3DCOLOR backgroundColor, int singleFrameWidth,
  int singleFrameHeight, int numFramesToUse, float timeBetweenFrames )
{
  originalFilename = filename ;
  if( !filename )
  {
    bail( "NULL filename received in sprite load" ) ;
  }

  // initialize internal clock and animation parameters
  n = 0 ;
  internalClock = 0 ;
  secondsPerFrame = timeBetweenFrames ;

  // save these off
  spriteWidth = singleFrameWidth ;
  spriteHeight = singleFrameHeight ;
  numFrames = numFramesToUse ;

  // Some parameter value checking
  if( !spriteWidth )
    warning( "Sprite width was 0.  Not changing anything, but are you sure that's what you want?" ) ;
  if( !spriteHeight )
    warning( "Sprite height was 0.  Not changing anything, but are you sure that's what you want?" ) ;
  if( !numFrames )
    warning( "numFrames was 0.  Not changing anything, but are you sure that's what you want?" ) ;


  HRESULT hr ;
    
  // Here, we're loading from a file like normal
  //!! This was a mistake, assetman enforcing its touching disk had no pt.
  hr = assetMan->loadTexturePow2( gpu, this, filename, backgroundColor ) ;

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

    int maxFramesAllowable = numAcross * numDown ; // could be 1 too.. whatever

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
    warning( "Texture %s didn't load using D3DX function, trying GDI+ function..", originalFilename ) ;

    GDIPlusTexture * gdiPlusTex = GDIPlusTexture::CreateFromFile( gpu, filename ) ;
      
    // haxx
    spriteWidth = imageInfo.Width = gdiPlusTex->getWidth() ;
    spriteHeight = imageInfo.Height = gdiPlusTex->getHeight() ;

      

    numFrames = 1 ;
    secondsPerFrame = SPRITE_INFINITY_LONG ; // no animation

    spritesheet = gdiPlusTex->getTexture() ;

    delete gdiPlusTex ;

    if( !spritesheet )
    {
      // Not extracting frames of an animated gif here,
      // but GIF2PNG (http://gnuwin32.sourceforge.net/packages/pngutils.htm)
      // for easy batch conversion of .gif to .png, even extracts
      // frames of an animated gif
      error( "Your texture %s has FAILED TO LOAD, does the file exist?  "
        "Supported file formats, d3d: .bmp, .dds, .dib, .hdr, .jpg, .pfm, .png, .ppm, and .tga. "
        "Supported file formats, gdi+: BMP, GIF, JPEG, PNG, TIFF, and EMF. "
        "Don't blame me, blame Microsoft. "
        "Loading placeholder texture instead, for now.", filename ) ;

      bail( "A texture failed to load" ) ;
    }
  }

  spriteMan->addSprite( id, this );
}