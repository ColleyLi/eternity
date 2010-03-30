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
  
  // Instead of loading
  // 1,000,000 sprites for
  // each of Inky, Pinky, Blinky and Sue
  // (which you COULD DO!!) I instead
  // load the "Ghost body" ONCE, and
  // the EYES separately.  Then when
  // the ghosts get drawn, the BODY
  // gets drawn in a shade of color
  // and the eyes get drawn on top.

  GhostBody,
  EyesRight,
  EyesUp,
  EyesDown,
  EyesDead,

  /// A marker for the goal node, used
  /// to debug pathfinding
  GoalNode,

  GameOverTitle

} ;

enum Sounds
{
  PacmanTitleThemeLoop = 1,
  PacmanGamePlayMusic,
  PacmanPellet1,
  PacmanPellet2,
  PacmanPellet3,
  Intro,
  PacmanHurt,
  GameOverRiff
} ;

enum Fonts
{
  Arial24 = 1,
  TimesNewRoman18,
  PacFont24
} ;

#endif