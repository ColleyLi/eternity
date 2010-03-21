#ifndef TILE_H
#define TILE_H

/// A Tile represents a game-world tile.
/// It has a spriteId that is used to draw it
/// and it has a "passable" variable which
/// indicates whether it can be walked over or not.

// We defined the 'level file format' as simply
// characters like '.' and '#' in a text file.  At game start
// parse and load lvl1.txt.

struct Tiles
{

  // Define all the constants
  // that relate to Tiles right here in
  // a separate struct.
  const static char PacmanStart = 'P' ;
  const static char Pellet      = '.' ;
  const static char PowerPellet = 'o' ;
  const static char Empty       = '-' ; //Clear,open tile with nothing there
  const static char Wall        = '#' ;
  const static char Inky        = 'i' ;
  const static char Blinky      = 'b' ;
  const static char Pinky       = 'p' ;
  const static char Sue         = 's' ;
  const static char Bonus       = 'B' ; 
  const static char Barrier     = '_' ; // the ghost door barrier that pacman
                                        // can't pass through but ghosts can

  // For future
  const static char FlameThrower = 'f';
  const static char Gun         = 'g' ;

} ;

class Tile
{
  int spriteId ;   // the sprite id to draw with
  bool passable ;  // Whether or not this tile
  // can be walked upon or not

public:
  Tile()
  {
    spriteId = 0 ;
    passable = true ;
  }

  // Grumble grumble..
  int getSpriteId(){ return spriteId; }
  void setSpriteId( int id ){ spriteId= id;}

  bool isPassable(){ return passable; }
  void setPassable( bool i_passable ){ passable = i_passable; }

  ///!! A function you call when the player
  /// runs into this tile.
  virtual void intersectAction() ;
} ;

// TILE NOTES:

// passable:  In this map we made a single
// bool "passable" to tell if a player can
// walk through this tile or not.
// You could also have a few other bools,
// like bool "passBullets",
// "passMagic", etc if you want certain
// tiles to block only people, but
// allow bullets or magic to pass through.

// We do not store x, y LOCATION
// in a Tile.  That's redundant.  Its x,y
// location is determined by
// where the Tile object is stored
// in the "level" map.
// Redundant information is not good,
// if you update one but forget to update
// the other, then you have an automatic bug.

// If you want layered background graphics,
// you can have each tile store a pointer to
// ANOTHER tile which is to be drawn above it...
// Tile* tileAbove ;
// Then in the draw, you can "walk up" the
// list of tiles, drawing each in the process,
// until you hit NULL.
#endif