#include<vector>
#include<forward_list>
#include<queue>

namespace _0934_Shortest_Bridge {
	using namespace std;

	// accepted 4ms beats 83%, memory 22MB beats 96%
	class Solution
	{
	public:
		int shortestBridge(vector<vector<int>>& grid)
		{
			// find first '1'
			int i, j;
			auto n = grid.size();
			bool found = false;
			for (i = 0; i < n; ++i)
			{
				for (j = 0; j < n; ++j)
				{
					if (grid[i][j] == 1)
					{
						grid[i][j] = 2;
						found = true;
						break;
					}
				}
				if (found)
				{
					break;
				}
			}

			queue<pair<int, int>>firstIsland;
			queue<pair<int, int>>adjacentOcean;

			firstIsland.emplace(i, j);
			while (!firstIsland.empty())
			{
				auto [r, c] = firstIsland.front();
				firstIsland.pop();
				if (r > 0 && grid[r - 1][c] != 2)
				{
					if (grid[r - 1][c] == 1)
					{
						firstIsland.emplace(r - 1, c);
					}
					else
					{
						adjacentOcean.emplace(r - 1, c);
					}
					grid[r - 1][c] = 2;
				}
				if (r < n - 1 && grid[r + 1][c] != 2)
				{
					if (grid[r + 1][c] == 1)
					{
						firstIsland.emplace(r + 1, c);
					}
					else
					{
						adjacentOcean.emplace(r + 1, c);
					}
					grid[r + 1][c] = 2;
				}
				if (c > 0 && grid[r][c - 1] != 2)
				{
					if (grid[r][c - 1] == 1)
					{
						firstIsland.emplace(r, c - 1);
					}
					else
					{
						adjacentOcean.emplace(r, c - 1);
					}
					grid[r][c - 1] = 2;
				}
				if (c < n - 1 && grid[r][c + 1] != 2)
				{
					if (grid[r][c + 1] == 1)
					{
						firstIsland.emplace(r, c + 1);
					}
					else
					{
						adjacentOcean.emplace(r, c + 1);
					}
					grid[r][c + 1] = 2;
				}
			}

			queue<pair<int, int>>adjacentOcean1;
			int alternater = 0;
			auto pCurrent = &adjacentOcean;
			auto pNext = &adjacentOcean1;
			int bridgeLength = 1;
			while (true)
			{
				while (!pCurrent->empty())
				{
					auto [r, c] = pCurrent->front();
					pCurrent->pop();
					if (r > 0 && grid[r - 1][c] != 2)
					{
						if (grid[r - 1][c] == 1)
						{
							return bridgeLength;
						}
						else
						{
							pNext->emplace(r - 1, c);
							grid[r - 1][c] = 2;
						}
					}
					if (r < n - 1 && grid[r + 1][c] != 2)
					{
						if (grid[r + 1][c] == 1)
						{
							return bridgeLength;
						}
						else
						{
							pNext->emplace(r + 1, c);
							grid[r + 1][c] = 2;
						}
					}
					if (c > 0 && grid[r][c - 1] != 2)
					{
						if (grid[r][c - 1] == 1)
						{
							return bridgeLength;
						}
						else
						{
							pNext->emplace(r, c - 1);
							grid[r][c - 1] = 2;
						}
					}
					if (c < n - 1 && grid[r][c + 1] != 2)
					{
						if (grid[r][c + 1] == 1)
						{
							return bridgeLength;
						}
						else
						{
							pNext->emplace(r, c + 1);
							grid[r][c + 1] = 2;
						}
					}
				}
				alternater = (alternater + 1) % 2;
				if (alternater == 0)
				{
					pCurrent = &adjacentOcean;
					pNext = &adjacentOcean1;
				}
				else
				{
					pCurrent = &adjacentOcean1;
					pNext = &adjacentOcean;
				}
				++bridgeLength;
			}
			return bridgeLength;
		}
	};
}