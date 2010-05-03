#include "cs-main.h"
GameWindow *window ;


#include "SimWorld.h"



// don't move this.  magic.
#include "carsimLinkHeader.h"








double t ; // global sense of time

// Start Carsim up
int carsimInit( char *simfile )
{
  // Initialize VS model
  t = vs_setdef_and_read(simfile, NULL, NULL);
  if( vs_error_occurred() ) 
    return 1;

  // Start up carsim
  vs_initialize( t, NULL, NULL ) ;

  // See if there are any messages
  puts( vs_get_output_message() ) ;// pointer to text from DLL

  if( vs_error_occurred() ) 
    return 1;
}

// Close Carsim
int carsimShutdown()
{
  // Terminate
  vs_terminate( t, NULL ) ;
  puts( vs_get_output_message() ) ;
  vs_free_all() ;
  return 0 ;
}

// Run single Carsim time step
int carsimUpdate()
{
  //int ibarg = 0 ;

  // Run. Each loop advances time one full step.
  //while(!vs_stop_run()) // don't stay locked in here
  vs_integrate( &t, NULL );

  if( vs_error_occurred() )
    return 1;

  //vs_bar_graph_update( &ibarg ) ; // update bar graph?

  //puts( vs_get_output_message() ) ;
}

void carsimSetup()
{
  HMODULE vsDLL = NULL; // DLL with VS API
  char   pathDLL[FILENAME_MAX], simfile[FILENAME_MAX]={"simfile"};

  ///// just paste in a name
  strcpy( simfile, "simfile" ) ;

  if (vs_get_dll_path(simfile, pathDLL)) return;
  vsDLL = LoadLibraryA(pathDLL);

  // get standard sets of API functions 

  // basic functions
  if (vs_get_api_basic(vsDLL, pathDLL))
    return; 

  // install functions
  if (vs_get_api_install_external(vsDLL, pathDLL))
    return;

  // model extension functions
  if (vs_get_api_extend(vsDLL, pathDLL))
    return;

  // road-related functions
  if (vs_get_api_road(vsDLL, pathDLL))
    return;


  // install external C functions
  vs_install_calc_function (&external_calc);
  vs_install_echo_function (&external_echo);
  vs_install_scan_function (&external_scan);
  vs_install_setdef_function (&external_setdef);

  // Make the run. If run was OK vs_run returns 0.
  /*
  if (vs_run (simfile)) 
  MessageBoxA(NULL, vs_get_error_message(), NULL, MB_ICONERROR);
  */

  carsimInit( simfile ) ;

  return;
}




void Init()
{
  #pragma region asset load
  // sprite loading
  window->loadSprite( Mario, ASSET("sprites/mario.png") ) ;

  // Animated sprite
  window->loadSprite( SixteenCounter, ASSET("sprites/16counter.png"), 0, 32, 32, 16, 0.5f ) ;

  window->loadSound( Screech, ASSET("sounds/Generic-Tire-01_Skid-01.wav") ) ;

  window->loadSound( EngineLowIn, ASSET("sounds/Generic_Engine_01_L4_2.4L_0.0_Load_02_Low_RPM.WAV") ) ;
  window->loadSound( EngineLowOut, ASSET("sounds/Generic_Engine_01_L4_2.4L_1.0_Load_02_Low_RPM.WAV") ) ;
  
  window->loadSound( EngineMidIn, ASSET("sounds/Generic_Engine_02_L4_2.4L_0.0_Load_03_Mid_RPM.WAV") ) ;
  window->loadSound( EngineMidOut, ASSET("sounds/Generic_Engine_02_L4_2.4L_1.0_Load_03_Mid_RPM.WAV") ) ;

  // !! pitch shift does not work.
  //set up pitch shift
  //window->createPitchShift() ;
  //window->loopSound( EngineMidIn,  1000 ) ;
  



  // Set the background clearing color to dk blue-gray
  window->setBackgroundColor( D3DCOLOR_ARGB( 255, 35, 35, 70 ) ) ;

  // Create a few fonts
  window->createFont( Fonts::Arial16, "Arial", 16, FW_BOLD, false ) ;
  
  window->createFont( Fonts::TimesNewRoman24, "Times New Roman", 24, FW_BOLD, true ) ;
  #pragma endregion

  // Rest of init
  simWorld = new SimWorld() ;

  // Load a CARSIM sim file
  simWorld->LoadCarSimFile( "filename.sim" ) ;  



  info( "Starting up CarSim . . . " ) ;
  carsimSetup() ;
}

void Update()
{
  
  // CARSIM UPDATE


  #pragma region camera motion
  if( simWorld->camMode == SimWorld::FreeCam )
  {
    float increment = 0.005f ;

    // move the camera by dy and dx
    window->getCamera()->stepPitch( - increment*window->getMouseDy() ) ;
    window->getCamera()->stepYaw( - increment*window->getMouseDx() ) ;

    float speed = 100 ;
    if( window->keyIsPressed( 'W' ) )
    {
      window->getCamera()->stepForward( speed*increment ) ;
    }

    if( window->keyIsPressed( 'S' ) )
    {
      window->getCamera()->stepForward( -speed*increment ) ;
    }

    if( window->keyIsPressed( 'D' ) ) 
    {
      window->getCamera()->stepSide( speed*increment ) ;
    }

    if( window->keyIsPressed( 'A' ) ) 
    {
      window->getCamera()->stepSide( -speed*increment ) ;
    }
    
    // Swithc to grounded mode.  grounded mode
    // is still freeCam, its just you can't FLY
    if( window->mouseJustPressed( Mouse::Right ) )
    {
      // toggle the cam mode
      if( window->getCamera()->getCamMode() == Camera3D::Fly )
        window->getCamera()->setCamMode( Camera3D::Grounded ) ;
      else
        window->getCamera()->setCamMode( Camera3D::Fly ) ;
    }
  }
  else // not freeCam, followCam
  {
    // FollowCam
    window->getCamera()->follow(
      simWorld->car->pos,
      simWorld->car->fwd, 
      8, // 8 units back
      2  // 2 units up
    ) ;

  }

  // set the window's view by whatever the camera's doing
  window->setByCamera() ;





  // Change fill mode
  if( window->keyJustPressed( '4' ) )
  {
    DWORD fillMode ;
    DWORD nextFillMode = D3DFILL_SOLID ;

    window->getGpu()->GetRenderState( D3DRS_FILLMODE, &fillMode ) ;
    if( fillMode == D3DFILL_SOLID )
      nextFillMode = D3DFILL_WIREFRAME ;
    else if( fillMode == D3DFILL_WIREFRAME ) 
      nextFillMode = D3DFILL_POINT ;
    else if( fillMode == D3DFILL_POINT )
      nextFillMode = D3DFILL_SOLID ;

    window->getGpu()->SetRenderState( D3DRS_FILLMODE, nextFillMode )  ;
  }

  // change camera mode
  if( window->keyJustPressed( '5' ) )
  {
    if( simWorld->camMode == SimWorld::FreeCam )
      simWorld->camMode = SimWorld::FollowCam ;
    else
      simWorld->camMode = SimWorld::FreeCam ;
  }



  



  #pragma endregion



  // Update carsim
  carsimUpdate() ;

  simWorld->car->update( window->getTimeElapsedSinceLastFrame() ) ;
  
  if( window->keyJustPressed( VK_ESCAPE ) )   bail( "simulation ended!", false ) ;
}


// DO ALL DRAWING HERE.
void Draw()
{
  // Reset the world xform
  D3DXMATRIX identity ;
  D3DXMatrixIdentity( &identity ) ;
  window->setWorld( &identity ) ;


  window->setDrawingMode( D2 ) ; // 2d
  char buf[300];
  
  int y = 20 ;
  sprintf( buf, "Heading: %.2f %.2f %.2f", simWorld->car->fwd.x, simWorld->car->fwd.y, simWorld->car->fwd.z ) ;
  window->drawString( Fonts::Arial16, buf, D3DCOLOR_ARGB( 255, 255, 255, 255 ), 20, y+=20, 200, 200, DT_LEFT | DT_TOP ) ;

  sprintf( buf, "pos: %.2f %.2f %.2f", simWorld->car->pos.x, simWorld->car->pos.y, simWorld->car->pos.z ) ;
  window->drawString( Fonts::Arial16, buf, D3DCOLOR_ARGB( 255, 255, 255, 255 ), 20, y+=20, 200, 200, DT_LEFT | DT_TOP ) ;

  sprintf( buf, "vel: %.2f %.2f %.2f", simWorld->car->vel.x, simWorld->car->vel.y, simWorld->car->vel.z ) ;
  window->drawString( Fonts::Arial16, buf, D3DCOLOR_ARGB( 255, 255, 255, 255 ), 20, y+=20, 200, 200, DT_LEFT | DT_TOP ) ;

  sprintf( buf, "acc: %.2f %.2f %.2f", simWorld->car->accelForce.x, simWorld->car->accelForce.y, simWorld->car->accelForce.z ) ;
  window->drawString( Fonts::Arial16, buf, D3DCOLOR_ARGB( 255, 255, 255, 255 ), 20, y+=20, 200, 200, DT_LEFT | DT_TOP ) ;

  
  window->setDrawingMode( D3 ) ; // 3d
  

  window->setVertexDeclaration( D3DWindow::PositionColor ) ;
  window->setLighting( false ) ;
  window->drawAxes( 50.0f ) ;


  simWorld->Draw() ;

  // draw the mouse cursor with this sprite.
  //window->drawMouseCursor( Mario ) ;

  window->drawFrameCounter(); // erase this if you don't like
  // the frame counter in the top right corner
}








// ^^ ^^ ^^ ^^ ^^ ^^ ^^ ^^ ^^ ^^ ^^ ^^ ^^ ^^ ^^
// Your code above.





// VV VV VV VV VV VV VV VV VV VV VV VV VV VV VV
// Engine code below.  You shouldn't need to
// code down here, EXCEPT for if you need to read
// WM_CHAR messages.
///////////////////////////////////////////
// WndProc says:  I AM WHERE "MESSAGES" GET "DISPATCHED" TO,
// WHEN "EVENTS" "HAPPEN" TO YOUR WINDOW.
LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam )
{
  // WndProc is WHERE you respond to
  // all messages that might happen
  // to your window.

  // When a key is pushed, when a mouse is clicked,
  // you can DO SOMETHING about that here, in WndProc
  switch( message )
  {
    // We can call each of these "case traps"
    // an "event handler".  These tidbits of code
    // "handle" the event where the user just
    // clicked in our window, or the user pressed a key.
  
    // A lot of these have been stripped away
    // vs the GDI+ version of this project..
    // Here, we only handle a few select
    // messages...
    
    // WM_CHAR:  for 'character' key presses
    // We use this for the user to type their name or something,
    // with proper preservation of capslocked letters etc
  case WM_CHAR:
    {
      //info( "You pushed %c, ascii=%d", wparam, wparam ) ;
    }
    return 0 ;

    // WM_INPUT messages are for FAST keyboard and mouse
    // These messages are FASTER than WM_KEYDOWN or WM_MOUSEMOVE.
    // Both keyboard AND mouse input events get picked up here
  case WM_INPUT: 
    {
      #pragma region pick up the raw input
      // DO NOT CODE HERE!  use the
      // window->justPressed(), and 
      // window->mouseJustPressed() functions,
      // in your Update() function.
      UINT dwSize;

      GetRawInputData( (HRAWINPUT)lparam, RID_INPUT,
                        NULL, &dwSize, sizeof( RAWINPUTHEADER ) ) ;
      LPBYTE lpb = new BYTE[ dwSize ] ;
      if( lpb == NULL )
      {
        return 0;
      }

      if( GetRawInputData( (HRAWINPUT)lparam, RID_INPUT,
          lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize )
      {
        error( "GetRawInputData doesn't return correct size !" ) ;
      }

      RAWINPUT *raw = (RAWINPUT*)lpb;

      // Check if it was keyboard or mouse input.
      if (raw->header.dwType == RIM_TYPEKEYBOARD) 
      {
        // We don't take keyboard input here.
        // We take it by using GetKeyboardState() function
        // in the window->step() function.
        //printRawKeyboard( raw ) ; // just so you can see
      }
      else if (raw->header.dwType == RIM_TYPEMOUSE)
      {
        //printRawMouse( raw ) ;  // just so you can see
        window->mouseUpdateInput( raw ) ;
      } 

      delete[] lpb ;
      #pragma endregion

      return 0;
    }

  case WM_ACTIVATE:
    switch( LOWORD(wparam) )
    {
    case WA_ACTIVE:         // got focus via alt-tab or something
    case WA_CLICKACTIVE:    // got focus via mouse click on window
      {
        info( "Welcome back!" ) ;
      }
      break;
    case WA_INACTIVE:       // LOST FOCUS!! OH NO!!
      {
        // This means the user is "focused" / has highlighted
        // another window other than our window.

        // You should probably pause the game here,
        // because some apps tend to hijack the input focus
        // which makes it really annoying.
        info( "But why would you click away?" ) ;
      }
    }
    return 0 ;

  case WM_PAINT:
    {
      // Let's NOT paint here anymore.
      // See Draw() function instead.
      HDC hdc ;
      PAINTSTRUCT ps;
      hdc = BeginPaint( hwnd, &ps ) ;
      // DO NOT PAINT HERE.
      EndPaint( hwnd, &ps ); // tell Windows that
      // we are done painting
    }
    return 0 ;

  case WM_DESTROY:
    PostQuitMessage( 0 ) ;
    return 0 ;
  }

  return DefWindowProc( hwnd, message, wparam, lparam ) ;
}


// In a windows program, instead of main(), we use WinMain()
// Refer to earlier lessons for comments and more detail.
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow )
{
  // Setup a console
  consoleCreate() ;
  consoleWhite() ;
  consoleMove( 32, 510 ) ;
  consoleRowsAndCols( 10, 120 ) ;

  // Start up the log.
  logStartup() ;
  
  // Put these after log start up, b/c start up inits them with some init values
  logOutputsForConsole = LOG_ERROR | LOG_WARNING | LOG_INFO ;  // See ERROR and WARNING messages at Console.
  logOutputsForFile = LOG_ERROR | LOG_WARNING | LOG_INFO ; // See all types of messages in file
  logOutputsForDebugStream = LOG_ERROR | LOG_WARNING ; // See all ERROR and WARNING messages at debugstream. Suppress 'info()' messages.


  // Start up GDI+, which we use to draw
  // For GDI+, used only for shape render
  GdiplusStartupInput gdiplusStartupInput;
  ULONG_PTR gdiplusToken;
  GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

  // Setup the window
  window = new GameWindow( hInstance, TEXT( "eternity engine base" ),
     32, 32, // x pos, y pos
     640, 480 // width, height
  ) ;

  // After the window comes up, we call Init
  // to load the game's content
  Init() ;

  MSG message ;
  while( 1 )
  {
    if( PeekMessage( &message, NULL, 0, 0, PM_REMOVE ) )
    {
      if( message.message == WM_QUIT ) // if we got a WM_QUIT message..
      {
        break ; // ..then end the program by jumping out of the while(1) loop.
      }
      
      // Send the message over to WndProc for
      // further processing.
      TranslateMessage( &message ) ;
      DispatchMessage( &message ) ;
    }
    else
    {
      // Run our game, one frame
      Update() ;
      window->step() ; // ^^ update fmod engine, grab updated keystates, etc.
      
      // Draw the game out, all at once
      if( window->beginDrawing() ) // only continue if beginDrawing() succeeds
      {
        Draw() ;

        // Inserting the call to flush3D here..
        // there might be a better spot for it.
        window->flush3D() ;

        window->endDrawing() ;
      }
    }
  }

  info( "Game over!" ) ;
  logShutdown() ;

  GdiplusShutdown(gdiplusToken);

  //system( "pause" ) ; // uncomment to make it pause before exit
  return 0 ;
}

