#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include "pacman-main.h"

#include <iostream>  /// bleeaaaaaaagh.  Because you love it.
#include <fstream>   /// blaaaaaaaaargh.  For ifstream.
#include <vector>
using namespace std ;

// This GameWorld class is a
// super-container for all
// the game world data.

// There should only ever be
// one copy of the GameWorld object
// in our game

// The GameWorld contains Tiles and GameObjects and
// stats, basically
#include "EnumDefinitions.h"
#include "Vector2.h"
#include "Tile.h"
#include "GameObject.h"
#include "FourDirectionMovingObject.h"
#include "Player.h"
#include "Ghost.h"

class GameWorld
{
public:
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

  // This game has a couple of states.
  // I number them explicitly for easy reference
  enum GameState
  {
    /// The splash screen is where you display your
    /// company logo etc
    Splash  = 1,

    /// The menu screen where you start
    /// to choose what mode to play in or whatever
    Menu    = 2,

    /// The game is ACTUALLY RUNNING.  Most time
    /// is spent in this state.
    Running = 3,

    /// The game is PAUSED, from the running state.
    /// You may choose to continue to run the music,
    /// or run some alternate form of music during
    /// the Paused state, BUT YOU STILL NEED to draw
    /// the game out!
    Paused  = 4,

    /// The game is over and the next logical step
    /// is to move back to the menu.
    GameOver= 5
  };

  #pragma region state-machines
  /*
  Allowable state transitions:
  Here's a rough ASCII state change diagram.
  Its a lot better to draw these in POWERPOINT
  or something.  This actually LOOKS a lot
  more complicated than it is.
  Its just a diagram of WHAT STATES
  transition to WHAT STATES and how
  it happens.

                  BOOT GAME
                    |
                    | (load minimal things to minimize wait)
                    v
                  Splash
                    |
                    | (load more things, takes a couple of sec.)
                    |
                    |
                    v   User "quit" ESC
              ---->Menu----------------> QUIT
   5 sec ----/      |                     ^
    ----/           |user "start game"    |
    |               |                     |user presses ESC
    |               |                     |
    |               |   -----------------/
    |               |  /
   /     user dies  v /   User presses 'P'
GameOver<--------Running <-------------> Paused
                    
  */
  #pragma endregion

private:
  // Define the array to hold
  // the game level.
  Tile* level[ mapRows ][ mapCols ] ; // OPEN lvl1.txt TO SEE
  // Notice we're using pointers...
  // Notice also this could be layered...

  // Define the ASCIIMAP object
  // used only for pathfinding,
  // NOT for drawing or anything else.
  AsciiMap *asciiMap ;
  //!! Note - you might find a way to factor 
  // together AsciiMap and that 2D
  // array of Tile* together.
  // If I re-coded this I might find a way to
  // unify AsciiMap and that Tile* array.

  // The GameState.
  GameState gameState ;

private:
  // offset to draw the game board at
  float xBoardOffset, yBoardOffset,
        xStatsOffset, yStatsOffset ;

  Player *pacman ;
  vector<Ghost*> ghosts ;


public:
  GameWorld() ;

  /// Gets the current gamestate
  GameState getState() ;

  /// Sets the gamestate to some new state
  void setState( GameState newState ) ;

  /// Loads a level
  void loadLevel( char *filename ) ;

  /// update the game just a fraction
  void step( float time ) ;

  /// Draw the game
  /// world board
  void drawBoard() ;

  /// Draws stat charts
  /// at right side of screen
  /// (lives, health, etc)
  void drawStats() ;

  /// Draws all game units
  /// order matters!  the last thing
  /// you draw shows up on top.
  void drawPeople() ;

private:
  void checkCoords( int & row, int & col ) ;

public:
  Tile* getTileNearestLeft( Vector2 & pos ) ;

  Tile* getTileNearestRight( Vector2 & pos ) ;

  Tile* getTileNearestUp( Vector2 & pos ) ;

  Tile* getTileNearestDown( Vector2 & pos ) ;

  /// Gets you a specific tile
  /// at some location
  Tile* getTileAt( Vector2 & pos ) ;

  Coord getNearestCoord( Vector2 & pos ) ;

  Vector2 getVectorAt( const Coord & coord ) ;

  /// Gets you the vector you should
  /// use to draw at a certain coordinate
  Vector2 getDrawingVectorAt( const Coord & coord ) ;

  /// Gets you the drawing vector
  /// you should use to draw (just
  /// adds offsetX and offsetY to
  /// the vector
  Vector2 getDrawingVectorAt( const Vector2& pos ) ;

  Player *getPlayer() ;

  AsciiMap *getAsciiMap() ;
} ;

extern GameWorld *game ; // an extern is
// like a "variable prototype".  Declare one here,
// and actually instantiate it in
// the GameWorld.cpp file

#endif