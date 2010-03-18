#ifndef SPRITEMAN_H
#define SPRITEMAN_H

#include <map>
using namespace std;

#include "Sprite.h"
#include "D3DVertex.h"

// Map type for "textures" (which are "images" in direct3d)
typedef map<int, Sprite*> /* as simply */ SpriteMap ;
typedef map<int, Sprite*>::iterator /* as simply */ SpriteMapIter ;

typedef map<int, ID3DXFont*> /* as simply */ FontMap ;
typedef map<int, ID3DXFont*>::iterator /* as simply */ FontMapIter ;

#define DEFAULT_FONT -1 /* drawString() draws with a default font */

enum SpriteCentering
{
  TopLeft,
  Center
} ;

class SpriteMan
{
protected:
  // <Direct3D objects>:  All must be
  // released on shut down and reset
  // when the device gets reset
  // The only reason these are PROTECTED
  // is so that GameWindow can access them
  // after they have been initialized,
  // to pass them to D3DWindow.
  ID3DXSprite *id3dxSpriteRenderer ;
  ID3DXFont *id3dxDefaultFont ;
  FontMap fonts ;
  // </Direct3D objects>

private:
  // Retain a copy of the gpu object
  // for convenience
  IDirect3DDevice9 * lgpu ;

  SpriteMap sprites ;
  Sprite *defaultSprite ; 
  Sprite *whitePixel ;    // used for drawing solid colored boxes.
  // a failsafe default sprite
  // who doesn't get loaded from a file, rather its
  // a hard-coded sprite in the code here that gets
  // generated at runtime.

  int screenWidth, screenHeight ;

public:
  SpriteMan() ;
  ~SpriteMan() ;
   
  // Fire up the sprite manager
  void initSpriteMan( IDirect3DDevice9 *theGpu, int scrWidth, int scrHeight ) ;

private:
  void initDefaultSprite() ;
  void initWhitePixel() ;

public:
  // Creates a surface you can draw to
  // using GDI+
  GDIPlusTexture* createGDISurface( int width, int height ) ;
  void addSpriteFromGDIPlusTexture( int id, GDIPlusTexture* tex ) ;
  
public:

  void spriteManStep( float time ) ;

  void spriteManSetWindowSize( int width, int height ) ;

  void drawBox( D3DCOLOR color, int x, int y, int width, int height ) ;
  void drawBox( D3DCOLOR color, RECT &r ) ;
  void drawBoxCentered( D3DCOLOR color, int xCenter, int yCenter, int width, int height ) ;
  void getBoxDimensions( char *str, RECT &r ) ;

  // Draws a string center screen.
  // If you pass DEFAULT_FONT(-1) for fontId,
  // you get a default font (which is just Arial 18)
  void drawString( int fontId, char *str, D3DCOLOR color ) ;

  // Draw a string with its top left corner at position x, y,
  // bounded by a box of boxWidth, boxHeight pixels.  Text
  // wraps automatically.
  void drawString( int fontId, char *str, D3DCOLOR color, float x, float y, float boxWidth, float boxHeight ) ;

  // formatOptions:
  // DT_LEFT - left align text
  // DT_RIGHT - right align text
  // DT_CENTER - centers the text horizontally
  // DT_VCENTER - centers text vertically
  // DT_TOP - mashes text to top of box
  // DT_BOTTOM - mashes text to bottom of box
  // So if you want TOP LEFT aligned text,
  // use (DT_TOP | DT_LEFT)
  void drawString( int fontId, char *str, D3DCOLOR color, float x, float y, float boxWidth, float boxHeight, DWORD formatOptions ) ;

  void drawString( int fontId, char *str, D3DCOLOR color, RECT &r ) ;
  void drawString( int fontId, char *str, D3DCOLOR color, RECT &r, DWORD formatOptions ) ;


  //////////////////
  // SPRITE RENDER
  // Draws a sprite centered @ (x,y)
  void drawSprite( int id, float x, float y ) ;

  // Draws at (x,y) "centered" where you want it
  void drawSprite( int id, float x, float y, SpriteCentering centering ) ;

  // draws a sprite centered @ (x,y) modulated by color specified
  // You can use this fucntion to make your sprite appear "ghostly"
  // (by using a translucent whitish color like ARGB(128,255,255,255))
  void drawSprite( int id, float x, float y, D3DCOLOR modulatingColor ) ;

  // draws a sprite centered @ (x,y) of width and height specified
  void drawSprite( int id, float x, float y, float width, float height ) ;

  void drawSprite( int id, float x, float y, float width, float height, SpriteCentering centering ) ;

  // draws a sprite centered @ (x,y) of width and height specified, using modulating color
  void drawSprite( int id, float x, float y, float width, float height, float angle ) ;

  void drawSprite( int id, float x, float y, float width, float height, float angle, D3DCOLOR modulatingColor ) ;

  void drawSprite( int id, float x, float y, float width, float height, float angle, D3DCOLOR modulatingColor, SpriteCentering centering ) ;



  /* Creates boxed text as a sprite
  Uses the default font (Arial 18) to draw the text
  in the textColor you specify */
  void boxedTextSprite( int spriteId, char *str, D3DCOLOR textColor ) ;

  /* Puts text against a square
  fitted background. */
  void boxedTextSprite( int spriteId, char *str, D3DCOLOR textColor, D3DCOLOR backgroundColor ) ;

  /* The "padding" parameter is how much space to pad out around the sides. */
  void boxedTextSprite( int spriteId, char *str, D3DCOLOR textColor, D3DCOLOR backgroundColor, int padding ) ;

  /* the RECT for padding specifies left, top, right, bottom padding. */
  void boxedTextSprite( int spriteId, char *str, D3DCOLOR textColor, D3DCOLOR backgroundColor, RECT padding ) ;
  
  /* Boxed text has fontName, size, and boldness you specify.
  Boldness:  Should a number between 0 and 1000.  400 is considered "normal", while 700 is considered "bold". */
  void boxedTextSprite( int spriteId, char *str, D3DCOLOR textColor, D3DCOLOR backgroundColor, RECT padding, char *fontName, float size, int boldness, bool italics ) ;

private:
  void boxedTextSprite( int spriteId, char *str, D3DCOLOR textColor, D3DCOLOR backgroundColor, RECT padding, ID3DXFont *font ) ;

public:
  void addSprite( int id, Sprite *sprite ) ;

  // Load a static sprite with only 1 frame (not animated)
  // Loads width and height values from file
  // Assume background color is Photoshop's "transparent"
  void loadSprite( int id, char *filename ) ;

  // Load a static sprite from a file
  // consider "backgroundColor" as
  // the transparent color (doesn't have to be
  // photoshop "transparent")
  void loadSprite( int id, char *filename, D3DCOLOR backgroundColor ) ;

  // Load an animated sprite from a file,
  // singleSpriteWidth and singleSpriteHeight
  // are the width and height of a single frame,
  // numFrames is the number of frames you've got
  // in the sprite.
  void loadSprite( int id, char *filename,
    D3DCOLOR backgroundColor,
    int singleSpriteWidth,
    int singleSpriteHeight,
    int numFrames,
    float timeBetweenFrames ) ;

  /*
  // Old signature.  The default parameters
  // look awkward in VS autocomplete.
  void loadSprite( int id, char *filename,
    D3DCOLOR backgroundColor = D3DCOLOR_ARGB( 0,0,0,0 ),
    int singleSpriteWidth = SPRITE_READ_FROM_FILE,
    int singleSpriteHeight = SPRITE_READ_FROM_FILE,
    int numFrames = SPRITE_READ_FROM_FILE,
    float timeBetweenFrames = 0.5f ) ;
  */

  // Gets you a random sprite id from the map.
  // Useful for testing, not much else
  int randomSpriteId() ;
  int randomSpriteId( int below ) ;

  //!! marked for deletion
  /* -- */ void drawAxes() ; /* -- */


  void addFont( int fontId, ID3DXFont* font ) ;

} ;

#endif