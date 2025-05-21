#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<algorithm>
#include<random>
#include<numeric>
#include<bitset>
#include<functional>

namespace _0910_Smallest_Range_II {

	using namespace std;

	class Solution
	{
	public:

		/*
		I may not need the average of the entire array, maybe I just need the average between min and max
		The error of this solution is that when an elements is changed (by either +k or -k), if it was the
		min or max, then min (or max) is also changed
		*/
		int smallestRangeII(vector<int>& nums, int k)
		{
			int s = accumulate(nums.begin(), nums.end(), 0);
			size_t len = nums.size();

			//int quo = s / len;
			//int rem = s % len;
			double avg = (double)s / (double)len;

			int mini = numeric_limits<int>::max();
			int maxi = 0;

			vector<int> withinRadius;

			for_each(nums.begin(), nums.end(), 
				[&](int const & i) 
				{
					if ((double)i <= avg - (double)k)
					{
						// if the array has only 1 element, I need to update both mini and maxi
						if (mini > i + k)
							mini = i + k;
						if (maxi < i + k)
							maxi = i + k;
					}
					else if ((double)i >= avg + (double)k)
					{
						if (mini > i - k)
							mini = i - k;
						if (maxi < i - k)
							maxi = i - k;
					}
					else
					{
						withinRadius.emplace_back(i);
					}
				});

			if (withinRadius.size() == nums.size())
			{
				// everything is with in the radius
				auto [minIter, maxIter] = minmax_element(nums.begin(), nums.end());
				mini = *minIter;
				maxi = *maxIter;
			}

			for_each(withinRadius.begin(), withinRadius.end(),
				[&](int const& i)
				{
					if (i + k <= maxi || i - k >= mini)
					{
						// no nothing
					}
					else if (mini - (i - k) >= (i + k) - maxi)
					{
						maxi = i + k;
					}
					else if (mini - (i - k) < (i + k) - maxi)
					{
						mini = i - k;
					}
				});
			return maxi - mini;
		}
	};


	class Solution2
	{
	public:

		pair<int, vector<int>> smallestRangeII(vector<int>& nums, int k)
		{
			auto [minIter, maxIter] = minmax_element(nums.begin(), nums.end());
			int mini = *minIter;
			int maxi = *maxIter;
			float avg = ((float)mini + (float)maxi) / 2.0f;
			mini = numeric_limits<int>::max();
			maxi = 0;
			
			vector<int> answer;
			for_each(nums.begin(), nums.end(),
				[&](int& n)
				{
					if ((float)n <= avg)
					{
						if (maxi < n + k)
							maxi = n + k;
						if (mini > n + k)
							mini = n + k;
						answer.emplace_back(n + k);
					}
					else
					{
						if (maxi < n - k)
							maxi = n - k;
						if (mini > n - k)
							mini = n - k;
						answer.emplace_back(n - k);
					}

				});
			return { maxi - mini, answer };
		}
	};


	class Solution3  // accepted but slow 23ms beat 6%, somehow memory 15MB beats 100%
	{
	public:

		/*
		* if every element is moved toward the same direction, the score will just be the same as it is when
		* none is moved. Move every element to the right (+k), "fold" down one by one from the maximum, see
		* if the score changes at any point of this process.
		* The key is that every element must be moved, either +k or -k, none can stay put.
		* Therefore if I move every element to the right (+k), the best chance to lower the score is by
		* folding down bigger elements one by one.
		*/
		int smallestRangeII(vector<int>& nums, int k)
		{
			sort(nums.begin(), nums.end());
			size_t len = nums.size();
			int score = nums[len - 1] - nums[0];
			int maxFoldedDown = nums[len - 1] - k;
			for_each(nums.begin(), nums.end(),
				[&](int& n)
				{
					n += k;
				});
			
			for (int i = (int)len - 1; i >= 0; --i)
			{
				if (i != 0)
				{
					if (nums[i] - 2 * k >= nums[0])
					{
						int potentialLowerScore = max(maxFoldedDown, nums[i - 1]) - nums[0];
						                            //\___________/  \________/
						                            //      |            |
						                            //      |            +-- maximum among all the numbers that have not been folded down
						                            //      +----------------maximum among all the numbers that have been folded down
						if (score > potentialLowerScore)
						{
							score = potentialLowerScore;
						}
					}
					else
					{
						int potentialLowerScore = max(maxFoldedDown, nums[i - 1]) - (nums[i] - 2 * k);
						if (score > potentialLowerScore)
						{
							score = potentialLowerScore;
						}
					}

					if (maxFoldedDown > nums[i - 1])
					{
						/*
						* if at some point maxFoldedDown is greater than nums[i-1], which means
						* max(maxFoldedDown, nums[i-1]) is going to yield maxFoldedDown, the subsequent iterations
						* are going to yield the same result because nums[i-1] is going to be smaller and smaller.
						* On the other hand, folding down nums[i] is going to yield smaller and smaller value.
						* so the potentialLowerScore won't become smaller.
						*/
						break;
					}
				}
				else
				{
					/*
					now I'm trying to fold down nums[0], because I have tried folding down every other one
					now if I do fold down nums[0], I'll end up folding down every one
					this is equivalent to folding down every one at the beginning
					the score is just the same as not folding any one
					then score will just be the same as "score" initial value
					if any of those "potential lower scores" is really lower, the score in this case won't matter
					*/
				}
			}
			return score;
		}
	};

	// this solution is wrong
	class Solution5
	{
		/*
		* The idea is to find the one that is closest to mean (not mediant) among all that are less than mean,
		* Use 'A' to denote that element.
		* and find the one that is closest to mean among all that are greater than mean. Use 'B' to denote that element.
		* Everyone else "fold" toward mean. After folding every one except 'A' and 'B', find the min and max among all but 'A' and 'B'.
		* For 'A' and 'B' action, there are only 4 possibilities. 
		* 'A' and 'B' action and the min and max among all other decide the result.
		*            *
		*               * E
		*                 * C
		*                     * A
		* -------------------------------------------------------- mean
		*                          * B
		*                                * D
		*                                       *
		*                                              *
		* 
		* If 'C' eventually becomes the deciding factor, if the action on 'C' is to fold down (C - k),
		* why won't 'A' become the deciding factor? If 'A' also fold down, 'A' is lower, 'A' should be the min instead of 'C'.
		* If the action on 'C' is fold up (C + k), why won't 'E' become the deciding factor?
		* 
		* (This still doesn't feel like a concrete proof)
		* 
		*/
		// nums are all non-negative numbers. 'k' is also non-negative.
	public:
		int smallestRangeII(vector<int>& nums, int k)
		{
			auto minElement = std::min_element(nums.cbegin(), nums.cend());
			auto maxElement = std::max_element(nums.cbegin(), nums.cend());
			int originalScore = *maxElement - *minElement;

			auto len = nums.size();
			unsigned long long total = 0;
			for (auto const& n : nums)
			{
				total += n;
			}
			double mean = (double)total / (double)len;

			double diff1 = (double)(INT_MAX); // diff of an element that is less than mean
			double diff2 = (double)(INT_MAX); // diff of an element that is greater than mean
			int index1 = -1;
			int index2 = -1;
			size_t equalMeanCount = 0;

			for (int i = 0; i < len; ++i)
			{
				if (nums[i] < mean)
				{
					if (diff1 > (mean - nums[i]))
					{
						diff1 = mean - nums[i];
						index1 = i;
					}
				}
				if (nums[i] > mean)
				{
					if (diff2 > (nums[i] - mean))
					{
						diff2 = nums[i] - mean;
						index2 = i;
					}
				}
				if (nums[i] == mean)
				{
					++equalMeanCount;
					index1 = -1;
					index2 = -1;
					break;
				}
			}

			int minInput = INT_MAX;
			int maxInput = INT_MIN;

			for (int i = 0; i < len; ++i)
			{
				if (index1 != -1 && nums[i] == nums[index1])
				{
					continue;
				}
				if (index2 != -1 && nums[i] == nums[index2])
				{
					continue;
				}
				if (i != index1 && i != index2 && nums[i] != mean)
				{
					int t;
					if (nums[i] > mean)
					{
						t = nums[i] - k;
					}
					else // (nums[i] < mean)
					{
						t = nums[i] + k;
					}
					if (t < minInput)
					{
						minInput = t;
					}
					if (t > maxInput)
					{
						maxInput = t;
					}
				}
			}

			int score = 0;

			if (equalMeanCount > 0)
			{
				int t = (int)mean;
				score = std::min((std::max(maxInput, t + k) - minInput), (maxInput - std::min(minInput, t - k)));
			}

			else
			{
				score = std::min({
					std::max({ maxInput, nums[index1] + k, nums[index2] + k }) - std::min({ minInput, nums[index1] + k, nums[index2] + k }),
					std::max( { maxInput, nums[index1] - k, nums[index2] - k }) - std::min({minInput, nums[index1] - k, nums[index2] - k}),
					std::max( { maxInput, nums[index1] + k, nums[index2] - k }) - std::min( { minInput, nums[index1] + k, nums[index2] - k })
					});
			}
			
			return std::min(score, originalScore);
		}
	};


	// accepted 5ms beats 34%
	class Solution6
	{
	public:
		int smallestRangeII(vector<int>& nums, int k)
		{
			sort(nums.begin(), nums.end());
			size_t len = nums.size();
			int score = nums[len - 1] - nums[0];
			int minElement, maxElement;
			for (int i = 0; i < len - 1; ++i)
			{
				minElement = min(nums[0] + k, nums[i + 1] - k);
				maxElement = max(nums[i] + k, nums[len - 1] - k);
				if (score > maxElement - minElement)
				{
					score = maxElement - minElement;
				}
			}
			return score;
		}
	};

	// accepted 0ms beats 100%
	// the only improvement between Solution7 and Solution6 is the "if" condition inside the "for" loop that can potentially break out of the loop faster.
	class Solution7
	{
	public:
		int smallestRangeII(vector<int>& nums, int k)
		{
			sort(nums.begin(), nums.end());
			size_t len = nums.size();
			int score = nums[len - 1] - nums[0];
			int minElement, maxElement;
			for (int i = 0; i < len - 1; ++i)
			{
				minElement = min(nums[0] + k, nums[i + 1] - k);
				maxElement = max(nums[i] + k, nums[len - 1] - k);
				if (score > maxElement - minElement)
				{
					score = maxElement - minElement;
				}

				if (nums[i] + k > nums[len - 1] - k && nums[0] + k < nums[i + 1] - k)
				{
					break;
				}
			}
			return score;
		}
	};

	/*
	* for example, nums have 4 numbers: a, b, c, d
	* An array of bits such as 0, 1, 1, 0 means a+k, b-k, c-k, d+k
	* This bit array can map to an integer "0110" -> 6
	* Enumerate all possible bit arrays, check the score of each one, 
	* record the bit array (the integer value it maps to) that yields the minimum score.
	*/
	static pair<int, vector<int>> brute_force(vector<int>& nums, int k)
	{
		size_t len = nums.size();
		int last = (int)pow(2, len);
		int minScore = numeric_limits<int>::max();
		vector<int>answer(len);
		int answerBinaryBitsArrayValue = 0;
		for (int i = 1; i <= last; i++)
		{
			bitset<32>bits(i - 1); // the bit at index 0 is not the left-most bit, but rather the least-significant bit, which is right-most
			int maxi = 0;
			int mini = numeric_limits<int>::max();
			int index = 0;
			vector<int>potentialAnswer(len);

			// iterator over every element in the input,
			// change it (plus k or minus k) according to a bit in "bits"
			// every element has 2 possible actions. an input of size n has 2^n total possible final states
			// test all 2^n of them.

			for_each(nums.begin(), nums.end(),
				[&bits, &index, &mini, &maxi, &potentialAnswer, &k](int& n)
				{
					// n + (-2 * bits[index] * k) + k
					// bits[index] == 0  ==>  n + k;
					// bits[index] == 1  ==>  n - k
					int new_n = n + (-2 * bits[index] * k) + k;
					if (mini > new_n)
						mini = new_n;
					if (maxi < new_n)
						maxi = new_n;
					potentialAnswer[index] = new_n;
					index++;
				});
			if (minScore > (maxi - mini))
			{
				minScore = maxi - mini;
				answerBinaryBitsArrayValue = i;
			}
		}
		// construct the array that yields the minimum score
		bitset<32>answerBits(answerBinaryBitsArrayValue - 1);
		for (int i = 0; i < len; ++i)
		{
			answer[i] = nums[i] + (-2 * answerBits[i] * k) + k;
		}
		return { minScore, answer };
	}


	static void automated_test()
	{
		int cases = 5; // number of test cases
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> dist(0, 30);
		vector<int> nums(10);
		Solution6 s;
		
		for (int i = 1; i <= cases; i++)
		{
			cout << "\n\ntest case " << i << "\n\n";
			
			//generate(nums.begin(), nums.end(), bind(dist, ref(gen)));
			generate(nums.begin(), nums.end(), [&dist, &gen]() {return dist(gen); });
			int k = gen() % 20;

			sort(nums.begin(), nums.end()); // the ordering of the element doesn't affect the result
			                                // in order to see some pattern, I sort this array then give it to brute-force solution.

			for (int& n : nums)
				cout << n << ' ';
			cout << "\n\nk: " << k << '\n';

			auto [score, arr] = brute_force(nums, k);
			
			//auto [algo_score, algo_arr] = s.smallestRangeII(nums, k);
			int algo_score = s.smallestRangeII(nums, k);
			
			if (score != algo_score)
			{
				cout << "\n\nERROR !!!!!!!!!!\n\n";
				cout << "brute force answer: " << score << '\n';
				for (int& n : arr)
					cout << n << ' ';

				cout << "\nalgorithm answer: " << algo_score << '\n';
				/*
				for (int& n : algo_arr)
					cout << n << ' ';
				*/
				cout << '\n';
				break;
			}
			else
			{
				cout << "\n\nPASS ------  min score: " << score << "\n\n";
				for (int& n : arr)
					cout << n << ' ';
				cout << "\n\n";
			}
		}
	}


	void Test_0910_Smallest_Range_II()
	{
		int test;
		int k;
		vector<int> nums;
		string numsInString;

		Solution6 s6;

		while (true)
		{
			cout << "manual test (1), automated test (2), or no test (0)? ";
			cin >> test;
			if (test == 0) break;
			if (test == 2)
			{
				automated_test();
			}
			else if (test==1)
			{
				nums.clear();
				numsInString.clear();
				cout << "k: ";
				cin >> k;
				cin.ignore();
				cout << "array: ";
				getline(cin, numsInString);
				istringstream ss(numsInString);
				copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(nums));
				
				//auto [score, arr] = s.smallestRangeII(nums, k);
				
				int solution_score = s6.smallestRangeII(nums, k);
				
				cout << "Solution score: " << solution_score << "\n";


				unsigned long long total = 0;
				auto len = nums.size();
				for (auto const& n : nums)
					total += n;

				double mean = (double)total / (double)len;
				cout << "mean: " << mean << "\n";

				auto [score, arr] = brute_force(nums, k);
				
				cout << "score: " << score << '\n';

				for (int& n : arr)
					cout << n << ' ';
				cout << '\n';
			}
		}
	}
}