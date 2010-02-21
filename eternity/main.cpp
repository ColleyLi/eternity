#include "ConsoleColors.h"
#include "WindowClass.h"

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
////////////////////////////////////////////////////////
// REQUIRES:  DIRECTX
//            FMOD


// Restructure inheritance:
// GameWindow : Direct3DWindow : BasicWindow
// Need an object for a SpriteManager (spriteMan)
//   (he records drawSprite calls)..
//    its functions are callable through GameWindow though
//    (gameWindow.drawSprite(x,y,z))

// For drawText, it should accept a va_list..
// and it should have state parameters.

// Perhaps using state parameters for all drawing ins't
// such a bad idea after all

// window->setPos( x, y ) ;
// window->drawSprite( 1 ) ; // uses its default width/height

// Need an object for 

// A change
//!!SPRITE COMMAND
// SEARCH FOR ALL !!

//!! TODO:
//!! add gdi+ render to texture support
//  - primitives
//  - link in .gif loading through GDI+

//!! render a 4x4 pixel patch of a color using LockRect()
// then blit that sprite out, stretching it using scale,
// or you could even use a 1x1 pixel patch.  the antialiasing
// settings might help make good antialais effects.
// it should be easy to write a simple primitive blitter then,
// if all that's missing from d3d is Ellipse and bezier splines.
// opengl supports quadrics through the glu functions,
// does direct3d?

//!! improve the framerate by batching properly

#include "GameObject.h"
vector<GameObject> gameObjects ;



Window *window ;

#define VIRTUAL_SECOND 0.016666666667f


//!! remember to increase Z so that
// you get a painter's algorithm effect.

int playerXPos, playerYPos ;

// Remember to compile to a .lib file


enum Sounds
{
  HumanMusic = 1,  // the numbers here
  TreeWhat   = 2,  // =1, =2, =3 aren't necessary
  ColdArrow1 = 3,  // but they show you
  ColdArrow2 = 4,  // each of these is really
  ColdArrow3 = 5   // just an int
} ;

void Init()
{
  // Load sounds
  window->loadSound( HumanMusic, "sounds/Human1.mp3", FMOD_CREATESTREAM ) ;

  window->loadSound( TreeWhat, "sounds/What2.wav" ) ;
  //window->loadSound( ColdArrow1, "ColdArrow1.wav" ) ;
  //window->loadSound( ColdArrow2, "ColdArrow2.wav" ) ;
  //window->loadSound( ColdArrow3, "ColdArrow3.wav" ) ;

  //window->playSound( HumanMusic ) ;
  //window->playSound( TreeWhat ) ;

  window->loadSprite( 1, "sprites/mario.png" ) ;
  window->loadSprite( 2, "sprites/16counter.png", 0, 32, 32, 16 ) ;

  window->loadSprite( 3, "sprites/Astos.png" ) ;
  window->loadSprite( 4, "sprites/Eye.png" ) ;
  window->loadSprite( 5, "sprites/Garland.png" ) ;
  window->loadSprite( 6, "sprites/Kary.png" ) ;
  window->loadSprite( 7, "sprites/Kraken.png" ) ;
  window->loadSprite( 8, "sprites/Lich.png" ) ;
  window->loadSprite( 9, "sprites/Phantom.png" ) ;
  window->loadSprite( 10, "sprites/Pirate.png" ) ;
  window->loadSprite( 11, "sprites/Tiamat.png" ) ;


  // Still like g?
  GDIPlusTexture *tex = window->createGDISurface( 128, 128 ) ;

  SolidBrush blueBrush( Color( 200, 0, 0, 255 ) ) ;

  tex->g->FillEllipse( &blueBrush, 0, 0, 40, 40 ) ;
  tex->g->FillEllipse( &blueBrush, 20, 20, 40, 40 ) ;

  window->addSpriteFromGDIPlusTexture( 21, tex ) ;
  
  delete tex ;



  SolidBrush redBrush( Color( 254, 255, 0,0 ) ) ;
  PointF origin( 0, 0 ) ;
  Font font( TEXT("Arial"), 24.0f, FontStyleBold ) ;

  tex = window->createGDISurface( 400, 40 ) ;
  /*
  tex->g->DrawString(
    TEXT( "Is that the best you can do???  CTRL!! Go on!" ),
    -1, &font, origin, &redBrush ) ;
    */
  RECT r ;
  SetRect( &r, 90, 8, 90, 8 ) ;
  tex->boxedText(
    TEXT("Is that the best you can do???  CTRL!! Go on!"),
    &font,
    r,
    Color( 254, 255, 0, 0 ),
    Color( 200, 0, 0, 255 ),
    StringAlignmentCenter
  ) ;
  window->addSpriteFromGDIPlusTexture( 22, tex ) ;

  delete tex ;


  GDIPlusTexture::D3DXCreateTextureFromFileViaGDIPlus( NULL, NULL ) ;

}




void Update()
{
  // update the game, happens 60 times a second
  
  if( window->justPressed( VK_F5 ) )
  {
    // Open up the log file in baretail
    system( "START baretail.exe lastRunLog.txt" ) ;
  }
  if( window->justPressed( VK_SPACE ) )
  {
    warning( "JUMP!" ) ;

    for( int i = 0 ; i < gameObjects.size(); i++ )
    {
      gameObjects[i].spriteIndex ++ ;
    }
  }
  if( window->isPressed( VK_CONTROL ) )
  {
    info( "adding a new guy!" ) ;

    GameObject go ;
    go.x = rand() % window->getWidth() ;
    go.y = rand() % window->getHeight() ;
    go.vx = randFloat( 3, 5 ) ;
    go.vy = randFloat() ;
    go.ax = randFloat( 10, 20 ) ;
    go.ay = randFloat( -10, 10 ) ;
    go.spriteIndex = window->randomSpriteId( 21 ) ;

    gameObjects.push_back( go ) ;
  }
  if( window->isPressed( VK_SHIFT ) )
  {
    // remove one
    if( !gameObjects.empty() )
      gameObjects.pop_back() ;
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
      gameObjects[i].x, gameObjects[i].y,
      32, 32 ) ;
  }

  

  window->drawMouseCursor( 2 ) ; // draw the mouse cursor with this sprite.
  
  char buf[ 100 ];
  int numSprites = gameObjects.size();
  sprintf( buf, "%d sprites", numSprites ) ;
  window->drawString( buf, 20, 20, 80, 40, Color::Yellow, DT_LEFT | DT_TOP ) ;

  if( numSprites == 0 )
  {
    // Draw this center screen
    window->drawString( "Hold CTRL down!  Go on!",
      0, 0, window->getWidth(), window->getHeight(),
      Color::Blue,
      DT_CENTER | DT_VCENTER ) ;
  }
  else if( !window->isSlow() )
  {
    // Draw this center screen
    /*
    window->drawString( "Is that the best you can do???  CTRL!! Go on!",
      0, 0, window->getWidth(), window->getHeight(),
      Color::Blue,
      DT_CENTER | DT_VCENTER ) ;
    */
    window->drawSprite( 22, window->getWidth()/2, window->getHeight()/2 ) ;
  }
  else
  {
    // Draw this center screen
    window->drawString( "OK now i'm struggling.\n"
      "Please take some away with SHIFT",
      0, 0, window->getWidth(), window->getHeight(),
      Color::Yellow,
      DT_CENTER | DT_VCENTER ) ;
  }
  
  
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
        /*
        printf("Kbd: "
          "VK=%04x key=%c "
          "MakeCode=%04x (key dp) "
          "Flags:%04x "
          "%s "
          "Reserved:%04x "
          "ExtraInformation:%08x "
          "msg=%04x "
          "WM_KEYDOWN=%d "
          "WM_KEYUP=%d\n", 
          raw->data.keyboard.VKey, raw->data.keyboard.VKey,

          raw->data.keyboard.MakeCode,  // key depression
          raw->data.keyboard.Flags,     // down or up
          raw->data.keyboard.Flags ? "up" : "down",
          raw->data.keyboard.Reserved,
          raw->data.keyboard.ExtraInformation,
          raw->data.keyboard.Message,
          raw->data.keyboard.Message == WM_KEYDOWN,
          raw->data.keyboard.Message == WM_KEYUP
        ) ;
        */

        switch( raw->data.keyboard.VKey )
        {
        case VK_ESCAPE :
          PostQuitMessage( 0 ) ;
          return 0 ;
        }
      }
      else if (raw->header.dwType == RIM_TYPEMOUSE)
      {
        /*
        printf("Mouse: "
          "usFlags=%04x "
          "ulButtons=%04x "
          "usButtonFlags=%04x "
          "usButtonData=%d "
          "ulRawButtons=%d "
          "lLastX=%d "
          "lLastY=%d \n",
          raw->data.mouse.usFlags, 
          raw->data.mouse.ulButtons, 
          raw->data.mouse.usButtonFlags, 
          raw->data.mouse.usButtonData, 
          raw->data.mouse.ulRawButtons, 
          raw->data.mouse.lLastX, 
          raw->data.mouse.lLastY
        );
        */

        
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
  consoleMove( 0, 500 ) ;
  consoleRowsAndCols( 10, 120 ) ;

  // Start up the log
  logStartup() ;

  // Start up GDI+, which we use to draw
  // For GDI+, used only for shape render
  GdiplusStartupInput gdiplusStartupInput;
  ULONG_PTR gdiplusToken;
  GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

  // Setup the window
  window = new Window( hInstance, TEXT( "Window title" ),
     0, 32, // x pos, y pos
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


