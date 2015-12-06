#ifndef includes_h
#define includes_h

#include "freeglut.h"

float xpos, ypos;
int column = 1;
const int numOfColumns = 3;
const int WINDOW_WIDTH = 1000, WINDOW_HEIGHT = 750, GRID_Z = 1000;

enum type { GROUND, HOLE, FIRE };
type grid[GRID_Z][numOfColumns];

#endif
