#include<iostream>
#include<math.h>
#include<vector>

namespace _0172_Factorial_Trailing_Zeroes {

	using namespace std;


	/*
	* Factorial is essentially a series of numbers multiply together.
	* (1). If there is a ten or ten's multiple, such as 10, 20, 30, it certainly adds a zero
	* (2). If there is a five or a number ending with 5, such as 5, 15, 25, 35, 45, it certainly adds a zero
	* (3). A 5 can't create a trailing zero by itself, it needs to team up with a 2. How about a 4, 6, 8, ...
	*      Yes, 5 can team up with 4, 6, 8 to create trailing zero, but 5 actually just teams up with the 2 in them.
	* (4). Summary so far, a series of numbers multiply together, a * b * c * d, ... * e
	*      each number can be further broken down, when there is a 2 and a 5, they create a zero.
	*      A 10 also creates a zero, but actually, 10 can be broken down to exactly a 2 and a 5.
	* (5). Can two number create two trailing zeroes at once? Yes 4 * 25 = 100, 36*25=900. But this doesn't break rule (4).
	*      In fact, 4 = 2 * 2, 25 = 5 * 5. There are exactly two pairs of 2 and 5, so 2 trailing zeroes are created.
	*      36 * 25 = 9 * 2 * 2 * 5 * 5, also two pairs of 2 and 5.
	* (6). In a factorial series, 2's are plenty, every even number contributes at least one 2. But 5's are comparetively scarse.
	*      I can almost think that there is unlimited supply of 2's, I just need to count how many 5's are in the series.
	* (7). 50, 500, 5000, ... these are ten's multiples, but they also carry a 5. When counting the number of 5's, don't forget
	*      to count these multiples of 50.
	* (8). 250, 1250, 2500, these numbers can be broken down to two parts: for example 1250 = 125 * 10 = 5^3 * 10
	*      
	*/


	/*
	* (1). count how many 10's are there, for example, 422:  422 / 10 = 42
	*      10, 20, 30, ..., 90, 100, 110, 120, ..., 190, 200, 210, ..., 290, 300, ..., 390, 400, 410, 420
	*      but some of these 10's actually carry 2 zeroes, such as 100, 200, 300, 400
	* (2). count home many 100's are there, 422 / 100 = 4
	* (3). 42 - 4 = 38, there are 38 numbers that end with one zero, there are 4 numbers that end with two zeroes
	* (4). How many numbers end with a 5? there is a 5 in every 10 numbers, for example, there is a 5 in 0 to 9
	*      there is a 5 in 370 to 379. 422 / 10 = 42, there are at least 42 numbers end with 5.
	*      The remain of n / 10 may have one more 5, for example, 426 / 10 = 42 but there is one more 5 in 420 to 426
	* (5). How many of these 5's are actually 5's power? 25 = 5^2, so it actually carries two 5's. 125 = 5^3.
	*      Constraints: 0 <= n <= 10^4.  5^5 = 3125,  5^6 = 15625 > 10000. So at most I just need to check 5^5
	*/
	class Solution
	{
	public:
		int trailingZeroes(int n)
		{
			vector<int> a = vector<int>(6, 0);
			
			int b = n;
			for (int i = 1; i <= 5; ++i)
			{
				a[i] = b / 10;
				b = a[i];

				if (a[i] == 0)
				{
					break;
					// at certain point if the quotient is alreay zero, don't need to try even bigger 10's power
					// for example, if n / 100 is already 0, don't bother trying n / 1000
				}

				// a[1]: how many 10's are there in n, but this includes all the 100's and 1000's and 10000's
				// a[2]: how many 100's are there in n, but this includes all the 1000's and 10000's
			}
			
			/*
			* for example, a is something like this:
			*    142,      14,       1
			*  
			* there are one-hundred-forty-two 10's and fourteen 100's and one 1000
			* but that is not accurate. there are actually thirteen 100's (subtract the amount of 1000)
			* and there are actually one-hundred-twenty-eight 10's (subtract the amount of double-zeroes and triple-zeroes)
			* 
			*/

			int totalTrailingZeroes = 0;
			int pre = 0; int current = 0;
			for (int i = 5; i >= 1; --i)
			{
				current = a[i];
				a[i] -= pre;

				totalTrailingZeroes += a[i] * i;

				pre = current;
			}

			// count amount of numbers that end with 5, one such number in every 10 numbers, for example 230 to 239
			int numberOfFives = n / 10;
			if (n % 10 >= 5)
			{
				++numberOfFives;
			}

			int powerOfFive = 25;
			for (int i = 2; i <= 6; ++i)
			{
				if (n >= powerOfFive)
				{
					
					// for example if 5^5 = 3125 is included in the factorial series
					// take 3125 out of the "numberOfFives", so minus 1 first
					// but 3125 actually contributes five 5's, so add a 5

					numberOfFives = numberOfFives - 1 + i;

					int multipleOfPowerOfFive = powerOfFive * 10;
					// test 250, 2500, 25000, ...
					
					while (n >= multipleOfPowerOfFive)
					{
						numberOfFives = numberOfFives + i; // don't need to minus 1, because 250, 2500, etc are not counted as numbers ending with 5.
						multipleOfPowerOfFive *= 10;
					}

					powerOfFive *= 5;
				}
				else
				{
					break;
				}
			}

			int multipleOfFifty = 50;
			for (int i = 1; i <= 6; ++i)
			{
				if (n >= multipleOfFifty)
				{
					++numberOfFives;
					multipleOfFifty *= 10;
				}
				else
				{
					break;
				}
			}

			return totalTrailingZeroes + numberOfFives;
		}
	};


	/*
	* Solution1 is all good but missed 1 case.
	* if n = 80, the correct result is 19.
	* There are eight 10's, 10, 20, 30, ..., 80
	* There are eight numbers ending with 5: 5, 15, 25, ..., 75
	* Amoung these ending 5's, 25 is a power of 5
	* 8     +    8      -    1          +        2
	* 10's       5's       take out 25          25 contributes two 5's
	* 50 contributes 1 more 5
	* So the total is 18, but 18 is wrong? Where did it go wrong?
	* "75" --->  3 * 25 = 3 * 5 * 5
	* 75 is counted as ending with 5, but 75 actually contributes two 5's.
	* This is because 75 is actually a multiple of 25 which is a power of 5.
	* So this solution only focuses on 5's.
	* calculate multiple of 5's, true multiple, not just 10 times: 1*5, 2*5, 3*5, 4*5, 5*5, 6*5, ... X*5
	* calculate multiple of 5^2: 1*25, 2*25, 3*25, 4*25, ...
	* 
	* multiple of 5's includes multiple of 25's and multiple of 125's
	* this is similar to the calculation of multiple of 10's, 100's, 1000's in Solution1
	* 
	*/

	// accepted 0ms beats 100%, memory 8.1MB beats 85%
	class Solution2
	{
	public:
		int trailingZeroes(int n)
		{

			int powerOfFive = 5;
			

			vector<int> a = vector<int>(6, 0);

			for (int i = 1; i <= 5; ++i)
			{
				a[i] = n / powerOfFive;
				if (a[i] == 0)
				{
					break;
				}
				powerOfFive *= 5;
			}

			int total = 0;
			int pre = 0;
			int current = 0;

			for (int i = 5; i >= 1; --i)
			{
				current = a[i];
				a[i] -= pre;
				total += a[i] * i;
				pre = current;
			}

			return total;
		}
	};

	/*
	* exact same idea as Solution2
	* just a little optimization to eliminate the vector
	*/
	// accepted 0ms beats 100%, memory 8.2MB beats 52%
	// this even used more memory than Solution2???
	class Solution3
	{
	public:
		int trailingZeroes(int n)
		{
			int total = 0;
			int current = n / 5;
			int powerOfFive = 25;
			int next = 0;
			int i = 1;
			while (true)
			{
				next = n / powerOfFive;
				total += (current - next) * i;
				
				if (next == 0)
				{
					break;
				}
				current = next;
				powerOfFive *= 5;
				++i;
			}
			return total;
		}
	};
}