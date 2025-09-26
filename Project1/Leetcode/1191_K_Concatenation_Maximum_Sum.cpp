#include "LibHeaders.h"

namespace _1191_K_Concatenation_Maximum_Sum {

	using namespace std;

	/*
	* if the sum of arr is positive, it's not true that I can just take the full concatenated array as answer
	* for example:  arr = [100, -99], k = 2
	* the sum of the concatenated array is 2. but obviously the subarray with maximum sum is "101"
	* 100, -99, 100, -99
	* \___________/
	*     take this subarray
	*/

	/*
	* k's range is [1, 10^5], if k is big, then any algorithm that runs O(n*k)
	* can be slow. All three solutions try to use fewer concatenations.
	*/

	/* 
	* Solution1 tries to use just 1 repitition.
	* i.e., if the input is "abcd", just make it "abcdabcd", and based on the
	* pattern in "abcdabcd", I try to figure out what the result would be, had I
	* contatenated k times. But solution 1 has errors.
	* 
	* Solution2 fixes the error, but made it a little too complicated.
	* 
	* Solution3 is the cleanest solution. All I need to find out is that if the sum
	* of the original array is positive. If it's, let's include as many of them as possible.
	* And if it's negative, let's not include any of them. There is no point to include some
	* cycles but not all. It's all or nothing.
	* for example: k = 4
	* |-------------|-------------|-------------|-------------|
	*        \________________________/
	*             this will never be the answer.
	* If the original array sum is positive, why don't I include both cycles in the middle?
	* If the original array sum is negative or zero, why don't I exclude both cycles in the middle?
	* 
	* a b c d e f g h i j k  ...  a b c d e f g h i j k  ...  a b c d e f g h i j k
	*             |                                               |
	* 
	* I first decide to include all the full arrays in the middle. Then I find out the biggest subarray
	* going from the end towards the beginning (i.e. [g <- k]) and the biggest subarray going from the
	* beginning to the end (i.e. [a -> c]) So the final result would be [g,k] + cycles + [a,c].
	* How to find out [g,k] and [a,c], just try to find the max sum subarray in a 1 repitition case.
	* i.e. find the max sum subarray within this:
	* a b c d e f g h i j k a b c d e f g h i j k
	* 
	* If the sum of the original array is positive, if I start looking for a subarray from the end of the array
	* there must be a subarray whose sum is positive. Maybe I have to go all the way to the beginning which is
	* essentially the full array. Starting at the beginning and going toward the end is the same principle.
	* Which means after including all the full arrays in the middle, I must be able to find the extensions
	* toward the left and right.
	* 
	* If the sum of the original array is 0 or negative, I don't include any full array in the middle.
	* I just treat the problem as if k is 2.
	*/

	class Solution
	{
	public:
		/*
		* This solution doesn't work on this case:
		* 
		* 55  -28  -74  30  48    k = 3
		* 
		* Correct answer:
		* 
		* 55  -28  -74  30  48 | 55  -28  -74  30  48 | 55  -28  -74  30  48
		*               \________________________________/
		*                          sum = 164
		*/

		int kConcatenationMaxSum(vector<int>& arr, int k)
		{
			int len = arr.size();

			if (k == 1)
			{
				int maxSum = 0, currentSum = 0;
				for (int i = 0; i < len; i++)
				{
					currentSum += arr[i];
					if (currentSum < 0)
						currentSum = 0;
					if (maxSum < currentSum)
					{
						maxSum = currentSum;
					}
				}
				return maxSum;
			}

			int currentSum = 0;
			int maxSum = 0;
			int start = 0;
			int end = 0;

			vector<int>arr2(len*2);
			copy(arr.begin(), arr.end(), arr2.begin());
			copy(arr.begin(), arr.end(), arr2.begin() + len);
			int len2 = len * 2;
			for (int i = 0; i < len2; i++)
			{
				currentSum += arr2[i];
				if (currentSum < 0)
					currentSum = 0;
				if (maxSum < currentSum)
				{
					maxSum = currentSum;
					end = i;
				}
			}

			if (maxSum == 0) return 0;

			currentSum = 0;
			for (int i = end; i >= 0; i--)
			{
				currentSum += arr2[i];
				if (currentSum == maxSum)
				{
					start = i;
					break;
				}
			}

			/*
			*   0 1 2 3 4 5  |  6 7 8 9 10 11
			*       |               |    |
			*     start      equivalent  end
			*                start
			* 
			* when does "end" crosses over "equivalent start"?
			* len = 6
			* start - 0 = 2
			* len + 2 = 8 = equivalent start = len + start
			*  
			*/

			if (end >= len + start)
			{
				int s1 = accumulate(arr.begin(), arr.end(), 0);

				int s2;
				if (start > 0)
					s2 = accumulate(arr.begin(), arr.begin() + start, 0);
				else
					s2 = 0;
				int s3;
				if (end < len2 - 1)
					s3 = accumulate(arr.begin() + (end - len + 1), arr.end(), 0);
				else
					s3 = 0;
				return s1 * k - s2 - s3;
			}
			else
			{
				return maxSum;
			}
		}
	};


	class Solution2
	{
	public:

		int kConcatenationMaxSum(vector<int>& arr, int k)
		{
			int len = arr.size();

			if (k <= 2)
			{
				int currentSum = 0;
				int maxSum = 0;
				for (int i = 0; i < len * k; i++)
				{
					int j = i % len;

					currentSum += arr[j];
					if (currentSum < 0)
						currentSum = 0;
					if (maxSum < currentSum)
					{
						maxSum = currentSum;
					}
				}
				return maxSum;
			}

			// k >= 3

			int currentSum = 0;
			int maxSum = 0;
			int end = 0;

			for (int i = 0; i < len * 3; i++)
			{
				int j = i % len;
				currentSum += arr[j];
				if (currentSum < 0)
					currentSum = 0;
				if (maxSum < currentSum)
				{
					maxSum = currentSum;
					end = i;
				}
			}

			if (maxSum == 0) return 0;

			if (end >= len * 2)
			{
				/*
				* With a concatenation of three "arr", the end is in the 3rd segment,
				* (the "start" must be in the 1st)
				* (if the "start" is in the 2nd, why didn't "start" and "end" end up in 1st and 2nd segment?)
				* This means the max subarray encompassed the entire 2nd segment,
				* so it can envelop as many whole arr's as possible
				*/
				int s = accumulate(arr.begin(), arr.end(), 0);
				return maxSum + (k - 3) * s;
			}
			else
			{
				/*
				* If in a two arr concatenation, the "end" crosses over the equivalent "start" (see Solution #1 for terminology)
				* the "end" must end up in the 3rd segment in a three-arr concatenation.
				* So if the "end" is not in the 3rd segment, it's either just in 1st or in 2nd without crossing over
				* equivalent start.
				*/
				return maxSum;
			}
		}
	};


	class Solution3
	{
		/*
		* Using this to demonstrate the logic. This solution doesn't consider overflowing the upper limit of "int"
		* 
		* What if the max-sum-subarray of the original array is somewhere in the middle?
		* a, b, c, d, e, f, g, h, i,
		*       \___________/
		*         Let's say this is the subarray that yields the max sum.
		* How can I just use this sum and add the the sum of the original array?
		* Note that if the max-sum-subarray is like this (i.e. in the middle), the sum of subarray [h, i] must be negative,
		* likewise, the sum of subarray [a, b] must be negative.
		* Otherwise two-cycle concatenation will yield something different.
		* So I actually won't add anything to this subarray's sum.
		*/
		int oneOrTwoMaxSum(vector<int>& arr, int k)
		{
			size_t len = arr.size();
			int currentSum = 0;
			int maxSum = 0;
			for (size_t i = 0; i < len * k; i++)
			{
				size_t j = i % len;

				currentSum += arr[j];
				if (currentSum < 0)
					currentSum = 0;
				if (maxSum < currentSum)
					maxSum = currentSum;
			}
			return maxSum;
		}
	public:
		int kConcatenationMaxSum(vector<int>& arr, int k) {
			if (k <= 2)
				return oneOrTwoMaxSum(arr, k);
			else
			{
				int arrSum = accumulate(arr.begin(), arr.end(), 0);

				int sum1 = oneOrTwoMaxSum(arr, 2);
				if (arrSum > 0)
					return sum1 + arrSum * (k - 2);
				else
					return sum1;
			}
		}
	};


	class Solution4  // accepted
	{
		int modBase = 1'000'000'007;

		struct SumRecord
		{
			int numberOfBases{ 0 };
			int extra{ 0 };
			SumRecord(int numberOfBases_, int extra_) :numberOfBases(numberOfBases_), extra(extra_) {}
		};

	public:

		/*
		* This could've been a simple max-subarray-sum algorithm, but because of the potential of overflowing the
		* upper limit of "int", the modulus operation is required, and the comparing between "currentSum"
		* and "maxSum" has to be conducted by comparing the actual value of those two, not just their remaining
		* part after modulo the base, so I must remember how many bases have been cut off from the real value.
		* 
		*/
		int oneOrTwoMaxSum(vector<int>& arr, int k)
		{
			size_t len = arr.size();
			SumRecord currentSum{0,0};
			SumRecord maxSum{0,0};
			
			for (size_t i = 0; i < len * k; i++)
			{
				size_t j = i % len;

				if (arr[j] >= 0)
				{
					currentSum.extra += arr[j];
					if (currentSum.extra >= modBase)
					{
						currentSum.extra %= modBase;
						currentSum.numberOfBases++;
					}
				}
				else
				{
					if (currentSum.extra >= abs(arr[j]))
						currentSum.extra += arr[j];   // attention, arr[j] is a negative number
					else
					{
						if (currentSum.numberOfBases > 0)
						{
							currentSum.numberOfBases--;
							currentSum.extra = modBase + currentSum.extra + arr[j];
						}
						else
						{
							currentSum.extra = 0;
						}
					}
				}

				if (maxSum.numberOfBases < currentSum.numberOfBases || (maxSum.numberOfBases == currentSum.numberOfBases && maxSum.extra < currentSum.extra))
					maxSum = currentSum;
			}
			return maxSum.extra;
		}


		/*
		* When I calculate the sum of the original array, I ignore the possibility that
		* the sum can overflow the limit of "int".
		* 
		* This is accepted by leetcode, 
		* maybe either they don't have a test case where sum of the original array overflows the limit of "int" 
		* or the test case where it does overflow has k = 2, so I don't really get to the "accumulate" call in that test case.
		* 
		* Question:
		* If I do consider that, is this code good enough?
		*
		* 
				auto addOp = [&](int& a, int& b)
				{
					a += b;
					a %= modBase;
					return a;
				};

				int arrSum = accumulate(arr.begin(), arr.end(), 0, addOp);
		*
		* What if at some point a+=b brings 'a' to negative? What is a negative modulus another number?
		*/
		int kConcatenationMaxSum(vector<int>& arr, int k)
		{
			if (k <= 2)
				return oneOrTwoMaxSum(arr, k);
			else
			{
				int arrSum = accumulate(arr.begin(), arr.end(), 0);
				int sum1 = oneOrTwoMaxSum(arr, 2);
				if (arrSum > 0)
				{
					int total = 0;
					for (int i = 1; i <= k - 2; i++)
					{
						total += arrSum;
						total %= modBase;
					}
					total += sum1;
					total %= modBase;
					return total;
				}
				else
					return sum1;
			}
		}
	};

	// accepted 4ms beat 30%
	class Solution5
	{
		const int MOD_BASE = 1'000'000'007;

	public:

		int oneOrTwoMaxSum(vector<int>& arr, int k, int& oneArrSum)
		{
			size_t len = arr.size();
			size_t len2 = len * k;

			long long int currentSum = 0ll;
			long long int maxSum = 0ll;
			long long int _oneArrSum = 0ll;
			size_t j = 0;
			for (size_t i = 0; i < len2; ++i)
			{
				if (i < len)
				{
					_oneArrSum += arr[i];
					j = i;
				}
				else
				{
					j = i % len;
				}
				currentSum += arr[j];
				if (currentSum < 0)
					currentSum = 0;
				if (maxSum < currentSum)
					maxSum = currentSum;
			}
			if (_oneArrSum > 0)
			{
				oneArrSum = _oneArrSum % MOD_BASE;
			}
			return maxSum % MOD_BASE;
		}

		int kConcatenationMaxSum(vector<int>& arr, int k)
		{
			int oneArrSum = 0;
			if (k <= 2)
			{
				return oneOrTwoMaxSum(arr, k, oneArrSum);
			}
			else
			{
				int sum1 = oneOrTwoMaxSum(arr, 2, oneArrSum);
				if (oneArrSum > 0)
				{
					int maxCapacity = MOD_BASE / oneArrSum;
					int maxLoad = maxCapacity * oneArrSum;
					int remain = k - 2;
					int total = 0;
					while (remain > maxCapacity)
					{
						total += maxLoad;
						total %= MOD_BASE;
						remain -= maxCapacity;
					}
					total += (remain * oneArrSum);
					total %= MOD_BASE;
					total += sum1;
					total %= MOD_BASE;
					return total;
				}
				else
				{
					return sum1;
				}
			}
		}
	};

	// accepted 0ms beat 100%
	class Solution6
	{
		const int MOD_BASE = 1'000'000'007;

	public:

		int oneOrTwoMaxSum(vector<int>& arr, int k, int& oneArrSum)
		{
			size_t len = arr.size();
			size_t len2 = len * k;

			long long int currentSum = 0ll;
			long long int maxSum = 0ll;
			long long int _oneArrSum = 0ll;
			size_t j = 0;
			for (size_t i = 0; i < len2; ++i)
			{
				if (i < len)
				{
					_oneArrSum += arr[i];
					j = i;
				}
				else
				{
					j = i % len;
				}
				currentSum += arr[j];
				if (currentSum < 0)
					currentSum = 0;
				if (maxSum < currentSum)
					maxSum = currentSum;
			}
			if (_oneArrSum > 0)
			{
				oneArrSum = _oneArrSum % MOD_BASE;
			}
			return maxSum % MOD_BASE;
		}

		int kConcatenationMaxSum(vector<int>& arr, int k)
		{
			int oneArrSum = 0;
			if (k <= 2)
			{
				return oneOrTwoMaxSum(arr, k, oneArrSum);
			}
			else
			{
				int sum1 = oneOrTwoMaxSum(arr, 2, oneArrSum);
				if (oneArrSum > 0)
				{
					long long unsigned total = (long long unsigned)(k - 2) * (long long unsigned)oneArrSum;
					total += sum1;
					return total % MOD_BASE;
				}
				else
				{
					return sum1;
				}
			}
		}
	};

	class BruteForceSolution
	{
	public:
		int kConcatenationMaxSum(vector<int>& arr, int k)
		{
			int len = arr.size();
			vector<int> concatenated(len*k, 0);
			for (int i = 0; i < k; i++)
			{
				copy(arr.begin(), arr.end(), (concatenated.begin() + (i * len)));
			}
			int concaLen = len * k;
			int maxSum = 0;
			for (int i = 1; i <= concaLen; i++)
			{
				for (int j = 0; j <= concaLen - i; j++)
				{
					int subarraySum = accumulate(concatenated.begin()+j, concatenated.begin()+j+i, 0);
					if (maxSum < subarraySum)
						maxSum = subarraySum;
				}
			}
			return maxSum;
		}
	};


	class AutoTest
	{
		Solution3 s;
		BruteForceSolution bs;
		random_device rd;
		mt19937 gen;
		uniform_int_distribution<> dist;

	public:
		AutoTest() : gen(rd()), dist(-100, 100) {}

		void generateArray(vector<int>&arr, int&k)
		{
			int arrLen = 0;
			while (arrLen < 8)
				arrLen = gen() % 15;
			while (k == 0)
				k = gen() % 8;

			arr.assign(arrLen, 0);
			generate(arr.begin(), arr.end(), bind(dist, ref(gen)));
		}

		bool runOneTest()
		{
			vector<int>arr;
			int k = 0;
			generateArray(arr, k);

			int answer = s.kConcatenationMaxSum(arr, k);
			int bs_answer = bs.kConcatenationMaxSum(arr, k);
			if (answer != bs_answer)
			{
				cout << "\narr: ";
				for (int& i : arr)
					cout << i << ' ';
				cout << "          k: " << k << '\n';
				cout << "ERROR !!!"<< "      answer: " << answer << "  brute answer: " << bs_answer << "\n\n";
				return false;
			}
			else
			{
				//cout << "answer: " << answer << "\n\n";
				return true;
			}
		}

		void runTests(int n)
		{
			for (int i = 1; i <= n; i++)
				if (!runOneTest())
					break;
			cout << "all pass\n";
		}
	};


	void Test_1191_K_Concatenation_Maximum_Sum()
	{
		vector<int> arr;
		string arrInStr;
		int k;
		int option;
		Solution5 s;
		BruteForceSolution bs;
		AutoTest atest;

		while (true)
		{
			
			cout << "1: manual test    2: auto test     0: exit   >   ";
			cin >> option;
			//cin.ignore();
			if (option == 0) break;
			if (option == 1)
			{
				arr.clear();
				arrInStr.clear();
				cin.ignore();
				cout << "array: ";
				getline(cin, arrInStr);
				cout << "k: ";
				cin >> k;
				//cin.ignore();
				istringstream ss(arrInStr);
				copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(arr));
				cout << "algorithmic solution: " << s.kConcatenationMaxSum(arr, k) 
					<< "   brute force solution: " << bs.kConcatenationMaxSum(arr, k) << "\n";
			}
			else if (option == 2)
			{
				int n;
				cout << "How many tests? ";
				cin >> n;
				//cin.ignore();
				atest.runTests(n);
			}
		}
	}
}