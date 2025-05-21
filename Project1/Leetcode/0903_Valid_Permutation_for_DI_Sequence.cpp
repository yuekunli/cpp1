#include "LibHeaders.h"

namespace _0903_Valid_Permutation_for_DI_Sequence {

	using namespace std;

	/*
	* 1 <= n <= 200
	* there could be 200 numbers forming such DI sequence
	*/

	class Solution
	{
	public:

		/*
		* 5  18  12   9    x    x    x    ...
		*             |    \______________/
		*          index=3
		* There will be len-3-1 positions to be decided.
		* There are len-3-1 numbers to be assigned.
		* How many of these len-3-1 numbers are less than the number in the last decided cell (it's cell#3 in this case)
		* i.e. how many of these len-3-1 numbers are less than '9'?
		*/

		int recursiveSolve(int n, string& s, vector<vector<int>>& r, int index, int previous, vector<bool>& available)
		{
			if (index == n+1)
				return 1;
			if (index == 0)
			{
				int total = 0;
				for (int i = 0; i <= n; i++)
				{
					available[i] = false;
					total += recursiveSolve(n, s, r, 1, i, available);
					available[i] = true;
				}
				return total;
			}

			int lessThanPrevious = count(available.begin(), available.begin()+previous, true);
			if (r[index][lessThanPrevious] != -1)
				return r[index][lessThanPrevious];

			bool direction = s[index - 1] == 'I' ? true : false; // true: increase;  false: decrease
			int total = 0;
			for (int i = 0; i <= n; i++)
			{
				if (available[i])
				{
					if ((direction && i > previous) || ((!direction) && i < previous))
					{
						available[i] = false;
						total += recursiveSolve(n, s, r, index + 1, i, available);
						available[i] = true;
					}
				}
			}
			r[index][lessThanPrevious] = total;
			return total;
		}

		int numPermsDISequence(string s)
		{
			int len = s.size();
			//int available = (1 << len) - 1;  // one integer doesn't have enough bits to represent all the numbers
			vector<bool> available(len+1, true);
			vector<vector<int>> r(len+1, vector<int>(len+1, -1));
			// if length is 4, I can use 0,1,2,3,4 total 5 numbers, the maximum count of numbers less than one of them is 4
			// when the "previous" number is 4, I need to say that there are 4 numbers less than previous one
			// I need to be able to do r[x][4]
			int answer = recursiveSolve(len, s, r, 0, -1, available);
			return answer;
		}
	};


	class Solution2  // accepted
	{
		size_t len;
		int modBase = 1'000'000'007;
		int recursiveSolve(string& di, vector<vector<int>>& r, size_t index, int lastNumber, vector<bool>& available, int lessThanLast)
		{
			/*
			* There are fixed number of numbers I can use, once I know which index I'm standing on
			* I know how many open spots need to be filled, and I know how many numbers are still available.
			* So the "index" information is equivalent to "number of numbers available".
			* Initially I think I need 3 pieces of information, (1) which index I'm standing on (this decides
			* the trend of the rest of the sequence). (2). how many numbers are available to use. (3) and the
			* order of "last number" among numbers that are still available. But 2 of these 3 pieces are
			* actually equivalent.
			*/
			if (index == len)
				return 1;
			if (r[index][lessThanLast] != -1)
				return r[index][lessThanLast];

			int total = 0;
			bool increase = di[index] == 'I';
			int lessThanCurrent = 0;
			for (size_t i = 0; i <= len; i++)
			{
				if (available[i])
				{
					if ((increase && i < lastNumber) || (!increase && i > lastNumber))
					{
						lessThanCurrent++;
						continue;
					}
					else
					{
						available[i] = false;
						int result = recursiveSolve(di, r, index + 1, i, available, lessThanCurrent);
						available[i] = true;
						total += result;
						total %= modBase;
						lessThanCurrent++;
						continue;
					}
				}
			}
			r[index][lessThanLast] = total;
			return total;
		}
	public:
		int numPermsDISequence(string s)
		{
			len = s.size();
			vector<bool> available(len + 1, true);
			vector<vector<int>>r(len, vector<int>(len + 1, -1));
			// if DI string's length is 10, there will be 11 numbers (0 -- 10).
			// once the very first one is decided, I have ten to decide, first call into "recrusiveSolve" will be standing on index#0
			// there are 10 numbers available, there can be 0 that are less than the last number (when I put '0' as the very first),
			// there can be 10 that are less than the last number (when I use '10' as the very first).
			// The size of the 2nd dimension of the recordbook array needs to be 0 -- 10
			int total = 0;
			for (int i = 0; i <= len; i++)
			{
				available[i] = false;
				total += recursiveSolve(s, r, 0, i, available, i);
				total %= modBase;
				available[i] = true;
			}
			return total;
		}
	};


	/*
	* For example the DI sequence is   D  I  I  D  D  I
	* A 3-letter DI sequence dictates the sequence of [0,1,2,3].
	* The actual value of each element in the sequence is not important.
	* What important is whether an element is relatively big or small.
	* For example, I D I -> 2,3,0,1. This DI sequence "IDI" can also map to 25, 29, 6, 14
	* Any 4 numbers (no duplicate), order them in ascending order, call them a, b, c, d.
	* The DI sequence "IDI" just maps to c, d, a, b, whatever 'a' 'b' 'c' 'd' values are.
	* Now let me list out all the possible permutations for "IDI".
	* The number of permutations that starts with 'a' and conform to "IDI" is n1.
	* The number of permutations that starts with 'b' and conform to "IDI" is n2.
	* The number of permutations that starts with 'c' and conform to "IDI" is n3.
	* The number of permutations that starts with 'd' and conform to "IDI" is n4.
	* 
	* Now I want to compute number of permutations that conform the "DIDI".
	* I have 5 numbers: 0,1,2,3,4
	* Number of permutations start with '0':  none, because none of 1,2,3,4 can be put after '0' and conform to 'D'.
	* Number of permutations start with '1': 
	*     I have 4 numbers left: 0, 2, 3, 4, (assign them an English letter) 0->a, 2->b, 3->c, 4->d
	*     In order to conform to the first 'D', I can only put '0' after '1'.
	*     Therefore, number of permutations start with '1' for "DIDI" = number of permutations start with 'a' for "IDI"
	* Number of permutations start with '2' for "DIDI" = number of permutations start with 'a' ('0') or 'b' ('1') for "IDI"
	* Number of permutations start with '3' for "DIDI" = number of permutations start with 'a' ('0') or 'b' ('1') or 'c' ('2') for "IDI"
	* Number of permutations start with '4' for "DIDI" = number of permutations start with 'a' ('0') or 'b' ('1') or 'c' ('2') or 'd' ('3') for "IDI"
	* 
	*   +-----------------------number of permutations start with 'a' for "IDI"
	*   |    +------------------number of permutations start with 'b' for "IDI"
	*   |    |    +-------------number of permutations start with 'c' for "IDI"
	*   |    |    |    +--------number of permutations start with 'd' for "IDI"
	*   |    |    |    |
	* +-------------------+
	* | n1 | n2 | n3 | n4 |
	* +-------------------+
	* 
	* +------------------------+
	* | m1 | m2 | m3 | m4 | m5 |
	* +------------------------+
	*    |    |    |    |    |
	*    |    |    |    |    +--number of permutations starts with '4' for "DIDI" = n1+n2+n3+n4
	*    |    |    |    +-------number of permutations starts with '3' for "DIDI" = n1+n2+n3
	*    |    |    +------------number of permutations starts with '2' for "DIDI" = n1+n2
	*    |    +-----------------number of permutations starts with '1' for "DIDI" = n1
	*    +----------------------number of permutations starts with '0' for "DIDI" = 0
	* 
	*/

	//accepted 1ms beats 86%
	class Solution3
	{
	public:
		int numPermsDISequence(string s)
		{
			int base = 1'000'000'007;
			size_t n = s.size();
			vector<vector<int>> r(2, vector<int>(n+1, 0));

			size_t c = 1;  // currentVectorBeingFilled 
			size_t prev = 0; // previous

			int i = n - 1;

			if (s[i] == 'I')
			{
				r[0][0] = 1;
				r[0][1] = 0;
			}
			else
			{
				r[0][0] = 0;
				r[0][1] = 1;
			}
			--i;

			/*
			* 
			* for example s = "I D I D I D"
			* 
			*         I         D          I           D           I            D
			*                                          |
			*                                          i points at this
			* 
			* Before first iteration:
			*     the last letter is 'D'
			*     r: r[0] has two elements filled
			*            0   1  ...
			*            X   X   X  ...
			* 
			* 2nd iteration:
			* 
			*         I         D          I           D           I            D
			*                                                      i
			*     r[0]:   0      1      ...
			*     r[1]:                 0
			*     r[1]:          1      0
			*     r[1]:  0+1=1   1      0
			*     
			* 
			* 3rd iteration:
			* 
			*         I         D          I           D           I            D
			*                                          i
			* 
			*     r[1]:   1       1      0    ...
			*     r[0]:   0
			*     r[0]:   0       1
			*     r[0]:   0       1     1+1
			*     r[0]:   0       1     1+1    1+1+0
			*     
			* 
			*/


			while (i >= 0)
			{
				if (s[i] == 'I')
				{
					int k = n - i;
					r[c][k] = 0;
					int accumulate = 0;
					--k;
					while (k >= 0)
					{
						accumulate += r[prev][k];
						accumulate %= base;
						r[c][k] = accumulate;
						--k;
					}
				}
				else
				{
					int k = 0;
					r[c][0] = 0;
					int accumulate = 0;
					++k;
					while (k <= n - i)
					{
						accumulate += r[prev][k-1];
						accumulate %= base;
						r[c][k] = accumulate;
						++k;
					}
				}

				c = (c + 1) % 2;
				prev = (prev + 1) % 2;
				--i;
			}


			c = (c + 1) % 2;

			auto total = 0ull;
			for (int j = 0; j < n + 1; ++j)
			{
				total += r[c][j];
				total %= base;
			}
			return total;
		}
	};


	class BruteForce
	{
	public:

		int numPermsDISequence(string& s)
		{
			int n = s.size();
			vector<int> v(n + 1);
			iota(v.begin(), v.end(), 0);
			int validCount = 0;
			do
			{
				bool valid = true;
				for (int i = 1; i <= n; i++)
				{
					if ((s[i - 1] == 'D' && v[i - 1] < v[i]) || (s[i - 1] == 'I' && v[i - 1] > v[i]))
					{
						valid = false;
						break;
					}
				}
				if (valid)
					validCount++;
			} while (next_permutation(v.begin(), v.end()));
			return validCount;
		}
	};

	template<typename SolutionType>
	class AutoTest
	{
		random_device rd;
		mt19937 gen;
		uniform_int_distribution<> uid;
		SolutionType solu;
		BruteForce bfs;

	public:

		AutoTest() : gen(rd()), uid(0, 1) {}

		void generateDIString(string& s)
		{
			vector<int> v(s.size());
			generate(v.begin(), v.end(), std::bind(uid, std::ref(gen)));
			transform(v.begin(), v.end(), s.begin(),
				[&](int& b)
				{
					return b==1 ? 'I' : 'D';
				});
		}

		void run(int length)
		{
			string s(length, ' ');
			generateDIString(s);
			int fastAnswer = solu.numPermsDISequence(s);
			int bruteAnswer = bfs.numPermsDISequence(s);
			cout << "fastAnswer:  " << fastAnswer << "    brute force answer:  " << bruteAnswer << '\n';
			if (fastAnswer != bruteAnswer)
			{
				for (char& c : s)
					cout << c << ' ';
				
				cout << "\n=========== ERROR ============\n";
				return;
			}
		}
	};


	void Test_0903_Valid_Permutation_for_DI_Sequence()
	{
		Solution3 s;
		AutoTest<Solution3> ats;

		int option;

		while (true)
		{
			cout << "manual test: 1     auto test: 2      exit: 0  > ";
			cin >> option;
			if (option == 0) break;
			if (option == 1)
			{
				string s1;
				cout << "s: ";
				cin >> s1;
				cout << s.numPermsDISequence(s1) << '\n';
			}
			else if (option == 2)
			{
				int len;
				cout << "string length: ";
				cin >> len;
				ats.run(len);
			}
		}
	}
}