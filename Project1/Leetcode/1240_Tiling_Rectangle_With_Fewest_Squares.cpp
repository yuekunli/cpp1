#include "LibHeaders.h"

namespace _1240_Tiling_Rectangle_With_Fewest_Squares {

	using namespace std;

	class Solution
	{
		int minTiles;
		int ROW;
		int COL;

		void backtrackSolve(vector<vector<bool>>& grid, int r, int c, int tilesUsed)
		{
			if (r >= ROW || c >= COL)
			{
				minTiles = tilesUsed;
				return;
			}

			if (tilesUsed == minTiles)
				return;

			if (grid[r][c])
			{
				if (c < COL - 1)
				{
					int i = c+1;
					while (i < COL && grid[r][i])
						i++;
					if (i < COL)
						backtrackSolve(grid, r, i, tilesUsed);
					else
						backtrackSolve(grid, r + 1, 0, tilesUsed);
				}
			}

			for (int tileSize = 1; tileSize <= ROW; tileSize++)
			{
				if (c + tileSize - 1 >= COL || r + tileSize - 1 >= ROW)
					return;
				else
				{
					if (!grid[r][c + tileSize - 1])
					{
						for (int i = r; i <= r + tileSize - 1; i++)
						{
							for (int j = c; j <= c + tileSize - 1; j++)
								grid[i][j] = true;
						}
						if (c + tileSize >= COL)
						{
							if (r < ROW - 1)
								backtrackSolve(grid, r + 1, 0, tilesUsed + 1);
							else
								backtrackSolve(grid, r, c + tileSize, tilesUsed + 1); // this call is only to tally the tiles used
						}
						else
							backtrackSolve(grid, r, c + tileSize, tilesUsed + 1);

						for (int i = r; i <= r + tileSize - 1; i++)
						{
							for (int j = c; j <= c + tileSize - 1; j++)
								grid[i][j] = false;
						}
					}
					else
						return;
				}
			}
		}

	public:
		int tilingRectangle(int n, int m)
		{
			if (n == m) return 1;

			if (n > m)
				return tilingRectangle(m, n);

			ROW = n;
			COL = m;
			minTiles = max(n, m);

			vector<vector<bool>>grid(n, vector<bool>(m, false));
			backtrackSolve(grid, 0, 0, 0);
			return minTiles;
		}
	};

	void Test_1240_Tiling_Rectangle_With_Fewest_Squares()
	{
		Solution solu;
		int n, m;
		while (true)
		{
			cout << "n: ";
			cin >> n;
			if (n == 0) break;
			cout << "m: ";
			cin >> m;
			cout << solu.tilingRectangle(n, m) << "\n\n";
		}
	}
}