#include<vector>
#include<iostream>
#include<string>
#include<sstream>
#include<algorithm>
#include<forward_list>

namespace _0164_Maximum_Gap {

	using namespace std;

	class Solution1
	{
		// counting sort
		/*
		* memory limit exceeded
		* I'm creating an array from 0 to maxElement.
		* I should've narrowed it down to minElement to maxElement
		*/
	public:
		int maximumGap(vector<int>& nums)
		{
			size_t n = nums.size();
			if (n == 1) return 0;
			int maxElement = *max_element(nums.begin(), nums.end());
			vector<int>a(maxElement + 1, 0);
			for (int& c : nums)
			{
				a[c] = 1;
			}
			int previous = -1;
			int maxGap = 0;
			for (int i = 0; i <= maxElement; i++)
			{
				if (a[i] == 1)
				{
					if (previous != -1)
					{
						if (i - previous > maxGap)
							maxGap = i - previous;
					}
					previous = i;
				}
			}
			return maxGap;
		}
	};


	class Solution2
	{
		/*
		* still counting sort, with some improvement
		* but still Time Limit Exceeded
		*/
	public:
		int maximumGap(vector<int>& nums)
		{
			if (nums.size() == 1) {
				return 0;
			}

			int minElement = nums[0];
			int maxElement = nums[0];

			for (auto const& c : nums)
			{
				if (c < minElement)
				{
					minElement = c;
				}
				if (c > maxElement)
				{
					maxElement = c;
				}
			}

			int potentialDistinctiveElementCount = maxElement - minElement + 1;
			vector<bool>isOccurred = vector<bool>(potentialDistinctiveElementCount, false);
			for (auto const& c : nums)
			{
				isOccurred[c - minElement] = true;
			}

			int maxGap = -1;
			size_t pre = 0;
			size_t i = 0;
			while (!isOccurred[i])
			{
				++i;
			}

			pre = i;

			if (i == potentialDistinctiveElementCount - 1)
			{
				return 0;
			}
			++i;
			while (i < potentialDistinctiveElementCount)
			{
				if (isOccurred[i])
				{
					maxGap = max(maxGap, static_cast<int>(i - pre));
					pre = i;
				}
				++i;
			}

			return maxGap;
		}
	};

	class Solution3
	{
		/*
		* still counting sort,
		* what if I use a bit to represent if a number appeared in the original array
		* (actually vector<bool> internally uses bits to represent each boolean value in the vector)
		*/
		/*
		* When the input number are scattered far apart, this exceeds time limit.
		* too many time spent on checking zero bits.
		*/
	public:
		int maximumGap(vector<int>& nums)
		{
			size_t n = nums.size();
			if (n == 1) return 0;
			int maxElement = *max_element(nums.begin(), nums.end());
			int minElement = *min_element(nums.begin(), nums.end());
			int bitsNeeded = maxElement - minElement + 1;
			int intNeeded = bitsNeeded / 32 + 1;
			
			vector<int32_t>a(intNeeded, 0);
			for (int& c : nums)
			{
				int diff = c - minElement;
				int whichInt = diff / 32;
				int whichBit = diff - (whichInt * 32);
				int32_t oneBit = 1;
				a[whichInt] = a[whichInt] | (oneBit << whichBit);
			}
			// find the first non-zero bit;
			int i = 0;
			while (a[i] == 0)
				i++;
			int shift = 0;
			while ((a[i] & (1 << shift)) == 0)
				shift++;
			int previous = 32 * i + shift;

			
			int maxGap = 0;
			while(i<intNeeded)
			{
				int shift = 0;
				while (shift < 32)
				{
					if ((a[i] & (1 << shift)) != 0)
					{
						int number = 32 * i + shift;
						int gap = number - previous;
						maxGap = max(gap, maxGap);
						previous = number;
					}
					shift++;
				}
				i++;
			}
			return maxGap;
		}
	};


	/*********************************************************************************
	* All counting sort efforts failed, when elements are sparse
	* and min and max are very far apart, there are a lot of empty
	* spots between min and max, counting sort spends too much time on those empty spots
	***********************************************************************************/



	// this is accepted but using a 2000 bucketCount only leads to "218 ms beats 6%, memory: 110MB beats 11%"
	/*
	* Combine bucket sort and counting sort
	* When elements are relatively concentrated to a small range, use counting sort. Otherwise, use bucket sort
	*/
	class Solution4
	{
		int bucketCount;

		/*
		* Counting sort
		*/
		int maximumGapCountingSort(vector<int>& nums, int minElement, int maxElement)
		{
			int potentialDistinctiveElementCount = maxElement - minElement + 1;
			
			vector<bool>isOccurred = vector<bool>(potentialDistinctiveElementCount, 0);

			// if min is 42, and max is 48, I only need a vector of 7 elements:
			// X  X  X  X  X  X  X
			// |                 |
			// |                represent the occurrence of 48
			// |
			// represents the occurrence of 42

			for (auto const& c : nums)
			{
				isOccurred[c - minElement] = true;
			}

			int maxGap = -1;
			size_t pre = 0;
			size_t i = 0;
			while (! isOccurred[i])
			{
				++i;
			}

			pre = i;
			// Why assign 'i' to "pre" instead of "i+42"
			// I'm looking for the "gap" between two elements in the input array.
			// The gap between 43 and 45 is the same as that between 1 and 3.
			// So for example, if the original array is like this: 42, 43, 45, 48
			// the occurrence array will be like this:
			// 1,   1,   0,   1,   0,   0,   1   <-- occurrence
			// 42   43   44   45   46   47   48  <-- the original element being represented
			// 0    1    2    3    4    5    6   <-- array index
			// Do I need to say "the smallest number appeared in the original array is 42".
			// No I don't need that, I can just pretent the smallest number appeared in the array is 0,
			// and the next one is 1 (instead of 43). This doesn't affect computing the gap between elements.

			if (i == potentialDistinctiveElementCount - 1)
			{
				// the first non-zero element in "occurrence" is already the last element
				// The original array has only 1 distinctive element, and that element repeated a few times.
				return 0;
			}
			++i;
			while (i < potentialDistinctiveElementCount)
			{
				if (isOccurred[i])
				{
					maxGap = max(maxGap, static_cast<int>(i - pre));
					pre = i;
				}
				++i;
			}

			return maxGap;
		}

		/*
		* bucket sort
		*/
		
		void putElementInBucket(forward_list<int>& bucket, int val)
		{
			auto it = bucket.cbegin();
			auto preIt = bucket.cbefore_begin();
			while (it != bucket.cend())
			{
				if (val < *it)
				{
					bucket.emplace_after(preIt, val);
					break;
				}
				else if (val == *it)
				{
					// the ultimate goal is to find gap between successive elements
					// duplicate elements don't matter in terms of this goal
					// so don't add this one to the linked-list
					break;
				}
				else
				{
					preIt = it;
					++it;
				}
			}
			if (it == bucket.cend())
			{
				bucket.emplace_after(preIt, val);
			}
		}

		int maximumGapBucketSort(vector<int>& nums, int minElement, int maxElement)
		{
			// if max and min are close, for example the input array only have very few *distinctive* elements,
			// in another word, there are a lot of duplicates. (maxElement - minElement) can be less than 10
			// divided by 10, the resulted bucketBoundry can be 0, laster I divided something by bucketBoundry
			// it can lead to "dividing-by-zero" exception.
			// So this scenario is handled by counting sort.

			// think of (maxElement - minElement) as a "distance", cut this distance into some segments
			// Exactly how many segments? -- determined by "bucketCount". If bucketCount is 100, I'm cutting the distance into 100 segments
			// 
			//     |---------|---------|---------|---------|---------|---------|
			//     |         |                       |
			//   minElement  |                       |
			//               boundry                 |
			//                                      an element falls here, calculate which bucket this element is in

			int bucketBoundry = (maxElement - minElement) / bucketCount;
			
			vector<forward_list<int>> bucketsArray(bucketCount, forward_list<int>{});
			for (auto const& c : nums)
			{
				int bucket = (c - minElement) / bucketBoundry;
				
				// buckets' indices in the bucketsArray are 0, 1, 2, 3, .... 9 (assuming 10 buckets)
				// The few very big elements in the original array would fall out of bucketsArray[9]
				// For example, min = 12, max = 73, bucketBoundry = (73-12) / 10 = 6
				// For element "73", bucket = (73 -12) / 6 = 10
				// Need to put this element in bucketsArray[9] not bucketsArray[10]
				bucket = (bucket >= bucketCount) ? bucketCount - 1 : bucket;
				putElementInBucket(bucketsArray[bucket], c);
			}

			// find the first non-empty bucket
			
			decltype(bucketsArray)::size_type i = 0;

			while (bucketsArray[i].empty())
			{
				++i;
			}

			int previous = bucketsArray[i].front();
			int maxGap = 0;
			int gap = 0;

			while (i < bucketCount)
			{
				if (!bucketsArray[i].empty())
				{
					for (auto const& c : bucketsArray[i])
					{
						gap = c - previous;
						maxGap = max(gap, maxGap);
						previous = c;
					}
				}
				i++;
			}
			return maxGap;
		}

	public:

		Solution4() : bucketCount(2000) {}

		int maximumGap(vector<int>& nums)
		{
			if (nums.size() == 1)
			{
				return 0;
			}
			
			int maxElement = *max_element(nums.begin(), nums.end());
			int minElement = *min_element(nums.begin(), nums.end());

			if (maxElement - minElement <= bucketCount)  // the criterium to use counting sort should be how far apart elements spread, not the sheer number of elements.
			{
				return maximumGapCountingSort(nums, minElement, maxElement);
			}
			else
			{
				return maximumGapBucketSort(nums, minElement, maxElement);
			}
		}
	};


	// this is accepted, but still not optimal

	class Solution5
	{
		int intendedNumberOfElementsInEachBucket;

		int maximumGapCountingSort(vector<int>& nums, int minElement, int maxElement)
		{
			int potentialDistinctiveElementCount = maxElement - minElement + 1;
			vector<bool>occur = vector<bool>(potentialDistinctiveElementCount, 0);
			for (auto const& c : nums)
			{
				occur[c - minElement] = true;
			}

			int maxGap = -1;
			size_t pre = 0;
			size_t i = 0;
			while (!occur[i])
			{
				++i;
			}

			pre = i;
			
			if (i == potentialDistinctiveElementCount - 1)
			{
				return 0;
			}
			++i;
			while (i < potentialDistinctiveElementCount)
			{
				if (occur[i])
				{
					maxGap = max(maxGap, static_cast<int>(i - pre));
					pre = i;
				}
				++i;
			}

			return maxGap;
		}

		void putElementInBucket(forward_list<int>& bucket, int val)
		{
			auto it = bucket.cbegin();
			auto preIt = bucket.cbefore_begin();
			while (it != bucket.cend())
			{
				if (val < *it)
				{
					bucket.emplace_after(preIt, val);
					break;
				}
				else if (val == *it)
				{
					break;
				}
				else
				{
					preIt = it;
					++it;
				}
			}
			if (it == bucket.cend())
			{
				bucket.emplace_after(preIt, val);
			}
		}

		int maximumGapBucketSort(vector<int>& nums, int minElement, int maxElement)
		{
			int bucketCount = nums.size() / intendedNumberOfElementsInEachBucket + 1;
			int bucketBoundry = (maxElement - minElement) / bucketCount;

			vector<forward_list<int>> bucketsArray(bucketCount, forward_list<int>{});
			for (auto const& c : nums)
			{
				int bucket = (c - minElement) / bucketBoundry;
				bucket = (bucket >= bucketCount) ? bucketCount - 1 : bucket;
				putElementInBucket(bucketsArray[bucket], c);
			}

			decltype(bucketsArray)::size_type i = 0;

			while (bucketsArray[i].empty())
			{
				i++;
			}

			int previous = bucketsArray[i].front();
			int maxGap = 0;
			int gap = 0;

			while (i < bucketCount)
			{
				if (!bucketsArray[i].empty())
				{
					for (auto const& c : bucketsArray[i])
					{
						gap = c - previous;
						maxGap = max(gap, maxGap);
						previous = c;
					}
				}
				i++;
			}
			return maxGap;
		}

	public:

		Solution5() : intendedNumberOfElementsInEachBucket(200) {}

		int maximumGap(vector<int>& nums)
		{
			if (nums.size() == 1)
			{
				return 0;
			}

			int maxElement = *max_element(nums.begin(), nums.end());
			int minElement = *min_element(nums.begin(), nums.end());

			if (maxElement - minElement <= intendedNumberOfElementsInEachBucket)
			{
				return maximumGapCountingSort(nums, minElement, maxElement);
			}
			else
			{
				return maximumGapBucketSort(nums, minElement, maxElement);
			}
		}
	};


	/*
	* Try use C++ built-in quick sort on each bucket
	* 
	* This is accepted 43ms beats 57%
	*/
	class Solution6
	{
	public:
		int maximumGap(vector<int>& nums)
		{
			int bucketCount = 1000;
			int maxElement = *max_element(nums.begin(), nums.end());
			int minElement = *min_element(nums.begin(), nums.end());
			
			int bucketBoundry = (maxElement - minElement) / bucketCount;
			
			bucketBoundry = (bucketBoundry == 0) ? maxElement : bucketBoundry;

			// need to check bucketBoundry one more time
			// If the original array is like this: [0, 0, 0], i.e. maxElement is 0
			// at this point, bucketBoundry is still 0

			bucketBoundry = (bucketBoundry == 0) ? 1 : bucketBoundry;

			vector<vector<int>> bucketsArray(bucketCount, vector<int>{});
			
			for (auto const& c : nums)
			{
				int bucket = (c - minElement) / bucketBoundry;
				bucket = (bucket >= bucketCount) ? bucketCount - 1 : bucket;
				bucketsArray[bucket].emplace_back(c);
			}

			for (auto & b : bucketsArray)
			{
				sort(b.begin(), b.end());
			}

			decltype(bucketsArray)::size_type i = 0;

			while (bucketsArray[i].empty())
			{
				++i;
			}

			int previous = bucketsArray[i][0];
			int maxGap = -1;
			
			while (i < bucketCount)
			{
				if (!bucketsArray[i].empty())
				{
					for (auto const& c : bucketsArray[i])
					{						
						maxGap = max(c - previous, maxGap);
						previous = c;
					}
				}
				++i;
			}
			return maxGap;
		}
	};



	void Test_0164_Maximum_Gap()
	{
		Solution3 s;
		string s1;
		vector<int>nums;
		while (true)
		{
			s1.clear();
			cout << "nums: ";
			getline(cin, s1);
			if (s1.size() == 0)break;
			stringstream ss(s1);
			nums.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(nums));
			cout << s.maximumGap(nums) << "\n\n";
		}
	}
}