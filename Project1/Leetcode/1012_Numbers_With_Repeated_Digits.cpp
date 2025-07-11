#include<iostream>
#include<string>
#include<bitset>
#include<vector>
#include <thread>
#include <mutex>

namespace _1012_Numbers_With_Repeated_Digits {

	using namespace std;

	/*
	* Doesn't have to be adjacent repeated digits.
	* Numbers like 1215 are counted as having repeated digits
	*/

	//accepted 0ms beat 100%, memory 6MB beat 100%
	class Solution
	{
		/*
		* example: n = 759
		* If I fix the 1st digit (left-most) to be 1, the number is certainly
		* less than n, the 2nd and 3rd digits can free run. Let's say there 
		* are x combinations of 2nd and 3rd digits so that 2nd and 3rd are 
		* unique and they are unique comparing to 1st digit, given the fact
		* that 1st digit is fixed to be '1'. Now I fix the 1st digit to be '2',
		* still 2nd and 3rd digits can free run, although the 1st digit changed,
		* it's still one fixed number, the possible combinations of 2nd and
		* 3rd digits for all 3 digits to be unique should be the same as when
		* the 1st digit is fixed to be '1'.
		* 
		* So a sub-problem is: I have x digits fixed. I have y digits to decide.
		* Those y digits can free run. Those x digits are know to be unique.
		* How many combinations can those y digits have?
		* 
		* In another word, decide y digits, I have (10-x) choices, how many combinations?
		*/
	public:
		/*
		* left-most digit is the 0th digit
		*/
		int a(vector<vector<int>>&r, bitset<10>& digitsMask, int currentIndex, bool alreadyLessThanN, string& n)
		{
			size_t len = n.size();
			if (currentIndex == len)  // I'm already 1 digit past the last digit (right most), a candidate solution is complete now.
				return 1;

			int used = 0;  // number of options already used
			for (int i = 0; i < 10; i++)
				if (digitsMask[i])
					used++;

			size_t toDecide = len - currentIndex;  // number of digits yet to be decided
												// example: length = 8, currentIndex = 6,  digits to be decided: 6th, 7th

			if (alreadyLessThanN && r[toDecide][10 - used] != -1)
				return r[toDecide][10 - used];

			if (alreadyLessThanN)
			{
				// I have a few digits to decide, they can free run, there are non-zero digits in front of them

				int result = 1;
				for (int i = 0; i < toDecide; i++)
				{
					result *= 10 - used - i;
				}
				r[toDecide][10 - used] = result;
				return r[toDecide][10 - used];
			}

			// previous digits are all up to the upper limit
			int limit = n[currentIndex] - '0';
			int result = 0;
			for (int i = 0; i <= limit; i++)
			{
				if (currentIndex == 0 && i == 0)
					continue;

				if (!digitsMask[i])
				{
					digitsMask[i]=true;
					result += a(r, digitsMask, currentIndex + 1, (i<limit), n);
					digitsMask[i] = false;
				}
			}
			return result;
		}

		int b(size_t k)
		{
			if (k == 1) return 9;

			int intermediateResult = 9 * 9; // valid 2-digit number in range [10, 99] excluding 11, 22, 33, 44, 55, 66, 77, 88, 99

			int total = intermediateResult;

			for (int i = 3; i <= k; i++)
			{
				intermediateResult = intermediateResult * (11 - i);
				total += intermediateResult;
			}
			return total + 9; // 9 unique 1-digit number, 0 is not included
		}

		int numDupDigitsAtMostN(int n)
		{
			if (n <= 9) return 0;

			string s = to_string(n);
			size_t len = s.size();
			
			/*
			* example n = 539, let me first find out the unique digit numbers in range [1, 100)
			* basically if n is x-digit number, first find out unique digit numbers with less than x digits
			*/
			int withoutRepeatWithLessDigits = b(len-1);

			/*
			* The reason I split the range into [1,100) and [100,539] is because if I just run backtracking on [1,539],
			* I can certainly choose first digit to be 0, and for example, I choose second digit to be 5. Now the third
			* digit can still use '0'. This each-number-used-once rule can't be applied universally, I have to deal with
			* '0' specially. If I only deal with [100,539], all I need to deal with is to remember to not pick '0' on the
			* very first digit.
			*/

			/*
			* now let's find out unique digit numbers in range [100, 539]
			*/
			vector<vector<int>> r(len, vector<int>(10,-1));
			bitset<10> mask;
			int withoutRepeatWithEqualDigits = a(r, mask, 0/*current index*/, false/*already less than n*/, s/*n in string form*/);
			return n - withoutRepeatWithLessDigits - withoutRepeatWithEqualDigits;
		}
	};

	
	/*
	* It feels that this is entirely combinatorial math, I don't really need dynamic programming.
	* Use 7382 as an example.
	* (1). fix the 1st digit to 0, I get 3 free-run digits, find the numbers with unique digits.
	* Such number can be a 3-digit number, or 2-digit number, or single-digit number.
	* (2). fix the 1st digit to 1 ~ 6, I get the next 3 digits that are "almost* free-run, as long as
	* they don't collide with what is assigned to the 1st digit.
	* (3). fix the 1st digit to 7 (which is its limit).
	* (4). fix the 2nd digit to 0 ~ 2, nothing in the range [0, 2] collides with '7'. I get next 2 digits
	* also "almost" free-run as long as they don't collide with what are assigned to the 1st and 2nd digits.
	* (5). fix the 2nd digit to 3.
	* (6). fix the 3rd digit to 0 ~ 7, now there is possibility of collision, because '7' and '3' are picked.
	* So the choice for the 3rd digit in this case is not 0 ~ 7, I can only pick from [0, 1, 2, 4, 5 ,6].
	* Then I get the next 1 digit that is "almost" free-run as long as it doesn't collide with the first 3 digits.
	* ...
	* 
	* A few things to handle specially:
	* (1). When the last digit is fixed to its limit, for example, 7382, when the 4th digit is fixed to '2'.
	* Don't forget to count the final number I get, which is 7382, which is a number with all unique digit.
	* 
	* (2). When the input 'n' is a single-digit number, utility function 'a' is going to be called with its
	* two arguments "numberOfFreeRunDigits" and "numberOfOptionsTaken" both being zero or one being zero.
	*/
	// accepted 0ms, memory 7.9MB
	class Solution2
	{
		int b(size_t k)
		{
			if (k == 1) return 9;

			int intermediateResult = 9 * 9; // valid 2-digit number in range [10, 99] excluding 11, 22, 33, 44, 55, 66, 77, 88, 99

			int total = intermediateResult;

			for (int i = 3; i <= k; i++)
			{
				intermediateResult = intermediateResult * (11 - i);
				total += intermediateResult;
			}
			return total + 9; // 9 unique 1-digit number, 0 is not included
		}

		int a(int numberOfFreeRunDigits, int numberOfOptionsTaken)
		{
			// if input 'n' is a single digit number, such as '1' or '5',
			// I'm going to call this function either with both input arguments being zeros or "freeRun" being zero and "optionsTaken" being non-zero
			if (numberOfFreeRunDigits == 0 && numberOfOptionsTaken == 0)
				return 0;

			// If I have fixed a few digits before calling this function, i.e. numberOfOptionsTaken > 0
			// and there is no more digit left for me to decide, then there is just 1 number that is unique, which is the digits that are already decided
			if (numberOfFreeRunDigits == 0 && numberOfOptionsTaken != 0)
				return 1;

			if (numberOfOptionsTaken == 0)
			{
				// this is a broader case, if say I have 5 free-run digits, I mean 1 ~ 99999
				// I need to consider 5-digit numbers, 4-digit numbers, ..., and single-digit numbers.
				// In another word, if 1st digit is 0, and 2nd digit is 0, that is not a number with repeating digit,
				// that is just a 3-digit number.
				return b(numberOfFreeRunDigits);
			}
			else
			{
				// this is a simpler case, if number of free-run digits is 5, and number of options taken is 2,
				// I know this is a 7-digit number certainly.
				int result = 1;
				for (int i = 1; i <= numberOfFreeRunDigits; ++i)
				{
					result *= (10 - numberOfOptionsTaken - i + 1);
				}
				return result;
			}
		}

	public:
		int numDupDigitsAtMostN(int n)
		{
			// convert n to digits array

			int base = 1;
			auto len = 1;
			for (int i = 0; i < 9; ++i)  // when i == 8, base = 1,000,000,000
			{
				base *= 10;
				if (n / base == 0)
				{
					break;
				}
				++len;
			}
			vector<char> digits(len, 0);
			auto ncopy = n;
			for (int i = len - 1; i >= 0; --i)
			{
				digits[i] = ncopy % 10;
				ncopy = ncopy / 10;
			}

			bitset<10> digitsMask;

			int digitsTaken = 0;

			auto totalUnique = 0;

			// Use 7382 as an example

			for (int i = 0; i < len; ++i)
			{
				if (i == 0)
				{
					// set the 1st digit to 0, let the rest free run
					totalUnique += a(len - 1, 0);
				}

				auto choices = 0;
				// let this digit be 1 ~ limit-1, for example, if this is the 1st digit, let it be 1 ~ 6
				// but if this really is the 1st digit, make sure this digit is greater than 1.
				// because if the very 1st digit is '1', I first consider it to be '0', then immediate I need to fix it to its limit.
				if ((i == 0 && digits[i] > 1) || (i > 0))
				{
					// between 1 ~ limit-1, are any of them already taken?
					if (i == 0)
					{
						choices = digits[0] - 1;
					}
					else
					{
						int j = 0;
						while (j < digits[i])
						{
							if (!digitsMask[j])
							{
								++choices;
							}
							++j;
						}
					}

					totalUnique += choices * a(len - i - 1, digitsTaken + 1);
				}

				// make this digit equal to its limit
				{
					// can I fix this digit on its limit?
					if (digitsMask[digits[i]])
					{
						break;
					}
					else
					{
						if (i == len - 1)
						{
							// this is the last digit, fixing it to its limit won't create a number with repeating digit.
							// i.e. the very last number in the input range is a digit-unique number.
							// for example, input 'n' is 7382. "7382" itself doesn't have repeating digit.
							++totalUnique;
						}
						else
						{
							++digitsTaken;
							digitsMask[digits[i]] = true;
						}
					}
				}
			}
			return n - totalUnique;
		}
	};

	class MultiThreadBruteForceSolution
	{
		mutex m;
		int total{ 0 };
	public:

		int run(int n)
		{
			total = 0;
			if (n < 1000)
				numDupDigitsAtMostN(1, n);
			else
			{
				vector<thread> workers(3);
				int segment = n / 4;
				int prevEnd = 0;
				for (int i = 0; i < 3; i++)
				{
					int start = prevEnd + 1;
					int end = i == 3 ? n : prevEnd + segment;
					prevEnd = end;
					workers[i] = thread(&MultiThreadBruteForceSolution::numDupDigitsAtMostN, this, start, end);
				}

				numDupDigitsAtMostN(prevEnd+1, n); // use main thread to process the last segment

				for (int i = 0; i < 3; i++)
				{
					workers[i].join();
				}
			}
			return total;
		}

		void numDupDigitsAtMostN(int start, int end)
		{
			bitset<10> mask;
			int tally = 0;
			for (int i = start; i <= end; i++)
			{
				mask.reset();
				bool foundRepeat = false;
				string s = to_string(i);
				for (char& c : s)
				{
					int p = c - '0';
					if (mask[p])
					{
						foundRepeat = true;
						break;
					}
					mask[p] = true;
				}
				if (foundRepeat)
					tally++;
			}
			unique_lock<mutex> ulk(m);
			total += tally;
			return;
		}
	};


	void Test_1012_Numbers_With_Repeated_Digits()
	{
		int n;
		Solution2 s;
		MultiThreadBruteForceSolution mtbfs;
		while (true)
		{
			cout << "(-1 exit) n: ";
			cin >> n;
			if (n == -1) break;
			cout << "fast solution: " << s.numDupDigitsAtMostN(n)
				<< ",  brute force solution: " << mtbfs.run(n) << '\n';
		}
	}
}