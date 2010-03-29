#include "GameWorld.h"
#include "CallbackFunctions.h"

// Here is where we create our
// single GameWorld instance.
// This variable is related to
// the "extern" declaration
// in GameWorld.h.  "extern GameWorld *game"
// was the "variable prototype" and this
// is the "variable declaration".
GameWorld *game ;

GameWorld::GameWorld()
{
  xBoardOffset = 50 ;
  yBoardOffset = 22 ;

  xStatsOffset = 400 ;
  yStatsOffset = 20 ;

  // Start at the splash screen
  gameState = GameState::Splash ;

  // Start the annoying intro music
  window->playSound( Sounds::Intro ) ; // Loop this sound forever,
  // or until we stop it using window->stopSound()

  // Set up a callback to call
  // the transitionToState callback
  // after 14.1 seconds
  window->addCallback(
    new Callback1<GameState>(
      14.1,
      transitionToState,
      GameState::Menu )
  ) ;
}

GameWorld::GameState GameWorld::getState()
{
  return gameState ;
}
void GameWorld::setState( GameWorld::GameState newState )
{
  // Now when the state transitions
  // I'm enumerating each to make sure
  // I've taken care of each one in the code.

  /*
  +-------------+-----------+------------+
  | FROM STATE  | TO STATE  |  ACTIONS   |
  +-------------+-----------+------------+
  | 1.  Splash  -> Menu
  | 2.  Menu    -> Running  : Load level 1
  | 3.  Running -> Paused
  | 4.  Paused  -> Running
  | 5.  Running -> GameOver
  */

  // 1.  Splash -> Menu
  if( gameState == GameState::Splash &&
      newState == GameState::Menu )
  {
    // Stop the intro music
    window->stopSound( Sounds::Intro ) ;

    // Start up the title theme music
    window->loopSound( Sounds::PacmanTitleThemeLoop ) ;
  }

  // 2.  MENU -> RUNNING
  else if( gameState == GameState::Menu &&
           newState == GameState::Running )
  {
    // Here's the code we want to run
    // when it is time to start the game
    // from the menu.

    // Stop the title theme music
    window->stopSound( Sounds::PacmanTitleThemeLoop ) ;

    // Start up the game play music
    window->playSound( Sounds::PacmanGamePlayMusic ) ;

    // Load "lvl1.txt"
    loadLevel( "lvl1.txt" ) ;

    // Change the background color to something else
    window->setBackgroundColor( D3DCOLOR_ARGB( 255, 0, 0, 64 ) ) ;
  }
  // 3. pause    RUNNING -> PAUSED
  else if( gameState == GameState::Running &&
           newState == GameState::Paused )
  {
    window->pause() ; // Pause the engine as well
    // This halts animations.

    info( "Paused..." ) ;
  }
  // 4. unpause  PAUSED -> RUNNING
  else if( gameState == GameState::Paused &&
           newState == GameState::Running )
  {
    window->unpause() ;

    info( "unpaused" ) ;
  }
  // 5. player died.  RUNNING -> GAMEOVER
  else if( gameState == GameState::Running &&
           newState == GameState::GameOver )
  {
    
  }
  else
  {
    warning( "ILLEGAL STATE TRANSITION FROM %d TO %d.  "
      "Not doing the transition, you should know that "
      "an illegal transition was attempted according to your own rules. "
      "Most likely it was a callback trying to transition "
      "too late.", gameState, newState ) ;

    return ;
  }
  gameState = newState ;
}


void GameWorld::loadLevel( char *filename )
{
  // Parse-out the level text from the filename
  
  // Because you are C++ HACKERS
  // I'm using ifstream here.

  // Open the file in read mode.
  ifstream in( filename ) ;

  for( int row = 0 ; row < mapRows ; row++ )
  {
    for( int col = 0 ; col < mapCols ; col++ )
    {
      char mapChar = in.get() ;
      
      // Set up the tile according
      // to the character loaded.
      // To avoid cluttering the Tile
      // class with level construction information,
      // I've chosen to put the SWITCH HERE
      // and _not_ in the Tile object.
      
      Tile *tile = new Tile();
      tile->setTile( mapChar ) ;
      
      switch( mapChar )
      {
      
        // put nothing in the tiles map..
      case Tiles::PacmanStart:
        // Pacman is not a tile...!!
        //tile->setSpriteId( Sprites::Pacman ) ;

        // He's a GameObject!
        // The difference between
        // Tiles and GameObjects is Tiles don't move.
        tile->setSpriteId( Sprites::Empty ) ;
        tile->setPassable( true ) ;

        pacman = new Player() ;

        // Create Pacman at this point.
        pacman->pos.x = col * tileSize ;
        pacman->pos.y = row * tileSize ;

        pacman->spriteId = Sprites::Pacman ;
        
        
        break;

      case Tiles::Bonus:
        tile->setSpriteId( Sprites::Bonus ) ;
        tile->setPassable( true ) ;
        break;

      case Tiles::Pellet:
        tile->setSpriteId( Sprites::Pellet ) ;
        tile->setPassable( true ) ;
        break;

      case Tiles::PowerPellet:
        tile->setSpriteId( Sprites::Powerpellet ) ;
        tile->setPassable( true ) ;
        break;

      case Tiles::Wall:
        tile->setSpriteId( Sprites::Wall ) ;
        tile->setPassable( false ) ;
        break;

      case Tiles::Empty:
        tile->setSpriteId( Sprites::Empty ) ;
        tile->setPassable( true ) ;
        break;

      case Tiles::Barrier:
        tile->setSpriteId( Sprites::Barrier ) ;
        tile->setPassable( true ) ;
        break;

      case Tiles::Inky:
      case Tiles::Blinky:
      case Tiles::Pinky:
      case Tiles::Sue:
        {
          // ghosts aren't tiles either
          // so set these tiles to empty tiles... THEN...
          tile->setSpriteId( Sprites::Empty ) ;
          tile->setPassable( true ) ;

          // ...THEN create a ghost object
          // in the place where this tile was
          Ghost * ghost = new Ghost() ;
          ghost->name = mapChar ;

          ghost->pos.x = col * tileSize ;
          ghost->pos.y = row * tileSize ;

          // (( eyes are overlaid on top of
          //    the GhostBody sprites ))
          ghost->spriteId = Sprites::GhostBody ;

          ghosts.push_back( ghost ) ;
        }
        break;

      default:
        // Just make it mario if there's
        // some sprite that isn't accounted
        // for here yet.  Mario will remind us
        // to make a sprite for it
        tile->setSpriteId( Sprites::Mario ) ;
        tile->setPassable( true ) ;
        break;
      }
      
      level[ row ][ col ] = tile ;

    }

    // The newline will be the 21st character
    // on each line, so we can discard it
    char newLine = in.get() ;
    if( !in.eof() && newLine != '\n' )
    {
      // Whoops!  The person must
      // have fed in a wrong file
      warning( "Line lengths are not %d characters...", mapCols ) ;
    }
  }

  in.close() ;
}

void GameWorld::step( float time )
{
  pacman->step( time ) ;

  foreach( vector<Ghost*>::iterator, ghost, ghosts )
  {
    (*ghost)->step( time ) ;
  }


  // Cause all object intersections to "happen"

}

void GameWorld::drawBoard()
{
  // Draw each tile
  for( int row = 0 ; row < mapRows; row++ )
  {
    for( int col = 0 ; col < mapCols ; col++ )
    {
      // Retrieve a pointer to the tile.
      // Notice this isn't a wasteful copy
      // because its only a pointer assignment.
      Tile *currentTile = level[ row ][ col ] ;

      // the X, Y position of each tile
      // depends on the row, col it is in
      // in the "level" array AS WELL AS
      // TileSize.  But notice the currentTile
      // object doesn't really know the x,y position.
      float x = xBoardOffset+ col * tileSize ;
      float y = yBoardOffset+ row * tileSize ;

      // Draw all tiles
      window->drawSprite( currentTile->getSpriteId(), x, y ) ;
    }
  }
}

void GameWorld::drawStats()
{
  window->drawString( Fonts::PacFont24, "Lives", Color::Yellow,
                      xStatsOffset, yStatsOffset, 200, 200,
                      DT_TOP | DT_LEFT ) ; // slam text to top left
  
  // Draw lives remaining.
  int startX = xStatsOffset + 20 ;
  for( int i = 0 ; i < pacman->getLives() ; i++ )
  {
    window->drawSprite( Sprites::Pacman, startX, 60 ) ;
    startX += 20 ;
  }

  char scoreBuf[ 60 ] ;
  sprintf( scoreBuf, "%d", pacman->getScore() ) ;
  window->drawString( Fonts::PacFont24, "Score", Color::Yellow,
                      xStatsOffset, yStatsOffset + 60, 200, 200,
                      DT_TOP | DT_LEFT ) ;

  startX = xStatsOffset + 20 ;
  window->drawString( Fonts::Arial24, scoreBuf, Color::White,
                      startX, yStatsOffset + 80, 200, 200,
                      DT_TOP | DT_LEFT ) ;
}

void GameWorld::drawPeople()
{
  // draw the items
  
  // draw the ghosts
  foreach( vector<Ghost*>::iterator, ghost, ghosts )
  {
    (*ghost)->draw();
  }

  // draw the player
  pacman->draw();

}

// private
void GameWorld::checkCoords( int & row, int & col )
{
  if( col < 0 )
  {
    warning( "Column index was negative, reset to 0" ) ;
    col = 0 ;
  }
  else if( col >= mapCols )
  {
    warning( "Column index out of bounds: %d too large!", col ) ;
    col = mapCols - 1 ;
  }
  if( row < 0 )
  {
    warning( "Row index was negative, reset to 0" ) ;
    row = 0 ;
  }
  if( row >= mapRows )
  {
    warning( "Row index out of bounds: %d too large!", row );
    row = mapRows - 1 ;
  }

}

Tile* GameWorld::getTileNearestLeft( Vector2 & pos )
{
  int col = floor( pos.x/tileSize ) ;
  int row = round( pos.y/tileSize ) ;

  checkCoords( row, col ) ;
  return level[ row ][ col ] ;
}

Tile* GameWorld::getTileNearestRight( Vector2 & pos )
{
  int col = ceil( pos.x/tileSize ) ;
  int row = round( pos.y/tileSize ) ;

  checkCoords( row, col ) ;
  return level[ row ][ col ] ;
}

Tile* GameWorld::getTileNearestUp( Vector2 & pos )
{
  int col = round( pos.x/tileSize ) ;
  int row = floor( pos.y/tileSize ) ;

  checkCoords( row, col ) ;
  return level[ row ][ col ] ;
}

Tile* GameWorld::getTileNearestDown( Vector2 & pos )
{
  int col = round( pos.x/tileSize ) ;
  int row = ceil( pos.y/tileSize ) ;  // particularly unintuitive..
  // but y _increases_ going down

  checkCoords( row, col ) ;
  return level[ row ][ col ] ;
}

Tile* GameWorld::getTileAt( Vector2 & pos )
{
  int col = round( pos.x/tileSize ) ; 
  int row = round( pos.y/tileSize ) ;

  checkCoords( row, col ) ;
  return level[ row ][ col ] ;
}

int GameWorld::getBoardOffsetX()
{
  return xBoardOffset ;
}

int GameWorld::getBoardOffsetY()
{
  return yBoardOffset ;
}

Player* GameWorld::getPlayer()
{
  return pacman ;
}
