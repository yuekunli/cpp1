#include<iostream>
#include<vector>

namespace _0174_Dungeon_Game {

	using namespace std;

	// accepted 5ms beats 8%, memory 9.2MB beats 100%
	class Solution1
	{
	public:
		int calculateMinimumHP(vector<vector<int>>& dungeon)
		{
			size_t ROW = dungeon.size();
			size_t COL = dungeon[0].size();

			vector<vector<int>>records(ROW, vector<int>(COL, -1));
			records[ROW - 1][COL - 1] = dungeon[ROW - 1][COL - 1] >= 0 ? 1 : (1 - dungeon[ROW - 1][COL - 1]);
			int r_start, c_start;
			
			// start from left, go north-east.
			//        15 14 12 9
			//        13 11 8  5
			//        10 7  4  2
			//        6  3  1  *
			// these number are the order of each cell being visited.
			
			if (COL == 1)
			{
				// what if there is only 1 colume, there is no cell to the left of the princess' prison.
				
				r_start = (int)(ROW - 2);
				c_start = 0;
			}
			else
			{
				r_start = (int)(ROW - 1);
				c_start = (int)(COL - 2);

				// I don't need to do special handling when there is only 1 row, the ROW-1 and COL-2 are still valid for having only 1 row
			}

			int r, c;
			int minHpIfGoDown, minHpIfGoRight;

			while (r_start >= 0 && c_start >= 0)
			{
				r = r_start;
				c = c_start;
				while (r >= 0 && c < COL)
				{
					minHpIfGoDown = numeric_limits<int>::max();
					minHpIfGoRight = numeric_limits<int>::max();
					if (r < ROW - 1)
					{
						/*
						*     -3   X    '-3' is the cell's value in "dungeon"
						*      5   X    '5' is the cell's requirement in "records"
						* Standing in the cell with value '-3', the cell down below requires at least '5' to enter.
						* If I want to take this path, I need at least 8 to enter the cell with '-3'.
						* 
						*      7   X
						*      5   X
						* Technically if I enter the cell, whose value is '7', with HP of '-2', I can get a boost and
						* successfully enter the cell down below, but when the HP is down to '-2', I should have died.
						*/
						minHpIfGoDown = (records[r + 1][c] - dungeon[r][c]) <= 0 ? 1 : (records[r + 1][c] - dungeon[r][c]);
					}
					if (c < COL - 1)
					{
						minHpIfGoRight = (records[r][c + 1] - dungeon[r][c]) <= 0 ? 1 : (records[r][c + 1] - dungeon[r][c]);
					}
					records[r][c] = min(minHpIfGoDown, minHpIfGoRight);

					r--;
					c++;
				}
				if (c_start > 0)
					c_start--;
				else
					r_start--;
			}

			return records[0][0];
		}
	};



	/*
	* Same algorithm as Solution1
	* only minor change when comparing two possible directions for each cell
	* Instead of calculating the required HP for going down or right,
	* just compare the required HP in the cell down below and the cell on the right
	* pick the smaller one, then calculate the required HP for current cell
	*/

	// accepted 0ms beats 100%  memory 13.12MB beats 21%
	// it's hard to tell why memory cost of these two solutions are drastically different
	// I did get rid of some intermediate variables in Solution2, but does that have such big effect?
	// And also the minor change caused a 5ms time cost optimization. does the change really help that much?
	class Solution2
	{
	public:
		int calculateMinimumHP(vector<vector<int>>& dungeon)
		{
			size_t ROW = dungeon.size();
			size_t COL = dungeon[0].size();

			vector<vector<int>>records(ROW, vector<int>(COL, -1));
			records[ROW - 1][COL - 1] = dungeon[ROW - 1][COL - 1] >= 0 ? 1 : (1 - dungeon[ROW - 1][COL - 1]);
			int r_start, c_start;

			if (COL == 1)
			{
				r_start = (int)(ROW - 2);
				c_start = 0;
			}
			else
			{
				r_start = (int)(ROW - 1);
				c_start = (int)(COL - 2);
			}

			int r, c;

			while (r_start >= 0 && c_start >= 0)
			{
				r = r_start;
				c = c_start;
				while (r >= 0 && c < COL)
				{
					if (r == ROW - 1)
					{
						// I can only go right
						records[r][c] = (records[r][c + 1] - dungeon[r][c]) <= 0 ? 1 : (records[r][c+1] - dungeon[r][c]);
					}
					else if (c == COL - 1)
					{
						// I can only go down
						records[r][c] = (records[r+1][c] - dungeon[r][c]) <= 0 ? 1 : (records[r + 1][c] - dungeon[r][c]);
					}
					else
					{
						records[r][c] = (min(records[r][c+1], records[r+1][c]) - dungeon[r][c]) <= 0 ? 1 : (min(records[r][c + 1], records[r + 1][c]) - dungeon[r][c]);
					}
					
					r--;
					c++;
				}
				if (c_start > 0)
					c_start--;
				else
					r_start--;
			}

			return records[0][0];
		}
	};



	void Test_0174_Dungeon_Game()
	{
		vector<vector<int>> d1 = {{-2, -3, 3}, {-5, -10, 1}, {10, 30, -5}};
		vector<vector<int>> d2 = { {0} };
		Solution1 s;
		cout << s.calculateMinimumHP(d1) << "\n\n";
		cout << s.calculateMinimumHP(d2) << "\n\n";
	}
}