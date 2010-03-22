#include "../eternity/eternity.h"

// VV VV VV VV VV VV VV VV VV VV VV VV VV VV VV
// Your code starts below this line.

///////////////////////////////////////
// SETTING UP CALLBACKS EXAMPLE CODE //
///////////////////////////////////////
// Callbacks are how you can set code up
// to execute some time in the future.
//
// SCROLL DOWN UNTIL YOU SEE THE LINE THAT
// SAYS: "START HERE"!!
///////////////////////////////

#define ASSET(x) ("../assets/"##x)

GameWindow *window ;  // the main window object

#pragma region some enums and globals
enum Sprites
{
  Mario = 1,
  SixteenCounter
} ;

enum Sounds
{
  ColdArrow3
} ;

enum Fonts
{
  Arial24,
  TimesNewRoman24
} ;

////
// A couple of globals for this example
float counterX, counterY ;
bool counterShows = false ;
#pragma endregion

#pragma region the sample-callback-functions - skip on first reading
// This is a simple "zero-argument"
// function (it takes nothing between
// its brackets when you call it.)
void SayHello()
{
  info( "HI!!  I am the callback with no arguments.. that's me.\n"
    "I'm done executing now" ) ;
}

void setCounterVisibility( bool visible )
{
  info( "Setting counter visibility=%d", visible ) ;

  counterShows = visible ;

  warning( "ALL DONE!  Now click the left mouse button!!" ) ;
}

// this function takes 2 arguments..
void showCounter( float x, float y )
{
  info( "I am the two arg callback.. you should see a 16-counter now" ) ;

  // Reset the 16 counter sprite to start its
  // animation at 0
  window->getSprite( Sprites::SixteenCounter )->setFrame( 0 ) ;

  counterShows = true ; // reveal the counter

  // set up these variables
  counterX = x ;
  counterY = y ;



  // Last example, one-argument callback,
  // turn off the counter 5 seconds AFTER
  // the counter had started to show.
  // Yes, you can do this!

  // Set up a 1-arg callback, that accepts
  // one bool argument
  Callback1<bool> * callback ;

  // Set it to run 5.0 seconds FROM NOW,
  // the function that should run is "setCounterVisibility",
  // and the value of the single argument we pass it WHEN it runs is TRUE
  callback = new Callback1<bool>(
    5.0,
    setCounterVisibility,
    false
  ) ;

  // REGISTER IT SO IT WILL ACTUALLY RUN!
  // DON'T FORGET THIS STEP!
  info( "REGISTERING Final callback to make 16-counter disappear in 5 s.." ) ;
  window->addCallback( callback ) ;
}
#pragma endregion

void Init()
{
  #pragma region load assets
  // Load sounds
  window->loadSound( ColdArrow3, ASSET("sounds/ColdArrow3.wav") ) ;

  // sprite loading
  window->loadSprite( Mario, ASSET("sprites/mario.png") ) ;

  // Animated sprite
  window->loadSprite( SixteenCounter, ASSET("sprites/16counter.png"), 0, 32, 32, 16, 0.5f ) ;

  // Set the background clearing color to dk blue-gray
  window->setBackgroundColor( D3DCOLOR_ARGB( 255, 35, 35, 70 ) ) ;

  // Create a few fonts
  window->createFont( Fonts::Arial24, "Arial", 24, FW_NORMAL, false ) ;
  window->createFont( Fonts::TimesNewRoman24, "Times New Roman", 24, FW_BOLD, false ) ;
  #pragma endregion

  warning( "If this is your first time running this program,\n"
    "just watch it until the 16-counter appears then disappears,\n"
    "then start clicking the left-mouse button!" ) ;

  ///////////////////////////////////////
  // SETTING UP CALLBACKS EXAMPLE CODE //
  ///////////////////////////////////////
  //
  //          "START HERE"!!
  //
  // SOMETIMES you want to set up code
  // to execute after a certain number
  // of seconds in the future.

  // CALLBACKS are how you can do that.
  
  // Let's create a "callback" or two.
  // A "callback" function is just
  // one you will execute LATER.

  // We're setting up functions to
  // execute after (x) # seconds here,
  // so, these are callback functions then.

  // BE SURE TO PAY SPECIAL ATTENTION TO THIS SYNTAX!
  // 0 is for 0-arg callback, meaning
  // the callback function doesn't need
  // an function(parameters,between,the,brackets)
  // it just gets called like function().

  // The next few lines sets up
  // SAYHELLO to execute after
  // exactly 5 seconds, then AGAIN,
  // after 15 seconds.  WATCH THE TIMER!!
  Callback0 *c0 = new Callback0(

    5.0,
    SayHello
    
  ) ;

  info( "REGISTERING First callback to SAYHELLO(), runs after 5.0 s.." ) ;
  window->addCallback( c0 ) ;

  Callback0 *c1 = new Callback0( 15.0, SayHello ) ;

  info( "REGISTERING Second callback to SAYHELLO(), runs after 15.0 s.." ) ;
  window->addCallback( c1 ) ;

  

  // OK!! HERE IS WHERE IT GETS TRICKY!
  // Do you like <angle brackets>??  GOOD!!!
  // We're going to SET UP "showCounter" to
  // run after 20.0 seconds after this bit
  // of code runs

  // Hmm. Ok.
  
  // you will need to be "more human than human"
  // to understand this, good luck.

  // Notice that showCounter takes TWO
  // arguments:  and both are FLOAT!
  // So, that's what goes between the
  // angle brackets.  we're setting up
  // a callback for a function that
  // has 2 parameters (hence CALLBACK2),
  // and the first parameter is a float,
  // as well the second parameter is a float.
  Callback2<float,float> *c2 ;

  // Are you more human than human yet?

  // I certainly hope so.
  
  // Now we initialize c2:
  c2 = new Callback2<float,float>(
    
    20.0,  // how many seconds to wait
           // before running the callback
    
    showCounter, // the callback function to call..

    320.0f,      // the first argument to pass to
                 // showCounter WHEN showCounter
                 // finally gets called (in 5.0 seconds!)
                 // [ this will actually be the x-pos to put
                 //   the counter at ]

    240.0f       // the second arg to pass to
                 // showCounter when it runs
  ) ;

  // FINALLY, DON'T FORGET TO REGISTER YOUR CALLBACK
  // TO THE window object!!
  info( "REGISTERING Third callback to SHOWCOUNTER().. runs after 20.0s" ) ;
  window->addCallback( c2 ) ;

  warning( "WATCH THE CLOCK!!" ) ;
  


  // FINALLY, A COUPLE OF NOTES YOU MUST
  // PAY ATTENTION TO!!
  // 1 -> CALLBACKS __MUST HAVE RETURN TYPE VOID__
  //
  // 2 -> CALLBACKS CAN TAKE ARGUMENTS OF ANY TYPE
  //      BUT IF YOU PASS A CALLBACK A POINTER,
  //      BE SURE NOT TO __DELETE__ THE POINTER OUT
  //      FROM UNDER IT BEFORE THE CALLBACK HAS
  //      A CHANCE TO RUN!  http://www.youtube.com/watch?v=i49_SNt4yfk
  //
  // 3 -> CALLBACKS CAN TAKE UP TO 4 ARGS,
  //      BUT YOU CAN EASILY EXTEND IT TO
  //      5, 6, 7, 8, 9 args!  JUST OPEN
  //      AND FOLLOW THE PATTERN IN eternity/Callback.h,
  //      OR SUBMIT A FEATURE REQUEST!! :D.
  //
  // 4 -> THESE CALLBACKS CANNOT CALL MEMBER FUNCTIONS,
  //      THEY MUST CALL C-STYLE REGULAR FUNCTIONS.
  //      YOU CAN USE WHAT IS CALLED A "TRAMPOLINE",
  //      HOWEVER, AND PASS YOUR OBJECT INSTANCE...
  //      SEE THE TRAMPOLINE EXAMPLE BELOW.

  // Callbacks are very handy,
  // can be setup whenever,
  // to do whatever time-sensitive
  // task you wish!
}



#pragma region advanced trampoline example
// According to RULE #4 above, you can't
// pass a member function of an object
// as a CALLBACK.  You ALWAYS have to pass
// a C-STYLE function as your callback function.

// So how can we have a C++ MEMBER FUNCTION of
// an object execute as a callback?  EASY!!

// Just write a C-Function that accepts the
// object whose member function you need to execute,
// along with any parameters you want to send the object.
// The function then has a single line of code in it,
// that calls the object's desired member func,
// and passes the arguments to it.

// Actually this is how C++ member functions actually work,
// there's a hidden "this" pointer passed every time
// you invoke a member function of an object
// (See http://www.learncpp.com/cpp-tutorial/87-the-hidden-this-pointer/)
struct SecretNumberPrinter
{
  void printNumber( int number )
  {
    info( "Uh, the secret number is '%d'", number ) ;
  }
} ;


// This CALLBACK function calls the "printNumber()" function
// of "SecretNumberPrinter" when it ends up running..
// the trampoline function is how you can have
// a member function of an object execute
// when the callback occurs.
void TrampolineFunction( SecretNumberPrinter * snp, int arg1 )
{
  snp->printNumber( arg1 ) ;

  // Clean up after.. there's no way to reach
  // "object" now, after this function terminates!

  delete snp ;
}
#pragma endregion


void Update()
{
  // update the game, happens 60 times a second

  // Quit if the user presses ESCAPE
  if( window->keyJustPressed( VK_ESCAPE ) )
  {
    bail( "game ended!" ) ;
  }


  // Yet another callback.. 
  // This one shows how to make
  // a callback that calls a 
  // member function of an object..
  // you have to use a "trampoline" function
  // to make the call work.
  if( window->mouseJustPressed( Mouse::Left ) )
  {
    int delay = 1 + rand() % 5 ;
    int secretNum = rand()%500 ;

    info( "REGISTERING CALLBACK... The secret number is %d,\n"
      "This callback will execute in %d seconds", secretNum, delay ) ;
    Callback2<SecretNumberPrinter*, int> *callback ;

    SecretNumberPrinter *so = new SecretNumberPrinter() ;

    callback = new Callback2<SecretNumberPrinter*, int>(
      delay, // wait 'delay' full seconds..
      TrampolineFunction, //.. before calling TrampolineFunction..
      so, //.. with so as its first argument..
      secretNum // ..and a random number as its second argument!
    ) ;

    // REGISTER IT!
    window->addCallback( callback ) ;

  }

}


// DO ALL DRAWING HERE.
void Draw()
{
  // Draw the game, happens 60 times a second

  if( counterShows )
  {
    window->drawSprite( Sprites::SixteenCounter,
      counterX, counterY,
      640, 480 ) ;
  }

  // draw the mouse cursor with this sprite.
  window->drawMouseCursor( Mario ) ;

  window->drawFrameCounter(); // erase this if you don't like
  // the frame counter in the top right corner

  window->drawTimer() ;

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

