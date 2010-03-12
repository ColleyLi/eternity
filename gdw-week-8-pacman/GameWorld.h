#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include <iostream>  /// bleeaaaaaaagh.  Because you love it.
#include <fstream>   /// blaaaaaaaaargh.  For ifstream.
using namespace std ;

// This GameWorld class is a
// super-container for all
// the game world data.

// There should only ever be
// one copy of the GameWorld object
// in our game

// The GameWorld contains Tiles and GameObjects, basically
#include "Tile.h"
#include "GameObject.h"
#include "EnumDefinitions.h"

class GameWorld
{
  // define game-wide vars
  // We have to have a WAY to __STORE AND REMEMBER__
  // game world data for the level.

  // We'll do this by keeping a 2D array
  // of pointers to "Tile" objects.
  // A "Tile" object represents a game-world tile,
  // and its defined in Tile.h.

  // For simplicity, we'll use a
  // FIXED height/width
  const static int mapRows = 27 ;
  const static int mapCols  = 21 ;
  const static int tileSize  = 16 ;  // Tile size, in pixels.
  // Each tile is 16x16 pixels.

  // Define the array to hold
  // the game level. 
  Tile* level[ mapRows ][ mapCols ] ;// OPEN lvl1.txt TO SEE
  // Notice we're using pointers...
  // Notice also this could be layered...

public:
  void loadLevel( char *filename )
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
          tile->setSpriteId( Sprites::Pacman ) ;
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

  // Draw the game world
  void draw()
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
        float x = col * tileSize ;
        float y = row * tileSize ;

        // Draw these tiles using
        // the TOP LEFT CORNER
        // as the origin
        window->drawSprite( currentTile->getSpriteId(), x, y, tileSize, tileSize, SpriteCentering::TopLeft ) ;
      }
    }
  }
} ;

#endif