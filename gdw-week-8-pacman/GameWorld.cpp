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
  | 6.  GameOver-> Menu
  */

  // 1.  Splash -> Menu
  if( gameState == GameState::Splash &&
      newState == GameState::Menu )
  {
    // Stop the intro music
    window->stopSound( Sounds::Intro ) ;

    // Start up the title theme music
    window->loopSound( Sounds::PacmanTitleThemeLoop ) ;

    window->setBackgroundColor( Color::Black ) ;
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

    window->setBackgroundColor( D3DCOLOR_ARGB( 255, 64, 0, 64 ) ) ;

    info( "Paused..." ) ;
  }
  // 4. unpause  PAUSED -> RUNNING
  else if( gameState == GameState::Paused &&
           newState == GameState::Running )
  {
    window->unpause() ;

    window->setBackgroundColor( D3DCOLOR_ARGB( 255, 0, 0, 64 ) ) ;

    info( "unpaused" ) ;
  }
  // 5. player died.  RUNNING -> GAMEOVER
  else if( gameState == GameState::Running &&
           newState == GameState::GameOver )
  {
    window->stopSound( Sounds::PacmanGamePlayMusic ) ;
    window->playSound( Sounds::GameOverRiff ) ;
    window->setBackgroundColor( Color::White ) ;
  }
  else if( gameState == GameState::GameOver &&
           newState == GameState::Menu )
  {
    // Start up the title theme music
    window->loopSound( Sounds::PacmanTitleThemeLoop ) ;
    window->setBackgroundColor( Color::Black ) ;
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
  
  #pragma region load the map from the file
  // Open the file in read mode.
  ifstream in( filename ) ;
  // Because you are C++ HACKERS
  // I'm using ifstream here.

  // Allocate a 2d array of char
  // large enough for this map
  char **tileMap = new char*[mapRows];
  for( int row = 0 ; row < mapRows ; row++ )
    tileMap[row] = new char[mapCols];  // allocate space

  // now load the map
  for( int row = 0 ; row < mapRows ; row++ )
  {
    for( int col = 0 ; col < mapCols ; col++ )
    {
      tileMap[row][col] = in.get() ;
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
  #pragma endregion

  // Create the AsciiMap object, based on tileMap
  asciiMap = new AsciiMap( tileMap, mapRows, mapCols ) ;
  
  // Set up the costs
  asciiMap->setTileCost( Tiles::Barrier, 99.0f ) ; // avoid ghosts going
  // back into the home by a high cost to the barrier

  // Now most of the rest of these are just
  // unit cost, meaning they're all "passable"
  // tiles and there's nothing special to say.
  asciiMap->setTileCost( Tiles::Blinky, 1.0f ) ;
  asciiMap->setTileCost( Tiles::Bonus, 1.0f ) ;
  asciiMap->setTileCost( Tiles::Empty, 2.0f ) ; // made empty tiles more expensive
  //
  asciiMap->setTileCost( Tiles::FlameThrower, 1.0f ) ;
  asciiMap->setTileCost( Tiles::Gun, 1.0f ) ;
  asciiMap->setTileCost( Tiles::Inky, 1.0f ) ;
  asciiMap->setTileCost( Tiles::PacmanStart, 1.0f ) ;
  asciiMap->setTileCost( Tiles::Pellet, 1.0f ) ;
  asciiMap->setTileCost( Tiles::Pinky, 1.0f ) ;
  asciiMap->setTileCost( Tiles::PowerPellet, 1.0f ) ;
  asciiMap->setTileCost( Tiles::Sue, 1.0f ) ;
  asciiMap->setTileCost( Tiles::Wall, IMPASSIBLE ) ;
  asciiMap->setTileCost( Tiles::Unwalkable, IMPASSIBLE ) ;
  
  // Finally, its a 4-way grid, not 8-way.
  asciiMap->setGridConnection( GridConnection::FourWay ) ;

  asciiMap->constructGraph() ;

  // NOTE HOWEVER if you wanted to you could
  // assign pellets different costs from
  // empty tiles, etc.  This gets really neat though because you
  // COULD make it so "empty" tiles are MORE EXPENSIVE
  // to walk on than tiles with pellets, which will
  // cause the ghosts to PREFER the areas with
  // pellets in them instead of the empty areas,
  // making the game more challenging.

  // This is the kind of thing that would
  // take some experimentation.

  // You could also make the powerpellet tiles
  // really cheap to walk on so that the ghosts
  // should tend to try to go that way as well.

  // that walking on PowerPellets for example,
  // is to be avoided by the ghosts (though you
  // COULD cause that to happen if you give
  // the powerpellets a high "cost")

  // Now process each char in the map
  #pragma region process each tile of the map
  for( int row = 0 ; row < mapRows ; row++ )
  {
    for( int col = 0 ; col < mapCols ; col++ )
    {
      // Set up the tile according
      // to the character loaded.
      // To avoid cluttering the Tile
      // class with level construction information,
      // I've chosen to put the SWITCH HERE
      // and _not_ in the Tile object.
      char mapChar = tileMap[row][col] ;

      Tile *tile = new Tile();
      tile->setTile( mapChar ) ;
      
      switch( mapChar )
      {
      case Tiles::PacmanStart:
        // Pacman is not a tile...!!
        //tile->setSpriteId( Sprites::Pacman ) ;
        // put nothing in the tiles map..

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

      case Tiles::Unwalkable:
        // Empty yet impassible.  Used for
        // completely enclosed BLANK areas
        // so ghosts don't try
        // to pathfind to them as a destination
        tile->setSpriteId( Sprites::Empty ) ;
        tile->setPassable( false ) ;
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
          ghost->setName( mapChar ) ;

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
  }
  #pragma endregion

  // Delete tileMap now, we are done with it,
  // all the information we need we stored
  // in the "level" variable, and in the asciiMap variable.
  for( int row = 0 ; row < mapRows; row++ )
    delete[] tileMap[row];
  delete[] tileMap;

  

}

void GameWorld::step( float time )
{
  pacman->step( time ) ;

  foreach( vector<Ghost*>::iterator, ghost, ghosts )
  {
    (*ghost)->step( time ) ;
  }


  // Cause all object intersections to "happen"
  foreach( vector<Ghost*>::iterator, ghost, ghosts )
  {
    // check if they collide
    if( (*ghost)->isIntersectingWith( pacman ) )
    {
      // Have them both hit each other
      (*ghost)->doIntersect( pacman ) ;

      pacman->doIntersect( *ghost ) ;
    }
  }


  // Now after this intersection,
  // the player may be dead.  If he is,
  // end the game
  if( pacman->getHealth() <= 0 )
  {
    setState( GameState::GameOver ) ;

    // Go back to the menu 5 seconds after THAT
    Callback1<GameState> * cb1 = new Callback1<GameState>(
      4.0f,
      transitionToState,
      GameState::Menu
    ) ;

    window->addCallback( cb1 ) ;
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

  char scoreBuf[ 60 ] ;
  sprintf( scoreBuf, "%d", pacman->getScore() ) ;
  window->drawString( Fonts::PacFont24, "Score", Color::Yellow,
                      xStatsOffset, yStatsOffset + 60, 200, 200,
                      DT_TOP | DT_LEFT ) ;

  startX = xStatsOffset + 20 ;
  window->drawString( Fonts::Arial24, scoreBuf, Color::White,
                      startX, yStatsOffset + 80, 200, 200,
                      DT_TOP | DT_LEFT ) ;


  window->drawString( Fonts::PacFont24, "Health", Color::Yellow,
                      xStatsOffset, yStatsOffset + 120, 200, 200,
                      DT_TOP | DT_LEFT ) ;

  // Draw the health bar
  // get % filled up
  float percentFull = (float)game->getPlayer()->getHealth() / 100.0f ; 

  float healthBarWidth = 120 ;
  float healthBarHeight = 10 ;

  // Draw red box as backing, to indicate missing health
  window->drawBox( Color::RosyBrown,
    xStatsOffset + 20, yStatsOffset + 145,
    120, 10
  ) ;

  // draw green box on top of red box
  window->drawBox( Color::Gainsboro,
    xStatsOffset + 20, yStatsOffset + 145,
    120 * percentFull, 10
  ) ;



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

Coord GameWorld::getNearestCoord( Vector2 & pos )
{
  int col = round( pos.x/tileSize ) ; 
  int row = round( pos.y/tileSize ) ;

  checkCoords( row, col ) ;
  return Coord( row, col ) ;
}

Vector2 GameWorld::getVectorAt( const Coord & coord )
{
  return Vector2(
    coord.col*tileSize,
    coord.row*tileSize
  ) ;
}

Vector2 GameWorld::getDrawingVectorAt( const Coord & coord ) 
{
  return Vector2(
    coord.col*tileSize + xBoardOffset,
    coord.row*tileSize + yBoardOffset
  ) ;
}

Vector2 GameWorld::getDrawingVectorAt( const Vector2& pos )
{
  return Vector2( pos.x + xBoardOffset, pos.y + yBoardOffset ) ;
}

Player* GameWorld::getPlayer()
{
  return pacman ;
}

AsciiMap* GameWorld::getAsciiMap()
{
  return asciiMap ;
}