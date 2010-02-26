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
  ColdArrow3 
} ;

void Init()
{
  // Load sounds
  window->loadSound( HumanMusic, "sounds/Human1.mp3", FMOD_CREATESTREAM ) ;

  window->loadSound( TreeWhat, "sounds/What2.wav" ) ;
  window->loadSound( ColdArrow1, "sounds/ColdArrow1.wav" ) ;
  window->loadSound( ColdArrow2, "sounds/ColdArrow2.wav" ) ;
  window->loadSound( ColdArrow3, "sounds/ColdArrow3.wav" ) ;

  window->playSound( HumanMusic ) ;
  window->playSound( TreeWhat ) ;

  // sprite loading
  window->loadSprite( Mario, "sprites/mario.png" ) ;
  window->loadSprite( SixteenCounter, "sprites/16counter.png", 0, 32, 32, 16, 0.1f ) ;
  window->loadSprite( Astos, "sprites/Astos.png" ) ;
  window->loadSprite( Eye, "sprites/Eye.png" ) ;
  window->loadSprite( Garland, "sprites/Garland.png" ) ;
  window->loadSprite( Kary, "sprites/Kary.png" ) ;
  window->loadSprite( Kraken, "sprites/Kraken.png" ) ;
  window->loadSprite( Lich, "sprites/Lich.png" ) ;
  window->loadSprite( Phantom, "sprites/Phantom.png" ) ;
  window->loadSprite( Pirate, "sprites/Pirate.png" ) ;
  window->loadSprite( Chaos, "sprites/Chaos.gif" ) ;

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

}

void Update()
{
  // update the game, happens 60 times a second
  if( window->isPressed( VK_CONTROL ) )
  {
    //info( "adding a new guy!" ) ;

    GameObject go ;
    go.x = rand() % window->getWidth() ;
    go.y = rand() % window->getHeight() ;
    go.vx = randFloat( 3, 5 ) ;
    go.vy = randFloat() ;
    go.ax = randFloat( 10, 20 ) ;
    go.ay = randFloat( -10, 10 ) ;
    go.spriteIndex = window->randomSpriteId( 10 ) ;

    gameObjects.push_back( go ) ;
  }
  if( window->isPressed( VK_SHIFT ) )
  {
    // remove one
    if( !gameObjects.empty() )
      gameObjects.pop_back() ;
  }
  if( window->justPressed( VK_F5 ) )
  {
    // F5 opens up the log file in baretail
    system( "START baretail.exe lastRunLog.txt" ) ;
  }
  
  window->step() ; // ^^ update fmod engine, grab updated keystates, etc.  LEAVE AS LAST LINE.
}


// DO ALL DRAWING HERE.
void Draw()
{
  // Draw the game, happens 60 times a second
  for( int i = 0 ; i < gameObjects.size() ; i++ )
  {
    window->drawSprite(
      gameObjects[i].spriteIndex,
      gameObjects[i].x, gameObjects[i].y ) ;
  }

  // Print # sprites.
  char buf[ 100 ];  //!!
  int numSprites = gameObjects.size();
  sprintf( buf, "%d sprites", numSprites ) ;
  window->drawBox( D3DCOLOR_ARGB( 235, 255, 0, 0 ), 10, 10, 100, 30 ) ;
  window->drawString( buf, Color::Yellow, 20, 6, 80, 40, DT_CENTER | DT_VCENTER | DT_NOCLIP ) ;


  // Print response message
  if( numSprites == 0 )
  {
    window->drawString( "Hold CTRL down!  Go on!",
      D3DCOLOR_ARGB( 255, 255, 255, 255 ) ,
      0, 0, window->getWidth(), window->getHeight() ) ;   // Draw this center screen
    // (using a box that starts in top left corner,
    // goes down to bottom right corner)
  }
  else if( !window->isSlow() )
  {
    // Draw this center screen
    window->drawSprite( Sprites::MessagePressCtrlTaunt, window->getWidth()/2, window->getHeight()/2 ) ;
  }
  else
  {
    // Draw this center screen
    window->drawSprite( Sprites::MessageStruggling, window->getWidth()/2, window->getHeight()/2 ) ;
  }

  window->drawMouseCursor( Sprites::Chaos ) ; // draw the mouse cursor with this sprite.

  window->drawFrameCounter(); // erase this if you don't like
  // the frame counter in the top right corner

}

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
  
  case WM_CREATE :
    {
      // This is the tidbit of code that
      // runs when our window has been created
    }
    return 0;

  case WM_LBUTTONDOWN:
    {
      info( "Legacy WM_LBUTTONDOWN message" ) ;
    }
    return 0 ;

  case WM_RBUTTONDOWN:
    {
      info( "Legacy WM_LBUTTONDOWN message" ) ;
    }
    return 0;

  case WM_KEYDOWN:
    {
      // Generally DON'T 
      if( wparam == VK_ESCAPE )
      {
        info( "Quitting..." ) ;
        //PostQuitMessage( 0 ) ;
      }
    }
    return 0;

  case WM_CHAR:
    {
      info( "You pushed %c, ascii=%d", wparam, wparam ) ;
    }
    return 0 ;

    // WM_INPUT messages are for RAW INPUT (faster
    // than WM_KEYDOWN or WM_MOUSEMOVE messages)
    // Both keyboard AND mouse input events get picked up here
  case WM_INPUT: 
    {
      UINT dwSize;

      GetRawInputData( (HRAWINPUT)lparam, RID_INPUT, NULL, &dwSize, sizeof( RAWINPUTHEADER ) ) ;
      LPBYTE lpb = new BYTE[ dwSize ] ;
      if( lpb == NULL )
      {
        return 0;
      }

      if( GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize )
      {
        error( "GetRawInputData doesn't return correct size !" ) ;
      }

      RAWINPUT *raw = (RAWINPUT*)lpb;

      
      if (raw->header.dwType == RIM_TYPEKEYBOARD) 
      {
        // We don't take keyboard input here.
        // We take it by using GetKeyboardState() function
        // in the window->step() function.
        //printRawKeyboard( raw ) ; // just so you can see

        switch( raw->data.keyboard.VKey )
        {
        case VK_ESCAPE :
          PostQuitMessage( 0 ) ;
          return 0 ;
        }
      }
      else if (raw->header.dwType == RIM_TYPEMOUSE)
      {
        //printRawMouse( raw ) ;  // just so you can see
        
        if( raw->data.mouse.ulButtons & RI_MOUSE_LEFT_BUTTON_DOWN )
        {
          info( "The left mouse button has been pushed" ) ;
          
          // purposefully test playing invalid sounds occassionally
          window->playSound( 3 + rand() % 5 ) ;

          ///window->setSize( window->getHeight(), window->getWidth(), false ) ;
        }
        else if( raw->data.mouse.ulButtons & RI_MOUSE_RIGHT_BUTTON_DOWN )
        {
          info( "The right mouse button has been pushed" ) ;

          ///window->setSize( window->getWidth(), window->getHeight(), true ) ;
        } 

        window->moveMouseX( raw->data.mouse.lLastX ) ;
        window->moveMouseY( raw->data.mouse.lLastY ) ;

      } 

      delete[] lpb; 
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
  window = new Window( hInstance, TEXT( "Window title" ),
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

