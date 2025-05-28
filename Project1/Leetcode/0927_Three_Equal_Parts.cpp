#include<bitset>
#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<random>
#include<functional>
#include<algorithm>
#pragma warning(disable: 4267)

namespace _0927_Three_Equal_Parts {

	using namespace std;

	class Solution
	{
		/*
		* Use KMP string matching algorithm pre-processing
		* a b | c d e f g | h i j k
		* for example if this is how to divide it.
		* I'll have "ab" and "cdefg" represent the same binary number.
		* So "cde" must all be 0 and "ab" must match "fg".
		* Likewise, "hi" must both be 0, and "ab" must match "jk"
		* This is all good, but what if the division leaves the first part longer than the other two?
		* a b c d e f | g h i | j k
		* What if this is the way to divide? -- Then the first part must have leading zeros
		* Run the KMP pre-processing starting from the first non-zero element
		*/

		bool isAllZero(vector<int>& arr, int start, int end)
		{
			// start index and end index inclusive
			return
				all_of(arr.begin() + start, arr.begin() + end + 1,
					[](int const& i)
					{
						return i == 0;
					});
		}

		vector<int> threeEqualParts_leadZeroStripped(vector<int>& arr)
		{
			int len = arr.size();
			int* prefixsuffix = new int[len];
			for (int i=0; i < len; i++)
				prefixsuffix[i] = 0;

			int i = 1, k = 0;
			while (i < len)
			{
				if (arr[i] == arr[k])
				{
					prefixsuffix[i] = k + 1;
					i++; k++;
					continue;
				}
				if (k == 0)
				{
					i++; continue;
				}
				else
				{
					k = prefixsuffix[k - 1];
				}
			}

			// if the end of the array matches a prefix of 10 elements, and if I can find an answer with such a presumption
			// I should continue, because perhaps the end of 10-element prefix can match another prefix.

			int matchLen = len;  // do this just so that the 1st iteration in the while loop matchLen = prefixsuffix[len-1]
			// if there exist three equal parts, certainly the 3rd part must match a certain prefix.
			// the end of the 3rd part is also the end of the entire array
			// let's see *all* the potential prefixes the end of the array can match.
			// Because I want see all of them, I use this "while" loop.
			while (matchLen = prefixsuffix[matchLen-1])  // this is assignment, not compare
			{
				for (int i = 1; i < len; i++)
				{
					if (prefixsuffix[i] == matchLen)
					{
						/*
						 0 1 2 3 4 5 6 7 8 9   prefixsuffix[9] = 3
									   0 1 2
						 if 3 to 6 are all zero, that will work

						 what if it over-matches?

						 0 1 2 3 4 5 6 7 8 9
							   0 1 2 3 4 5 6   this won't work

						the match must not be longer than half

						if 'i' is odd
						 0 1 2 3 4 5 6 7 8 9
								   0 1 2 3 4    this is maximum, prefix length = 5

						if 'i' is even
						 0 1 2 3 4 5 6 7 8
								   0 1 2 3    this is the maximum, prefix length = 4

						if i == 9, (i+1)/2 = 5
						if i == 8, (i+1)/2 = 4

						 */
					MATCH_LENGTH_FOUND:

						if (matchLen > (i + 1) / 2)
							continue;

						// will there be leading zeros for the 2nd part?
						if (i - matchLen == matchLen - 1)
						{
							// this is the case:
							// 0 1 2 3 4 5 6 7 8 9
							//           0 1 2 3 4    this is maximum, prefix length = 5
							// 2nd part is exactly the same length as the 1st part, no leading zeros
						}
						else
						{
							// there will be leading zeros
							int start = matchLen;
							int end = i - matchLen;
							if (!isAllZero(arr, start, end))
								continue;  // leading digits not all zero, continue the search for 2nd part
						}

						// at this point, I've found the 2nd part
						// I need to verify the digits between the end of the 2nd part and
						// the matching segment of the 3rd part
						{
							int thirdPartLen = len - 1 - i;
							if (thirdPartLen < matchLen) // 2nd part encroches over to potential 3rd part
								continue;

							if (thirdPartLen == matchLen)
								return { matchLen - 1, i + 1 };

							// 0 1 2 3  | 4 5 6 7 8 | 9 10 11 12 13 14
							//              0 1 2 3         0  1  2  3
							// thirdPartLen = 6
							// i = 8
							// matchLen = 4

							int start = i + 1;
							int end = start + thirdPartLen - matchLen - 1;
							if (isAllZero(arr, start, end))
								return { matchLen - 1, i + 1 };
						}
					}
					else if (prefixsuffix[i] > matchLen)
					{
						// see if I can shorten the prefix length that matches the segment ending at i
						int k = prefixsuffix[i];
						while (k > 0)
						{
							if (k == matchLen)
								goto MATCH_LENGTH_FOUND;
							k = prefixsuffix[k - 1];
						}
					}
				}
			}
			return { -1, -1 };
		}

	public:

		vector<int> threeEqualParts(vector<int>& arr)
		{
			if (arr[0] == 1)
				return threeEqualParts_leadZeroStripped(arr);

			auto it = find(arr.begin(), arr.end(), 1);
			
			// what if the input has no one, but all zeros
			if (it == arr.end())
				return { -1, -1 };
			
			vector<int>arr_copy(it, arr.end());

			vector<int>const& answer = threeEqualParts_leadZeroStripped(arr_copy);
			if (answer[0] != -1)
			{
				//  0 0 0 1 ........
				//                8       13    <-- answers after stripping leading zeros
				//  add 1 leading zero, 8 becoms 9, 13 becomes 14
				//  add x leading zeros, 8 becomes 8+x, 13 becomes 13+x
				// number of leading zeros: it - begin()
				int leadZeros = static_cast<int>(distance(arr.begin(), it));
				return { answer[0] + leadZeros, answer[1] + leadZeros };
			}
			else
				return { -1,-1 };
		}
	};

	// accepted 107ms beat 5%
	class Solution2
	{
	public:

		// assume the two parts being verified are continugous, i.e. the second part's start is right after first part's end
		bool verifyEqual(vector<int>& arr, int firstStart, int firstEnd, int secondEnd)
		{
			int i = firstEnd;
			int j = secondEnd;
			while (true)
			{
				if (i >= firstStart && j > firstEnd)
				{
					if (arr[i] != arr[j])
						return false;

					i--; j--;
				}
				else if (i >= firstStart && j == firstEnd)
				{
					if (arr[i] == 1)
						return false;

					i--;
				}
				else if (i == firstStart-1 && j > firstEnd)
				{
					if (arr[j] == 1)
						return false;

					j--;
				}
				else // (i == firstStart - 1 && j == firstEnd)
					break;
			}
			return true;
		}

		vector<int> threeEqualParts(vector<int>& arr)
		{
			int len = arr.size();

			int ones = (int)count(arr.begin(), arr.end(), 1);

			if (ones == 0)
			{
				return { 0,2 };
			}

			if (ones % 3 != 0)
			{
				return { -1,-1 };
			}

			int onesInEach = ones / 3;
			int firstEnd = -1, thirdStart = -1;

			int onesCount = 0;

			for (int i = 0; i < len; i++)
			{
				if (arr[i] == 1)
				{
					onesCount++;
					if (onesCount == onesInEach)
					{
						if (firstEnd == -1)
						{
							firstEnd = i;
							onesCount = 0;
						}
						else
						{
							thirdStart = i + 1;
							break;
						}
					}
				}
			}

			int trailingZeros = 0;
			for (int i = len - 1; i > thirdStart; i--)
			{
				if (arr[i] == 0)
					trailingZeros++;
				else
					break;
			}

			int thirdLeadingZeros = 0;
			for (int i = thirdStart; i < len; i++)
			{
				if (arr[i] == 0)
					thirdLeadingZeros++;
				else
					break;
			}

			if (thirdLeadingZeros < trailingZeros)
				return { -1, -1 };

			thirdStart = thirdStart + trailingZeros;  // give some zeros to 2nd part
			
			// now 2nd and 3rd parts are potentially settled

			int secondLeadingZeros = 0;
			for (int i = firstEnd + 1; i < thirdStart; i++)
			{
				if (arr[i] == 0)
					secondLeadingZeros++;
				else
					break;
			}

			if (secondLeadingZeros < trailingZeros)
				return { -1,-1 };

			firstEnd = firstEnd + trailingZeros;

			// Now all three parts' delimiters are potentially settled

			if (verifyEqual(arr, 0, firstEnd, thirdStart - 1) && verifyEqual(arr, firstEnd + 1, thirdStart - 1, len - 1))
				return { firstEnd, thirdStart };
			else
				return { -1, -1 };

		}
	};

	// accepted 3ms beat 42%

	class Solution3 
	{
		const int HI1 = 0;
		const int LOW1 = 1;
		const int LOWBIT = 2;

	public:
		vector<int> threeEqualParts(vector<int>& arr) 
		{
			auto len = arr.size();
			auto ones = count(arr.cbegin(), arr.cend(), 1);
			if (ones == 0)
			{
				return { 0, 2 };
			}
			if (ones % 3 != 0)
			{
				return { -1, -1 };
			}

			auto onesInEach = ones / 3;

			size_t count = 0;
			int part = 0;
			vector<vector<size_t>>r(3, vector<size_t>(3, 0));
			r[2][LOWBIT] = len - 1;

			for (auto i = 0; i < len; ++i)
			{
				if (arr[i] == 1)
				{
					++count;
					if (count == 1)
					{
						r[part][HI1] = i;
					}
					if (count == onesInEach)
					{
						r[part][LOW1] = i;
						count = 0;
						++part;
					}
				}
			}

			// number of 0 at the end of part 3: 
			auto temp1 = r[2][LOWBIT] - r[2][LOW1];

			// Is there enough 0 between the end of part 2 and the start of part 3?
			if (r[2][HI1] - r[1][LOW1] - 1 < temp1)
			{
				return { -1, -1 };
			}
			
			// Is there enough 0 between the end of part 1 and the start of part 2?
			if (r[1][HI1] - r[0][LOW1] - 1 < temp1)
			{
				return { -1, -1 };
			}

			r[0][LOWBIT] = r[0][LOW1] + temp1;
			r[1][LOWBIT] = r[1][LOW1] + temp1;

			// if the distance between the high '1' and the lowest bit is the same in all 3 parts
			if (r[0][LOWBIT] - r[0][HI1] != r[1][LOWBIT] - r[1][HI1])
			{
				return { -1, -1 };
			}
			if (r[2][LOWBIT] - r[2][HI1] != r[1][LOWBIT] - r[1][HI1])
			{
				return { -1, -1 };
			}

			int countOfOnes = 0;
			int i = r[0][HI1], j = r[1][HI1], k = r[2][HI1];
			while (countOfOnes < onesInEach)
			{
				if (arr[i] != arr[j] || arr[j] != arr[k])
				{
					return { -1, -1 };
				}
				if (arr[i] == 1)
				{
					++countOfOnes;
				}
				++i;
				++j;
				++k;
			}

			return { (int)r[0][LOWBIT], (int)r[1][LOWBIT] + 1 };
		}
	};

	// accepted 0ms beats 100%
	// The key of this solution to reach the maximum speed is to absolutely not waste any iteration over the input array
	// Initially I must iterate the entire array at least once to count the total number of ones.
	// After that, even though there are a few "for" loops and "while" loops in this code, every one of these loops on
	// iterate part of the input array. Overall speaking, all these "for" loops and "while" loops only collectively 
	// iterate the whole array once.
	class Solution4
	{
	public:
		vector<int> threeEqualParts(vector<int>& arr)
		{
			int len = arr.size();
			int ones = count(arr.cbegin(), arr.cend(), 1);
			if (ones == 0)
			{
				return { 0, 2 };
			}
			if (ones % 3 != 0)
			{
				return { -1, -1 };
			}

			int onesInEach = ones / 3;

			int count = 0;
			int hi1p1  = -1;
			int i = 0;
			for (i = 0; i < len; ++i) // find the high 1 bit of part 1, break out after reaching low 1 bit of part 1
			{
				if (arr[i] == 1)
				{
					++count;
				}
				if (count == 1 && hi1p1 == -1)
				{
					hi1p1 = i;
				}
				if (count == onesInEach)
				{
					break;
				}
			}
			++i;
			count = 0;
			while (arr[i] == 0) // start after low 1 bit of part 1, find the high 1 bit of part 2
			{
				++i;
			}
			int hi1p2 = i;
			int j = hi1p1;
			while (count < onesInEach) // start from high 1 bit of part 2, compare part 1 and part 2, stop after reaching the low 1 bit of part 2
			{
				if (arr[j] != arr[i])
				{
					return { -1, -1 };
				}
				if (arr[j] == 1)
				{
					++count;
				}
				++j;
				++i;
			}
			while (arr[i] == 0) // start after low 1 bit of part 2, find the high 1 bit of part 3
			{
				++i;
			}
			int hi1p3 = i;
			j = hi1p1;
			int k = hi1p2;
			count = 0;
			while (count < onesInEach) // start from high 1 bit of part 3, stop after reaching low 1 bit of part 3, compare part 1 and part 3
			{
				// don't need to write this "if" condition like this:
				// if (arr[j] != arr[i] || arr[k] != arr[i])
				// I already compared part 1 and part 2, from high 1 to low 1, part 1 and part 2 match.
				// I just need to compare part 1 or part 2 with part 3
				if (arr[j] != arr[i])
				{
					return { -1, -1 };
				}
				if (arr[j] == 1)
				{
					++count;
				}
				++i;
				++j;
				++k;
			}

			--i;
			--j;
			--k;
			// at this point, i, j, k, points at low 1 bit of part 3, part 1, part 2, respectively.
			auto temp = len - 1 - i; // number of zeros between low 1 bit of part 3 and the end of input (i.e. the zeros that matter)

			if (temp > hi1p3 - 1 - k || temp > hi1p2 - 1 - j) // there must be enough zeros after part 1 and part 2.
			{
				return { -1, -1 };
			}

			return { j + temp, k + temp + 1 };
		}
	};

	class AutomatedTest
	{
		Solution2 s;

		vector<int> generateSolvableArray(vector<int>&answer)
		{
			vector<int>arr;
			vector<int>base(4);
			random_device rd;
			mt19937 gen(rd());
			uniform_int_distribution<int> dist(0, 1);
			generate(base.begin(), base.end(), bind(dist, ref(gen)));
			int _leadzeros[3] = { 0,0,0 };
			for (int i = 1; i <= 3; i++)
			{
				int leadzeros = gen() % 5;
				_leadzeros[i - 1] = leadzeros;
				if (leadzeros > 0)
				{
					for (int j = 1; j <= leadzeros; j++)
						arr.push_back(0);
				}
				copy(base.begin(), base.end(), back_inserter(arr));
			}
			answer[0] = _leadzeros[0] + base.size() - 1;
			answer[1] = _leadzeros[0] + base.size() + _leadzeros[1] + base.size();
			return arr;
		}

		vector<int> generateArray()
		{
			vector<int> arr(20);
			random_device rd;
			mt19937 gen(rd());
			uniform_int_distribution<int> dist(0, 1);
			generate(arr.begin(), arr.end(), bind(dist, ref(gen)));
			return arr;
		}

		int getValue(vector<int>& arr, int start, int end)
		{
			
			stringstream ss;
			copy(arr.begin() + start, arr.begin() + end + 1, ostream_iterator<int>(ss));

			string s = ss.str();

			bitset<32> bi{ s };
			unsigned long n = bi.to_ulong();
			return static_cast<int>(n);
		}

		vector<int> bruteForce(vector<int>& arr)
		{
			int len = arr.size();
			for (int i = 0; i <= len - 3; i++)
			{
				for (int j = i + 2; j < len; j++)
				{
					int first = getValue(arr, 0, i);
					int second = getValue(arr, i + 1, j - 1);
					int third = getValue(arr, j, len - 1);
					if (first == second && second == third)
						return { i , j };
				}
			}
			return { -1, -1 };
		}

		bool RunOneSolvableTest()
		{
			vector<int>answer(2);
			vector<int>arr;
			do {
				arr = generateSolvableArray(answer);
			} while (all_of(arr.begin(), arr.end(), [](int const& i) {return i == 0; }));
			

			vector<int> algoAnswer = s.threeEqualParts(arr);
			for (int const& i : arr)
				cout << i << ' ';
			cout << '\n';
			if (answer[0] == algoAnswer[0] && answer[1] == algoAnswer[1])
			{
				cout << "\nPASS\n";
				cout << answer[0] << "  " << answer[1] << '\n';
				return true;
			}
			else
			{
				cout << "\n! ! ! ERROR ! ! !\n";
				cout << answer[0] << "  " << answer[1] << "  " << algoAnswer[0] << "  " << algoAnswer[1] << '\n';
				return false;
			}
		}

		bool RunOneRandomTest()
		{
			vector<int>arr;
			do {
				arr = generateArray();
			} while (all_of(arr.begin(), arr.end(), [](int const& i) {return i == 0; }));

			vector<int> bruteAnswer = bruteForce(arr);
			vector<int> algoAnswer = s.threeEqualParts(arr);

			for (int const& i : arr)
				cout << i << ' ';
			cout << '\n';

			if (bruteAnswer[0] == algoAnswer[0] && bruteAnswer[1] == algoAnswer[1])
			{
				cout << "\nPASS\n";
				cout << bruteAnswer[0] << "  " << bruteAnswer[1] << '\n';
				return true;
			}
			else
			{
				cout << "\n! ! ! ERROR ! ! !\n";
				cout << bruteAnswer[0] << "  " << bruteAnswer[1] << "  " << algoAnswer[0] << "  " << algoAnswer[1] << '\n';
				return false;
			}
		}

public:

		void RunTest(int cases, int solvableOrRandom)
		{
			if (solvableOrRandom == 0)
			{
				for (int i = 1; i <= cases; i++)
				{
					if (!RunOneRandomTest())
						break;
				}
			}
			else
			{
				for (int i = 1; i <= cases; i++)
				{
					if (!RunOneSolvableTest())
						break;
				}
			}
		}
	};


	void Test_0927_Three_Equal_Parts()
	{
		vector<int> arr;
		string arrInString;
		int test;
		char p[] = "abc";
		Solution4 s;
		while (true)
		{
			cout << "manual test (1), automated test (2), or no test (0)? ";
			cin >> test;
			if (test == 0) break;
			if (test == 1)
			{
				arr.clear();
				arrInString.clear();
				cin.getline(p, 1);
				cout << "array: ";
				getline(cin, arrInString);
				istringstream ss(arrInString);
				copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(arr));
				vector<int>const& answer = s.threeEqualParts(arr);
				cout << answer[0] << " " << answer[1] << '\n';
			}
			else if (test == 2)
			{
				AutomatedTest atest;
				int numberCases;
				int solvable_or_random;
				cout << "solvable (1) or random (0)? ";
				cin >> solvable_or_random;
				cout << "number of test cases: ";
				cin >> numberCases;
				
				atest.RunTest(numberCases, solvable_or_random);

			}
		}
	}
}