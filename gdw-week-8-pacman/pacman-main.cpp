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

// Notes:
// The font is PacFont from http://www.fontspace.com/font-a-licious/pacfont-good
// I created images of the text though
// because you probably don't have PacFont installed,
// and we're not running installers here

// PACMAN is a tile-based game.

// Tile-based games are nice because we can
// specify levels with a simple .txt file,
// one character per block.

// But movement between the tiles is continuous.  So we
// clearly can't use TILES for the player
// and ghost objects.



Window *window ;  // the main window object

#pragma region explaining the ASSET macro
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
#pragma endregion
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

// All of the enums for Sprites, Sounds and Fonts
#include "EnumDefinitions.h"

// For all other game-specific variables,
// they will be stored in the GameWorld
// object.
#include "GameWorld.h"
GameWorld *game ;

void Init()
{
  #pragma region asset load and font create
  // Load sounds.  Notice how
  // we use the ASSET() macro to
  // make all filenames start with "../assets/"
  window->loadSound( Sounds::PacmanTitleThemeLoop, ASSET("sounds/pacman-theme-loop.mp3"), FMOD_CREATESTREAM ) ;
  window->loadSound( Sounds::PacmanGamePlayMusic, ASSET("sounds/powerpill.mp3") ) ;
  window->loadSound( Sounds::PacmanPellet1, ASSET("sounds/pacman-pellet-1.wav" ) ) ;
  window->loadSound( Sounds::PacmanPellet2, ASSET("sounds/pacman-pellet-2.wav" ) ) ;
  window->loadSound( Sounds::PacmanPellet3, ASSET("sounds/pacman-pellet-3.wav" ) ) ;

  window->loopSound( Sounds::PacmanTitleThemeLoop ) ; // Loop this sound forever,
  // or until we stop it using window->stopSound()



  // sprite loading
  window->loadSprite( Sprites::Bonus, ASSET("sprites/cherries.png"), 0, 16, 16, 4, 0.5f ) ;
  window->loadSprite( Sprites::Mario, ASSET("sprites/mario.png") ) ;

  window->loadSprite( Sprites::Pacman, ASSET("sprites/pacman-spritesheet.png"), 0, 16, 16, 4, 0.4f ) ;
  window->loadSprite( Sprites::PacmanTitle, ASSET("sprites/pacman-title-ii.png") ) ;

  window->loadSprite( Sprites::Pellet, ASSET("sprites/normal-pellet.png") ) ;
  window->loadSprite( Sprites::Powerpellet, ASSET("sprites/powerpellet.png") ) ;
  window->loadSprite( Sprites::Wall, ASSET( "sprites/wall.png" ) ) ;
  window->loadSprite( Sprites::Barrier, ASSET( "sprites/ghost-door-barrier.png" ) ) ;

  // It may seem silly to load and draw an empty sprite,
  // but it makes programming the game logic easier for a tile
  // with "nothing in it"
  window->loadSprite( Sprites::Empty, ASSET( "sprites/empty.png" ) ) ;

  // Create a few fonts
  window->createFont( Fonts::Arial24, "Arial", 24, FW_NORMAL, false ) ;
  window->createFont( Fonts::TimesNewRoman18, "Times New Roman", 18, FW_BOLD, false ) ;

  // Black bkg color to start
  window->setBackgroundColor( D3DCOLOR_ARGB( 255, 0, 0, 0 ) ) ;
  #pragma endregion

  // Start up the game world
  info( "Starting up the GameWorld..." ) ;
  game = new GameWorld() ;
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
    // In the menu state, ALL we're doing
    // is listening for a click.  A click
    // means the user wants to start the game.
    if( window->mouseJustPressed( Mouse::Left ) )
    {
      // There was a click.  The person
      // wants to start the game.

      // Here's the code we want to run
      // when it is time to start the game
      // from the menu.

      // Stop the title theme music
      window->stopSound( Sounds::PacmanTitleThemeLoop ) ;

      // Start up the game play music
      window->playSound( Sounds::PacmanGamePlayMusic ) ;

      
      // Load "lvl1.txt"
      game->loadLevel( "lvl1.txt" ) ;

      // Change the background color to something else
      window->setBackgroundColor( D3DCOLOR_ARGB( 255, 0, 0, 64 ) ) ;

      // Finally, TRANSITION STATE to Running.
      gameState = GameState::Running ; // Start the game.
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
    // End the game and transition to menu
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

  // If you picture a money sorter
  // this kind of makes sense
  switch( gameState )
  {
  case GameState::Menu:
    // Draw the title
    window->drawSprite( Sprites::PacmanTitle, window->getWidth()/2, 150 ) ;

    // Draw the start message
    window->drawString( Fonts::Arial24, "Click the mouse to start", Color::White );
    break;

  case GameState::Running:
    {
      // When the game is running,
      // ask the GameWorld object
      // to draw itself
      game->draw();
    }
    break;

  case GameState::Paused:
    // When paused..
    // we also must draw the
    // game world, or else
    // you'll see nothing.
    // The only difference when
    // paused is we limit our
    // response to input commands
    // to only "unpause" command
    {
      // Really we could have
      // put this in the same
      // case as GameState::Running
      // for DRAW.
      game->draw();
    }
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
  
  // See ERROR and WARNING messages at Console.
  logOutputsForConsole = LOG_ERROR | LOG_WARNING | LOG_INFO ;
  
  // See all types of messages in file
  logOutputsForFile = LOG_ERROR | LOG_WARNING | LOG_INFO ;
  
  // See all ERROR and WARNING messages at debugstream. Suppress 'info()' messages.
  logOutputsForDebugStream = LOG_ERROR | LOG_WARNING ;
  
  // Start up GDI+, which we use to draw
  // For GDI+, used only for shape render
  GdiplusStartupInput gdiplusStartupInput;
  ULONG_PTR gdiplusToken;
  GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

  // Setup the window
  window = new Window( hInstance, TEXT( "pacman" ),
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

