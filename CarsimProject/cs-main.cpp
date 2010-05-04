#include "cs-main.h"
GameWindow *window ;


#include "SimWorld.h"


#include "carsimLinkHeader.h"





bool __RUNNING__ = true ;




double t ; // global sense of time

// Start Carsim up
int carsimInit( char *simfile )
{
  // Initialize VS model
  t = vs_setdef_and_read( simfile, NULL, NULL ) ;

  // VS
  /*
  vs_read_configuration (const char *simfile,
    int *n_import,
    int *n_export,
    double *tstart,
    double *tstop,
    double *tstep);
  */


  if( vs_error_occurred() ) 
  {
    error( vs_get_error_message() ) ;
    return 1;
  }

  // Start up carsim
  vs_initialize( t, NULL, NULL ) ;

  // See if there are any messages
  puts( vs_get_output_message() ) ;// pointer to text from DLL

  if( vs_error_occurred() ) 
    return 1;

  return 0 ;
}

// Close Carsim
int carsimShutdown()
{
  // Terminate
  vs_terminate( t, NULL ) ;
  info( vs_get_output_message() ) ;
  vs_free_all() ;
  return 0 ;
}

// Run single Carsim time step
int carsimUpdate()
{
  //int ibarg = 0 ;

  // Run. Each loop advances time one full step.
  //while(!vs_stop_run()) // don't stay locked in here

  // CarSim 1000 FPS:  cannot be reduced without
  // affecting accuracy of simulation.  Run
  // CarSim 16 times per frame, then.
  for( int i = 0 ; i < 16; i++ )
    vs_integrate( &t, NULL );

  /// THIS WAY LETS US SET THE VALUE OF TIME..
  // !! but it doesn't work either.
  //vs_integrate_io( window->getTimeElapsedSinceGameStart(), 0, 0 ) ;


  //printf( "Time is %f\n", t ) ;

  // Check for errors every time step
  if( vs_error_occurred() )
  {
    error( vs_get_error_message() ) ;
    info( "Last output message was: %s", vs_get_output_message() ) ;
    return 1;
  }

  //vs_bar_graph_update( &ibarg ) ; // update bar graph?
  //puts( vs_get_output_message() ) ;

  return 0 ;
}

/// Starts up CarSim, opening
/// simfile to know what parsfiles
/// to load, and what DLL solver to use.
/// The correct format for SIMFILE's
/// is documented on PAGE ?? of ???
void carsimSetup( char* simfile )
{
  // The DLL to use is loaded
  // dynamically based on what
  // solver the SIMFILE wants to use.
  HMODULE vsDLL = NULL ;

  // Allocate memory for the path of the dll,
  // which will be retrieved from the simfile
  char pathDLL[ MAX_PATH ] ;

  // vs_get_dll_path goes into
  // simfile and looks for what
  // 
  if( vs_get_dll_path( simfile, pathDLL ) )
  {
    error( "carsim: vs_get_dll_path: Couldn't get DLL path or something" ) ;
    return ;// no point in continuing if
    // couldn't load the dll
  }


  // Load the DLL.
  vsDLL = LoadLibraryA( pathDLL ) ;

  // get standard sets of API functions 

  // basic functions
  if( vs_get_api_basic( vsDLL, pathDLL ) )
    error( "carsim: vs_get_api_basic: couldn't even load BASIC api functions!" ) ;

  // install functions
  if( vs_get_api_install_external( vsDLL, pathDLL ) )
    error( "carsim: vs_get_api_install_external: couldn't load api functions!" ) ;

  // model extension functions
  if( vs_get_api_extend( vsDLL, pathDLL ) )
    error( "carsim: vs_get_api_extend: couldn't load api functions!" ) ;  

  // road-related functions
  if( vs_get_api_road(vsDLL, pathDLL ) )
    error( "carsim: vs_get_api_road: couldn't load road api functions!" ) ;  


  // install external C functions

  // calc is where you insert code to run
  // at start and end of timestep.
  vs_install_calc_function( &external_calc ) ;

  // echo 
  vs_install_echo_function( &external_echo ) ;
  vs_install_scan_function( &external_scan ) ;
  vs_install_setdef_function( &external_setdef ) ;

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
  window->loadSprite( Road1, ASSET( "sprites/Road1.png" ) ) ;
  window->loadSprite( Road2, ASSET( "sprites/Road2.png" ) ) ;

  window->loadSound( Screech, ASSET("sounds/Generic-Tire-01_Skid-01.wav") ) ;

  window->loadSound( EngineLowIn, ASSET("sounds/Generic_Engine_01_L4_2.4L_0.0_Load_02_Low_RPM.WAV") ) ;
  window->loadSound( EngineLowOut, ASSET("sounds/Generic_Engine_01_L4_2.4L_1.0_Load_02_Low_RPM.WAV") ) ;
  
  window->loadSound( EngineMidIn, ASSET("sounds/Generic_Engine_02_L4_2.4L_0.0_Load_03_Mid_RPM.WAV") ) ;
  window->loadSound( EngineMidOut, ASSET("sounds/Generic_Engine_02_L4_2.4L_1.0_Load_03_Mid_RPM.WAV") ) ;


  window->loopSound( EngineMidIn ) ;

  FMOD_CHANNEL *c = window->getFmodChannel( EngineMidIn ) ;

  window->createPitchShiftToChannel( c ) ;
  
  
  




  // Set the background clearing color to dk blue-gray
  window->setBackgroundColor( D3DCOLOR_ARGB( 255, 35, 35, 70 ) ) ;

  // Create a few fonts
  window->createFont( Fonts::Arial16, "Arial", 16, FW_BOLD, false ) ;
  
  window->createFont( Fonts::TimesNewRoman24, "Times New Roman", 24, FW_BOLD, true ) ;
  #pragma endregion

  // Rest of init
  simWorld = new SimWorld() ;


  // LOAD AND START UP CARSIM
  info( "Starting up CarSim . . . " ) ;


  // change to the carsim data directory
  window->cd( "D:/CarSim_Data" ) ;

  carsimSetup( "simfile" ) ;

  // revert to root
  window->cdPop() ;
}

void Update()
{
  
  #pragma region camera motion and other ui funcs



  const static float increment = 0.005f ; 

  if( simWorld->camMode == SimWorld::FreeCam )
  {
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
  else if( simWorld->camMode == SimWorld::CockpitCam )
  {
    window->getCamera()->follow(
      simWorld->car->getPos(),
      simWorld->car->getFwd(), 
      0.2f, // units back
      0.5f  // units up
    ) ;
  }
  else if( simWorld->camMode == SimWorld::FollowCam )
  {
    // FollowCam
    window->getCamera()->follow(
      simWorld->car->getPos(),
      simWorld->car->getFwd(), 
      8, // 8 units back
      2  // 2 units up
    ) ;

  }

  // set the window's view by whatever the camera's doing
  window->setViewBYCAMERA() ;







  // Change fill mode
  if( window->keyJustPressed( '`' ) )
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
  if( window->keyJustPressed( '4' ) )
  {
    simWorld->camMode = SimWorld::FreeCam ;
    window->getCamera()->reset() ;
  }

  if( window->keyJustPressed( '5' ) )
  {
    simWorld->camMode = SimWorld::CockpitCam ;
  }

  if( window->keyJustPressed( '6' ) )
  {
    simWorld->camMode = SimWorld::FollowCam ;
  }


  if( window->keyJustPressed( VK_ESCAPE ) )
  {
    // pop to tld
    while( window->cdPop() ) ;

    __RUNNING__ = false ;
  }

  if( window->keyJustPressed( 'R' ) )
  {
    // reset the camera
    info( "Camera reset" ) ;
    simWorld->camMode = SimWorld::FreeCam ;
    window->getCamera()->reset() ;
  }

  if( window->keyIsPressed( 'G' ) )
  {
    window->getCamera()->goTo( simWorld->car->getPos() ) ;
  }
  #pragma endregion



  // Update carsim.  this steps
  // the carsim simulator by one frame.
  carsimUpdate() ;

  //!! DO NOT CALL THIS HERE.
  // Let CarSim call it.
  ///simWorld->car->update( window->getTimeElapsedSinceLastFrame() ) ;
  


}


// DO ALL DRAWING HERE.
void Draw()
{
  // Reset the world xform
  D3DXMATRIX identity ;
  D3DXMatrixIdentity( &identity ) ;
  window->setWorld( &identity ) ;


  
  window->setDrawingMode( D3 ) ; // 3d
  

  // draw axes, 50 units in length
  window->setVertexDeclaration( D3DWindow::PositionColor ) ;
  window->setLighting( false ) ;
  window->drawAxes( 50.0f ) ;


  // Call ThreeDMan's draw function.
  // This draws the road.
  window->draw3DObjects() ;


  simWorld->Draw() ;

  // draw the mouse cursor with this sprite.
  //window->drawMouseCursor( Mario ) ;

  //window->drawFrameCounter(); // erase this if you don't like
  // the frame counter in the top right corner

  window->drawTimer() ;
}








// ^^ ^^ ^^ ^^ ^^ ^^ ^^ ^^ ^^ ^^ ^^ ^^ ^^ ^^ ^^
// main code above.





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
      // NO CODE HERE!  use the
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

        // we should probably pause the simulation here,
        info( "But why would you click away?" ) ;
      }
    }
    return 0 ;

  case WM_PAINT:
    {
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
  window = new GameWindow( hInstance, TEXT( "carsim project - mohammad elbagoury" ),
     32, 32, // x pos, y pos
     640, 480 // width, height
  ) ;

  // After the window comes up, we call Init
  // to start up carsim
  Init() ;

  MSG message ;
  while( __RUNNING__ )
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

        window->endDrawing() ;
      }
    }
  }

  info( "simulation ended!" ) ;

  carsimShutdown() ;

  logShutdown() ;

  GdiplusShutdown(gdiplusToken);

  //system( "pause" ) ; // uncomment to make it pause before exit
  return 0 ;
}

