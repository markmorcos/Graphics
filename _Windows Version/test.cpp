#include <bits/stdc++.h>
using namespace std;

#define endl '\n'

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

int main()
{
	for(int i = 0; i < COLUMNS; ++i) grid[0][i] = grid[1][i] = GROUND;
	for(int i = 2; i < ROWS - 1; i += 2) for(int j = 0; j < COLUMNS; ++j) grid[i][j] = GROUND;
	for(int i = 2; i < ROWS - 2; i += 3)
	{
		int idx = randomNumber() % COLUMNS;
		int t = randomNumber() % 3;
		grid[i][idx] = !t ? FIRE : HOLE;
		if(t) grid[i + 1][idx] = grid[i + 2][idx] = HOLE;
	}
	for(int i = 0; i < ROWS; ++i)
	{
		for(int j = 0; j < COLUMNS; ++j)
		{
			std::cout << (grid[i][j] == 0 ? "GROUND" : (grid[i][j] == 1 ? "HOLE" : "FIRE")) << "\t";
		}
		std::cout << '\n';
	}
}
