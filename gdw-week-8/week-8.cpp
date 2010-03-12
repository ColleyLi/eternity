#include "../eternity/eternity.h"
/*

      ___         ___           ___           ___           ___                     ___         ___     
     /\  \       /\  \         /\  \         /\  \         /\__\          ___      /\  \       |\__\    
    /::\  \      \:\  \       /::\  \       /::\  \       /::|  |        /\  \     \:\  \      |:|  |   
   /:/\:\  \      \:\  \     /:/\:\  \     /:/\:\  \     /:|:|  |        \:\  \     \:\  \     |:|  |   
  /::\~\:\  \     /::\  \   /::\~\:\  \   /::\~\:\  \   /:/|:|  |__      /::\__\    /::\  \    |:|__|__ 
 /:/\:\ \:\__\   /:/\:\__\ /:/\:\ \:\__\ /:/\:\ \:\__\ /:/ |:| /\__\  __/:/\/__/   /:/\:\__\   /::::\__\
 \:\~\:\ \/__/  /:/  \/__/ \:\~\:\ \/__/ \/_|::\/:/  / \/__|:|/:/  / /\/:/  /     /:/  \/__/  /:/~~/~   
  \:\ \:\__\   /:/  /       \:\ \:\__\      |:|::/  /      |:/:/  /  \::/__/     /:/  /      /:/  /     
   \:\ \/__/   \/__/         \:\ \/__/      |:|\/__/       |::/  /    \:\__\     \/__/       \/__/      
    \:\__\                    \:\__\        |:|  |         /:/  /      \/__/    
     \/__/                     \/__/         \|__|         \/__/                                           


*/

////////////////////////////////////////////
//             GDW - WEEK - 8
//
Window *window ;  // the main window object

// The ASSET macro adds
// "../assets/" IN FRONT of
// to whatever you want.
// So:
//    ASSET("sprites/Chaos.gif")
// turns into
//    "../assets/sprites/Chaos.gif"
// In the pre-processor.
// The reason this is done is so all the separate
// projects ("game", "gdw-week-8" etc.)
// can share the same ASSETS folder,
// so we don't have to keep duplicating assets
// and wasting MB.
// This also makes it work if you double-click
// the .exe file in the /Build folder.
// Your game will still be able to find
// the assets.
#define ASSET(x) ("../assets/"##x)

// This game has a couple of states
enum GameState
{
  Menu,
  Running,
  Paused,
  GameOver
};

// Declare a global GameState,
// to be used .. game-wide.
GameState gameState ;

#pragma region a few enums
enum Sprites
{
  Mario = 1,
  MenuNewGame
} ;

enum Sounds
{
  HumanMusic = 1,
  TreeWhat,
  ColdArrow1,
  ColdArrow2,
  ColdArrow3
} ;

enum Fonts
{
  Arial24 = 1,
  TimesNewRoman18
} ;
#pragma endregion

void Init()
{
  #pragma region asset load and font create
  // Load sounds.  Notice how
  // we use the ASSET() macro to
  // make all filenames start with "../assets/"
  window->loadSound( HumanMusic, ASSET("sounds/Human1.mp3"), FMOD_CREATESTREAM ) ;
  window->loadSound( TreeWhat, ASSET("sounds/What2.wav") ) ;
  window->loadSound( ColdArrow1, ASSET("sounds/ColdArrow1.wav") ) ;
  window->loadSound( ColdArrow2, ASSET("sounds/ColdArrow2.wav") ) ;
  window->loadSound( ColdArrow3, ASSET("sounds/ColdArrow3.wav") ) ;

  window->playSound( TreeWhat ) ;
  window->loopSound( HumanMusic ) ; // Loop this sound forever

  // sprite loading
  window->loadSprite( Mario, ASSET("sprites/mario.png") ) ;
  window->loadSprite( Sprites::MenuNewGame, ASSET("sprites/MenuNewGame.png") ) ;

  // Create a few fonts
  window->createFont( Fonts::Arial24, "Arial", 24, FW_NORMAL, false ) ;
  window->createFont( Fonts::TimesNewRoman18, "Times New Roman", 18, FW_BOLD, false ) ;

  // Black bkg color to start
  window->setBackgroundColor( D3DCOLOR_ARGB( 255, 0, 0, 0 ) ) ;
  #pragma endregion

}

void Update()
{
  // update the game, happens 60 times a second

  // How we update the game really
  // depends on __WHAT STATE__
  // the game is IN.

  switch( gameState )
  {
  case GameState::Menu:
    if( window->mouseJustPressed( Mouse::Left ) )
    {
      // There was a click.  Hmm.  Was it
      // on a button?  Let's assume the
      // person clicked the button.

      // You can figure out how to see if a person
      // clicked within a rectangle pretty easily
      int xClick = window->getMouseX() ;
      int yClick = window->getMouseY() ;
      // if( click location is in box of button ... ) 
      // You could even make a Button class...



      // Anyway, here's the code to run
      // when it is time to start the game
      // from the menu
      gameState = GameState::Running ; // Start the game.
      window->setBackgroundColor( D3DCOLOR_ARGB( 255, 64, 0, 0 ) ) ;
    }
    break;

  case GameState::Running:
    // Here we run the game.
    // Allow PAUSE by pressing 'P'
    if( window->keyJustPressed( 'P' ) )
    {
      gameState = GameState::Paused;
      window->pause() ; // Pause the engine as well
      // This halts animations.

      info( "Paused..." ) ;
    }
    break;

  case GameState::Paused:
    // ((do nothing except check))
    // for P again to unpause
    if( window->keyJustPressed( 'P' ) )
    {
      gameState = GameState::Running ;
      window->unpause() ;
      info( "UNPAUSED" ) ;
    }
    break;

  case GameState::GameOver:
    //
    break;
  }

  // This code runs every frame
  // regardless of what "state"
  // the game is in.
  // Quit if the user presses ESCAPE
  if( window->keyJustPressed( VK_ESCAPE ) )
  {
    bail( "game ended!" ) ;
  }
}


// DO ALL DRAWING HERE.
void Draw()
{
  // Draw the game, happens 60 times a second

  // What we draw
  // also depends
  // on game state
  switch( gameState )
  {
  case GameState::Menu:
    window->drawString(
      Fonts::Arial24,
      "Click the mouse to start",
      Color::White );
    break;

  case GameState::Running:
    break;

  case GameState::Paused:
    break;

  case GameState::GameOver:
    break;
  }


  // Run these draw calls regardless of game state
  window->drawMouseCursor( Mario ) ;

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
  window = new Window( hInstance, TEXT( "eternity engine base" ),
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

