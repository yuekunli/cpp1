#include<iostream>
#include<sstream>
#include<string>
#include<vector>
#include<random>
#include<functional>

namespace _0154_Find_Min_in_Rotated_Sorted_Array_II{

	using namespace std;

	// 1 5 5 5 5
	// 5 5 5 5 7 1

	/*
	* 
	* Original array:
	*                            /
	*                           /
	*    /---------------------/
	*   /
	*  /
	* 
	* 
	* after rotation:
	* 
	*          /
	*         /
	*   -----/     --------------------
	*             /
	*            /
	*           /
	*                    |
	*                    a points to here. the surrounding elements don't provide any help in terms of where the minimum is.
	* 
	*/

	class Solution
	{
		size_t n;

		int solve(vector<int>& nums, size_t lo, size_t hi)
		{
			if (lo == hi && lo != n - 1) return nums[lo + 1];

			size_t a = (hi - lo) / 2 + lo;
			if (a != n - 1)
			{
				if (nums[0] <= nums[a] && nums[a] > nums[a + 1])
					return nums[a + 1];

				else if (nums[0] <= nums[a] && nums[a] < nums[a + 1])
					return solve(nums, a + 1, hi);

				else if (nums[0] < nums[a] && nums[a] <= nums[a + 1])  // must not combine this and previous situation, nums[0] == nums[a] == nums[a+1] doesn't belong to either case
					return solve(nums, a + 1, hi);

				// basically if there is difference between nums[0], nums[a], nums[a+1], and it's increasing
				// index 'a' is in the part that has been rotated to the front, so the original minimum point
				// is to the right of this. What if the rotation just ends with the same original array?
				// nums[0], nums[a], nums[a+1] meet such increasing condition, but minimum point is obviously
				// to the left of 'a'. It's Ok because I eventually will reach the end of the array and will
				// wrap around and return nums[0]

				else if (nums[0] > nums[a + 1] && nums[a] <= nums[a + 1])
					return solve(nums, lo, a - 1);

				else if (nums[0] >= nums[a + 1] && nums[a] < nums[a + 1])
					return solve(nums, lo, a - 1);

				else // nums[0] == nums[a] == nums[a+1]  I don't know if I should go to the left or the right, What do I do? ---  Just solve both sides, take the smaller result.
				{
					int b1 = a < hi ? solve(nums, a + 1, hi) : nums[a];
					if (b1 < nums[0])
						return b1;
					else
					{
						int b2 = lo < a ? solve(nums, lo, a - 1) : nums[a];
						return min(b1, b2);
					}
				}
			}
			else
			{
				return nums[0];
			}
		}

	public:
		int findMin(vector<int>& nums)
		{
			n = nums.size();
			return solve(nums, 0, n - 1);
		}
	};



	/*
	* The decision is driven by the relation between nums[0] nums[a] and nums[a+1]
	* Can I alternatively using nums[0], nums[a], and nums[end-1] to make decision?
	*/



	/**
	*                   .
	*                 . .
	*               . . .
	*           . . . . .
	*     . . . . . . . .
	*   . . . . . . . . .
	*   . . . . . . . . .     .
	*   . . . . . . . . .   . .
	*   . . . . . . . . . . . .
	* 
	*   |<------------->| |<->|
	*     uphill            truth segment
	*   
	*   Very likely that the minimum is rotated to the back of the array.
	*   Call the first ascending segment "uphill segment.
	*   As long as I'm in the uphill segment, I should keep moving to the right.
	*   What will make this a wrong decision?
	*   Only when the rotations shift the array back to its original position (as if there is no rotation)
	*   the minimum is the first element, the entire array is one uphill segment.
	*   The handling of such corner case is to keep moving to the right and if I'm standing on the last element,
	*   make sure to compare the last and the first element.
	* 
	*   I start at the first element, jump to the middle, compare the preivous position (first element in this case) with the current position.
	*   If current is strictly greater than the previous position, I must be still in uphill segment. Abandon the left half, jump half of the remaining array.
	* 
	*	If current is equal previous position, it's hard to tell.
	* 
	*   . . .     . . . . . . .
	*   . . .   . . . . . . . .
	*   . . . . . . . . . . . .
	* 
	*               ^
	*              mid
	* 
	*   There is an extreme case:
	*   5 5 5 5 5 5 5 1 5 5 5 5 5 5 5 5 5.....5 5 5 5 5 
	* 
	* 
	*   The array has many many 5's and only one 1.
	*/

	// verified 0ms beats 100%

	class Solution2
	{
		int solve(vector<int> const& n, size_t lo, size_t hi)
		{
			size_t jumpTo = 0;

			while (true)
			{
				if (lo == hi)
				{
					return n[lo];
				}

				if (lo + 1 == hi)
				{
					return min(n[lo], n[hi]);
				}

				if (n[lo] < n[hi])
				{
					// the entire segment is in the uphill segment, so "lo" must be the minimum.
					// This also eliminate the possiblity for the following code that when I keep moving toward the right side,
					// the minimum is actually the first element.
					return n[lo];
				}


				jumpTo = (hi - lo) / 2 + lo;

				if (n[jumpTo - 1] > n[jumpTo] && n[jumpTo] <= n[jumpTo + 1])  // How do I know jumpTo must not be the first or last?
				{                                                             // in another word, how do I know jumpTo-1 and jumpTo+1 don't underflow or overflow?
					                                                          // this relies on the first 3 condition checks, basically, 
																			  // if jumpTo is at the either end of the array, "lo" and "hi" are either equal
																			  // or differ by 1.
					// n[jumpTo] <= n[jumpTo + 1]
					// the "equal" in this condition is very important
					// example: 2 2 1 1 1
					// jumpTo point to right the middle, which is the original first element before rotation
					return n[jumpTo];
				}


				if (n[lo] < n[jumpTo])
				{
					lo = jumpTo + 1;
					continue;
				}

				if (n[lo] > n[jumpTo])
				{
					hi = jumpTo - 1;
					continue;
				}

				if (n[lo] == n[jumpTo])
				{
					// I'm in the "I-don't-know-where-to-go" case. I can just solve both halves now
					// but in order to reduce operations to my best effort
					// I can check some special case. Basically comparing jumpTo with "hi" end.
					// I only need to handle n[jumpTo] > n[hi]. I don't need to handle n[jumpTo] < n[hi]
					// because if n[jumpTo] < n[hi], n[lo] is also less than n[hi] (the condition to reach here is that n[lo] == n[jumpTo])
					if (n[jumpTo] > n[hi])
					{
						/*
						*   - - - - - - - - - - - - - 
						*                                      -
						*                                    /
						*                                   /
						*                                  /
						*                                 -
						*                               /
						*                             -
						*  |----uphill---------------|-----truth--|
						*                      |
						*                   jumpTo
						*/

						lo = jumpTo + 1;
						continue;

					}
					else
					{
						int b1 = solve(n, lo, jumpTo);
						int b2 = solve(n, jumpTo + 1, hi);
						return min(b1, b2);
					}
				}
			}
		}


	public:
		int findMin(vector<int>& n)
		{
			return solve(n, 0, n.size() - 1);
		}
	};



	template<typename SolutionType>
	class AutoTest
	{
		random_device rd;
		mt19937 gen;
		uniform_int_distribution <int> uid;
		SolutionType & s;
	public:
		AutoTest(SolutionType & _s)
			:gen(rd()),
			uid(1, 2),
			s(_s)
		{}

		AutoTest() = delete;

		void generateArray(vector<int>& arr)
		{
			arr.assign(10,0);
			generate(arr.begin(), arr.end(), std::bind(uid, ref(gen)));
			sort(arr.begin(), arr.end());
			int ro = gen() % 10;
			rotate(arr.begin(), arr.begin() + ro, arr.end());
		}

		void run()
		{
			vector<int> a;
			int i = 0;
			while (i < 3000)
			{
				generateArray(a);
				int correct = *min_element(a.begin(), a.end());
				int test = 0;
				try
				{
					test = s.findMin(a);
				}
				catch (std::runtime_error e)
				{
					copy(a.begin(), a.end(), ostream_iterator<int>(std::cout, " "));
					cout << "\n";
					cout << test << "\n\n";
					return;
				}
				if (test != correct)
				{
					copy(a.begin(), a.end(), ostream_iterator<int>(std::cout, " "));
					cout << "\n";
					cout << test << "\n\n";
					return;
				}
				i++;
			}
			cout << "all pass\n\n";
		}
	};

	void Test_0154_Find_Min_in_Rotated_Sorted_Array_II()
	{
		Solution2 s;
		AutoTest ats{s};
		string s1;
		vector<int>nums;
		int option;
		while (true)
		{
			cout << "1: manual test    2: auto test    0: exit  > ";
			cin >> option;
			cin.ignore();
			if (option == 1)
			{
				s1.clear();
				cout << "array: ";
				getline(cin, s1);
				stringstream ss(s1);
				nums.clear();
				copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(nums));
				cout << s.findMin(nums) << "\n\n";
			}
			else if (option == 2)
			{
				ats.run();
			}
			else if (option == 0)
				break;
		}
	}
}