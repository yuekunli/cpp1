#include<iostream>
#include<vector>
#include<numeric>

namespace _0887_Super_Egg_Drop {

	using namespace std;


	/*
	*         |  1 floor  | 2 floors   3 floors ....   n floors
	* --------+-----------+------------------------------------
	* 1 egg   |           |
	* --------+-----------+------------------------------------
	* 2 eggs  |           |
	* --------+-----------+------------------------------------
	* ....    |           |
	* --------+-----------+------------------------------------
	* k eggs  |           |
	* 
	* The gist is to solve this matrix.
	* 
	* for a certain row, x eggs with n floors.
	* The middle is likely to give me two close results.
	* for example 100 floors with 5 eggs.
	* try floor #50
	* if first drop breaks the egg, I need to solve 49 floors with 4 eggs next.
	* If first drop doesn't break the egg, I need to solve 50 floors with 5 eggs next.
	* 
	* In each scenario, I must take the worse case.
	* If say try floor 'x', if break first egg, I need 30 moves next, 31 in total.
	* If first egg doesn't break, I need 24 moves next, 25 in total.
	* I must say I need at least 31 moves to solve this scenario. 31 is *just* enough if first egg breaks.
	* It's *more than* enough if first egg doesn't break.
	* But I can't say I only need 25 moves, because if first egg breaks, 25 moves won't be enough.
	* 
	*  X  X  X  X  X  X  X  X  X  X  X  X  X  X  X  X  X
	*                                      |
	*                                  if I try here as first drop, the unbalanceness of the two scenarios are
	* very obvious, so the longer part determines the find result of this scenario.
	* The farther away from the middle, the more moves the "longer" part demands, and that "demaind" is the result for that scenario.
	* So I don't need to try every floor as the first drop.
	* If at some point the result is already worse than what I have so far, I don't need to try farther away from the middle, because
	* those results are just going to be worse.
	*/


	class Solution
	{
	public:
		int superEggDrop(int k, int n)
		{
			vector<vector<int>> t(k+1, vector<int>(n+1, 1));
		
			iota(t[1].begin(), t[1].end(), 0);

			for (int i = 2; i <= k; i++) // i eggs
			{
				// t[i][1] should be 1, but don't need to do that here, because every cell in the table is initialized to 1
				for (int j = 2; j <= n; j++) // j floors
				{
					int minimum = j;
					for (int firstTry = 1; firstTry <= j; firstTry++)
					{
						int movesNeededIfFirstTryBreak;
						int movesNeededIfFirstTrySurvive;

						if (firstTry > 1)
							movesNeededIfFirstTryBreak = t[i - 1][firstTry - 1] + 1;
						else
							movesNeededIfFirstTryBreak = 1;  // first try at floor#1, and it breaks, I don't need any more trys. Overall attempt is just 1.

						if (firstTry < j)
							movesNeededIfFirstTrySurvive = t[i][j - firstTry] + 1;
						else
							movesNeededIfFirstTrySurvive = 1;

						int m = max(movesNeededIfFirstTryBreak, movesNeededIfFirstTrySurvive);
						if (m < minimum)
							minimum = m;
					}
					t[i][j] = minimum;
				}
			}
			return t[k][n];
		}
	};

	class Solution2
	{
	public:

		int superEggDrop(int k, int n)
		{
			vector<vector<int>> t(k + 1, vector<int>(n + 1, 0));

			iota(t[1].begin(), t[1].end(), 0);

			for (int i = 0; i <= k; i++)
				t[i][1] = 1;

			for (int i = 2; i <= k; i++) // i eggs
			{
				for (int j = 2; j <= n; j++) // j floors
				{
					int minimum = j;
					for (int firstTry = 1; firstTry <= j; firstTry++)
					{
						int movesNeededIfFirstTryBreak;
						int movesNeededIfFirstTrySurvive;

						movesNeededIfFirstTryBreak = t[i - 1][firstTry - 1] + 1;
						movesNeededIfFirstTrySurvive = t[i][j - firstTry] + 1;

						int m = max(movesNeededIfFirstTryBreak, movesNeededIfFirstTrySurvive);
						if (m < minimum)
							minimum = m;
					}
					t[i][j] = minimum;
				}
			}
			return t[k][n];
		}
	};

	class Solution3
	{
	public:
		int superEggDrop(int k, int n)
		{
			vector<vector<int>>r(k + 1, vector<int>(n + 1, 0));
			iota(r[1].begin(), r[1].end(), 0);
			for (int i = 0; i <= k; ++i)
			{
				r[i][1] = 1;
			}
			for (int eg = 2; eg <= k; ++eg)
			{
				for (int fl = 2; fl <= n; ++fl)
				{
					int minimum = fl;
					int movesNeededIfFirstTryBreak;
					int movesNeededIfFirstTrySurvive;

					if (fl % 2 == 0)
					{
						// for example 8 floors, try 4 and 5
						// say I have 6 eggs
						// when trying floor #3
						//     --  break:    2 floors with 5 eggs
						//     --  survive:  5 floors with 6 eggs 
						// 
						// when trying floor #4
						//     --  break:    3 floors with 5 eggs
						//     --  survive:  4 floors with 6 eggs
						// when trying floor #5
						//     --  break:    4 floors with 5 eggs    * this one is bigger in this case
						//     --  survive:  3 floors with 6 eggs
						int firstTry = fl / 2;
						movesNeededIfFirstTryBreak = r[eg - 1][firstTry - 1] + 1;
						movesNeededIfFirstTrySurvive = r[eg][fl - firstTry] + 1;
						minimum = max(movesNeededIfFirstTryBreak, movesNeededIfFirstTrySurvive);

						for (int i = firstTry - 1; i >= 2; --i)
						{
							movesNeededIfFirstTryBreak = r[eg - 1][i - 1] + 1;
							movesNeededIfFirstTrySurvive = r[eg][fl - i] + 1;
							int m = max(movesNeededIfFirstTryBreak, movesNeededIfFirstTrySurvive);
							if (m < minimum)
							{
								minimum = m;
							}
							else
							{
								// the result of trying at this floor is already not affecting the overall result
								// keep lowering the first try floor will have less chance to affect the overall result
								// when keep lowering the first try, the "moves needed if first try break" decreases (5 eggs with fewer and fewer floors)
								// when keep lowering the first tyr, the "moves needed if first try survive" increases ( 6 eggs with more and more floors)
								// 
								if (movesNeededIfFirstTryBreak <= movesNeededIfFirstTrySurvive)
								{
									break;
								}
							}
						}

					}
					else
					{
						// for example 9 floors, try 4, 5, 6
						// say I have 10 eggs
						// when trying floor #4 
						//      --  break:    3 floors with 9 eggs    
						//                                            * no clear winner (bigger) in this case
						//      --  survive:  5 floors with 10 eggs
						// when trying floor #5
						//      --  break:    4 floors with 9 eggs    * this one is bigger in this case
						//      --  survive   4 floors with 10 eggs
						// when trying floor #6
						//      --  break:    5 floors with 9 eggs    * this one is bigger in this case
						//      --  survive:  3 floors with 10 eggs
                        // 
						//
						// just need to find the minimum among { bigger of case#1, bigger of case #2 (4 floors with 9 eggs), bigger of case #3 (5 floors with 9 eggs) }
						// obviously "4 floors with 9 eggs" is smaller than "5 floors with 9 eggs"
						// 
						// 

						int firstTry = fl / 2 + 1;
						movesNeededIfFirstTryBreak = r[eg - 1][firstTry - 1] + 1;
						movesNeededIfFirstTrySurvive = r[eg][fl - firstTry] + 1;
						minimum = max(movesNeededIfFirstTryBreak, movesNeededIfFirstTrySurvive);

						for (int i = firstTry - 1; i >= 2; --i)
						{
							movesNeededIfFirstTryBreak = r[eg - 1][i - 1] + 1;
							movesNeededIfFirstTrySurvive = r[eg][fl - i] + 1;
							int m = max(movesNeededIfFirstTryBreak, movesNeededIfFirstTrySurvive);
							if (m < minimum)
							{
								minimum = m;
							}
							else
							{
								// the result of trying at this floor is already not affecting the overall result
								// keep lowering the first try floor will have less chance to affect the overall result
								// when keep lowering the first try, the "moves needed if first try break" decreases (5 eggs with fewer and fewer floors)
								// when keep lowering the first tyr, the "moves needed if first try survive" increases ( 6 eggs with more and more floors)
								// 
								if (movesNeededIfFirstTryBreak <= movesNeededIfFirstTrySurvive)
								{
									break;
								}
							}
						}
					}
					r[eg][fl] = minimum;
				}
			}
			return r[k][n];
		}
	};

	// accepted but too slow 1183 ms beats 5%
	class Solution4
	{
		int solve(vector<vector<int>>& r, int egg, int floor)
		{
			if (r[egg][floor] != -1)
			{
				return r[egg][floor];
			}
			int firstTry;
			if (floor % 2 == 0)
			{
				firstTry = floor / 2;
			}
			else
			{
				firstTry = floor / 2 + 1;
			}
			int minimum;
			int movesNeededIfFirstTryBreak;
			int movesNeededIfFirstTrySurvive;
			movesNeededIfFirstTryBreak = solve(r, egg - 1, firstTry - 1) + 1;
			movesNeededIfFirstTrySurvive = solve(r, egg, floor - firstTry) + 1;
			minimum = max(movesNeededIfFirstTryBreak, movesNeededIfFirstTrySurvive);

			// the scenario above (firstTry = floor / 2 or floor /2 + 1) is the most likely
			// scenario to get the minimum move with this many eggs.
			// but try the lower floors for first drop, once on a certain floor, the "moves" needed to solve the problem
			// is no longer less than current optimal result, stop the search, because the farther away from middle,
			// the more moves needed to solve the problem.

			for (int i = firstTry - 1; i >= 2; --i)
			{
				movesNeededIfFirstTryBreak = solve(r, egg - 1, i - 1) + 1;
				movesNeededIfFirstTrySurvive = solve(r, egg, floor - i) + 1;
				int m = max(movesNeededIfFirstTryBreak, movesNeededIfFirstTrySurvive);
				if (m < minimum)
				{
					minimum = m;
				}
				else
				{
					if (movesNeededIfFirstTryBreak <= movesNeededIfFirstTrySurvive)
					{
						break;
					}
				}
			}
			r[egg][floor] = minimum;
			return minimum;
		}

	public:
		int superEggDrop(int k, int n)
		{
			vector<vector<int>> records(k + 1, vector<int>(n + 1, -1));
			iota(records[1].begin(), records[1].end(), 0);
			for (int i = 1; i <= k; ++i)
			{
				records[i][0] = 0;
				records[i][1] = 1;
			}
			records[0][0] = 0;

			return solve(records, k, n);
		}
	};

	// accepted 17ms beats 79%
	class Solution5
	{
		int solve(vector<vector<int>>& r, int eg, int fl)
		{
			if (r[eg][fl] != -1)
			{
				return r[eg][fl];
			}
			if (fl == 0 && eg != 0)
			{
				r[eg][0] = 0;
				return 0;
			}
			if (eg == 1 && fl >= 1)
			{
				r[eg][fl] = fl;
				return fl;
			}
			if (fl == 1 && eg >= 1)
			{
				r[eg][fl] = 1;
				return 1;
			}
			int movesNeededIfFirstTryBreak;
			int movesNeededIfFirstTrySurvive;
			int firstTry;
			
			int lo = 1;
			int hi = fl;
			int minAbsoluteDiff = fl;
			int minimumMoves;
			while (true)
			{
				firstTry = lo + (hi - lo) / 2;
				movesNeededIfFirstTryBreak = solve(r, eg - 1, firstTry - 1) + 1;
				movesNeededIfFirstTrySurvive = solve(r, eg, fl - firstTry) + 1;
				if (movesNeededIfFirstTryBreak == movesNeededIfFirstTrySurvive)
				{
					r[eg][fl] = movesNeededIfFirstTryBreak;
					return r[eg][fl];
				}
				else
				{
					int ab = abs(movesNeededIfFirstTryBreak - movesNeededIfFirstTrySurvive);

					if (ab < minAbsoluteDiff)
					{
						minAbsoluteDiff = ab;
						minimumMoves = max(movesNeededIfFirstTryBreak, movesNeededIfFirstTrySurvive);
					}
					if (movesNeededIfFirstTryBreak > movesNeededIfFirstTrySurvive)
					{
						hi = firstTry - 1;	
					}
					else
					{
						lo = firstTry + 1;
					}
					if (hi < lo)
					{
						break;
					}
				}
			}
			r[eg][fl] = minimumMoves;
			return r[eg][fl];
		}
	public:
		int superEggDrop(int k, int n)
		{
			int movesIfEnoughEggs = (int)(floor(log2((float)n))) + 1;
			if (k >= movesIfEnoughEggs)
			{
				return movesIfEnoughEggs;
			}
			vector<vector<int>>r(k + 1, vector<int>(n + 1, -1));
			return solve(r, k, n);
		}
	};

	void Test_0887_Super_Egg_Drop()
	{
		//Solution s;
		Solution5 s2;
		int k, n;
		while (true)
		{
			cout << "Floors: ";
			cin >> n;
			if (n == 0) break;
			cout << "Eggs: ";
			cin >> k;
			//std::cout << s.superEggDrop(k, n) << "   |   " << s2.superEggDrop(k, n) << '\n';
			std::cout << s2.superEggDrop(k, n) << '\n';
		}
	}
}