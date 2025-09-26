#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<algorithm>
#include<numeric>
#include<cstdint>

namespace _0493_Reverse_Pairs {

	using namespace std;

	class Solution   // this solution is unnecessarily complex, give it up.
	{

		void addToBIT(vector<int>& bit, int index)
		{
			while (index < bit.size())
			{
				bit[index]++;
				index = index + (index & (-index));
			}
		}

		int getFromBIT(vector<int>& bit, int index)
		{
			int total = 0;
			while (index > 0)
			{
				total += bit[index];
				index = index - (index & (-index));
			}
			return total;
		}
	public:
		int reversePairs(vector<int>& nums)
		{
			size_t n = nums.size();

			unordered_set<signed long long> seen(nums.begin(), nums.end());
			vector<long long>composite(n);
			transform(nums.begin(), nums.end(), composite.begin(), [](int& a) {return (long long)a; });
			for (int i = 0; i < n; i++)
			{
				long long k = (long long)nums[i];
				if (seen.count(k * 2) == 0)
				{
					composite.emplace_back(k * 2);
					seen.emplace(k * 2);
				}
			}
			size_t n2 = composite.size();
			vector<int>indexes(n2);
			iota(indexes.begin(), indexes.end(), 0);
			sort(indexes.begin(), indexes.end(), [&](int& a, int& b) {return composite[a] < composite[b]; });
			vector<int>indexesInSortedOrder(n2);
			for (int i = 0; i < n2; i++)
			{
				int a = indexes[i];
				indexesInSortedOrder[a] = i + 1; // remember the indexes in binary indexed tree starts from '1'
			}
			vector<int>bit(n2 + 1);

			int validPairs = 0;
			for (int i = 0; i < n; i++)
			{
				int value = nums[i];
				long long target = (long long)nums[i] * 2;
				auto it = lower_bound(indexes.begin(), indexes.end(), target,
					[&](int const & i, long long const & t)
					{
						return composite[i] < t;
					});
				int indexOfTarget = (int)distance(indexes.begin(), it)+1;// remember the indexes in binary indexed tree starts from '1'
				int indexOfValue = indexesInSortedOrder[i];

				int count = getFromBIT(bit, indexOfTarget);
				count = i - count; // if 'i' is '7', I already put 7 elements in an imaginary array,
				                   // if there are 4 less than or equal to target, there should be 3 greater than target
				validPairs += count;
				
				addToBIT(bit, indexOfValue);
			}

			return validPairs;
		}
	};

	class Solution2
	{
		/*
		* same algorithm as Solution 1.
		* constraint: -2^31 <= nums[i] <= 2^31-1
		* if a number is too close to 2^31 - 1, it won't have twice of it.
		* If a number is too close to -2^31, every other number is greater than twice of it.
		*/
	};


	class Solution3  // accepted by leetcode, 527ms beat 40%
	{
		void addToBIT(vector<int>& bit, int index)
		{
			while (index < bit.size())
			{
				bit[index]++;
				index = index + (index & (-index));
			}
		}

		int getFromBIT(vector<int>& bit, int index)
		{
			int total = 0;
			while (index > 0)
			{
				total += bit[index];
				index = index - (index & (-index));
			}
			return total;
		}
	public:
		int reversePairs(vector<int>& nums)
		{
			size_t n = nums.size();

			unordered_map<long long, size_t>m;

			vector<long long>composite;  // this vector has the original input numbers and their multiply of 2, but no duplicate

			for (int i = 0; i < n; i++)
			{
				if (!m.contains(nums[i]))
				{
					m[nums[i]] = 0;  // this is just to create an entry in the map
					composite.emplace_back(nums[i]);
				}
				if (m.count((long long)nums[i] * 2) == 0)
				{
					m[(long long)nums[i] * 2] = 0;
					composite.emplace_back((long long)nums[i] * 2);
				}
			}

			sort(composite.begin(), composite.end());
			size_t n2 = composite.size();
			for (size_t i = 0; i < n2; i++)
			{
				long long a = composite[i];
				m[a] = i;  // if say m[3592] = 4, it means 3592 is the 5th (0-indexed) smallest number. 3592 can be one of the original numbers or twice multiply of an original number or even both.
			}

			vector<int>bit(n2 + 1);
			int validPairs = 0;
			for (int i = 0; i < n; i++)
			{
				int a = nums[i];
				long long b = (long long)a * 2;
				size_t aIndex = m[a]+1;
				size_t bIndex = m[b]+1;

				int count = getFromBIT(bit, (int)bIndex);
				// counting the number of numbers that are smaller than 'b', those numbers should appear before 'a' in the original vector.
				// original vector:  5, 9, 4
				// '5' and '9' are in the BIT. Now I'm on '4' whose 2-time multiple is '8'. I know 8 has an index of 'x' in the composite array.
				// Got to the BIT array, start at index 'x'. Iteratively check responsible-range, going from index 'x' all the way to index '0'.
				// Every element in the BIT tells how many numbers have tallied in its responsible range.
				// there is 1 number ('5') that is smaller than it.
				// '4' is the 3rd number in the original vector, so i is '2'. 2 is also the number of numbers that appear before '4' in the original vector.
				// So 2 numbers appear before '4', one of them is smaller than '8', so 2-1=1 of them is greater than '8'.
				// that number (the one greater than '8'), together with '4' form a "reverse-pair"
				validPairs += (i - count);

				addToBIT(bit, (int)aIndex);
			}
			return validPairs;
		}
	};

	// accepted 109ms beat 94%
	class Solution4
	{
		void addToBIT(vector<int>& bit, int index)
		{
			while (index < bit.size())
			{
				bit[index]++;
				index = index + (index & (-index));
			}
		}

		int getFromBIT(vector<int>& bit, int index)
		{
			int total = 0;
			while (index > 0)
			{
				total += bit[index];
				index = index - (index & (-index));
			}
			return total;
		}
	public:
		int reversePairs(vector<int>& nums)
		{
			vector<int> originalInputCopy = nums;

			size_t n = nums.size();

			// value to index mapping
			// value is either "the value of an element in the original array" or "the value of an element in the original array multiplying 2"
			unordered_map<int64_t, int>m;  // input length maximum is 5 * 10^4, merging two arrays equally long, 2 times that is 10^5, an "int" should be enough

			sort(nums.begin(), nums.end());

			int i = 0, j = 0;

			int64_t original, multiple2;

			original = (int64_t)nums[i];
			multiple2 = original * 2;

			int64_t previous;
			int index = 0;

			// Do a merge sort, one array is the sorted original input "nums",
			// another array consists of every element of nums multiplied by 2.
			// don't really need to construct the 2nd array
			// Don't really need to construct the final merged array either,
			// all I care about is the elements' indices in the merged array.
			// One extra requirement than a plain merge sort is "no duplicate in the merged array"

			if (original <= multiple2)
			{
				m[original] = index++;
				previous = original;
				++i;
				if (i < n) // even have to be defensive here, if input is a 1-element array, need this check here
					original = (int64_t)nums[i];
			}
			else //(multiple2 < original)
			{
				m[multiple2] = index++;
				previous = multiple2;
				++j;
				if (j < n)
					multiple2 = (int64_t)nums[j] * 2;
			}

			while (i < n && j < n)
			{
				if (original <= multiple2)
				{
					if (original != previous)
					{
						m[original] = index++;
						previous = original;
						++i;
						if (i < n)
							original = (int64_t)nums[i];
					}
					else
					{
						++i;
						if (i < n)
							original = (int64_t)nums[i];
					}
				}
				else // (multiple2 < original)
				{
					if (multiple2 != previous)
					{
						m[multiple2] = index++;
						previous = multiple2;
						++j;
						if (j < n)
							multiple2 = (int64_t)nums[j] * 2;
					}
					else
					{
						++j;
						if (j < n)
							multiple2 = (int64_t)nums[j] * 2;
					}
				}
			}

			while (j < n)
			{
				multiple2 = (int64_t)nums[j] * 2;

				if (multiple2 != previous)
				{
					m[multiple2] = index++;
					previous = multiple2;
					++j;
				}
				else
				{
					++j;
				}
			}
			
			while (i < n)
			{
				original = (int64_t)nums[i];
				if (original != previous)
				{
					m[original] = index++;
					previous = original;
					++i;
				}
				else
				{
					++i;
				}
			}
			
			// at this point, "index" is exactly the number of elements added to the unordered_map, don't decrease "index"
			index;

			vector<int>bit(index + 1);
			int validPairs = 0;
			for (int k = 0; k < n; ++k)
			{
				auto a = originalInputCopy[k];
				auto b = (int64_t)a * 2;
				auto aIndex = m[a] + 1;
				auto bIndex = m[b] + 1;

				int count = getFromBIT(bit, bIndex);

				validPairs += (k - count);

				addToBIT(bit, aIndex);
			}
			return validPairs;
		}
	};



	/*
	*  augument original array so that each element carries a small structure
	*  
	* 
	* 
	*       +-- index of this element in the sorted composite array (for definition of "composite array" see Solution3)
	*       |
	*       +-- index of this element's multiple of 2 in the sorted composite array
	*       |
	*       |
	*      +---+
	*      |   |
	*      +---+
	*           \
	*         an element in the original input array
	* 
	* 
	* make yet another copy of the input array:
	* 
	*       +-- index of this element in the original array
	*       |
	*       |
	*    +-----+
	*    |     |
	*    +-----+
	* 
	*   for example if the input is 1 6 9 2 4
	* 
	*   make the array this way:
	*  +---+
	*  | 0 |  1   2   3   4
	*  | 1 |  6   9   2   4
	*  +---+
	* 
	*   after sorting by value:
	*   0  3  4  1  2
	*   1  2  4  6  9
	*/
	class Solution5
	{

	};


	class BruteForseSolution
	{
	public:
		int reversePairs(vector<int>& nums)
		{
			size_t n = nums.size();
			int validPairs = 0;
			for (int i = 0; i < n; i++)
			{
				for (int j = i + 1; j < n; j++)
				{
					if (nums[i] > 2 * nums[j])
						validPairs++;
				}
			}
			return validPairs;
		}
	};


	void Test_0493_Reverse_Pairs()
	{
		//Solution solu;
		Solution4 solu;

		string s;

		vector<int>nums;

		while (true)
		{
			s.clear();
			cout << "nums: ";
			getline(cin, s);
			if (s.size() == 0) break;
			stringstream ss(s);
			nums.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(nums));
			cout << solu.reversePairs(nums) << "\n\n";
		}
	}
}