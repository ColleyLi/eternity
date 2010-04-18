#include "GameWindow.h"

GameWindow::GameWindow( HINSTANCE hInst, TCHAR* windowTitleBar,
                       int windowXPos, int windowYPos,
                       int windowWidth, int windowHeight ) :
  D3DWindow( hInst, windowTitleBar,
             windowXPos, windowYPos,
             windowWidth, windowHeight )
{
  initSpriteMan( gpu, windowWidth, windowHeight ) ;

  // Some coupling..
  // it isn't possible to reach D3DWindow
  // from SpriteMan, so the tie together
  // is actually here, in this class
  registerFont( id3dxDefaultFont ) ;
  registerSpriteRenderer( id3dxSpriteRenderer ) ;
  registerLine( id3dxLine ) ;

  initInputMan( hwnd, windowWidth, windowHeight ); // clip zone gets set here too
  
  spriteManSetWindowSize( windowWidth, windowHeight ) ;
  initSoundMan() ;
  paused = false ;
}

GameWindow::~GameWindow()
{
}

void GameWindow::step()
{
  d3dWindowStep() ;  

  inputManStep() ;

  soundStep();

  // Advance sprite animations.
  // This is the only thing that
  // is PAUSED when the game is
  // PAUSED.
  if( !paused )
  {
    spriteManStep( timer.time_since_last_frame ) ;
  }



  // Call all callbacks due to execute now
  runCallbacks() ;

  timer.lock( 60 ) ; // // ^^Leave as last line: YES, RECOMMENDED.  Use this line to LOCK FRAMERATE
  // at 60 fps max.  This will "waste" any idle time at the end of
  // processing a frame.
  
  //timer.update();  // NOT RECOMMENDED.  Use this line to SIMPLY UPDATE
  // the frame counter WITHOUT frame limiting.
  
  // This mode lets the game run AS FAST AS IT POSSIBLY CAN
  // on this machine, and you might see frame rates of
  // 300fps or so.  This means your game will vary in
  // speed though depending on how much "stuff" is on
  // the screen.  NOT RECOMMENDED becuase your game will
  // run at varying speeds on different machines.  also
  // you will see tearing, which doesn't look very good.
  
  // NOTE:  YOU MUST ALSO FIND AND UNCOMMENT THE LINE
  // that says FRAMERATE::UNBOUNDED
}


// Some may argue that you could expose the "paused"
// variable as a public member.. but using member functions
// like this allows you to actually perform "clean-up" type
// stuff on pause or unpause.  For example, here we're calling
// FMOD's pause function.  We get the opportunity to do it
// once, immediately as the pause() function is called.
// See?  OOP isn't all that bad!
void GameWindow::pause()
{
  // If the game wasn't already paused..
  if( !paused )
  {
    // ..then pause everything
    soundPause() ;

    paused = true ;  // this member is checked
    // in Window::step()
  }
}
void GameWindow::unpause()
{
  if( paused )
  {
    soundUnpause() ;

    paused = false ;
  }
}

bool GameWindow::isPaused()
{
  return paused ;
}

bool GameWindow::isSlow()
{
  return timer.frames_per_second < 55 ;
}

double GameWindow::getTimeElapsedSinceLastFrame()
{
  return timer.time_since_last_frame ;
}

double GameWindow::getTimeElapsedSinceGameStart()
{
  return timer.game_time ;
}

void GameWindow::drawMouseCursor(int spriteId, bool showCursorCoordinates)
{
  drawSprite( spriteId, getMouseX(), getMouseY(), SpriteCentering::TopLeft ) ;

  if( showCursorCoordinates )
  {
    char buf[ 300 ] ;
    sprintf( buf, "mouse\n(%d, %d)", getMouseX(), getMouseY() ) ;

    RECT r ;
    getBoxDimensions( DEFAULT_FONT, buf, r ) ;
    drawBox( D3DCOLOR_ARGB( 120, 255, 255, 255 ), getMouseX(), getMouseY(), r.right-r.left, r.bottom-r.top ) ;
    drawString( DEFAULT_FONT, buf, D3DCOLOR_ARGB( 255, 0, 0, 120 ), getMouseX(), getMouseY(), r.right-r.left, r.bottom-r.top, DT_LEFT | DT_TOP ) ;
  }
}

// Show fps
void GameWindow::drawFrameCounter()
{
  static char buf[ 60 ];
  sprintf( buf, "FPS:  %.1f", timer.frames_per_second ) ;

  /*
  // There's no point in computing this.  its a jittery box.
  RECT r ;
  getBoxDimensions( buf, r ) ;
  
  r.left = getWidth() - r.right - 10  ;
  r.right = getWidth() - 10 ;
  r.top += 10 ;
  r.bottom += 10 ;
  */
  
  int left = getWidth() - 10 - 100 ;
  drawBox( D3DCOLOR_ARGB( 235, 0, 0, 128 ), left, 10, 100, 30 ) ;
  drawString( DEFAULT_FONT, buf, Color::White, left, 10, 100, 30 ) ;

  sprintf( buf, "+ %.3f ms", timer.extra_time*1000 ) ;
  drawBox( D3DCOLOR_ARGB( 235, 0, 0, 128 ), left, 40, 100, 30 ) ;
  drawString( DEFAULT_FONT, buf, Color::White, left, 40, 100, 30 ) ;
}


void GameWindow::drawTimer()
{
  static char buf[ 60 ];
  sprintf( buf, "Time: %.2f", timer.game_time ) ;

  /*
  // There's no point in computing this.  its a jittery box.
  RECT r ;
  getBoxDimensions( buf, r ) ;
  
  r.left = getWidth() - r.right - 10  ;
  r.right = getWidth() - 10 ;
  r.top += 10 ;
  r.bottom += 10 ;
  */
  
  int left = getWidth() - 10 - 100 ;
  drawBox( D3DCOLOR_ARGB( 235, 0, 0, 128 ), left, 70, 100, 30 ) ;
  drawString( DEFAULT_FONT, buf, Color::White, left, 70, 100, 30 ) ;

}

void GameWindow::createFont( int fontId, char *fontName, float size, int boldness, bool italics )
{
  ID3DXFont *font ;
  DX_CHECK( D3DXCreateFontA( gpu, size, 0, boldness, 1,
    italics, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
    DEFAULT_PITCH | FF_DONTCARE, fontName, &font ), "Create custom font" ) ;

  // Register the font with D3DWindow, so
  // it will be released and reset ondevicelost
  // and ondevicereset
  registerFont( font ) ;

  // Now add it to the map in SpriteMan
  addFont( fontId, font ) ;

}


bool GameWindow::setSize( int width, int height, bool fullScreen )
{
  D3DWindow::setSize( width, height, fullScreen ) ;

  info( "GameWindow setSize( %d, %d, %d )", width, height, fullScreen ) ;

  // Now NOTIFY INPUTMAN AND SPRITEMAN
  // OF THESE CHANGES!
  RECT wndSize ;
  wndSize.left = 0 ;
  wndSize.right = width ;
  wndSize.top = 0 ;
  wndSize.bottom = height ;

  inputManSetClipZone( wndSize ) ;
  
  
  spriteManSetWindowSize( width, height ) ;


  return true ;


}

void GameWindow::addCallback( Callback *callback )
{
  // Add time to make it "when" seconds from now
  callback->when += timer.game_time ;

  callbacks.push_back( callback ) ;
}

void GameWindow::addCallbackAbsolute( Callback *callback )
{
  // Check if its expired..
  if( callback->when < timer.game_time )
  {
    warning( "A callback was set to execute some time "
      "in the past, so it will execute immediately..!  "
      "Are you sure you set the callback correctly?" ) ;
  }

  callbacks.push_back( callback ) ;
}


void GameWindow::runCallbacks()
{
  // rather than iterate let's just
  // loop backwards using numerical access
  for( int i = callbacks.size() - 1 ;
       i >= 0 ;
       i-- )
  {

    // If its time to exec it..
    if( timer.game_time >= callbacks[i]->when ) 
    {
      // exec it..
      callbacks[i]->exec() ;

      // and erase it..
      delete (callbacks[ i ]) ;

      // and remove it from
      // the container
      callbacks.erase( callbacks.begin()+i ) ;

      info( "A callback executed.  %d callbacks remain on the buffer", callbacks.size() ) ;
    }
  }
}

void GameWindow::drawTri( D3DVertexC &a, D3DVertexC &b, D3DVertexC &c )
{
  vertices.push_back( a ) ;
  vertices.push_back( b ) ;
  vertices.push_back( c ) ;
}

void GameWindow::drawQuad( D3DVertexC &a, D3DVertexC &b, D3DVertexC &c, D3DVertexC &d )
{
  vertices.push_back( a ) ;
  vertices.push_back( b ) ;
  vertices.push_back( c ) ;

  vertices.push_back( c ) ;
  vertices.push_back( d ) ;
  vertices.push_back( a ) ;
}

// Must call this to actually flush out
// the 3d draw calls
void GameWindow::flush3D()
{
  HRESULT hr ;

  // flush triangles
  if( vertices.size() > 0 )
  {
    //!! really better if we could use a
    // vertex array but that would require..
    // some very neat set up, right now it
    // doesn't make sense (since models aren't
    // being loaded at all, this is only for
    // simple primitives.)
    // Later we can provide a 'model' class..
    hr = gpu->DrawPrimitiveUP(
      D3DPT_TRIANGLELIST,
      vertices.size()/3,
      &vertices[0], 
      sizeof(D3DVertexC)
    ) ;

    DX_CHECK( hr, "flush3D: DrawPrimitive" ) ;

    vertices.clear() ;
  }



}