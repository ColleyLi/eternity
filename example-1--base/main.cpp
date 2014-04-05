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


THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 
IN NO EVENT SHALL THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 
Permission to use, copy and distribute this software and its documentation for any purpose is hereby granted without fee, provided that the above copyright notices and this permission notice appear in all copies of the software and related documentation.

*/

// ascii text by http://www.network-science.de/ascii

#define ASSET(x) ("../assets/"##x)

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

enum Fonts
{
  Arial8,
  TimesNewRoman24,
  Elephant16
} ;

void Init()
{
  // Load sounds
  #pragma region load up sounds
  new Sound( HumanMusic, ASSET("sounds/Human1.mp3"), FMOD_CREATESTREAM ) ;
  new Sound( TreeWhat, ASSET("sounds/What2.wav") ) ;
  new Sound( ColdArrow1, ASSET("sounds/ColdArrow1.wav") ) ;
  new Sound( ColdArrow2, ASSET("sounds/ColdArrow2.wav") ) ;
  new Sound( ColdArrow3, ASSET("sounds/ColdArrow3.wav") ) ;

  new Sound( NerzuhlWillHaveYourHead, ASSET("sounds/Odpissd3.wav") ) ;
  
  $->soundMan->playSound( TreeWhat ) ;
  $->soundMan->loopSound( HumanMusic ) ; // Loop this sound forever
  #pragma endregion

  // sprite loading
  #pragma region load up sprites
  new Sprite( Mario, ASSET("sprites/mario.png") ) ;

  // Animated sprite
  new Sprite( SixteenCounter, ASSET("sprites/16counter.png"), 0, 32, 32, 16, 0.5f ) ;

  // other sprites
  new Sprite( Astos, ASSET("sprites/ff/Astos.png") ) ;
  new Sprite( Eye, ASSET("sprites/ff/Eye.png") ) ;
  new Sprite( Garland, ASSET("sprites/ff/Garland.png") ) ;
  new Sprite( Kary, ASSET("sprites/ff/Kary.png") ) ;
  new Sprite( Kraken, ASSET("sprites/ff/Kraken.png") ) ;
  new Sprite( Lich, ASSET("sprites/ff/Lich.png") ) ;
  new Sprite( Phantom, ASSET("sprites/ff/Phantom.png") ) ;
  new Sprite( Pirate, ASSET("sprites/ff/Pirate.png") ) ;
  new Sprite( Chaos, ASSET("sprites/ff/Chaos.gif") ) ;
  #pragma endregion

  #pragma region create boxed text sprites
  // Create boxed text as a sprite.  If you do this
  // to your text before hand, it may run faster than if you
  // simply running drawBox() and drawString() every call.
  $->spriteMan->boxedTextSprite( BoxedTextTest,
    "Some boxed text",
    D3DCOLOR_ARGB( 255, 255, 0, 0 ), // fg color
    D3DCOLOR_ARGB( 200, 129, 47, 0 ),// bg color
    15  // "padding" around the edges of the text,
    // try increasing / reducing this value to see
    // the way it looks
  ) ;
  #pragma endregion

  // Set the background clearing color to dk blue-gray
  $->window->setBackgroundColor( D3DCOLOR_ARGB( 255, 35, 35, 70 ) ) ;


  // Create a few fonts
  $->createFont( Fonts::Arial8, "Arial", 8, FW_NORMAL, false ) ;
  
  $->createFont( Fonts::TimesNewRoman24, "Times New Roman", 24, FW_BOLD, true ) ;

  // If you don't have this font, you should get Arial instead.
  $->createFont( Fonts::Elephant16, "Elephant", 16, FW_NORMAL, false ) ;


  char cwd[260];
  GetCurrentDirectoryA( 260, cwd ) ;
  info( "Current working directory is '%s'", cwd ) ;

}

void Update()
{
  // update the game, happens 60 times a second

  // Quit if the user presses ESCAPE
  if( $->inputMan->keyJustPressed( VK_ESCAPE ) )
  {
    bail( "game ended!" ) ;
  }

  // Press the "S" key to turn off the music
  if( $->inputMan->keyJustPressed( 'S' ) )
  {
    // Letter keys don't have VK_*
    // constants.
    
    // Instead you check 
    // if they are pressed exactly as
    // we've shown here,
    //   window->keyJustPressed( 'J' )
    // MAKE SURE TO USE UPPERCASED LETTERS
    // between 'SINGLE QUOTES'

    $->soundMan->stopSound( Sounds::HumanMusic ) ;
  }

  // Check for numbers across the top
  // of the keyboard like this...
  if( $->inputMan->keyIsPressed( '1' ) )
  {
    info( "Uh.. ONE.  That's what you wanted, right?" ) ;
  }

  // Check for NUMPAD using VK_NUMPAD0-9
  if( $->inputMan->keyIsPressed( VK_NUMPAD1 ) )
  {
    info( "NUMPAD 1.  Numpad on laptop is awkward, so probably avoid it." ) ;
  }
  
  // Mouse presses
  if( $->inputMan->mouseJustPressed( Mouse::Left )  )
  {
    info( "LEFT MOUSE BUTTON was PUSHED!" ) ;
    $->soundMan->playSound( Sounds::ColdArrow1 ) ;
  }
  
  if( $->inputMan->mouseJustReleased( Mouse::Right ) ) 
  {
    info( "RIGHT MOUSE BUTTON was RELEASED!!" ) ;
    
    // purposefully test playing invalid sound
    $->soundMan->playSound( 502005 ) ;
  }

  int x=0;
  if( $->inputMan->mouseIsPressed( Mouse::Middle ) )
  {
    info( "MIDDLE MOUSE is being HELD DOWN!" ) ;
  }
  
}


// DO ALL DRAWING HERE.
void Draw()
{
  // Draw the game, happens 60 times a second

  // Draw Chaos centered at (280, 230)
  $->spriteMan->drawSprite( Sprites::Chaos, 280, 230 ) ;

  if( $->inputMan->keyIsPressed( VK_SPACE ) )
  {
    // When space is down, draw the eye
    // shaded in red, partly see-thru

    // Create color to shade it with.
    D3DCOLOR seeThruRed = D3DCOLOR_ARGB( 180, 255, 0, 0 ) ;
    // This is a partly see-thru RED color.
    // The first number is ALPHA which is "solidness".
    // 255 is FULLY SOLID, 0 is completely see thru
    // the second number is RED,
    // then GREEN
    // then BLUE.
    // Hence "ARGB", standing for
    // "Alpha Red Green Blue"

    // Draw the eye in partly see-thru
    $->spriteMan->drawSprite(
      Sprites::Eye, // Sprite to draw (previously loaded with loadSprite)
      320,  // x position
      240,  // y position
      seeThruRed    // color of light to "shine" on him,
      // or consider it the color to "shade him in" with
    ) ;

    // show text
    $->spriteMan->drawString( Fonts::Arial8, "Hello!", seeThruRed, 360, 200, 90, 90 ) ;

  }
  else
  {
    // When space is NOT down, just draw
    // the eye "as if he were in pure white light"
    // (which means to totally use the original colors)
    //window->drawSprite( Sprites::Eye, 320, 240, Color::White ) ;
    
    // The above line is completely equivalent to
    $->spriteMan->drawSprite( Sprites::Eye, 320, 240 ) ; // just draw in
    // original colors, without changing the light color
    // that would be shining on him
  }

  // Font samples.  'W' checks if the
  // W key is pushed down.  There is no VK_W.
  if( $->inputMan->keyIsPressed( 'W' ) ) 
  {
    $->spriteMan->drawString( Fonts::TimesNewRoman24, "Times New Roman 24 font!!!", 
      D3DCOLOR_ARGB( 255, 255, 255, 255 ),
      20, 20, 300, 300, DT_CENTER | DT_VCENTER ) ;

    $->spriteMan->drawString( Fonts::Arial8, "Hello, this is Arial 8 font",
      D3DCOLOR_ARGB( 255, 255, 255, 255 ),
      20, 60, 300, 300, DT_CENTER | DT_VCENTER ) ;

    $->spriteMan->drawString( Fonts::Elephant16, "Hello, this is Elephant font", 
      D3DCOLOR_ARGB( 255, 255, 255, 255 ),
      20, 100, 300, 300, DT_CENTER | DT_VCENTER ) ;

  }

  // draw the mouse cursor with this sprite.
  $->drawMouseCursor( Mario ) ;

  $->drawFrameCounter(); // erase this if you don't like
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
        $->inputMan->mouseUpdateInput( raw ) ;
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
  Console::initConsole() ;
  Console::white();
  Console::moveTo( 32, 500 ) ;
  Console::setRowsAndCols( 10, 120 ) ;

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
  $ = new GamePak(
     hInstance, TEXT( "eternity engine base" ),
     32, 32, // x pos, y pos
     640, 480, // width, height
     GraphicsWindow::DirectX
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
      $->step() ; // ^^ update fmod engine, grab updated keystates, etc.
      // The reason this happens after game update is
      // the inputs are zeroed out..

      // Draw the game out, all at once
      if( $->window->beginDrawing() ) // only continue if beginDrawing() succeeds
      {
        Draw() ;
        $->window->endDrawing() ;
      }
    }
  }

  info( "Game over!" ) ;
  logShutdown() ;

  GdiplusShutdown(gdiplusToken);

  //system( "pause" ) ; // uncomment to make it pause before exit
  return 0 ;
}

