#ifndef controller_h
#define controller_h

#include "includes.hpp"

void generate()
{
	for(int i = 0; i < COLUMNS; ++i) grid[0][i] = grid[1][i] = GROUND;
	for(int i = 2; i < ROWS; i += 2) for(int j = 0; j < COLUMNS; ++j) grid[i][j] = HOLE;
	for(int i = 3; i < ROWS; i += 2) for(int j = 0; j < COLUMNS; ++j) grid[i][j] = GROUND;
	for(int i = 3; i < ROWS; i += 2)
	{
		int idx = randomNumber() % COLUMNS;
		int t = randomNumber() % 2;
		grid[i][idx] = t ? HOLE : FIRE;
	}
	/*for(int i = 0; i < ROWS; ++i)
	{
		for(int j = 0; j < COLUMNS; ++j)
		{
			std::cout << grid[i][j] << " ";
		}
		std::cout << '\n';
	}*/
}

#endif

