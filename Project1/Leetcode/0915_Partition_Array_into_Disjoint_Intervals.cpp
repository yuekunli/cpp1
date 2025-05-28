#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<algorithm>

namespace _0915_Partition_Array_into_Disjoint_Intervals {

	using namespace std;

	/*
	*    x x x x x x x  o o o o o o  u
	*    \___________/  \_________/  |
	*     
	* 
	* Let's say I have the max of the 'x' segment, I'm looking at the first 'o', it's greater than the max of 'x' segment,
	* I keep moving forward, every one in 'o' segment is greater than the max of 'x' segment, I also keep record of the max
	* of the 'o' segment. The gap between 'x' and 'o' has the potential to become the partitioning cut I'm looking for.
	* All of sudden, 'u' is less than the max of 'x' segment, the cut definitely has to move, it has to move to the gap right
	* after 'u' at least. Then what is the max of the left part so far? The left part now consists of 'x' segment and 'o' segment
	* and 'u'. The max is of course, the bigger of max of 'x' segment and max of 'o' segment.
	*/
	// accepted 186ms beats 10%, but memory is good 94MB, beats 100%
	class Solution
	{
	public:

		int partitionDisjoint(vector<int>& nums)
		{
			size_t len = nums.size();
			if (len == 2)
				return 1;  // guaranteed a valid answer

			size_t potentialLeftEnd = 0;  // index of the end of left
			int maxOfLeft = nums[0];
			int maxSinceLeftEnd = nums[1];
			for (size_t i = 1; i < len; i++)
			{
				if (nums[i] >= maxOfLeft)
				{
					if (maxSinceLeftEnd < nums[i])
					{
						maxSinceLeftEnd = nums[i];
					}
				}
				else
				{
					potentialLeftEnd = i;
					if (maxOfLeft < maxSinceLeftEnd)
						maxOfLeft = maxSinceLeftEnd;
					if (i < len - 1)
						maxSinceLeftEnd = nums[i + 1];

				}
			}
			return (int)potentialLeftEnd + 1;
		}
	};


	// accepted 0ms beats 100%
	// only improvement over Solution1 is the early break out of the for loop.
	// But this actually iterate entire array at least once to find the max,
	// maybe using the built-in STL API max_element is fast, so the cost incurred by max_element
	// is neglectable comparing to the gain brought by "if (nums[i] == *maxIt)"
	class Solution2
	{
		int partitionDisjoint(vector<int>& nums)
		{
			auto maxIt = max_element(nums.cbegin(), nums.cend());
			size_t len = nums.size();
			if (len == 2)
				return 1;  // guaranteed a valid answer

			size_t potentialLeftEnd = 0;  // index of the end of left
			int maxOfLeft = nums[0];
			int maxSinceLeftEnd = nums[1];
			for (size_t i = 1; i < len; i++)
			{
				if (nums[i] >= maxOfLeft)
				{
					if (nums[i] == *maxIt)
					{
						// the max of the whole input array must be in the right partition
						// (similarly, the min of the whole array must be in the left. (althought this therom doesn't really matter here))
						// so if I alreay arrived at the max, the final answer can't go beyond that
						// so there is no point moving farther right, I should break out.
						break;
					}
					if (maxSinceLeftEnd < nums[i])
					{
						maxSinceLeftEnd = nums[i];
					}
				}
				else
				{
					potentialLeftEnd = i;
					if (maxOfLeft < maxSinceLeftEnd)
						maxOfLeft = maxSinceLeftEnd;
					if (i < len - 1)
						maxSinceLeftEnd = nums[i + 1];

				}
			}
			return (int)potentialLeftEnd + 1;

		}
	};

	void Test_0915_Partition_Array_into_Disjoint_Intervals()
	{
		vector<int> nums;
		string numsInString;
		int test;
		char p[] = "abc";
		Solution s;
		while (true)
		{
			cout << "test?";
			cin >> test;
			if (test == 0)break;
			nums.clear();
			numsInString.clear();
			cin.getline(p, 1);
			cout << "array: ";
			getline(cin, numsInString);
			istringstream ss(numsInString);
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(nums));
			cout << s.partitionDisjoint(nums) << '\n';
		}
	}

}