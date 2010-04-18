#include "../eternity/eternity.h"

#define ASSET(x) ("../assets/"##x)

GameWindow *window ;  // the main window object

enum Sprites
{
  Mario = 1,
  SixteenCounter
} ;

enum Sounds
{
  HumanMusic = 1
} ;

enum Fonts
{
  Arial8,
  TimesNewRoman24
} ;

void Init()
{
  // Load sounds
  window->loadSound( HumanMusic, ASSET("sounds/Human1.mp3"), FMOD_CREATESTREAM ) ;

  // sprite loading
  window->loadSprite( Mario, ASSET("sprites/mario.png") ) ;

  // Animated sprite
  window->loadSprite( SixteenCounter, ASSET("sprites/16counter.png"), 0, 32, 32, 16, 0.5f ) ;

  // Set the background clearing color to dk blue-gray
  window->setBackgroundColor( D3DCOLOR_ARGB( 255, 35, 35, 70 ) ) ;

  // Create a few fonts
  window->createFont( Fonts::Arial8, "Arial", 16, FW_BOLD, false ) ;
  
  window->createFont( Fonts::TimesNewRoman24, "Times New Roman", 24, FW_BOLD, true ) ;

}

void Update()
{
  // update the game, happens 60 times a second

  // Quit if the user presses ESCAPE
  if( window->keyJustPressed( VK_ESCAPE ) )
  {
    bail( "game ended!" ) ;
  }

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



  window->setByCamera() ;
}


// DO ALL DRAWING HERE.
void Draw()
{
  // Draw the game, happens 60 times a second

  window->setDrawingMode( D3 ) ; // 3d
  
  window->drawAxes( 8.0f ) ;

  window->drawTri(
    D3DVertexC( 0, 0, 0, 255, 0, 0 ),
    D3DVertexC( 0, 5, 0, 0, 255, 0 ),
    D3DVertexC( -5, 5, 0, 0, 0, 255 )
  ) ;

  window->drawQuad(

    D3DVertexC( 1.2f, 0.5f, 1, 255, 255, 255 ),
    D3DVertexC( 5, 0, 1, 0, 255, 0 ),
    D3DVertexC( 2.5, 5, 1, 0, 255, 255 ),
    D3DVertexC( 0, 5, 1, 0, 0, 255 )

  ) ;


  // This isn't really necessary
  // due to how id3dx sprite works
  window->setDrawingMode( D2 ) ; // 2d

  window->drawLine(
    D3DXVECTOR2( 100, 100 ),
    D3DXVECTOR2( 200, 100 ),
    D3DCOLOR_ARGB( 255, 255, 255, 0 ),
    5.0f
  ) ;

  
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
  consoleMove( 32, 500 ) ;
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

