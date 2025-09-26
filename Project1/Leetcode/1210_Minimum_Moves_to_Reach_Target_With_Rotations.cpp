#include<vector>
#include<deque>
#include<iostream>

namespace _1210_Minimum_Moves_to_Reach_Target_With_Rotations {

	using namespace std;

	// accepted 79ms beat 28%
	class Solution
	{
	public:
		int minimumMoves(vector<vector<int>>& grid)
		{
			deque<pair<pair<int, int>, pair<int, int>>>queues[2];
			deque<pair<pair<int, int>, pair<int, int>>>* currentQ = &queues[0];
			deque<pair<pair<int, int>, pair<int, int>>>* nextQ = &queues[1];
			int i = 0;
			size_t n = grid.size();
			vector<vector<unsigned char>>visited(n, vector<unsigned char>(n, 0));
			visited[0][1] = 1;
			// "visited" records the snake's head's position
			// visited[x][y] == 0 : snake's head has not be in this cell
			// visited[x][y] == 1 : snake's head has been in this cell and the snake is flat
			// visited[x][y] == 2 : snake's head has been in this cell and the snake is vertical

			// piecewise_construct, forward_as_tuple( ), forward_as_tuple( )

			(*currentQ).emplace_back(piecewise_construct, forward_as_tuple(0, 1), forward_as_tuple(0, 0));
			int movesCount = 0;
			while(!(*currentQ).empty())
			{
				while (!(*currentQ).empty())
				{
					auto const& p = (*currentQ).front();
					if (p.first.first == n - 1 && p.first.second == n - 1 && p.second.first == n - 1 && p.second.second == n - 2)
					{
						return movesCount;
					}
					else if (p.first.first == p.second.first) // snake is flat
					{
						if (p.first.second != n - 1 && grid[p.first.first][p.first.second + 1] == 0 && visited[p.first.first][p.first.second + 1] != 1)
						{
							(*nextQ).emplace_back(piecewise_construct, forward_as_tuple(p.first.first, p.first.second + 1), forward_as_tuple(p.first.first, p.first.second));
							visited[p.first.first][p.first.second + 1] = 1;
						}

						if (p.first.first != n - 1 && grid[p.first.first + 1][p.first.second] == 0 && grid[p.second.first + 1][p.second.second] == 0 && visited[p.first.first + 1][p.first.second] != 1)
						{
							(*nextQ).emplace_back(piecewise_construct, forward_as_tuple(p.first.first + 1, p.first.second), forward_as_tuple(p.second.first + 1, p.second.second));
							visited[p.first.first + 1][p.first.second] = 1;
						}
						if (p.second.first != n - 1 && grid[p.first.first + 1][p.first.second] == 0 && grid[p.second.first + 1][p.second.second] == 0 && visited[p.second.first + 1][p.second.second] != 2)
						{
							(*nextQ).emplace_back(piecewise_construct, forward_as_tuple(p.second.first + 1, p.second.second), forward_as_tuple(p.second.first, p.second.second));
							visited[p.second.first + 1][p.second.second] = 2;
						}
					}
					else // snake is vertical
					{
						if (p.first.second != n - 1 && grid[p.first.first][p.first.second + 1] == 0 && grid[p.second.first][p.second.second + 1] == 0 && visited[p.first.first][p.first.second + 1] != 2)
						{
							(*nextQ).emplace_back(piecewise_construct, forward_as_tuple(p.first.first, p.first.second + 1), forward_as_tuple(p.second.first, p.second.second + 1));
							visited[p.first.first][p.first.second + 1] = 2;
						}

						if (p.first.first != n - 1 && grid[p.first.first + 1][p.first.second] == 0 && visited[p.first.first + 1][p.first.second] != 2)
						{
							(*nextQ).emplace_back(piecewise_construct, forward_as_tuple(p.first.first + 1, p.first.second), forward_as_tuple(p.second.first + 1, p.second.second));
							visited[p.first.first + 1][p.first.second] = 2;
						}
						if (p.second.second != n - 1 && grid[p.first.first][p.first.second + 1] == 0 && grid[p.second.first][p.second.second + 1] == 0 && visited[p.second.first][p.second.second + 1] != 1)
						{
							(*nextQ).emplace_back(piecewise_construct, forward_as_tuple(p.second.first, p.second.second + 1), forward_as_tuple(p.second.first, p.second.second));
							visited[p.second.first][p.second.second + 1] = 2;
						}
					}
					(*currentQ).pop_front();
				}
				++movesCount;
				nextQ = currentQ;
				i = (i + 1) % 2;
				currentQ = &queues[i];
			}
			return -1;
		}
	};


	/*
	* Try only searching a path for a single dot.
	* As if the moving object is a single dot that occupies a single cell.
	* But when this "dot" makes a turn, it needs a 4-cell clearance.
	*      #  #  #  #  #  #  #  X
	*                        #  #
	*                           #
	*                           #
	*                           #
	*  '#' indicates open cells.
	*  The object can move along the '#' path from top left to bottom right.
	* The turning point has a 4-cell square that is clear.
	* This also works:
	*      #  #  #  #  #  #  X  #
	*                        #  #
	*                        #
	*                        #
	*                        #
	* 
	* Note that when the snake's head is at 'X', it takes two moves for it to turn vertical.
	* This is true for both above cases.
	* 1st case, rotate, then move to right while the snake is in vertical
	* 2nd case, move to right (one cell over the vertical lane), then rotate
	* 
	* The same principle applies to turning from vertical to flat too.
	*       #
	*       #
	*       #
	*       #  #
	*       X  #  #  #  #  #
	* 
	*       #
	*       #
	*       #
	*       X  #  #  #  #  #
	*       #  #
	* 
	* Likewise, it takes 2 moves for the snake to turn flat when its head is at cell 'X'.
	* this is true for both cases.
	* 
	* From top left to bottom right, this is like Manhattan distance, because I can only go
	* right and down, the basic shape of the route is something like this:
	*  --> --> --> --> -->
	*                     |
	*                     V
	*                     |
	*                     V --> --> -->
	* 
	* Because turns take 2 moves, the fewer the turn, the faster the route.
	* Two turns is the minimum. Illustrated above.
	* 
	* Is 1-turn possible? for example moving along the border,
	* move on the top row, all the way to the right-most column, then go straight down.
	* No, this is not a valid route, when going straight down on the right-most column,
	* the snake head reaches the bottom right cell, but the requirement is to have the
	* snake in flat position and head in bottom right cell.
	* 
	* Do a rotate on the first move, make the snake vertical on the left-most column,
	* then go down all the way to the bottom, then rotate to make it flat on the bottom row,
	* then move toward right side all the way, this is a good route, but it's as good as a
	* two-turn route illustrated above, even though it seems faster because its first move
	* (i.e. the rotate from flat to vertical) seems to achieve its goal in 1 move.
	* However, for a optimal two-turn route, the snake head is not actually in the top-left cell,
	* it's actually in the 2nd cell of the top row, so it saves a move there.
	*/
	class Solution2
	{

	};

	static void test1()
	{
		Solution solu;
		vector<vector<int>> grid{ 
			{0, 0, 0, 0, 0, 1},
			{1, 1, 0, 0, 1, 0},
			{0, 0, 0, 0, 1, 1},
			{0, 0, 1, 0, 1, 0},
			{0, 1, 1, 0, 0, 0},
			{0, 1, 1, 0, 0, 0} };

		cout << solu.minimumMoves(grid) << endl;
	}

	void Test_1210_Minimum_Moves_to_Reach_Target_With_Rotations()
	{
		test1();
	}
}