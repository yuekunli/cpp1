#include<vector>
#include<iostream>
#include<unordered_set>
#include<sstream>
#include<iomanip>
namespace _1250_Check_Good_Array
{
	using namespace std;

	/*
	* when 'a' and 'b' two numbers can make this equation a % b = 1,
	* 'a' and 'b' can form a linear equation whose result is 1.
	* Modulus operation essentially is linear combination.
	* a % b is essentially a - b - b - b ... - b, i.e. subtract 'b' from 'a' one or multiple times.
	* So initially the idea is to get any two numbers from the input, calculate the modulus (bigger % smaller)
	* if the result is 1, then problem solved, if the result is not 1, put that result to the end of the input,
	* keep trying every possible two number pair, with the candidate pool potentially growing, eventually
	* see if any two number pair can yield to modulus operation with result of 1.
	* This is at least O(n^2) cost.
	* 
	* Say here are two numbers: 20 and 7. 19 % 7 = 5, according to the idea above, I would put '5' into the pool.
	* But why don't I keep doing modulus? 7 % 5 = 2, why don't I put 2 into the pool too?
	* What does this modulus operation back and forth between two numbers resemble? -- Greatest Common Divisor.
	* The gcd algorithm is to keep doing modulus back and forth until it gets the smallest.
	* Therefore if two numbers, 'a' and 'b', if gcd(a, b) == 1, 'a' and 'b' can form a linear combination whose
	* result is 1, because the process of getting to 1 is repeated modulus operation which is essentially
	* repeated subtractions.
	* 
	*/

	// try modulus of any two numbers in the input
	class Solution
	{
	public:
		bool isGoodArray(vector<int>& num)
		{
			if (num.size() == 1 && num[0] == 1)
			{
				return true;
			}
			unordered_set<int> s;
			for (int i = 0; i < num.size(); ++i)
			{
				for (int j = i + 1; j < num.size(); ++j)
				{
					int rem = num[i] > num[j] ? num[i] % num[j] : num[j] % num[i];
						
					if (rem == 1)
					{
						return true;
					}
					else if (rem != 0)
					{
						if (!s.contains(rem))
						{
							num.emplace_back(rem);
						}
					}
				}
			}
			return false;
		}
	};

	// try greatest-common-divisor of any two numbers in the input
	// this solution fails in this case: [6, 10, 15], obviously this is good array.
	// gcd(6, 10) = 2, gcd(6, 15) = 3, gcd(10, 15) = 5, none of these gcd's is 1.
	class Solution2
	{
		int gcd(int a, int b)
		{
			if (b > a)
				return gcd(b, a);

			if (b == 0)
				return a;

			return gcd(b, a % b);
		}
	public:
		bool isGoodArray(vector<int>& num)
		{
			size_t len = num.size();
			if (len == 1 && num[0] == 1)
			{
				return true;
			}
			for (int i = 0; i < len; ++i)
			{
				for (int j = i + 1; j < len; ++j)
				{
					if (num[i] != 1 && num[j] != 1 && gcd(num[i], num[j]) == 1)
					{
						return true;
					}
				}
			}
			return false;
		}
	};

	// if two numbers gcd is not 1, put that result back into the candidate pool
	// but this exceeds time limit.
	class Solution3
	{
		int gcd(int a, int b)
		{
			if (b > a)
				return gcd(b, a);

			if (b == 0)
				return a;

			return gcd(b, a % b);
		}
	public:
		bool isGoodArray(vector<int>& num)
		{
			size_t len = num.size();
			unordered_set<int> originalInputSet;
			unordered_set<int> gcdSet;
			vector<int> gcdArray;
			int grcodi;
			for (int i = 0; i < len; ++i)
			{
				if (num[i] == 1)
				{
					return true;
				}
				if (originalInputSet.contains(num[i]))
				{
					continue;
				}
				else
				{
					originalInputSet.insert(num[i]);
					
					for (int j = i + 1; j < len; ++j)
					{
						if (num[j] == 1)
						{
							return true;
						}
						
						grcodi = gcd(num[i], num[j]);
						
						if (grcodi == 1)
						{
							return true;
						}
						else
						{
							if (gcdSet.contains(grcodi))
							{
								continue;
							}
							else
							{
								gcdSet.insert(grcodi);
								gcdArray.emplace_back(grcodi);
							}
						}
					}
				}
				
			}

			len = gcdArray.size();
			for (int i = 0; i < len; ++i)
			{
				for (int j = i + 1; j < len; ++j)
				{
					if (gcd(gcdArray[i], gcdArray[j]) == 1)
					{
						return true;
					}
				}
			}

			return false;
		}
	};

	/*
	* If there is a '2' and an odd number, just the linear combination of '2' and the odd number can result in 1,
	* because 2 can also by multiplied and becomes the even number right next to that odd number.
	* 
	* Two even numbers are likely to form a linear combination that result in 2.
	* How to test if two even numbers can do that? Again use gcd. just compute the gcd of those two even numbers.
	* 
	* Two even numbers will never have 1 as their gcd, because they are both even, at least 2 is their gcd.
	* 
	* Say two numbers, 'a' and 'b', say 'b' is fixed, the smaller 'a' is, the more likely gcd(a, b) = 1.
	* Say there are 3 numbers 'a', 'b', and 'c', I want to see if 'c' and pair with 'a' or 'b' to get a gcd equal to 1.
	* I don't really need to calculate gcd(a, c) and gcd(b, c). I actually just need to do gcd( gcd(a, b), c).
	* (I don't have solid mathematical proof on this, but this just feels correct).
	* 
	* When I iterate the input, I can "summarize" the elements I have seen so far.
	* Among the odd numbers, if none two pair can have gcd as 1, I just need to keep track of the smallest gcd.
	* Keep computing the gcd of previous result of the new element.
	* Do the same for even numbers. Once I get a '2' among even numbers, I don't even need to keep doing gcd,
	* because '2' is already the smallest element among even numbers.
	*/
	// accepted 0ms beat 100%
	class Solution4
	{
		int gcd(int a, int b)
		{
			if (b > a)
				return gcd(b, a);

			if (b == 0)
				return a;

			if (b == 1)      // with and without this shortcut check, it's the difference between 0ms and 3ms
				return 1;

			return gcd(b, a % b);
		}
	public:
		bool isGoodArray(vector<int>& num)
		{
			int lastOdd = 0;
			int lastEven = 0;
			bool has2 = false;
			int tmp;
			for (auto const& n : num)
			{
				if (n == 1)
				{
					return true;
				}
				if (n % 2 == 1)
				{
					if (lastOdd == 0)
					{
						lastOdd = n;
					}
					else if (n != lastOdd)
					{
						tmp = gcd(lastOdd, n);  // save the gcd value in "tmp" 
												// so that I don't have to compute it again should I need to assign the gcd to lastOdd,
												// this and the same treatment in the Even case (line 216) combined save 1ms.
						if (tmp == 1) // two odd numbers may have a gcd equal to 1
						{
							return true;
						}
						else
						{
							lastOdd = tmp; // record the common composing element among previous odd numbers
						}
					}
				}
				else
				{
					if (lastEven == 0)
					{
						lastEven = n;
					}
					else if (lastEven != n)
					{
						tmp = gcd(lastEven, n);
						// two even numbers' gcd is never 1, can only likely be 2
						if (tmp == 2)
						{
							has2 = true;
						}
						else
						{
							lastEven = tmp;
						}
					}
				}

				// no matter the current number if odd or even, these two criteria should always be checked
				// 1. if the so-far-smallest odd element and even element can produce gcd equal to 1
				//     ("element" doesn't mean an element in the array. It means the common composing element of previous numbers.)
				// 2. if I have "a '2' and an odd number" situation

				if (lastOdd != 0 && lastEven != 0 && gcd(lastOdd, lastEven) == 1)
				{
					return true;
				}

				if (lastOdd != 0 && has2)
				{
					return true;
				}

			}
			return false;
		}
	};


	void Test_1250_Check_Good_Array()
	{
		int opt;
		Solution solu;
		string s;
		while (true)
		{
			s.clear();
			cout << "nums: ";
			getline(cin, s);
			if (s.size() == 0)
				return;
			stringstream ss(s);
			vector<int> tmp;
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(tmp));

			cout <<boolalpha << solu.isGoodArray(tmp) << endl;
		}
	}
}