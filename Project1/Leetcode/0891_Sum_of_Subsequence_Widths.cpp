#include "LibHeaders.h"

namespace _0891_Sum_of_Subsequence_Widths {

	using namespace std;

	// time limit exceeded
	class Solution
	{
		/*
		* If the sorted array is something like this:  a, b, c, d, e, f, g
		* Let's say I take a 3-element subsequence from the sorted array, I get "adg".
		* These 3 elements must be in the original input in *some* order. If I take
		* all possible subsequences from the original input, this sequence "adg"
		* will be picked out sooner or later. Since I'm just considering the max
		* and min in a subsequence, so considering subsequences in the sorted array
		* is just as good as considering subsequences in the original input.
		* Now the sorted array is this: a, b, c, d, e, f, g
		*                                  |           |
		* If I pick 'b' and 'f' as the min and max of a subsequence, how many possible
		* subsequences can I form? Basically pick elements in between 'b' and 'f'.
		* How many ways to pick? If there are 3 elements in between, then I have 2^3 ways
		* to pick. All these subsequences have the same width.
		* So fix the min, fix the max (to the right of min), count the elements in between
		* (for example there are k elements in between) the same width will repeat 2^k times.
		*/
	public:
		int sumSubseqWidths(vector<int>& nums)
		{
			int base = 1'000'000'007;
			sort(nums.begin(), nums.end());
			int len = nums.size();
			unsigned long long s = 0;
			for (int L = 1; L <= len - 1; ++L) // number of hops from segment start to segment end, 1-element subsequence has 0 hop, but 1-element subsequence's width=0
			{
				for (int i = 0; i <= len - L - 1; ++i)   // len = 9;   0,1,2,3,4,5,6,7,8  if L=3, the maximum of i is 5
				{
					int j = i + L;
					int width = nums[j] - nums[i];
					if (width > 0)
					{
						int between = L - 1;
						auto count = static_cast<unsigned long long>(pow(2, between));
						//s += (width * count);
						//s %= base;
						auto k = static_cast<decltype(count)>(1);
						while (k <= count)
						{
							s += width;
							s %= base;
							++k;
						}
					}
				}
			}
			return s;
		}
	};

	// Time limit exceeded, but 64 of 68 test cases passed, this is perhaps close to a correct solution.
	class Solution2
	{
	public:
		int sumSubseqWidths(vector<int>& nums)
		{
			int base = 1'000'000'007;
			sort(nums.begin(), nums.end());
			int len = nums.size();
			unsigned long long s = 0;
			auto count = static_cast<decltype(s)>(1); // number of subsequences when the min and max are fixed
			auto total = static_cast<decltype(s)>(0); // total width of all subsequences for a certain pair of min and max.
			for (int L = 1; L <= len - 1; ++L)
			{
				total = 0;
				for (int i = 0; i <= len - L - 1; ++i)
				{
					total += nums[i+L] - nums[i];
				}
				total %= base;
				count %= base;

				s += (total * count);
				s %= base;

				count *= 2;
			}
			return s;
		}
	};


	// accepted 35ms beats 49%, memory 60MB beats 39%
	class Solution3
	{
	public:
		int sumSubseqWidths(vector<int>& nums)
		{
			if (nums.size() == 1)
				return 0;

			if (nums.size() == 2)
				return abs(nums[0] - nums[1]);

			int base = 1'000'000'007;
			sort(nums.begin(), nums.end());
			int len = nums.size();
			vector<unsigned>count(len - 1, 0ull);
			for (int i = 0; i <= len - 2; ++i)
			{
				nums[i] = nums[i + 1] - nums[i];
			}

			count[0] = 1;
			for (int i = 1; i <= len - 2; ++i)
			{
				count[i] = count[i - 1] * 2;
				count[i] %= base;
			}

			// geometric series sum
			// a1 * ( 1 - q^n) / (1 - q)
			// a1 = 1, q = 2, n = len - 1

			// count[0] = 2^0
			// count[1] = 2^1
			// count[len-2] = 2^(len-2)
			// need 2^(len-1)
			//
			// if nums.size == 100
			// coefficients: 1, 2, 4, 2^3, ... 2^98
			// a subsequence has the min and max of the original array, there are other 98 elements, their appearences count for 2^98 different combinations.
			// 2^0 + 2^1 + ... + 2^98 (99 items in total)

			unsigned long long qn = count[len - 2] * 2;
			qn %= base;

			unsigned long long geometricSeriesSum = qn - 1;

			unsigned long long widthSum = 0;

			int i = 0, j = len - 2;
			auto coeffSum = geometricSeriesSum;

			// if the original input nums has only 2 elements
			// the diff array has only 1 element
			// count array has only 1 element
			// len = 2
			// in the very first iteration, I have i = 0 and j = 0
			// i and j point to the same element in the very first iteration
			// execution falls into the "if (i == 0)" block.
			// The correct way is to use that 1 element multiply coeffSum.
			// But this code will use nums[i] + nums[j], which doubles that 1 element, therefore it's wrong in case the original array only has 2 elements

			while (i <= j)
			{
				if (i == 0)
				{
					widthSum += (nums[i] + nums[j]) * coeffSum;
					widthSum %= base;
				}
				else
				{
					geometricSeriesSum = geometricSeriesSum + 2 * base - count[i - 1] - count[j + 1];
					coeffSum += geometricSeriesSum;
					coeffSum %= base;
					if (i < j)
						widthSum += (nums[i] + nums[j]) * coeffSum;
					else
						widthSum += nums[i] * coeffSum;
					widthSum %= base;
				}

				++i;
				--j;
			}
			return widthSum;
		}
	};

	void Test_0891_Sum_of_Subsequence_Widths()
	{
		vector<int> nums;
		string numsInString;
		Solution3 s;
		while (true)
		{
			nums.clear();
			numsInString.clear();
			cout << "array:";
			getline(cin, numsInString);
			if (numsInString.size() == 0) break;
			istringstream ss(numsInString);
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(nums));
			cout << s.sumSubseqWidths(nums) << '\n';
		}
	}
}