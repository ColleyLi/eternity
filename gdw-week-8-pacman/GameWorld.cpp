#include "GameWorld.h"

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

  gameState = GameState::Menu ;
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
  | 1.  Menu    -> Running   : Load level 1
  | 2.  
  */

  // 1.  MENU -> RUNNING
  if( gameState == GameState::Menu &&
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
  // pause    RUNNING -> PAUSED
  else if( gameState == GameState::Running &&
           newState == GameState::Paused )
  {
    window->pause() ; // Pause the engine as well
    // This halts animations.

    info( "Paused..." ) ;
  }
  // unpause  PAUSED -> RUNNING
  else if( gameState == GameState::Paused &&
           newState == GameState::Running )
  {
    window->unpause() ;
    info( "unpaused" ) ;
  }
  // user died.  RUNNING -> GAMEOVER
  else if( gameState == GameState::Running &&
           newState == GameState::GameOver )
  {
    
  }
  else
  {
    warning( "ILLEGAL STATE TRANSITION FROM %d TO %d.  "
      "Transitioning anyway, but you should know that "
      "was just illegal according to your own rules.", gameState, newState ) ;
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

        pacman = new Player() ;

        // Create Pacman at this point.
        pacman->pos.x = xBoardOffset + col * tileSize ;
        pacman->pos.y = yBoardOffset + row * tileSize ;

        pacman->spriteId = Sprites::Pacman ;
        
        
        break;

      case Tiles::Bonus:
        tile->setSpriteId( Sprites::Bonus ) ;
        break;

      case Tiles::Pellet:
        tile->setSpriteId( Sprites::Pellet ) ;
        break;

      case Tiles::PowerPellet:
        tile->setSpriteId( Sprites::Powerpellet ) ;
        break;

      case Tiles::Wall:
        tile->setSpriteId( Sprites::Wall ) ;
        break;

      case Tiles::Empty:
        tile->setSpriteId( Sprites::Empty ) ;
        break;

      case Tiles::Barrier:
        tile->setSpriteId( Sprites::Barrier ) ;
        break;

      case Tiles::Inky:
      case Tiles::Blinky:
      case Tiles::Pinky:
      case Tiles::Sue:
        {
          tile->setSpriteId( Sprites::Empty ) ;

          Ghost * ghost = new Ghost() ;
          ghost->name = mapChar ;

          ghost->pos.x = xBoardOffset + col * tileSize ;
          ghost->pos.y = yBoardOffset + row * tileSize ;

          ghost->spriteId = Sprites::Inky ;

          ghosts.push_back( ghost ) ;
        }
        break;

      default:
        // Just make it mario if there's
        // some sprite that isn't accounted
        // for here yet.  Mario will remind us
        // to make a sprite for it
        tile->setSpriteId( Sprites::Mario ) ;
        break;
      }
      
      level[ row ][ col ] = tile ;

      // Now if the mapChar was one of
      // the NPC ones, we need to add
      // a GameObject to the OBJECTS array

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



