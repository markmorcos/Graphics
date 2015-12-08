#ifndef includes_h
#define includes_h

#include <ctime>
#include <iostream>
#include "freeglut.h"

float xpos, ypos;
int row = 0, column = 1;
const int WINDOW_WIDTH = 1000, WINDOW_HEIGHT = 750;
const int ROWS = 1000, COLUMNS = 3;

enum type { GROUND, HOLE, FIRE };
type grid[ROWS][COLUMNS];

long long seed = time(0);

unsigned long long randomNumber()
{
  unsigned long long x = 2523523 * seed, y = 1288924102 * seed, z = 58327598 * seed;
  unsigned long long t;
  x ^= x << 16;
  x ^= x >> 5;
  x ^= x << 1;
  t = x;
  x = y;
  y = z;
  z = t ^ x ^ y;
  return seed = z;
}

#endif
