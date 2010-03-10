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
//
// ascii text by http://www.network-science.de/ascii
//
// REQUIRES:  DIRECTX
//            FMOD


// VV VV VV VV VV VV VV VV VV VV VV VV VV VV VV
// Your code starts below this line.

#include "GameObject.h"
vector<GameObject> gameObjects ; // a vector of game objects

Window *window ;  // the main window object

enum Sprites
{
  Mario = 1,
  SixteenCounter,
  Astos,
  Eye,
  Garland,
  Kary,
  Kraken,
  Lich,
  Phantom,
  Pirate,
  Chaos,
  BoxedTextTest,
  GDIPlusTextureTest,
  MessagePressCtrlTaunt,
  MessageStruggling
} ;

enum Sounds
{
  HumanMusic = 1,
  TreeWhat,
  ColdArrow1,
  ColdArrow2,
  ColdArrow3,
  NerzuhlWillHaveYourHead
} ;

void Init()
{
  // Load sounds
  #pragma region load up sounds
  window->loadSound( HumanMusic, "sounds/Human1.mp3", FMOD_CREATESTREAM ) ;
  window->loadSound( TreeWhat, "sounds/What2.wav" ) ;

  // Load a sound we don't intend to loop
  window->loadSound( ColdArrow1, "sounds/ColdArrow1.wav" ) ;

  // If you're going to LOOP a sound,
  // you MUST load it with FMOD_SOFTWARE
  // OR with FMOD_CREATESTREAM.
  // Use FMOD_CREATESTREAM for long (5 minute)
  // songs that you will loop as background music
  // and FMOD_SOFTWARE for short, 2-5 second sounds
  // that you want to loop (e.g. a crackling fire,
  // or dripping water).
  window->loadSound( ColdArrow2, "sounds/ColdArrow2.wav", FMOD_SOFTWARE ) ; // load the sound we intend to loop
  window->loadSound( ColdArrow3, "sounds/ColdArrow3.wav" ) ;

  window->loadSound( NerzuhlWillHaveYourHead, "sounds/Odpissd3.wav" ) ;
  
  window->playSound( TreeWhat ) ;
  window->playSound( HumanMusic ) ;
  #pragma endregion

  // sprite loading
  #pragma region load up sprites
  window->loadSprite( Mario, "sprites/mario.png" ) ;

  // Animated sprite
  window->loadSprite( SixteenCounter, "sprites/16counter.png", 0, 32, 32, 16, 0.5f ) ;

  // other sprites
  window->loadSprite( Astos, "sprites/Astos.png" ) ;
  window->loadSprite( Eye, "sprites/Eye.png" ) ;
  window->loadSprite( Garland, "sprites/Garland.png" ) ;
  window->loadSprite( Kary, "sprites/Kary.png" ) ;
  window->loadSprite( Kraken, "sprites/Kraken.png" ) ;
  window->loadSprite( Lich, "sprites/Lich.png" ) ;
  window->loadSprite( Phantom, "sprites/Phantom.png" ) ;
  window->loadSprite( Pirate, "sprites/Pirate.png" ) ;
  window->loadSprite( Chaos, "sprites/Chaos.gif" ) ;
  #pragma endregion

  #pragma region create boxed text sprites
  // Create boxed text as a sprite.  If you do this
  // to your text before hand, it may run faster than if you
  // simply running drawBox() and drawString() every call.
  window->boxedTextSprite( BoxedTextTest,
    "Some boxed text",
    D3DCOLOR_ARGB( 255, 255, 0, 0 ), // fg color
    D3DCOLOR_ARGB( 200, 129, 47, 0 ),// bg color
    15  // "padding" around the edges of the text,
    // try increasing / reducing this value to see
    // the way it looks
  ) ;
  
  // You can have different padding on all the edges as well.
  RECT padding = { 35,12,35,12 } ; // left, top, bottom, left.
  padding.right = 12 ; // can also assign this way.

  // Create a sprite with this text in it..
  // referenced by the MessagePressCtrlTaunt,
  // enum'd value (which is really just an int)
  window->boxedTextSprite( MessagePressCtrlTaunt,
    "Is that the best you can do???  CTRL!! Go on!",
    D3DCOLOR_ARGB( 235, 255, 0, 0 ),
    D3DCOLOR_ARGB( 235, 255, 255, 0 ),
    padding ) ;

  // Final message, shown when frame rate
  // starts to drop.
  window->boxedTextSprite( 
    MessageStruggling, "OK now i'm struggling.\n"
        "Please take some away with SHIFT",
    D3DCOLOR_ARGB( 255, 255, 255, 0 ),
    D3DCOLOR_ARGB( 235, 0, 0, 128 ),
    padding ) ;
  #pragma endregion

  // Set the background clearing color to dk blue-gray
  window->setBackgroundColor( D3DCOLOR_ARGB( 255, 35, 35, 70 ) ) ;

}

void Update()
{
  // update the game, happens 60 times a second

  // Key presses
  if( window->keyJustPressed( VK_ESCAPE ) )
  {
    bail( "game ended!" ) ;
  }

  if( window->mouseJustPressed( Mouse::Left )  )
  {
    //info( "LEFT MOUSE BUTTON was PUSHED!" ) ;

    window->playSound( NerzuhlWillHaveYourHead ) ;
  }

  // Mouse presses
  if( window->mouseJustReleased( Mouse::Right ) ) 
  {
    //info( "RIGHT MOUSE BUTTON was RELEASED!!" ) ;
    
    // purposefully test playing invalid sound
    //window->playSound( 502005 ) ;

    window->stopSound( Sounds::HumanMusic ) ;
  }

  if( window->mouseIsPressed( Mouse::Middle ) )
  {
    info( "MIDDLE MOUSE is being HELD DOWN!" ) ;
  }
  
}


// DO ALL DRAWING HERE.
void Draw()
{
  // Draw the game, happens 60 times a second

  if( window->keyIsPressed( VK_SPACE ) )
  {
    // When space is down, draw the eye
    // "as if he were in pure red light",
    // and make him say hello
    window->drawString( "Hello!", Color::Red, 200, 200, 90, 90 ) ;
    window->drawSprite( Sprites::Mario, 320, 240, Color::Red ) ;
  }
  else
  {
    // When space is NOT down, just draw
    // the eye "as if he were in pure white light"
    // (which means to totally use the original colors)
    //window->drawSprite( Sprites::Eye, 320, 240, Color::White ) ;
    
    // The above line is completely equivalent to
    window->drawSprite( Sprites::Mario, 320, 240 ) ; // just draw in
    // original colors, without changing the light color
    // that would be shining on him
  }


  // draw the mouse cursor with this sprite.
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

