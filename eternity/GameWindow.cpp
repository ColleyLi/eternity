#include "GameWindow.h"
#include "ObjFileParse.h"  // to initialize it.

GameWindow::GameWindow( HINSTANCE hInst, TCHAR* windowTitleBar ):
  D3DWindow( hInst, windowTitleBar )
{
  init();
}

GameWindow::GameWindow( HINSTANCE hInst, TCHAR* windowTitleBar,
                       int windowXPos, int windowYPos,
                       int windowWidth, int windowHeight ) :
  D3DWindow( hInst, windowTitleBar,
             windowXPos, windowYPos,
             windowWidth, windowHeight )
{
  init();
}

GameWindow::~GameWindow()
{
}

void GameWindow::init()
{
  initSpriteMan( gpu, getWidth(), getHeight() ) ;
  initThreeDMan( gpu ) ;

  // Some coupling..
  // it isn't possible to reach D3DWindow
  // from SpriteMan, so the tie together
  // is actually here, in this class
  registerFont( id3dxDefaultFont ) ;
  registerSpriteRenderer( id3dxSpriteRenderer ) ;
  registerLine( id3dxLine ) ;

  initInputMan( hwnd, getWidth(), getHeight() ); // clip zone gets set here too
  
  spriteManSetWindowSize( getWidth(), getHeight() ) ;
  initSoundMan() ;
  paused = false ;


  // Initialize the objfileloader
  ObjFile::init( this ) ;
}

void GameWindow::step()
{
  d3dWindowStep() ;  

  inputManStep() ;


  //!! is this the best place for this?
  // You must need to update the
  // listener position in FMOD as well.
  FMOD_VECTOR v;
  v.x=v.y=v.z=0;
  setListener(
    (FMOD_VECTOR*)(&camera.getEye()),
    &v,
    (FMOD_VECTOR*)(&camera.getForward()),
    (FMOD_VECTOR*)(&camera.getUp())
    ) ;


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

  timer.lock( 60 ) ;  // ^^Leave as last line: YES, RECOMMENDED.  Use this line to LOCK FRAMERATE
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




// UNFORUTNATLEY I had to putthis function
// here in GameWindow instead of ThreeDMan.
// What really happened here is since
// GameWindow ISN'T accessible to 3d man,
// but 3dman is aceessible to gw, i coulnd't
// use VC or VT or VTN from ehre.

// really we need a proper eveyr-where acessible
// SINGLETON for GameWindow..

// Must call this to actually flush out
// the 3d draw calls.  BE SURE to
// set drawing mode to D3 before calling!
void GameWindow::draw3DObjects()
{
  HRESULT hr ;

  //!! uh, you didn't account for
  // xform mats.  as it stands
  // everything sent via drawQuads()
  // must use the same WORLD matrix.
  // Which is the identity matrix.
  // really that's SENSIBLE if you think about it,
  // but misleading, because someone may try
  // to change the world matrix before claling
  // drawQuad(), and get unexpected results.

  // ONE WAY to fix this is to READ THE WOLRD MATRIX,
  // APPLY THE TRANSFORMATION IN C++ at DRAWQUAD().
  // Then, that infomration gets stored in the array.

  // I thik thi sis USEFL for speicyfing static
  // geometry, but it should be made konwnw that..
  // the array DOESN'T get cleared out each frame,
  // andonce you drawQuad(), well, it'll be there
  // forever.

  // drawQuad() should only be used when setting up
  // massive amoutnts of static geometry

  // ALSO interesting is if you did it as
  // createQuad(), that has better meaning,
  // and later it can be used to BOTH create
  // a collisoin object, and the quad, at the same time.

  // I think a createQuad() function is necessary for na enigne..
  // b'doy!
  D3DXMATRIX identity ;
  D3DXMatrixIdentity( &identity ) ;
  setWorld( &identity ) ;

  // flush triangles
  if( !verticesC.empty() )
  {
    //!! really better if we could use a
    // vertex array but that would require..
    // some very neat set up, right now it
    // doesn't make sense (since models aren't
    // being loaded at all, this is only for
    // simple primitives.)
    // Later we can provide a 'model' class..

    // I wanted to use a call to D3DWindow::setVertexDeclaration(),
    // but 2 things:
    //   1)  not possible from this class,
    //       this function would have to move
    //       to GameWindow
    //   2)  it interferes with the user's
    //       current vertex declaration selection.
    //       if vdecl is reset after a clal to
    //       this draw function, then its all good
    //       from that regard.
    setVertexDeclaration( VertexType::PositionColor ) ;
    hr = gpu->DrawPrimitiveUP(
      D3DPT_TRIANGLELIST,
      verticesC.size()/3,
      &verticesC[0], 
      sizeof(VertexC)
    ) ;

    DX_CHECK( hr, "flush3D: DrawPrimitiveC" ) ;
  }


  if( !mapVerticesTC.empty() )
  {
    setVertexDeclaration( VertexType::PositionTextureColor ) ;

    // Now use the correct texture
    foreach( MapIntVertexTCIter, iter, mapVerticesTC )
    {
      // Set the texture for this one
      setActiveTexture( iter->first ) ;

      // Now draw all quads in this group that use this texture
      hr = gpu->DrawPrimitiveUP(
        D3DPT_TRIANGLELIST,
        iter->second.size()/3,
        &iter->second[0], 
        sizeof(VertexTC)
      ) ;

      DX_CHECK( hr, "flush3D: DrawPrimitiveTC" ) ;

    }
  }


  if( ! linesC.empty() )
  {
    // Draw th elines
    setVertexDeclaration( VertexType::PositionColor ) ;


    hr = gpu->DrawPrimitiveUP(
      D3DPT_LINELIST,
      linesC.size()/2,
      &linesC[0], 
      sizeof(VertexC)
    ) ;

    DX_CHECK( hr, "flush3D: DrawPrimitiveC" ) ;

    //printf( "There are %d lines\n", linesC.size() ) ;

    /////
    // NOW CLEAR THE LINES
    linesC.clear() ;
  }
}


void GameWindow::setViewBYCAMERA()
{
  D3DXVECTOR3 eye = camera.getEye() ;
  D3DXVECTOR3 look = camera.getLook() ;
  D3DXVECTOR3 up = camera.getUp() ;

  setCamera( eye, look, up ) ;
}