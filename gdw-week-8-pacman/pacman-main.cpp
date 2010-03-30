#include "pacman-main.h"

// OUR VERSION of PACMAN is a tile-based game.
// The original one wasn't really tile-based,
// but this one is.

// Tile-based games are nice because we can
// make levels with a simple .txt file,
// one character per block.  See lvl1.txt.

//////////////////////////////////////////
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//(If there are enough exclamation marks,
// students MIGHT read it)
//
// THE WAY THIS PROGRAM IS STRUCTURED. 
// -----------------------------------
// So, how's it going.  You decided
// to read the comments.  Good for you!
// So, there are 2 main objects.
//   - Window
//   - GameWorld
// 
// WINDOW:
//
GameWindow *window ;  // the main window object, which
// provides:
//  - SOUND LOAD&PLAY
//  - SPRITE LOAD&DRAW
//  - INPUT MANAGEMENT from mouse, keyboard and gamepad
// Please browse InputMan.h, SoundMan.h and SpriteMan.h
// for further details.
//
// GAMEWORLD:
// Our entire game will be
// stored in the GameWorld
// object.
#include "GameWorld.h"
// ! RULE:
//   ----
//   Every code file here that wants the
//   privilege of accessing "game" (THE
//   one and only GameWorld object) AND
//   "window" (to do window->drawSprite() etc)
//   __must__ #include "pacman-main.h".
// 
//   More detail:  If you look in "pacman-main.h", you
//   will see it has a magical "extern" declaration..
//   an "extern" variable is like a c++ function
//   prototype, only for VARIABLES.. This is hard
//   to explain, so if you really want to understand it,
//   you should see me during office hours or read up on the internet.
//   
// ! RULE THAT YOU REALLY SHOULD FOLLOW:
//   ----------------------------------
//   + Always split your source code
//     into a .h and .cpp files or you
//     __will__ run into problems!
//     
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

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

void Init()
{
  // randomize
  srand( time(0) ) ;

  #pragma region asset load and font create
  // Load sounds.  Notice how
  // we use the ASSET() macro to
  // make all filenames start with "../assets/"
  window->loadSound( Sounds::PacmanTitleThemeLoop, ASSET("sounds/pacman/pacman-theme-loop.mp3"), FMOD_CREATESTREAM ) ;
  window->loadSound( Sounds::PacmanGamePlayMusic, ASSET("sounds/pacman/powerpill.mp3"), FMOD_CREATESTREAM ) ;
  window->loadSound( Sounds::PacmanPellet1, ASSET("sounds/pacman/pacman-pellet-1-l.wav" ) ) ;
  window->loadSound( Sounds::PacmanPellet2, ASSET("sounds/pacman/pacman-pellet-2-l.wav" ) ) ;
  window->loadSound( Sounds::PacmanPellet3, ASSET("sounds/pacman/pacman-pellet-3-l.wav" ) ) ;
  window->loadSound( Sounds::Intro, ASSET("sounds/pacman/gypsy-jazz.mp3" ) ) ;
  window->loadSound( Sounds::PacmanHurt, ASSET( "sounds/pacman/efire.wav" ) ) ;
  window->loadSound( Sounds::GameOverRiff, ASSET( "sounds/pacman/gameover.wav" ) ) ;

  // sprite loading
  window->loadSprite( Sprites::Bonus, ASSET("sprites/pacman/cherries.png"), 0, 16, 16, 4, 0.5f ) ;
  window->loadSprite( Sprites::Mario, ASSET("sprites/mario.png") ) ;

  window->loadSprite( Sprites::Splash, ASSET("sprites/gdw-splash.png") ) ;

  window->loadSprite( Sprites::Pacman, ASSET("sprites/pacman/pacman-spritesheet.png"), 0, 16, 16, 4, 0.05f ) ;
  window->loadSprite( Sprites::PacmanTitle, ASSET("sprites/pacman/pacman-title.png") ) ;

  window->loadSprite( Sprites::Pellet, ASSET("sprites/pacman/normal-pellet-sheet.png"), 0, 16, 16, 4, 0.2f ) ;
  window->loadSprite( Sprites::Powerpellet, ASSET("sprites/pacman/powerpellet-sheet.png"), 0, 16, 16, 4, 0.2f ) ;
  window->loadSprite( Sprites::Wall, ASSET( "sprites/pacman/wall.png" ) ) ;
  window->loadSprite( Sprites::Barrier, ASSET( "sprites/pacman/ghost-door-barrier.png" ) ) ;



  // It may seem silly to load and draw an empty sprite,
  // but it makes programming the game logic easier for a tile
  // with "nothing in it"
  window->loadSprite( Sprites::Empty, ASSET( "sprites/pacman/empty.png" ) ) ;

  window->loadSprite( Sprites::Gun, ASSET( "sprites/pacman/gun.png" ) ) ;
  window->loadSprite( Sprites::Flamethrower, ASSET( "sprites/pacman/flamethrower.png" ) ) ;
  window->loadSprite( Sprites::Uzi, ASSET( "sprites/pacman/uzi.png" ) ) ;

  // Load ghost body and eyes
  window->loadSprite( Sprites::GhostBody, ASSET( "sprites/pacman/ghost-body-white.png" ), 0, 16, 16, 2, 0.2f ) ;
  window->loadSprite( Sprites::EyesRight, ASSET( "sprites/pacman/eyes-right.png" ) ) ;
  window->loadSprite( Sprites::EyesUp, ASSET( "sprites/pacman/eyes-up.png" ) ) ;
  window->loadSprite( Sprites::EyesDown, ASSET( "sprites/pacman/eyes-down.png" ) ) ;
  window->loadSprite( Sprites::EyesDead, ASSET( "sprites/pacman/eyes-dead.png" ) ) ;

  window->loadSprite( Sprites::GoalNode, ASSET( "sprites/pacman/goal-node.png" ) ) ;
  window->loadSprite( Sprites::GameOverTitle, ASSET( "sprites/pacman/game-over.png" ) ) ;

  // Create a few fonts
  window->createFont( Fonts::Arial24, "Arial", 24, FW_NORMAL, false ) ;
  window->createFont( Fonts::TimesNewRoman18, "Times New Roman", 18, FW_BOLD, false ) ;
  window->createFont( Fonts::PacFont24, "PacFont Good", 24, FW_NORMAL, false ) ;

  // Black bkg color to start
  window->setBackgroundColor( D3DCOLOR_ARGB( 255, 0, 0, 0 ) ) ;
  #pragma endregion

  // Start up the game world
  info( "Starting up the GameWorld..." ) ;
  game = new GameWorld() ;  // create GameWorld object.
}

void Update()
{
  // update the game, happens 60 times a second

  // How we update the game really
  // depends on __WHAT STATE__
  // the game is IN.

  // ALT+ENTER for full screen mode..
  if( window->keyIsPressed( VK_MENU ) &&
      window->keyJustPressed( VK_RETURN ) )
  {
    window->fullScreenInMaxResolution() ;
  }



  // I chose to leave this mammoth switch in here,
  // because I like how Update() ties everything together.
  // Note though, if you wanted to, you could migrate
  // all this code in the switch here (and in the Draw() function)
  // into GameWorld.h
  switch( game->getState() )
  {
  case GameWorld::GameState::Splash:

    
    if( //window->anyKeyPushed() || // If any key was pushed..
        // bah.  key pushed is annoying.  using mouse instead.
        window->mouseJustPressed( Mouse::Left )
      )
    {
      // Jump to the menu
      game->setState( GameWorld::GameState::Menu ) ;
    }
    break ;

  case GameWorld::GameState::Menu:
    // In the menu state, ALL we're doing
    // is listening for a click.  A click
    // means the user wants to start the game.
    if( window->mouseJustPressed( Mouse::Left ) )
    {
      // There was a click.  The person
      // wants to start the game.

      // TRANSITION STATE to Running.
      // IN the handler code for when
      // a transition from Menu to Running
      // occurs is the level load, etc.
      game->setState( GameWorld::GameState::Running ) ; // Start the game.
    }
    break;

  case GameWorld::GameState::Running:

    // Here we run the game.

    game->step( window->getTimeElapsedSinceLastFrame() ) ;

    // Allow PAUSE by pressing 'P'
    if( window->keyJustPressed( 'P' ) )
    {
      game->setState( GameWorld::GameState::Paused ) ;
    }
    break;

  case GameWorld::GameState::Paused:
    // ((do nothing except check))
    // for P again to unpause
    if( window->keyJustPressed( 'P' ) )
    {
      game->setState( GameWorld::GameState::Running ) ;
    }
    break;

  case GameWorld::GameState::GameOver:
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
  switch( game->getState() )
  {
  case GameWorld::GameState::Splash:
    {
      // This only happens right at
      // game start, so we'll just use
      // the game's global clock
      // to determine amount of fade in

      // WHEN:  0 < time < 5 :  Fade in
      //        5 < time < 9 :  stay solid
      //        9 < time < 14 : fade out
      
      int alpha = 0 ;
      double time = window->getTimeElapsedSinceGameStart() ;
      if( time < 5.0 )
      {
        // fade-in phase
        float fadeInPercent = time / 5.0 ;
       
        // And alpha is a linear interpolation say
        alpha = lerp( 0, 255, fadeInPercent ) ;

        clamp( alpha, 0, 255 ) ;
      }
      else if( 5.0 < time && time < 9.0 )
      {
        // stay solid
        alpha = 255 ;
      }
      else
      {
        // fade out
        float fadeOutPercent = ( 14.0 - time ) / 5.0 ;

        alpha = lerp( 0, 255, fadeOutPercent ) ;

        clamp( alpha, 0, 255 ) ;
      }

      // Fade out the logo
      window->drawSprite(
        Sprites::Splash,
        window->getWidth()/2,
        window->getHeight()/2,
        D3DCOLOR_ARGB( alpha, 255, 255, 255 )
      ) ;
    }
    break;

  case GameWorld::GameState::Menu:
    // Draw the title
    window->drawSprite( Sprites::PacmanTitle, window->getWidth()/2, 150 ) ;

    // Draw the start message
    window->drawString(
      Fonts::Arial24,
      "Click the mouse to start",
      Color::White );
    break;

  // When paused..
  // we also must draw the
  // game world, or else
  // you'll see nothing.
  // The only difference when
  // paused is we limit our
  // response to input commands
  // to only "unpause" command
  case GameWorld::GameState::Running:
  case GameWorld::GameState::Paused:
    {

      // When the game is running,
      // ask the GameWorld object
      // to draw itself
      game->drawBoard() ;
      game->drawStats() ;

      // Now draw the player and enemies
      game->drawPeople() ;




      /*
      //!! DEBUG:  draw the map nodes
      Graph* graph = game->getAsciiMap()->graph ;
      D3DCOLOR dColor = D3DCOLOR_ARGB( 128, 255, 0, 0 ) ;
      for( int i = 0 ; i < graph->nodes.size() ; i++ )
      {
        float x = graph->nodes[i]->coord.col * game->tileSize;
        float y = graph->nodes[i]->coord.row * game->tileSize;
        Vector2 dv = game->getDrawingVectorAt( Vector2(x,y) ) ;
        window->drawBoxCentered( dColor, dv.x, dv.y, 20, 20 ) ;
      }
      */
    }
    break;

  case GameWorld::GameState::GameOver:
    // Draw the game over sprite
    window->drawSprite( Sprites::GameOverTitle,
      window->getWidth()/2, 200 ) ;
    break;
  }


  // Run these draw calls regardless of game state

  window->drawFrameCounter(); // erase this if you don't like
  // the frame counter in the top right corner

  window->drawTimer() ;  // draw global sense of game_time

  // remember to draw the mouse last so
  // it always appears on TOP of everything.
  window->drawMouseCursor( Sprites::Mario ) ;





  

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
  window = new GameWindow( hInstance, TEXT( "pacman" ),
     32, 32, // x pos, y pos
     640, 480// width, height
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

