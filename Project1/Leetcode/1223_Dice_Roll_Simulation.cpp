#include<vector>
#include<iostream>

namespace _1223_Dice_Roll_Simulation {

	using namespace std;

	/*
	* If the rollMax is this: [1, 1, 1, 2, 2, 3]
	* Let's say I roll the die 5 times, I get a number of 5-digit sequences.
	* for example: 12345, 13442, 35226, 61321, ...
	* For all the sequences that comply with the no-more-consecutive-than-roll-max rule,
	* I can categorize all the valid sequences into these groups:
	* (1). ends with a '1'
	* (2). ends with a '2'
	* (3). ends with a '3'
	* (4). ends with one '4'
	* (5). ends with two '4's
	* (6). ends with one '5'
	* (7). ends with two '5's
	* (8). ends with one '6'
	* (9). ends with two '6's
	* (10). ends with three '6's
	* 
	* Let's say count[5] means the number of valid sequences in group (5).
	* 
	* Compare this problem with 1220 Count Vowels Permutation.
	* 
	* From 5-digit sequences to 6-digit sequences, there are a set of rules when appending one more digit.
	* For sequences in group (1), I can append    2, 3, 4, 5, 6 to it
	* For sequences in group (2), I can append 1,    3, 4, 5, 6 to it
	* For sequences in group (3), I can append 1, 2,    4, 5, 6 to it
	* For sequences in group (4), I can append 1, 2, 3, 4, 5, 6 to it
	* For sequences in group (5), I can append 1, 2, 3,    5, 6 to it
	* For sequences in group (6), I can append 1, 2, 3, 4, 5, 6 to it
	* For sequences in group (7), I can append 1, 2, 3, 4,    6 to it
	* For sequences in group (8), I can append 1, 2, 3, 4, 5, 6 to it
	* For sequences in group (9), I can append 1, 2, 3, 4, 5, 6 to it
	* For sequences in group (10) I can append 1, 2, 3, 4, 5    to it
	* 
	* count[7][n] denotes the number of valid sequences in group (7) after rolling the die n times, i.e. n-digit sequence
	* 
	* count[7][n] = count[6][n-1]
	* the only way to get valid n-digit sequences in group (7) is to append another '5' to valid n-1-digit sequences in group (6)
	* 
	* Likewise, I can get all the correlation between valid n-digit sequences in each group and valid n-1-digit sequences in each group.
	* Then this becomes a more complex form of problem 1220 Count Vowels Permutation.
	* 
	*/

	class Solution
	{
	public:
		int dieSimulator(int n, vector<int>& rollMax)
		{

		}
	};
}