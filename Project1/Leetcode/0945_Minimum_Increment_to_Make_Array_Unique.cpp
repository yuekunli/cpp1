#include<vector>
#include<algorithm>
#include<string>
#include<iostream>
#include<sstream>

namespace _0945_Minimum_Increment_to_Make_Array_Unique {

	using namespace std;

	/*
	* if, after sorting, the array is like this:
	*     1    1    2    2    3    7
	* 
	*     1    2    3    4    5    7   <-  this is the final result
	* 
	* to achieve the final result, going from smallest to the biggest element,
	* every element should be at least 1 bigger than the previous.
	* So if the 1st element is 1, the minimum for the 2nd element to be in order
	* to make it unique is 2, if the actually value is less than 2, than that means
	* I need to increment it. As to increment by how many, that is just the diff
	* between that minimum and the actual value.
	* If an element is a jump from the previous element, i.e. value non-contiguous and
	* greater than the previous (after increment) by more than 1, than that value can
	* remain the same. and the minimum for the next value would be current element plus 1.
	*/

	//accepted 30ms beat 87%
	class Solution
	{
	public:
		int minIncrementForUnique(vector<int>& nums)
		{
			sort(nums.begin(), nums.end());
			auto len = nums.size();
			int minimum = nums[0] + 1;
			int incrementsNeeded = 0;
			for (int i = 1; i < len; ++i)
			{
				if (minimum > nums[i])
				{
					incrementsNeeded += (minimum - nums[i]);
					++minimum;
				}
				else
				{
					minimum = nums[i] + 1;
				}
			}
			return incrementsNeeded;
		}
	};



	/*
	* Is there a way that doesn't require the sorting?
	* get the min and max of the array.
	* turn the array into a min heap.
	* take the head of the heap one by one.
	* I know the total count of the initial array.
	* At any point I know the count of the elements left in the heap.
	* If at some point, the head returns 50, I know the max of the input array is 55.
	* And I know at that point, there are 12 elements left in the heap.
	* Than I know that these 12 elements have a lot duplicates.
	* I know the final result for these 12 elements will be: 50, 51, 52, ..., 61.
	* But in order to know exactly how many increments I need to make, I still need to see
	* every element of these 12.
	* 
	* Actually the final resual may not even be 50, 51, 52, ..., 61.
	* If these 12 elements are 50, 54, 54, 54, 54, ..., 55 (there are 10 "54"s in the middle)
	*/


	/*
	* This solution is derived from a quick browsing of the fastest solution given on Leetcode
	* This is a little bit cheating, the sorting is essentially done by counting sort.
	* The constraints are:
	* 1 <= nums.length <= 10^5
	* 0 <= nums[i] <= 10^5
	* Essentially saying the the range of each element's value is the same as the upper limit of
	* the size of the input.
	* So if the size of the input is 8000, and the max value of the input is about 8000,
	* then if I create a counts (occurrences, or frequence) array to count each values' occurrence.
	* the size of such array is also about 8000.
	* (Counting sort won't be a good approach if the value range is much bigger than the initial input size)
	* Then iterate the counts array.
	* If I see element with value 7 appear 5 times, I know these 5 elements eventually should be in an
	* arithemic series: 7, 8, 9, 10, 11. Calculate the sum of this arithemic series, and calculate the sum
	* of 5 sevens, the diff is the increments needed.
	* After such calculation, the minimum for the next element should 12. If the next element I encounter is
	* for example, 25, then raise that minimum to 25, check the count of value 25, if there are more than one,
	* the repeat the same process.
	* If after those 5 sevens, the next element I encounter is 10, and there are 4 tens.
	* These 4 tens should eventually be in such series: 12, 13, 14, 15, because the minimum for the start
	* of this series is 12. Again calculate the sum of the arithemic series and 4 tens, calculate the diff.
	*/
	// accepted 0ms beat 100%
	class Solution2
	{
	public:
		int minIncrementForUnique(vector<int>& nums)
		{
			auto max_it = max_element(nums.begin(), nums.end());
			int maxValue = *max_it;

			vector<int>count(maxValue+1, 0);

			for (auto const& n : nums)
			{
				++count[n];
			}

			int incrementsNeeded = 0;
			int minimum = -1;
			for (int i = 0; i <= maxValue; ++i)
			{
				if (count[i] > 0)
				{
					if (minimum == -1)
					{
						minimum = i;
					}

					if (i > minimum)
					{
						minimum = i;
					}
					auto arithemicSeriesTotal = (minimum + (minimum + count[i] - 1)) * count[i] / 2;
					incrementsNeeded += (arithemicSeriesTotal - i * count[i]);

					minimum = minimum + count[i];
				}
			}
			return incrementsNeeded;
		}
	};


	void Test_0945_Minimum_Increment_to_Make_Array_Unique()
	{
		int option;
		Solution2 so;
		string wholeInput;

		while (true)
		{
			cout << "input array: ";
			getline(cin, wholeInput);
			if (wholeInput.size() == 0)
			{
				return;
			}
			stringstream ss(wholeInput);
			vector<int>nums;
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(nums));
			cout << so.minIncrementForUnique(nums) << "\n";
		}
	}
}