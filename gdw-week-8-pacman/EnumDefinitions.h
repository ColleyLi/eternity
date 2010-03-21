#ifndef ENUM_DEFS_H
#define ENUM_DEFS_H

// put these in a header file
// so lots of files can use them
enum Sprites
{
  Mario = 1,
  Splash,
  PacmanTitle,
  Pacman,
  Pellet,
  Powerpellet,
  Bonus,
  Wall,
  Empty,

  /// The ghost door barrier
  Barrier,

  Gun,
  Flamethrower,
  Uzi,

  Inky,
  Blinky,
  Pinky,
  Sue

} ;

enum Sounds
{
  PacmanTitleThemeLoop = 1,
  PacmanGamePlayMusic,
  PacmanPellet1,
  PacmanPellet2,
  PacmanPellet3
} ;

enum Fonts
{
  Arial24 = 1,
  TimesNewRoman18,
  PacFont24
} ;

#endif