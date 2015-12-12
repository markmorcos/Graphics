#ifndef controller_h
#define controller_h

#include "includes.hpp"

void generate()
{
	for(int i = 0; i < COLUMNS; ++i) grid[0][i] = grid[1][i] = grid[2][i] = grid[3][i]= grid[4][i]= grid[5][i] = grid[6][i] = GROUND;
	for(int i = 7; i < ROWS - 1; i += 2) for(int j = 0; j < COLUMNS; ++j) grid[i][j] = GROUND;
	for(int i = 7; i < ROWS - 2; i += 3)
	{
		int idx = randomNumber() % COLUMNS;
		int t = randomNumber() % 5;
		grid[i][idx] = !t ? FIRE : HOLE;
		int ind = randomNumber() % COLUMNS;
		if(t) grid[i + 1][idx] = grid[i + 2][idx] = grid[i][ind] = grid[i + 1][ind] = HOLE;
	}
//	for(int i = 0; i < ROWS; ++i)
//	{
//		for(int j = 0; j < COLUMNS; ++j)
//		{
//			std::cout << (grid[i][j] == 0 ? "GROUND" : (grid[i][j] == 1 ? "HOLE" : "FIRE")) << "\t";
//		}
//		std::cout << '\n';
//	}
}

#endif

