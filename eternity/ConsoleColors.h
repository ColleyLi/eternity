
#ifndef CONSOLE_COLORS_H
#define CONSOLE_COLORS_H

#include <windows.h>
#include <stdio.h>

void consoleCreate() ;

void consoleRowsAndCols( int rows, int cols ) ;

void consoleMove( int x, int y ) ;

void consoleSizePixels( int width, int height ) ;

// Makes the console the topmost window
void consoleMakeTopmostWindow() ;

void consoleSetColor( WORD color ) ;

void consoleBlack() ;

void consoleBrightRed() ;

void consoleBrightGreen() ;

void consoleBrightBlue() ;

void consoleBrightYellow() ;

void consoleBrightMagenta() ;

void consoleBrightCyan() ;

void consoleLightGray() ;

void consoleDarkGray() ;

void consoleWhite() ;

void consoleDarkRed() ;

void consoleDarkGreen() ;

void consoleDarkBlue() ;

void consoleDarkYellow() ;

void consoleDarkMagenta() ;

void consoleDarkCyan() ;

#endif