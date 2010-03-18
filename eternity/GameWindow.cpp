#include "GameWindow.h"

GameWindow::GameWindow( HINSTANCE hInst, TCHAR* windowTitleBar,
                       int windowXPos, int windowYPos,
                       int windowWidth, int windowHeight ) :
  D3DWindow( hInst, windowTitleBar,
             windowXPos, windowYPos,
             windowWidth, windowHeight )
{
  initSpriteMan( gpu, windowWidth, windowHeight ) ;
  initInputMan( hwnd, windowWidth, windowHeight );
  initSoundMan() ;
  paused = false ;
}

GameWindow::~GameWindow()
{
}

void GameWindow::step()
{
  // check the d3ddevice, in case its been lost
  d3dDeviceCheck() ;
  


  soundStep();

  // Advance sprite animations.
  // This is the only thing that
  // is PAUSED when the game is
  // PAUSED.  TODO:  Pause sounds here as well...
  if( !paused )
  {
    spriteManStep( timer.time_since_last_frame ) ;
  }

  inputManStep() ;

  
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

float GameWindow::getTimeElapsedSinceLastFrame()
{
  return timer.time_since_last_frame ;
}


void GameWindow::d3dLoseDevice()
{
  //!!
}
bool GameWindow::d3dResetDevice( D3DPRESENT_PARAMETERS & pps )
{
  return true ;
}
void GameWindow::d3dDeviceCheck()
{
  /*
  switch( hr )
  {
  case D3DERR_DRIVERINTERNALERROR:
    error( "Hmm, the driver experienced an internal error.  This is unusual." ) ;

    // Try and re-initialize d3d.  If it fails, quit.
    if( !initD3D() )
    {
      bail( "Experienced D3DERR_DRIVERINTERNALERROR" ) ;
    }
    break ;

  case D3DERR_DEVICELOST:
    if( !isDeviceLost )  // if the device wasn't already lost..
    {
      // tell the spriteman that the device has just been lost
      // to call the onLostDevice function of every id3dx object
      spriteManD3DDeviceLost() ;
    }
    break ;

  case D3DERR_DEVICENOTRESET:
    if( isDeviceLost )
    {
      // The device was lost, but now we have the chance
      // to reset it.
      info( "Resetting the gpu device.." ) ;
      d3dResetDevice( d3dpps ) ;

      DX_CHECK( id3dxDefaultFont->OnResetDevice(), "font onlostdevice" ) ;
      DX_CHECK( id3dxSpriteRenderer->OnResetDevice(), "sprite renderer onlostdevice" ) ;

    }
    else
    {
      // This should not happen, but if it does, we want to know about it
      error( "Device wasn't lost, yet we were given D3DERR_DEVICENOTRESET" ) ;
    }
    break ;
  
  case D3D_OK:
  default:
    // Device is ok, so don't do anything here
    break ;
  }
  */

}


void GameWindow::drawMouseCursor(int spriteId, bool showCursorCoordinates)
{
  drawSprite( spriteId, getMouseX(), getMouseY(), SpriteCentering::TopLeft ) ;

  if( showCursorCoordinates )
  {
    char buf[ 300 ] ;
    sprintf( buf, "mouse\n(%d, %d)", getMouseX(), getMouseY() ) ;

    RECT r ;
    getBoxDimensions( buf, r ) ;
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
}