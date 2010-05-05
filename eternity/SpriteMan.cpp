#include "SpriteMan.h"


SpriteMan::SpriteMan()
{
  // nuffin.  Everything happens
  // in initSpriteMan().
}

SpriteMan::~SpriteMan()
{
  // Delete all the sprites
  foreach( SpriteMapIter, iter, sprites )
  {
    delete iter->second ;
  }

  // Release all the fonts
  foreach( FontMapIter, fontIter, fonts )
  {
    SAFE_RELEASE( fontIter->second ) ;
  }

  // release the default font and default sprite renderer
  SAFE_RELEASE( id3dxSpriteRenderer ) ;
  SAFE_RELEASE( id3dxDefaultFont ) ;

}

void SpriteMan::initSpriteMan( IDirect3DDevice9 *theGpu, int scrWidth, int scrHeight )
{
  lgpu = theGpu ; 
  initDefaultSprite() ;
  initWhitePixel() ;

  D3DXCreateFontA( lgpu, 18, 0, FW_BOLD, 1,
  FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
  DEFAULT_PITCH | FF_DONTCARE, "Arial", &id3dxDefaultFont ) ;

  D3DXCreateSprite( lgpu, &id3dxSpriteRenderer ) ;

  // The above 2 (defaultFont + defaultSprite)
  // are registered with D3DWindow when
  // this function call returns

  D3DXCreateLine( lgpu, &id3dxLine ) ;

}

void SpriteMan::initDefaultSprite()
{
  IDirect3DTexture9 *tex ;
  int w = 16, h = 16 ;
  DX_CHECK( D3DXCreateTexture( lgpu, w, h, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &tex ), "Create default sprite" ) ;

  D3DLOCKED_RECT lockedRect ;

  DX_CHECK( tex->LockRect( 0, &lockedRect, NULL, 0 ), "Lock default sprite" ) ;
  DWORD *ptr = reinterpret_cast<DWORD*>(lockedRect.pBits) ;
  RECT topLeft = { 0, 0, w/2, h/2 } ; // ltrb
  RECT topRight = { w/2, 0, w, h/2 } ;
  RECT bottomLeft = { 0, h/2, w/2, h } ;
  RECT bottomRight = { w/2, h/2, w, h } ;

  D3DCOLOR red = D3DCOLOR_ARGB( 200, 255, 0, 0 ) ;
  D3DCOLOR green = D3DCOLOR_ARGB( 200, 0, 255, 0 ) ;
  D3DCOLOR blue = D3DCOLOR_ARGB( 200, 0, 0, 255 ) ;
  D3DCOLOR yellow = D3DCOLOR_ARGB( 200, 255, 255, 0 ) ;
  setRectangle( ptr, topLeft, w, h, &blue ) ;
  setRectangle( ptr, topRight, w, h, &yellow ) ;
  setRectangle( ptr, bottomLeft, w, h, &green ) ;
  setRectangle( ptr, bottomRight, w, h, &red ) ;
  
  DX_CHECK( tex->UnlockRect( 0 ), "Unlock default sprite" ) ;

  defaultSprite = new Sprite( w, h, tex ) ;
  //addSprite( 0, whitePixel ) ; // no need to save a ref in the sprites map.

}

void SpriteMan::initWhitePixel()
{
  IDirect3DTexture9 *tex ;
  DX_CHECK( D3DXCreateTexture( lgpu, 1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &tex ), "Create 1x1 pixel patch" ) ;

  D3DLOCKED_RECT lockedRect ;

  DX_CHECK( tex->LockRect( 0, &lockedRect, NULL, 0 ), "Lock 1x1 pixel patch" ) ;
  ((__int32*)lockedRect.pBits)[0] = D3DCOLOR_ARGB( 255,255,255,255 ) ;
  DX_CHECK( tex->UnlockRect( 0 ), "Unlock 1x1 pixel patch" ) ;

  whitePixel = new Sprite( 1, 1, tex ) ;
  //addSprite( 0, whitePixel ) ; // no need to save a ref in the sprites map.

}


// You need this wrapper function to hide 'gpu'
GDIPlusTexture* SpriteMan::createGDISurface( int width, int height )
{
  GDIPlusTexture *gdiPlusTex = new GDIPlusTexture( lgpu, width, height, true ) ;
  return gdiPlusTex ;
}

void SpriteMan::addSpriteFromGDIPlusTexture( int id, GDIPlusTexture* tex )
{
  Sprite *sprite = new Sprite(
    tex->getWidth(), tex->getHeight(), tex->getTexture() ) ;

  addSprite( id, sprite ) ;
}


void SpriteMan::spriteManStep( float time )
{
  foreach( SpriteMapIter, iter, sprites )
  {
    iter->second->advance( time ) ;
  }
}

//This is a little bit of cruft
void SpriteMan::spriteManSetWindowSize( int width, int height )
{
  screenWidth = width ;
  screenHeight = height ;
}









void SpriteMan::drawBox( D3DCOLOR color, RECT &r )
{
  drawBox( color, r.left, r.top, r.right - r.left, r.bottom - r.top ) ;
}

void SpriteMan::drawBox( D3DCOLOR color, int xLeft, int yTop, int width, int height )
{
  D3DXVECTOR2 vec2Pos( xLeft, yTop ) ;
  D3DXVECTOR2 vec2Scale( width, height ) ;
  
  D3DXMATRIX tMatrix ;
  
  D3DXMatrixTransformation2D(
    &tMatrix,
    NULL,  // leave it in the top left corner
    0.0f,  // wacky warp behavior -- don't use
    &vec2Scale, // how much to stretch the pixelPatch
    NULL,  // rotate from top left corner..
    0,     // rotation angle is always 0:  you shouldn't be rotating this.  you should
           // be rotating the sprite AFTER its been produced and you're using it to draw.
    &vec2Pos   // translation:  relative to top left corner
  ) ;
  
  DX_CHECK( id3dxSpriteRenderer->Begin( D3DXSPRITE_ALPHABLEND ), "Begin draw box, sprite" ) ;
  DX_CHECK( id3dxSpriteRenderer->SetTransform( &tMatrix ), "Set matrix" ) ;
  DX_CHECK( id3dxSpriteRenderer->Draw( whitePixel->getTexture(), NULL, NULL, NULL, color ), "Draw box" ) ;
  DX_CHECK( id3dxSpriteRenderer->End(), "End draw box, sprite" ) ;
}

void SpriteMan::drawBoxCentered( D3DCOLOR color, int xCenter, int yCenter, int width, int height )
{
  D3DXVECTOR2 vec2Pos( xCenter - width/2, yCenter - height/2 ) ;
  D3DXVECTOR2 vec2Scale( width, height ) ;

  D3DXMATRIX tMatrix ;
  
  D3DXMatrixTransformation2D(
    &tMatrix,
    NULL,  // leave it in the top left corner
    0.0f,  // wacky warp behavior -- don't use
    &vec2Scale, // how much to stretch the pixelPatch
    NULL,  // rotate from top left corner..
    0,     // rotation angle is always 0:  you shouldn't be rotating this.  you should
           // be rotating the sprite AFTER its been produced and you're using it to draw.
    &vec2Pos   // translation:  relative to top left corner
  ) ;
  
  DX_CHECK( id3dxSpriteRenderer->Begin( D3DXSPRITE_ALPHABLEND ), "Begin draw box, sprite" ) ;
  DX_CHECK( id3dxSpriteRenderer->SetTransform( &tMatrix ), "Set matrix" ) ;
  DX_CHECK( id3dxSpriteRenderer->Draw( whitePixel->getTexture(), NULL, NULL, NULL, color ), "Draw box centered" ) ;
  DX_CHECK( id3dxSpriteRenderer->End(), "End draw box, sprite" ) ;
}

void SpriteMan::getBoxDimensions( int fontId, char *str, RECT &r )
{
  // Retrieve the font
  ID3DXFont *font = id3dxDefaultFont ;

  if( fontId != DEFAULT_FONT )
  {
    FontMapIter fontEntry = fonts.find( fontId ) ;

    if( fontEntry == fonts.end() )
      warning( "Font %d does not exist, using default font instead", fontId ) ;
    else
      font = fontEntry->second ;
  }

  SetRect( &r, 0, 0, 1, 1 ) ;
  int height = font->DrawTextA(
    id3dxSpriteRenderer, str, -1,
    &r,
    DT_CALCRECT /* | dtOptions */, 0 ) ;
}

void SpriteMan::drawString( int fontId, char *str, D3DCOLOR color )
{
  RECT rect ;
  SetRect( &rect, 0, 0, screenWidth, screenHeight ) ;

  drawString( fontId, str, color, rect, DT_CENTER | DT_VCENTER ) ;
}

void SpriteMan::drawString( int fontId, char *str, D3DCOLOR color, float x, float y, float boxWidth, float boxHeight )
{
  RECT rect ;
  SetRect( &rect, x, y, x + boxWidth, y + boxHeight ) ;

  drawString( fontId, str, color, rect, DT_CENTER | DT_VCENTER ) ;
}

void SpriteMan::drawString( int fontId, char *str, D3DCOLOR color, float x, float y, float boxWidth, float boxHeight, DWORD formatOptions )
{
  RECT rect ;
  SetRect( &rect, x, y, x + boxWidth, y + boxHeight ) ;

  drawString( fontId, str, color, rect, formatOptions ) ;
}

void SpriteMan::drawString( int fontId, char *str, D3DCOLOR color, RECT &rect )
{
  drawString( fontId, str, color, rect, DT_CENTER | DT_VCENTER ) ;
}

void SpriteMan::drawString( int fontId, char *str, D3DCOLOR color, RECT &rect, DWORD formatOptions )
{
  // Retrieve the font
  ID3DXFont *font = id3dxDefaultFont ;

  if( fontId != DEFAULT_FONT )
  {
    FontMapIter fontEntry = fonts.find( fontId ) ;

    if( fontEntry == fonts.end() )
      warning( "Font %d does not exist, using default font instead", fontId ) ;
    else
      font = fontEntry->second ;
  }

  font->DrawTextA( NULL, str, -1, &rect, formatOptions, color ) ;
}


// Generates you a texture with colored
// text on a clear background
void SpriteMan::boxedTextSprite( int spriteId, char *str, D3DCOLOR textColor )
{
  RECT rect = { 0 } ;
  boxedTextSprite( spriteId, str, textColor, 0, rect, id3dxDefaultFont ) ;
}

void SpriteMan::boxedTextSprite( int spriteId, char *str, D3DCOLOR textColor, D3DCOLOR backgroundColor )
{
  RECT rect = { 0 } ;
  boxedTextSprite( spriteId, str, textColor, backgroundColor, rect, id3dxDefaultFont ) ;
}

void SpriteMan::boxedTextSprite( int spriteId, char *str, D3DCOLOR textColor, D3DCOLOR backgroundColor, int padding )
{
  RECT rect = { padding, padding, padding, padding } ;
  boxedTextSprite( spriteId, str, textColor, backgroundColor, rect, id3dxDefaultFont ) ;
}

void SpriteMan::boxedTextSprite( int spriteId, char *str, D3DCOLOR textColor, D3DCOLOR backgroundColor, RECT padding )
{
  boxedTextSprite( spriteId, str, textColor, backgroundColor, padding, id3dxDefaultFont ) ;
}

void SpriteMan::boxedTextSprite( int spriteId, char *str, D3DCOLOR textColor, D3DCOLOR backgroundColor, RECT padding, char *fontName, float size, int boldness, bool italics )
{
  ID3DXFont *id3dxFont ;

  D3DXCreateFontA( lgpu, size, 0, boldness, 1, italics,
    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
    DEFAULT_PITCH | FF_DONTCARE, fontName, &id3dxFont ) ;

  boxedTextSprite( spriteId, str, textColor, backgroundColor, padding, id3dxFont ) ;
  
  SAFE_RELEASE( id3dxFont ) ;
}

//private:
void SpriteMan::boxedTextSprite( int spriteId, char *str, D3DCOLOR textColor, D3DCOLOR backgroundColor, RECT padding, ID3DXFont *font ) 
{
  #pragma region compute the limiting rect
  RECT computedRect = { 0, 0, 1, 1 } ;
  
  int height = font->DrawTextA(
    id3dxSpriteRenderer, str, -1,
    &computedRect,
    DT_CALCRECT /* | dtOptions */, 0 ) ;

  info( "computed rect %d %d %d %d, height %d",
    computedRect.left, computedRect.top,
    computedRect.bottom, computedRect.right, height ) ;

  //computedRect.bottom += height ;

  RECT boxRect = computedRect ;

  boxRect.right += padding.left + padding.right ;
  boxRect.bottom += padding.top + padding.bottom ;

  RECT textRect = boxRect ;
  textRect.left += padding.left ;
  textRect.right -= padding.right ;
  textRect.top += padding.top ;
  textRect.bottom -= padding.bottom ;
  #pragma endregion

  // Create the render target surface to be the right
  // size for the text that is going to be drawn.
  #pragma region create the render target and pixel patch
  HRESULT hr ;
  IDirect3DTexture9 *renderTargetTexture ;
  int w = boxRect.right - boxRect.left ;
  int h = boxRect.bottom - boxRect.top ;
  
  hr = D3DXCreateTexture( lgpu, w, h, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &renderTargetTexture ) ;
  DX_CHECK( hr, "Create render target texture" ) ;

  IDirect3DTexture9 *pixelPatch4x4 ;
  
  hr = D3DXCreateTexture( lgpu, 4, 4, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pixelPatch4x4 ) ;
  DX_CHECK( hr, "Create 4x4 pixel patch" ) ;
  #pragma endregion

  // Lock the pixelpatch's rect and write the color data.
  #pragma region lock the rect
  D3DLOCKED_RECT lockedRect ;
  DX_CHECK( pixelPatch4x4->LockRect( 0, &lockedRect, NULL, 0 ), "Lock 4x4 pixel patch rect" ) ;
  __int32 *pData = reinterpret_cast<__int32*>( lockedRect.pBits ) ;  // 32 bit ints
  for( int i = 0 ; i < 4*4 ; i++ )
    pData[ i ] = backgroundColor ;
  DX_CHECK( pixelPatch4x4->UnlockRect( 0 ), "Unlock 4x4 pixel patch" ) ;
  #pragma endregion

  // Now draw out a rectangle sprite
  // using that pixel patch, stretching it
  #pragma region set up render target
  IDirect3DSurface9 *rtSurface, *screenSurface ;
  DX_CHECK( renderTargetTexture->GetSurfaceLevel( 0, &rtSurface ), "Get surface for render target" ) ;
  
  // Get the old surface target
  DX_CHECK( lgpu->GetRenderTarget( 0, &screenSurface ), "Get old render target" ) ;
  DX_CHECK( lgpu->SetRenderTarget( 0, rtSurface ), "Set new render target" ) ;
  #pragma endregion

  // Draw out the box shape and the text to that box.
  if( !DX_CHECK( lgpu->BeginScene(), "Begin scene on RT" ) )
  {
    error( "Are you trying to call boxedTextSprite() in the Draw() function!!  "
      "You should call it in the Init() function, generate the sprite, and save off "
      "the id." ) ;
    return ;
  }
  id3dxSpriteRenderer->Begin( D3DXSPRITE_ALPHABLEND ) ;
  
  // Because its a 4x4 pixel patch.
  D3DXVECTOR2 vec2Scale( w/4.0f, h/4.0f ) ;
  
  D3DXMATRIX tMatrix ;
  
  D3DXMatrixTransformation2D(
    &tMatrix,
    NULL,  // leave it in the top left corner
    0.0f,  // wacky warp behavior -- don't use
    &vec2Scale, // how much to stretch the pixelPatch
    NULL,  // rotate from top left corner..
    0,     // rotation angle is always 0:  you shouldn't be rotating this.  you should
           // be rotating the sprite AFTER its been produced and you're using it to draw.
    NULL   // translation:  start in top left corner
  ) ;
  
  id3dxSpriteRenderer->SetTransform( &tMatrix ) ;
  id3dxSpriteRenderer->Draw( pixelPatch4x4, NULL, NULL, NULL, D3DCOLOR_ARGB(255,255,255,255) ) ;

  D3DXMatrixIdentity( &tMatrix ) ;
  id3dxSpriteRenderer->SetTransform( &tMatrix ) ;
  font->DrawTextA( id3dxSpriteRenderer, str, -1, &textRect, DT_CENTER | DT_VCENTER, textColor ) ;
  
  id3dxSpriteRenderer->End() ;
  DX_CHECK( lgpu->EndScene(), "End scene on RT" ) ;

  DX_CHECK( lgpu->SetRenderTarget( 0, screenSurface ), "Set render target back to screen surface" ) ;
  


  // register the sprite
  Sprite *sprite = new Sprite( w, h, renderTargetTexture ) ;
  addSprite( spriteId, sprite ) ;

  info( "Successfully created your boxed text sprite, id=%d", spriteId ) ;

  // Destroy that 4x4 pixel patch
  SAFE_RELEASE( pixelPatch4x4 ) ;
}

void SpriteMan::drawSprite( int spriteId, float x, float y )
{
  drawSprite( spriteId, x, y, SPRITE_READ_FROM_FILE, SPRITE_READ_FROM_FILE, 0.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ), SpriteCentering::Center ) ;
}

void SpriteMan::drawSprite( int spriteId, float x, float y, SpriteCentering centering )
{
  drawSprite( spriteId, x, y, SPRITE_READ_FROM_FILE, SPRITE_READ_FROM_FILE, 0.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ), centering ) ;
}

void SpriteMan::drawSprite( int spriteId, float x, float y, D3DCOLOR modulatingColor )
{
  drawSprite( spriteId, x, y, SPRITE_READ_FROM_FILE, SPRITE_READ_FROM_FILE, 0.0f, modulatingColor, SpriteCentering::Center ) ;
}

void SpriteMan::drawSprite( int spriteId, float x, float y, float width, float height )
{
  drawSprite( spriteId, x, y, width, height, 0.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ), SpriteCentering::Center ) ;
}

void SpriteMan::drawSprite( int spriteId, float x, float y, float width, float height, SpriteCentering centering )
{
  drawSprite( spriteId, x, y, width, height, 0.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ), centering ) ;
}

void SpriteMan::drawSprite( int spriteId, float x, float y, float width, float height, float angle )
{
  drawSprite( spriteId, x, y, width, height, angle, D3DCOLOR_ARGB( 255, 255, 255, 255 ), SpriteCentering::Center ) ;
}

void SpriteMan::drawSprite( int spriteId, float x, float y, float width, float height, float angle, D3DCOLOR modulatingColor )
{
  drawSprite( spriteId, x, y, width, height, angle, modulatingColor, SpriteCentering::Center ) ;
}

void SpriteMan::drawSprite( int spriteId, float x, float y, float width, float height, float angle, D3DCOLOR modulatingColor, SpriteCentering centering )
{
  Sprite *sprite = getSprite( spriteId ) ;
  
  // There was a very subtle bug here
  // when someone tries to scale
  // by EXACTLY -1, it would show the
  // sprite full size.  To fix this,
  // we chose an extremely large constant
  // value for SPRITE_READ_FROM_FILE,
  // (-5925499), which no one will ever
  // chose (you'd never want to draw something
  // -5925499 pixels would you?)
  if( width == SPRITE_READ_FROM_FILE )
    width = sprite->getSpriteWidth() ;
  if( height == SPRITE_READ_FROM_FILE )
    height = sprite->getSpriteHeight() ;

  float spriteWidth = sprite->getSpriteWidth() ;
  float spriteHeight = sprite->getSpriteHeight() ;

  float scaleX = width / spriteWidth ;
  float scaleY = height / spriteHeight ;

  //D3DXVECTOR2 vec2Scale( sprite->getScaleXFor( width ), sprite->getScaleYFor( height ) ) ;
  D3DXVECTOR2 vec2Scale( scaleX, scaleY ) ;
  D3DXVECTOR2 vec2Trans( x, y ) ;

  //printf( "Scale %f %f\n", vec2Scale.x, vec2Scale.y ) ;
  
  D3DXVECTOR3 vec3Center( 0.0f, 0.0f, 0.0f ) ;
  if( centering == SpriteCentering::Center )
  {
    vec3Center.x = sprite->getCenterX() ;
    vec3Center.y = sprite->getCenterY() ;
  }
  // (otherwise its 0, for top left corner "centering")

  //D3DXVECTOR3 vec3Pos( x, y, 0 ) ; ; // do not use this.

  D3DXMATRIX matrix ;
  //D3DXMatrixIdentity( &matrix ) ;

  // convert angle to radians,
  // because it should be received in degrees,
  // but D3DXMatrix* wants radians
  angle = RADIANS( angle ) ;
  D3DXMatrixTransformation2D( &matrix, NULL, 0, &vec2Scale, NULL, angle, &vec2Trans ) ;

  //!! performance.  All this starting and stopping
  // the sprite rendered is going to hurt performance.
  // Really should batch these.
  id3dxSpriteRenderer->Begin( D3DXSPRITE_ALPHABLEND ) ;
  id3dxSpriteRenderer->SetTransform( &matrix ) ;

  RECT rect = sprite->getRect() ;
  id3dxSpriteRenderer->Draw(
    
    sprite->getTexture(),
    &rect,
    &vec3Center,   // The center of the sprite.  (0,0) is the upper left hand corner. OF THE SPRITE.
    NULL,      // the position of the sprite, (assuming you haven't translated it using a matrix already, which we DID do here!)
    modulatingColor
  
  ) ;
  id3dxSpriteRenderer->End();
}




void SpriteMan::drawLine( D3DXVECTOR2 & start, D3DXVECTOR2 & end, D3DCOLOR color, float thickness )
{
  // Batching. INACTIVE
  //lines.push_back( Line( start, end, color, thickness ) ) ;

  //!! DRAW IMMEDIATLEY FOR NOW.  This really
  // should be fixed to batched lines

  static D3DXVECTOR2 line[2];

  DX_CHECK( id3dxLine->SetWidth( thickness ), "line thickness set" ) ;
  DX_CHECK( id3dxLine->Begin(), "line begin" ) ;
  
  line[0] = start ;
  line[1] = end ;

  // Draw the line
  DX_CHECK( id3dxLine->Draw( line, 2, color ), "Drawing line" ) ;

  DX_CHECK( id3dxLine->End(), "line end" ) ;
}



void SpriteMan::loadSprite( int spriteId, char *filename )
{
  loadSprite( spriteId, filename, D3DCOLOR_ARGB( 0,0,0,0 ),
    SPRITE_READ_FROM_FILE, SPRITE_READ_FROM_FILE,
    SPRITE_READ_FROM_FILE, 0.5f ) ;
}

void SpriteMan::loadSprite( int spriteId, char *filename, D3DCOLOR backgroundColor )
{
  loadSprite( spriteId, filename, backgroundColor,
    SPRITE_READ_FROM_FILE, SPRITE_READ_FROM_FILE,
    SPRITE_READ_FROM_FILE, 0.5f ) ;
}

// id is how you will refer to this sprite after its been loaded
// filename is just the filename on the disk drive
void SpriteMan::loadSprite( int spriteId, char *filename,
                         D3DCOLOR backgroundColor,
                         int singleSpriteWidth, int singleSpriteHeight,
                         int numFrames, float timeBetweenFrames )
{
  Sprite *sprite = new Sprite( lgpu, filename, backgroundColor,
    singleSpriteWidth, singleSpriteHeight, numFrames, timeBetweenFrames ) ;
  addSprite( spriteId, sprite ) ;
}

int SpriteMan::randomSpriteId()
{
  SpriteMapIter iter = sprites.begin() ;
  advance( iter, rand()%sprites.size() ) ;

  return iter->first ;
}

Sprite* SpriteMan::getSprite( int spriteId )
{
  Sprite *sprite = defaultSprite ;
  SpriteMapIter spriteEntry = sprites.find( spriteId ) ;

  if( spriteEntry != sprites.end() )
    sprite = spriteEntry->second ;
  else
    warning( "Sprite %d not loaded, you got the default sprite instead!", spriteId ) ;

  return sprite ;
}

// Try 3 times.  if you miss 3 times,
// (x--then proceed forward through from low to high
// if that fails--x), tell them with a warning,
// and just return the first item in the map
int SpriteMan::randomSpriteId( int below )
{
  SpriteMapIter iter ;

  int numSprites = sprites.size() ;
  if( numSprites < below )
  {
    //warning( "There aren't %d sprites", below ) ;
    below = numSprites ; // better chance of
    // getting # below if use this lower range..
  }

  for( int i = 0 ; i < 3 ; i++ )
  {
    iter = sprites.begin() ;
    int advAmt = rand()%below ;
    advance( iter, advAmt ) ;

    if( iter->first < below )
      return iter->first ;
  }
  
  warning( "I tried 3 times and couldn't find you a sprite lower than %d, returning first", below ) ;
  return sprites.begin()->first ;
}

int SpriteMan::getNextSpriteId()
{
  // We can't really assume the
  // sprites were created in ORDER.

  // Unfortunately we have to walk through
  // the entire collection and return the largest
  // id so far
  int largestSpriteId = 0 ;
  foreach( SpriteMapIter, iter, sprites )
  {
    if( iter->first > largestSpriteId )
      largestSpriteId= iter->first ;
  }

  return largestSpriteId + 1 ;
}

void SpriteMan::drawAxes( float LEN )
{
  static VertexC axis[] = {

    // x-axis is red
    VertexC( -LEN, 0, 0, 255, 0, 0 ),
    VertexC( +LEN, 0, 0, 255, 0, 0 ),

    // y-axis green
    VertexC( 0, -LEN, 0, 0, 255, 0 ),
    VertexC( 0, +LEN, 0, 0, 255, 0 ),

    // z-axis blue
    VertexC( 0, 0, -LEN, 0, 0, 255 ),
    VertexC( 0, 0, +LEN, 0, 0, 255 )

  } ;


  HRESULT hr = lgpu->SetTexture( 0, NULL ) ;
  DX_CHECK( hr, "unset the SetTexture" ) ;
  
  hr = lgpu->DrawPrimitiveUP( D3DPT_LINELIST, 3, axis, sizeof( VertexC ) ) ;
  DX_CHECK( hr, "DrawPrimitiveUP FAILED!" ) ;

  DWORD oldPtSize ;
  lgpu->GetRenderState( D3DRS_POINTSIZE, &oldPtSize ) ;

  // One of the rare 'good' uses for reinterpret_cast
  //float fOldPtSize = reinterpret_cast<float>( oldPtSize ) ;

  static float pointSize = 8.0f ;
  lgpu->SetRenderState( D3DRS_POINTSIZE, CAST_AS_DWORD( pointSize ) ) ;

  // Draw points at end of axis.
  static VertexC points[] = {
    VertexC( LEN, 0, 0, 255, 0, 0 ),
    VertexC( 0, LEN, 0, 0, 255, 0 ),
    VertexC( 0, 0, LEN, 0, 0, 255 ),
  } ;

  hr = lgpu->DrawPrimitiveUP( D3DPT_POINTLIST, 3, points, sizeof( VertexC ) ) ;
  DX_CHECK( hr, "DrawPrimitiveUP FAILED!" ) ;

  lgpu->SetRenderState( D3DRS_POINTSIZE, CAST_AS_DWORD( oldPtSize ) ) ;

}

void SpriteMan::addSprite( int spriteId, Sprite *sprite )
{
  SpriteMapIter existingSpritePtr = sprites.find( spriteId ) ;
  if( existingSpritePtr != sprites.end() )
  {
    // the sprite with that id existed.  erase it
    warning( "Sprite with id=%d already existed.  Destroying it..", spriteId ) ;
    warning( "Is this happening unexpectedly?  "
      "If it is, try doing all your loadSprite calls "
      "__before__ any calls to OBJFILE, the model loading "
      "functions" ) ;
    // The reason for this is the objfile loading
    // automatically loads textures,
    delete existingSpritePtr->second ;
  }

  // now add it
  sprites.insert( make_pair( spriteId, sprite ) ) ;
}


void SpriteMan::addFont( int fontId, ID3DXFont* font )
{
  FontMapIter fontEntry = fonts.find( fontId ) ;

  if( fontEntry != fonts.end() )
  {
    warning( "Font %d already existed, destroying and replacing..", fontId ) ;
    SAFE_RELEASE( fontEntry->second ) ;
  }

  fonts.insert( make_pair( fontId, font ) ) ;
}

void SpriteMan::setActiveTexture( int spriteId )
{
  if( !spriteId )
  {
    // Understood to mean you want to turn off texturing
    lgpu->SetTexture( 0, 0 ) ;
    return ;
  }

  Sprite *sprite = getSprite( spriteId ) ;
  lgpu->SetTexture( 0, sprite->getTexture() ) ;
}