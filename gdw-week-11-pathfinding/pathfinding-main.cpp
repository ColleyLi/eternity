#include "../eternity/eternity.h"

///////////////////////////////
// PATHFINDING EXAMPLE

// (Tilesets from http://rivendell.fortunecity.com/goddess/268/tilesets.html)

// USE WASD and the ARROW KEYS
// to move the start and goal node
// around respectively.
// Press '4' to have 4-way grid pathfinding
// or '8' to have 8-way grid pathfinding

// How did I get "smooth noise"?  Why
// Dr. Perlin, of course!! :D  Check
// out the fillRandom function
// in the AsciiMap class!

#pragma region endless banter about release builds
// BE SURE TO COMPILE IN RELEASE MODE!!!
// -------------------------------------
// You'll notice a performance boost all around
// your application when you compile it in
// RELEASE MODE.

// Release mode is nice because the C++ compiler
// does some neato optimizations that make
// all your code run faster.

// Sometimes you can't notice, but with
// the pathfinding code, it really makes
// a HUGE difference.  Pathfinding can get extremely
// expensive and it performs _MUCH_ better
// with the Release mode optimizations turned on.

// The way you do RELEASE build in Visual Studio,
// is by selecting the DROP DOWN in the toolbar,
// AT THE TOP OF THE SCREEN,
// (normally it says "Debug", right next
// to this elusive dropdown is Green compile/run Arrow Button

// Choose RELEASE from that drop down, instead
// of DEBUG.

// Now that you have chosen a RELEASE build,
// you have to RIGHT CLICK YOUR PROJECT,
// GO TO PROPERTIES, left-hand pane:
// Configuration Properties / General.
// 
// Notice in that dialog the Output Directory and Intermediate
// Directory settings ARE ALL WRONG!!  They are
// NOT set to $(SolutionDir)\Build anymore!
// How did it get undone???
// The DEBUG and RELEASE
// configurations are actually COMPLETELY different.

// So, you have to re-configure it..
// Use the following values for Output Directory, Intermediate directory,
// and Build Log File:

//Output Directory:        $(SolutionDir)\Release
//Intermediate Directory:  $(SolutionDir)\Release\Files
//Build Log File:          $(SolutionDir)\Release\Files\BuildLog-$(InputName).htm

// The setup is similar to how you first set up
// your project, except instead of
// using $(SolutionDir)\Build, you are
// now using $(SolutionDir)\Release.

// NOTE THAT JUST USING THE "Release" DIRECTORY
// __DOES NOT MAKE IT A RELEASE BUILD__!! What
// makes it a RELEASE build is that you have
// "RELEASE" selected in the drop down menu
// at the screen (in the Visual Studio toolbar).
#pragma endregion

#define ASSET(x) ("../assets/"##x)

GameWindow *window ; 

#pragma region some enums
enum Sprites
{
  Mario = 1,
  SixteenCounter,
  Grass,
  GrassBlueFlowers,
  GrassBumpy,
  GrassCobblestone,
  GrassHole,
  GrassMailbox,
  GrassPurpleFlowers,
  GrassRocks,
  GrassTombstone,
  Trees1,
  Trees2,
  Trees3,
  Water,
  WaterDeep,
  WaterPuddle,
  OnPath,
  StartNode,
  EndNode
} ;

enum Sounds
{
  HumanMusic = 1,
  TreeWhat
} ;

enum Fonts
{
  Arial12,
  Arial18,
  Arial24,
  TimesNewRoman24,
} ;
#pragma endregion

#pragma region definitions of the tiles
/// You should replace
/// these definitions of what each
/// character in the map means
/// with your own definitions..
struct Tiles
{
  const static char Grass              = '.' ;
  const static char GrassBlueFlowers   = 'f' ;
  const static char GrassBumpy         = '`' ;
  const static char GrassCobblestone   = ',' ;
  const static char GrassHole          = 'o' ;
  const static char GrassMailbox       = 'M' ;
  const static char GrassPurpleFlowers = 'p' ;
  const static char GrassRocks         = '-' ;
  const static char GrassTombstone     = 'T' ;
  const static char Trees1             = '1' ;
  const static char Trees2             = '2' ;
  const static char Trees3             = '3' ;
  const static char Water              = 'w' ;
  const static char WaterDeep          = 'W' ;
  const static char WaterPuddle        = 'u' ;
} ;
#pragma endregion

#pragma region handy-dandy globals for this example

/// A convenient way to map tiles to sprites
map<char, Sprites> tileToSpriteMapping ;

/// A COPY of the cost map that
/// the pathfinder will use for
/// determining optimal paths
CostMap tileCostMapping ;

// Of course, the representation of the
// game world map in ASCII characters...
AsciiMap *asciiMap ; // this will be filled
// with random values at program start (in the
// Init() function).  Could just as easily
// be loaded from file though.

// Set up globals for the start, end pos's
Coord start ; // the start pos is the beginning of the map
Coord end ;   // the end position
DequeCoord solutionPath ; // stores the solution to the A* path-find

// ???????????????????????????????????
// ?????WTF IS A DEQUE, YOU ASKUE?????
// ???????????????????????????????????

// Well sir, a <DEQUE> is short for
// "double-ended queue".

// You know how an STL <vector> is a
// list of objects, and you can
// use vector.push_back( "item" )
// and "item" will be added to the
// back of the list of things?

// WELL a <DEQUE> is just.. a two-sided
// <vector>, kind of.  The only difference
// really is you can call:
//   deque.push_front( "item" )   add an item to the front of the deque
//   deque.push_back( "item" )    add an item to the back of the deque
//   deque.pop_front()            remove item at front of deque
//   deque.pop_back()             remove item at back of deque
// 
// Its really that simple!  A deque
// is a flexible and useful data
// structure.  Pathfinding uses it
// because when you path-find you
// actually need to BACKTRACK from
// the end node to the start node,
// following your poo-trail of shortest-pathess-ness,
// so in the AStar::solve function, you'll
// see a bunch of push_front()'s..
// because we're walking backwards
// from the goal-node to the start node.
// Pathfinding is an advanced subject
// so please don't be pissed if that
// just confused you.

GridConnection gridConnection ;
#pragma endregion



void Init()
{
  #pragma region asset loading
  // Load sounds
  window->loadSound( HumanMusic, ASSET("sounds/Human1.mp3"), FMOD_CREATESTREAM ) ;
  window->loadSound( TreeWhat, ASSET("sounds/What2.wav") ) ;

  // sprite loading
  window->loadSprite( Mario, ASSET("sprites/mario.png") ) ;

  // Animated sprite
  window->loadSprite( SixteenCounter, ASSET("sprites/16counter.png"), 0, 32, 32, 16, 0.5f ) ;
  window->loadSprite( Grass, ASSET("sprites/pathfinding-example/grass.png" ) ) ;
  window->loadSprite( GrassBlueFlowers, ASSET("sprites/pathfinding-example/grass-blue-flowers.png" ) ) ;
  window->loadSprite( GrassBumpy, ASSET("sprites/pathfinding-example/grass-bumpy.png" ) ) ;
  window->loadSprite( GrassCobblestone, ASSET("sprites/pathfinding-example/grass-cobblestone.png" ) ) ;
  window->loadSprite( GrassHole, ASSET("sprites/pathfinding-example/grass-hole.png" ) ) ;
  window->loadSprite( GrassMailbox, ASSET("sprites/pathfinding-example/grass-mailbox.png" ) ) ;
  window->loadSprite( GrassPurpleFlowers, ASSET("sprites/pathfinding-example/grass-purple-flowers.png" ) ) ;
  window->loadSprite( GrassRocks, ASSET("sprites/pathfinding-example/grass-rocks.png" ) ) ;
  window->loadSprite( GrassTombstone, ASSET("sprites/pathfinding-example/grass-tombstone.png" ) ) ;
  window->loadSprite( Trees1, ASSET("sprites/pathfinding-example/trees-1.png" ) ) ;
  window->loadSprite( Trees2, ASSET("sprites/pathfinding-example/trees-2.png" ) ) ;
  window->loadSprite( Trees3, ASSET("sprites/pathfinding-example/trees-3.png" ) ) ;
  window->loadSprite( Water, ASSET("sprites/pathfinding-example/water.png" ) ) ;
  window->loadSprite( WaterDeep, ASSET("sprites/pathfinding-example/water-deep.png" ) ) ;
  window->loadSprite( WaterPuddle, ASSET("sprites/pathfinding-example/water-puddle.png" ) ) ;
  window->loadSprite( OnPath, ASSET("sprites/pathfinding-example/on-path.png" ) ) ;
  window->loadSprite( StartNode, ASSET("sprites/pathfinding-example/start-node.png" ) ) ;
  window->loadSprite( EndNode, ASSET("sprites/pathfinding-example/end-node.png" ) ) ;

  // fill out the mapping of tiles to sprites
  tileToSpriteMapping[ Tiles::Grass ] = Sprites::Grass ;
  tileToSpriteMapping[ Tiles::GrassBlueFlowers ] = Sprites::GrassBlueFlowers ;
  tileToSpriteMapping[ Tiles::GrassBumpy ] = Sprites::GrassBumpy ;
  tileToSpriteMapping[ Tiles::GrassCobblestone ] = Sprites::GrassCobblestone ;
  tileToSpriteMapping[ Tiles::GrassHole ] = Sprites::GrassHole ;
  tileToSpriteMapping[ Tiles::GrassMailbox ] = Sprites::GrassMailbox ;
  tileToSpriteMapping[ Tiles::GrassPurpleFlowers ] = Sprites::GrassPurpleFlowers ;
  tileToSpriteMapping[ Tiles::GrassRocks ] = Sprites::GrassRocks ;
  tileToSpriteMapping[ Tiles::GrassTombstone ] = Sprites::GrassTombstone ;
  tileToSpriteMapping[ Tiles::Trees1 ] = Sprites::Trees1 ;
  tileToSpriteMapping[ Tiles::Trees2 ] = Sprites::Trees2 ;
  tileToSpriteMapping[ Tiles::Trees3 ] = Sprites::Trees3 ;
  tileToSpriteMapping[ Tiles::Water ] = Sprites::Water ;
  tileToSpriteMapping[ Tiles::WaterDeep ] = Sprites::WaterDeep ;
  tileToSpriteMapping[ Tiles::WaterPuddle ] = Sprites::WaterPuddle ;

  // Create a few fonts
  window->createFont( Fonts::Arial12, "Arial", 12, 500, false ) ;
  window->createFont( Fonts::Arial18, "Arial", 18, 500, false ) ;
  window->createFont( Fonts::Arial24, "Arial", 24, FW_NORMAL, false ) ;
  window->createFont( Fonts::TimesNewRoman24, "Times New Roman", 24, FW_BOLD, true ) ;
  #pragma endregion

  #pragma region create the map
  //////////////////
  // SET UP THE MAP
  // Create a 30x30 map.
  // You could have already loaded
  // this map from a file, an example
  // of that is in the Pacman project now.
  asciiMap = new AsciiMap( 16, 20 ) ;

  // SET UP TILE COSTS
  // We must tell the ASCII MAP
  // the COST of travelling onto each
  // tile.  It uses this to determine
  // the best path from A to B.
  // If you assign a tile the special cost IMPASSIBLE,
  // then the pathfinder will NEVER travel
  // over that tile!
  asciiMap->setTileCost( Tiles::Grass, 1.0f ) ;
  asciiMap->setTileCost( Tiles::GrassBlueFlowers, 1.5f ) ;
  asciiMap->setTileCost( Tiles::GrassBumpy, 1.5f ) ;
  asciiMap->setTileCost( Tiles::GrassCobblestone, 0.8f ) ; // you like cobblestone
  asciiMap->setTileCost( Tiles::GrassHole, 10.0f ) ;
  asciiMap->setTileCost( Tiles::GrassMailbox, 5.0f ) ; // because you stop to check it
  asciiMap->setTileCost( Tiles::GrassPurpleFlowers, 1.5f ) ;
  asciiMap->setTileCost( Tiles::GrassRocks, 5.0f ) ; // they trip you up 
  asciiMap->setTileCost( Tiles::GrassTombstone, IMPASSIBLE ) ;  // too big, so you can't walk on it
  asciiMap->setTileCost( Tiles::Trees1, 2.0f ) ;  // one tree, not hard to get around
  asciiMap->setTileCost( Tiles::Trees2, 2.0f ) ; 
  asciiMap->setTileCost( Tiles::Trees3, 3.0f ) ; 
  asciiMap->setTileCost( Tiles::Water, 11.0f ) ; 
  asciiMap->setTileCost( Tiles::WaterDeep, 14.0f ) ; 
  asciiMap->setTileCost( Tiles::WaterPuddle, 8.0f ) ; 


  // See fillRandom() function
  // for details on how this is done
  asciiMap->fillRandom( time(0) ) ; // time(0) says
  // to seed with program START TIME,
  // which is essentially, random, since
  // every time you start the program,
  // its at a "random" and different time,
  // than all the other times you started it..
  




  // Set up the start and end pos's
  start.row = randInt( 0, asciiMap->getRows() ) ;
  start.col = randInt( 0, asciiMap->getCols() ) ;
  
  // random end 
  end.row = randInt( 0, asciiMap->getRows() ) ;
  end.col = randInt( 0, asciiMap->getCols() ) ;


  // Retrieve the cost map.
  // NOTE HOW WE ONLY NEED TO DO THIS
  // ONCE, AT PROGRAM START, NOT IN THE
  // MAIN LOOP!
  tileCostMapping = asciiMap->getCostMapCopy() ;

  #pragma endregion

}

void Update()
{
  // update the game, happens 60 times a second

  // Quit if the user presses ESCAPE
  if( window->keyJustPressed( VK_ESCAPE ) )
  {
    bail( "game ended!" ) ;
  }


  ////////
  // SOLVE THE PATHFINDING PROBLEM:
  // ------------------------------
  // UPDATE THE SOLUTION by re-solving
  // the map.  solutionPath is a global
  // variable so it will be accessible to
  // the Draw() function when it comes time
  // to draw out the solution.
  solutionPath = asciiMap->solve( start, end ) ;
  // solutionPath is now a series
  // of Coords describing the shortest path
  // from start to end.






  if( window->keyJustPressed( '4' ) )
  {
    gridConnection = GridConnection::FourWay ;
    asciiMap->setGridConnection( gridConnection ) ;
  }
  if( window->keyJustPressed( '8' ) )
  {
    gridConnection = GridConnection::EightWay ;
    asciiMap->setGridConnection( gridConnection ) ;
  }

  if( window->keyJustPressed( VK_RIGHT ) )
    end.col++ ;
  if( window->keyJustPressed( VK_LEFT ) )
    end.col--;
  if( window->keyJustPressed( VK_UP ) )
    end.row--;
  if( window->keyJustPressed( VK_DOWN ) )
    end.row++ ;

  if( window->keyJustPressed( 'W' ) )
    start.row--;
  if( window->keyJustPressed( 'A' ) )
    start.col--;
  if( window->keyJustPressed( 'S' ) )
    start.row++;
  if( window->keyJustPressed( 'D' ) )
    start.col++ ;

  if( window->keyJustPressed( 'R' ) )
  {
    int seed = time( 0 ) ;
    info( "Re-seeding the map generator '%d'", seed ) ;
    asciiMap->fillRandom( seed ) ;
  }

  // Make sure don't step out of bounds
  start.clampBetween( 0, asciiMap->getRows()-1, 0, asciiMap->getCols()-1 ) ;
  end.clampBetween( 0, asciiMap->getRows()-1, 0, asciiMap->getCols()-1 ) ;


  
}

// Draws the legend
// with the cost maps out for you
void drawLegend()
{
  float x = 700 ; // left edge x
  float y = 60 ;  // starting y

  window->drawString( Fonts::Arial18,
    "Tile cross costs",
    Color::White,
    x - 20, y, 120, 32, DT_LEFT | DT_TOP ) ;
  y += 34 ;
  // iterate through the cost map
  foreach( CostMapIter, iter, tileCostMapping )
  {
    char symbol = iter->first ;
    float cost = iter->second ;

    Sprites sprite = tileToSpriteMapping[ symbol ] ;

    // draw the sprite and its cost next to it
    window->drawSprite( sprite, x, y ) ;
    

    // Print the symbol and cost into the string
    static char buf[ 80 ] ;

    // Special case for impassible tiles, their cost is infinite
    if( cost == IMPASSIBLE )
      sprintf( buf, "infinite" ) ;
    else
      sprintf( buf, "%.2f", cost ) ;
    
    window->drawString( Fonts::Arial18,
      buf, Color::White,
      x + 32, y - 10, 200, 400, DT_LEFT | DT_TOP ) ;

    y+= 34 ; // increase y for next time around

  }
}

void drawInstructions()
{
  char *instructions = "(WASD) move start / "
    "(<-, ->) move end / "
    "(R) new map / "
    "(4) 4-way path / "
    "(8) 8-way path\n"
    "Path won't pass thru tombstones\n"
    "(they are \"impassible\"!)";

  window->drawString(
    Fonts::Arial12,
    instructions,
    Color::Yellow,
    0, 550,
    window->getWidth(), 32
  ) ;
}

// DO ALL DRAWING HERE.
void Draw()
{
  // Draw the game, happens 60 times a second


  // Draw the map.
  #pragma region drawing the map and solution path out
  // note x,y position has relationship with 
  // map row and col
  float tileSize = 32.0f ; // every tile is 32 px x 32 px

  // Offsets the whole map by this much:
  float offsetX = tileSize / 2 ;
  float offsetY = tileSize / 2 ;

  float x,y ; // x, y position to DRAW AT

  for( int row = 0 ; row < asciiMap->getRows() ; row++ )
  {
    for( int col = 0 ; col < asciiMap->getCols() ; col++ )
    {
      // COMPUTE xPosition, yPosition
      // to DRAW AT
      x = offsetX + (col * tileSize) ;
      y = offsetY + (row * tileSize) ;

      // Retrieve the ascii character used
      // at this position in the map.
      char mapChar = (*asciiMap)[row][col] ;

      // Now look up what Sprite to use to
      // draw that character.  We could
      // have a big ol' switch here, but
      // using a map<char,Sprites>is __loads__
      // better!
      Sprites spriteToUse = tileToSpriteMapping[ mapChar ] ; // quick lookup
      window->drawSprite( spriteToUse, x, y ) ;
    }
  }

  // Draw the solution by overlaying boxes
  foreach( DequeCoordIter, iter, solutionPath )
  {
    x = offsetX + (iter->col * tileSize) ;
    y = offsetY + (iter->row * tileSize) ;
    window->drawSprite( Sprites::OnPath, x, y ) ;
  }

  // Draw the startPos, endPos.  These are
  // drawn after the solution because
  // startPos and endPos are nodes
  // that overlap the solution path
  x = offsetX + (start.col * tileSize) ;
  y = offsetY + (start.row * tileSize) ;
  window->drawSprite( Sprites::StartNode, x, y ) ;
  
  x = offsetX + (end.col * tileSize) ;
  y = offsetY + (end.row * tileSize) ;
  window->drawSprite( Sprites::EndNode, x, y ) ;
  #pragma endregion

  #pragma region draw mode and other info
  // Tell what mode we're in
  char *dirStr = "4-way grid" ; //assume its 4-way..
  if( gridConnection == GridConnection::EightWay )
  {
    // ..but change it if its 8-way
    dirStr = "8-way grid" ; 
  }
  // Draw dirStr
  window->drawBoxCentered(
    D3DCOLOR_ARGB( 140, 20, 0, 130 ),
    window->getWidth()/2, 32, 300, 32 ) ; 
  window->drawString( Fonts::Arial24, dirStr, Color::White,
                      0, 18, window->getWidth(), 28 ) ;

  drawLegend() ;


  drawInstructions() ;
  #pragma endregion


  // See performance
  char buf[128];
  sprintf( buf, "Path found in %.6f seconds", asciiMap->getLastSolutionTime() ) ;

  window->drawString( Fonts::Arial12, buf, Color::White,
    0, 580, 100, 32, DT_LEFT ) ;



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
  consoleMove( 32, 620 ) ;
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
     800, 600 // width, height
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

