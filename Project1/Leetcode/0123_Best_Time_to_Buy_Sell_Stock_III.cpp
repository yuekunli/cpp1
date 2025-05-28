#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#pragma warning(disable: 4267 4244)

namespace _0123_Best_Time_to_Buy_Sell_Stock_III {

	using namespace std;


	/*
	* This is the optimal solution, I didn't come up this by myself.
	* All the other solutions are my own attempts to basically achieve the same optimal complexity but with a different approach
	*/
	class Solution1
	{
		/*
		 * when I reach t1's sell point, I must have recorded the lowest point -- among all points in front of t1's sell point -- as t1's cost.
		 * It may not be the lowest point among the entire array, but it must be the lowest among all points precedes t1's sell point.
		 * Likewise, when I reach t2's sell point, t2Cost must have been recorded with the lowest (prices[i] - t1Profit) value among all such values
		 * precedes t2's sell point.
		 */

		 /*
		 * Is it possible that I recorded a t2cost at some price point
		 * that is between t1's buy and sell points? i.e. I buy t2 before selling
		 * t1? No it's not possible. For example, these are prices:
		 * a, b, c, d, e, f, g, h, i, j, k, m, n
		 *
		 * Let's say 'c' is a low price, I record t1cost and t2cost at 'c'.
		 * As long as I don't sell t1, t1profit will be 0, "price - t1profit" will
		 * just be price.
		 * As soon as I sell t1, for example I sell it at 'f'. There is a t1profit
		 * which is "f - c". Then I try to see if I should buy it back immediately
		 * at 'f'. Actually I will try to do just that, because at that point,
		 * "price - t1profit", it's still 'c', but it won't violate the not-holding-2-shares-simultaneously
		 * rule because I would pretend to sell and immediately buy back on the same day.
		 * Then if the very next day's price is lower, I won't change where I sell t1,
		 * but "price - t1profit" becomes lower, so I would change where I buy t2.
		 *
		 * Therefore, before I find a valid t1 selling point, i.e. if the first a few
		 * price points are in downward trend, the 3rd and 4th lines inside the for loop
		 * actually don't matter.
		 *
		 */
		 // accepted 0ms beats 100%
	public:
		int maxProfit(vector<int>& prices) {
			int t1Cost = INT_MAX,
				t2Cost = INT_MAX;
			int t1Profit = 0,
				t2Profit = 0;

			for (int price : prices) {
				// the maximum profit if only one transaction is allowed
				t1Cost = min(t1Cost, price);
				t1Profit = max(t1Profit, price - t1Cost);
				// re-invest the gained profit in the second transaction
				t2Cost = min(t2Cost, price - t1Profit);
				t2Profit = max(t2Profit, price - t2Cost);
			}

			return t2Profit;
		}
	};



	/*
	* Essentially I need to find 4 price points:
	* 
	* t1Buy    t1Sell   t2Buy   t2Sell
	* 
	* Overall profit = (t2Sell - t2Buy) + (t1Sell - t1Buy)
	* 
	* the chronical order of these 4 price points are: t1Buy, t1Sell, t2Buy, t2Sell
	* 
	* iterate over the prices array, at every element, evaluate the extremes of all the intermediate values and final value
	* 
	* intermediate 1: t1Buy (in the overall equation, t1Buy is being subtracted, so I want the minimum of t1Buy)
	* 
	* intermediate 2: t1Sell - t1Buy (this part, as a module, is being added in the overall equation, so I want the maximum of this module)
	* 
	* intermediate 3: The very key of this problem is to identify this intermediate module
	* transform the equation:
	* (t2Sell - t2Buy) + (t1Sell - t1Buy) = t2Sell - (t2Buy - t1Sell + t1Buy)
	*                                                \______________________/
	*                                                 this is the intermediate 3
	* intermediate 3 is being subtracted in the overall equation, so I want the minimum of this module.
	* 
	* Final value: t2Sell - (t2Buy - t1Sell + t1Buy), obviously I want the maximum of this final value.
	* 
	* Every possible final value has its corresponding set of these 4 values: t1Buy, t1Sell, t2Buy, t2Sell.
	* I don't have to worry about question like this:
	* What if I get final value 'x', but this 'x' comes from t1Buy, t1Sell, t2Buy, t2Sell 
	* but the chronological order of them is t1Buy t2Buy t1Sell t2Sell.
	* 
	* When I update intermediate value 'x', I must have a valid intermediate value 'x-1'
	* For example, if I update (t2Buy - t1Sell + t1Buy), I must have a valid (t1Sell - t1Buy)
	* 
	* at some point, I may update intermediate value 1, 2, ... x, but I don't udpate intermediate value 'x+1', but I do update intermediate 'x+2'.
	* 
	* for example this is the illustration of a price array:
	*                     *
	*                *   *
	*               * * *
	*              *   *
	*             *
	*            *
	* when I'm in the 2nd upward trend, here is the change of the 3 intermediate values and the final:
	* intermediate 1: t1Buy                                     don't update
	* intermediate 2: (t1Sell - t1Buy),                         update
	* intermediate 3: (t2Buy - t1Sell + t1Buy)                  don't udpate
	* final         : t2Sell - (t2Buy - t1Sell + t1Buy)         update
	* 
	* every time I update "final", I rely on an "old" "intermediate 3", but that is OK, because even an "old" "intermediate 3"
	* has a set of buy/sell values that are chronologically correctly ordered.
	*/


	//--------------------------------------------------------------------------------------//
	


	// Incorrect solution
	// I'm really trying to mimic the correct solution.
	// The intuition is that a big down turn after selling 1st share is a good thing for buying 2nd share.
	// So I track such said "down turn".
	// but the problem is that this said "down turn" is only the result of current_price - t1Sell.
	// If this "down turn" can replace the "intermediate 3" in the correct solution, then this solution can be correct too.
	// but obviously the calculation of this said "down turn" is different than that of the "intermediate 3" in the correct solution.

	class Solution11
	{
		int maxProfit(vector<int>& prices)
		{
			int t1Buy = prices[0];
			int t1Sell;
			int t1Profit = 0;

			int downTurnDelta = 0;
			int t2Buy = 0;

			int overallProfit = 0;
			
			for (int p : prices)
			{
				if (p < t1Buy)
				{
					t1Buy = p;
				}
				if (t1Profit < p - t1Buy)
				{
					t1Profit = p - t1Buy;
					t1Sell = p;
					downTurnDelta = 0;
					
				}
				if (t1Sell > p)
				{
					if (downTurnDelta < t1Sell - p)
					{
						downTurnDelta = t1Sell - p;
						t2Buy = p;
					}
				}
				if (t2Buy != 0)
				{
					if (overallProfit < t1Profit + (p - t2Buy))
					{
						overallProfit = t1Profit + (p - t2Buy);
					}
				}
				else
				{
					overallProfit = t1Profit;
				}
			}
			return overallProfit;
		}
	};



	class Solution2
	{
		/*
		* the strategy is to find the max sum subarray of the diff array and then
		* find the 2nd-maximum sum subarray in front of max sum subarray or after it.
		* Or sometimes I need to split the real max subarray.
		* When looking for 2nd-max subarray after the real max subarray,
		* I decide to jump over any lingering positive numbers and start searching
		* when I see a zero. but this doesn't work in this case: 1 7 2 4
		*/

		/*
		* prices:        2    1    2    0    1
		* diff:         -1    1   -2    1
		* subarrSums:    0    1    0    1
		*/

		/*
		* prices:        6    1    3    2    4    7
		* diff:         -5    2   -1    2    3
		* subarrSums:    0    2    1    3    6
		*/

		/*
		* prices:         1    2    4    2    5    7    2    4    9    0
		* diff:           1    2   -2    3    2   -5    2    5   -9
		* subarrSums:     1    3    1    4    6    1    3    8    0
		*/

		/*
		* prices:        1    7    2    4
		* diff:          6   -5    2
		* subarrSums:    6    1    3
		*/

	public:
		int maxProfit(vector<int>& prices)
		{
			int n = prices.size();
			
			if (n == 1) 
				return 0;
			
			vector<int>diff(n - 1);
			for (int i = 1; i < n; ++i)
			{
				diff[i - 1] = prices[i] - prices[i - 1];
			}

			vector<int>subarrSums(n - 1);
			int sum = 0;
			for (int i = 0; i < n - 1; ++i)
			{
				sum += diff[i];
				sum = sum > 0 ? sum : 0;
				subarrSums[i] = sum;
			}

			int start1, end1;

			auto it = max_element(subarrSums.begin(), subarrSums.end());
			end1 = distance(subarrSums.begin(), it);
			int i;
			for (i = end1 - 1; i >= 0; i--)
			{
				if (subarrSums[i] == 0)
					break;
			}
			// prices:  1  8    17    25   0    19  33
			// diff:    7  9    8     -25  19   14
			// sum:     7  16   24    0    19   33
			//                        |
			//                       i points to this when breaking out of the for loop, the day to buy is i+1
			start1 = i + 1;

			int secondaryMax = 0;

			if (start1 > 0)
			{
				it = max_element(subarrSums.begin(), subarrSums.begin() + start1);
				secondaryMax = *it;
			}
			if (end1 < subarrSums.size() - 1)
			{
				/*
				* If the arrays are like this: 
				* 
				* indix:0  1  2  3  4  5  6  7  8  9  10
				* 
				* prices:                 p6 p7
				*                         |  |
				* diff:                   x
				*                         |
				* sum:  3  5  0  0  6  9  4  0  0  2
				*                         |
				*           this '4' is after '9', this means 'x' in the diff array is a negative number.
				*           x = p7 - p6, this means p6 > p7.
				*           
				* p6 is a high price, if there was to calculate max profit for a single transaction,
				* I'll want to sell at the end of day 6.
				* I'm searching another window to make another transaction after day 6. 
				* what should be the criteria to find the start of this 2nd window?
				* I should not buy at the beginning of day 7, because day 7 is lower than day 6,
				* and day 8 is even lower. day 7 is in the middle of a downward trend, 
				* obviously I want to find the bottom of the downward trend
				*/

				//---------- This next section of this solution need revisit.---------------

				int j = end1 + 1;
				while (j < subarrSums.size() && subarrSums[j] > 0)
					j++;

				if (j < subarrSums.size())
				{
					for (; j < subarrSums.size(); j++)
					{
						secondaryMax = max(secondaryMax, subarrSums[j]);
					}
				}
			}

			int schema1 = subarrSums[end1] + secondaryMax;

			if (end1 - start1 > 1)
			{
				int schema2 = 0;
				for (int j = end1-1; j > start1; j--)
				{
					if (subarrSums[j-1] + subarrSums[end1] - subarrSums[j] > schema2)
					{
						schema2 = subarrSums[j - 1] + subarrSums[end1] - subarrSums[j];
					}
				}
				return schema1 > schema2 ? schema1 : schema2;
				
			}

			return schema1;
		}
	};




	class Solution3     // correct but slow
	{
		int maxProfit2Transactions(vector<int>const & lowsHighs)
		{
			// Note that this function assumes that lowsHighs must end with a high point.
			// If the original prices array ends with a downward trend, that trend is abandoned,
			// because it's useless to assess the best time to buy and sell stock.
			// 
			// what is the max profit with 1 transaction in a subarray of lowsHighs?
			// for example, lows highs are like this:
			//            hi1          hi2           hi3
			//     lo1           lo2           lo3
			//     |
			//    what is the best I can do when I look at a subarray starting at lo1 ending at the very end?
			// There are two possibilities when trying to find out the best I can do.
			// i.e. buy at lo1 or not buy at lo1
			// (1). If I buy at lo1, I need to find the best price to sell, 
			//      which is essentially to find the highest high in subarray [lo1, end)
			// (2). If I don't buy at lo1, then the best I can do is the same as the best
			// I can do when I look at the subarray [lo2, end).
			// 
			// If I don't buy at lo1, it's really pointless to buy at any price that is between lo1 and hi1.
			// lo1 to hi1 is a little upward trend, if I don't buy at the bottom (which is lo1), I can't get
			// a better profit if I buy at any price in between lo1 and hi1, because any other price is higher
			// than lo1.
			// If I don't buy at lo1, I can probably get a better profit if I buy at lo2, because lo2 may be
			// much lower than lo1. So if I buy at lo2 and sell at one of the high prices after lo2, I may
			// get a better profit.
			//
			size_t n = lowsHighs.size();
			vector<int> oneTranMax(n, 0);
			size_t i = n;
			if (n >= 2)
			{
				// 'i' is of size_t which is unsigned, I want to go backward,
				// I have to use a do-while loop, and make sure the very first iteration
				// is valid, i.e. if n == 1, then i - 2 becomes 1-2.
				do {
					i = i - 2;
					int maxProfitWithBuy = 0;
					int highestHigh = 0;
					for (size_t j = i + 1; j < n; j = j + 2)
					{
						if (lowsHighs[j] > highestHigh)
						{
							highestHigh = lowsHighs[j];
						}
					}
					maxProfitWithBuy = highestHigh - lowsHighs[i];

					int profit_notBuy_on_i = i < n - 2 ? oneTranMax[i + 2] : 0;
					oneTranMax[i] = max(profit_notBuy_on_i, maxProfitWithBuy);
				} while (i > 0);
			}

			int maxProfit = 0;
			for (size_t i = 0; i < n; i = i + 2)
			{
				int buy = lowsHighs[i];
				for (size_t j = i + 1; j < n; j = j + 2)
				{
					int firstTranProfit = lowsHighs[j] - buy;
					int secondTranProfit = j < n - 1 ? oneTranMax[j + 1] : 0;
					maxProfit = max(maxProfit, firstTranProfit + secondTranProfit);
				}
			}

			return maxProfit;
		}

		vector<int> findLowsAndHighs(vector<int>& prices) // this implementation doesn't feel 100% correct
		{
			size_t n = prices.size();

			vector<int>lowsHighs;
			bool isUp = false;
			// the goal is to find the first low point, that is a potential buy point.
			// If the prices start with a downward trend, find the low point, remember it.
			// If the prices start with a upward trend, then prices[0] is the first low point.
			int i;
			if (prices[0] > prices[1])
			{
				isUp = false;
				i = 1;
			}
			else if (prices[0] < prices[1])
			{
				lowsHighs.push_back(prices[0]);
				isUp = true;
				i = 1;
			}
			else
			{
				i = 1;
				while (i < n && prices[0] == prices[i])
				{
					i++;
				}
				if (i < n)
				{
					if (prices[i - 1] < prices[i])
					{
						lowsHighs.push_back(prices[i - 1]);
						isUp = true;
					}
					else
						isUp = false;
				}
			}

			for (; i < n; ++i)
			{
				if (isUp)
				{
					if (prices[i - 1] > prices[i])
					{
						lowsHighs.push_back(prices[i - 1]); // found a high point
						isUp = false;
					}
				}
				else
				{
					if (prices[i - 1] < prices[i])
					{
						lowsHighs.push_back(prices[i - 1]); // found a low point
						isUp = true;
					}
				}
			}
			if (isUp)
				lowsHighs.push_back(prices[i - 1]);

			// if the prices array ends with a downward trend, don't need to record the last low point

			return lowsHighs;
		}

		vector<int> findLowsAndHighs2(vector<int>& prices)
		{
			vector<int> lowsHighs;
			bool isUp = false;;
			lowsHighs.push_back(prices[0]);
			/*
			* Just put the first price value in lowsHighs and say we're in a downward trend.
			* Why does this work?
			* If the next one is higher, then the first price value is kept and the next
			* price value is also put in and we keep looking for if the next after that is
			* even higher. Basically the first value is a low point and is recorded.
			* 
			* If the next one is lower, the first value in lowsHighs will be replaced.
			*/
			size_t n = prices.size();
			for (size_t i = 1; i < n; ++i)
			{
				if (isUp)
				{
					if (*lowsHighs.rbegin() < prices[i])
						*lowsHighs.rbegin() = prices[i];
					else
					{
						lowsHighs.push_back(prices[i]);
						isUp = false;
					}
				}
				else
				{
					if (*lowsHighs.rbegin() > prices[i])
						*lowsHighs.rbegin() = prices[i];
					else
					{
						lowsHighs.push_back(prices[i]);
						isUp = true;
					}
				}
			}
			// if the last few price values are in a downward trend, I don't need
			// to record a low point at the end of the lowsHighs array, because I'm not
			// going to sell on that low point for sure, and I can't buy on that downward trend
			// because there isn't another high point after it.
			if (!isUp)
			{
				lowsHighs.erase(lowsHighs.rbegin().base(), lowsHighs.end());
			}
			return lowsHighs;
		}

	public:
		int maxProfit(vector<int>& prices)
		{
			if (prices.size() < 2)
				return 0;

			if (prices.size() == 2 && prices[0] >= prices[1])
				return 0;

			vector<int>const& lowsHighs = findLowsAndHighs2(prices);

			return maxProfit2Transactions(lowsHighs);
		}
	};







	// Incorrect solution
	class Solution4
	{
		/*
		* Using the same principle as the classic stock profit problem.
		* First turn the every day price to price deltas.
		* Go through the array of deltas as if I'm looking for a max profit for 1 trade.
		* for example, this is the deltas' array:
		* a  b  c  d  e  f  g  h  i  j  k
		*                         |
		*                       for example if I want to use 'i' as the 2nd trade, I should use
		* the best trade among 'a' to 'h' as the 1st trade. That is also the max profit for a
		* single trade among 'a' to 'h'. So I just keep a record of the max number as I move
		* forward, when I arrive at 'i', that max number is the best first trade. I try to
		* use a 2-trade strategy, see what I can get by make that best first trade and the 2nd
		* trade at 'i'. If that yields a better outcome for a 2-trade strategy, record that.
		* By the end of the array, I have the max number of the array which is the max profit by
		* a single trade. I also tried all the possible 2-trade scenarios.
		* The key is that when I record a better single trade, I need to reset the accumulation
		* for the 2nd trade. For example, if I'm at 'e' and I find that is a better sell point
		* for a single trade, I record that as the profit for a single trade. At that point, I've
		* sold the stock and in order to do the 2nd trade, the accumulation has to start at 0.
		*/

		/*
		* example: 6 1 3 2 4 7
		* 7             *
		* 6   *
		* 5
		* 4           *
		* 3       *   |
		* 2         * |
		* 1     *     |
		*             this is a better sell price for single trade profit.
		*             When I update firstTradeMax, I also reset accum2 to 0, as if this is the buy price for the 2nd trade
		*             As the price keeps going up, I keep counting the profit for 2nd trade assuming the buy price for the
		*             2nd trade is here. Obivously this is no optimal, because the best buy price for the 2nd price is at $2
		*/
	public:
		int maxProfit(vector<int> prices)
		{
			size_t len = prices.size();
			if (len == 1) return 0;
			if (len == 2 && prices[0] >= prices[1]) return 0;

			vector<int> deltas = vector<int>(len - 1, 0);
			for (size_t i = 1; i < len; i++)
			{
				deltas[i - 1] = prices[i] - prices[i - 1];
			}

			int firstTradeMax = deltas[0] > 0 ? deltas[0] : 0;
			int secondTradeMax = 0;
			int twoTradesMax = 0;
			int accum1 = firstTradeMax, accum2 = 0;
			for (size_t i = 1; i < len - 1; i++)
			{
				if (deltas[i] + accum1 > 0)
					accum1 += deltas[i];
				else
					accum1 = 0;

				if (deltas[i] + accum2 > 0)
				{
					accum2 += deltas[i];
					if (twoTradesMax < firstTradeMax + accum2)
						twoTradesMax = firstTradeMax + accum2;
				}
				else
					accum2 = 0;

				if (firstTradeMax < accum1)
				{
					firstTradeMax = accum1;
					accum2 = 0;
				}
			}
			return max(firstTradeMax, twoTradesMax);

		}
	};




	class Solution5
	{
		/*
		* This is the same principle as solution 4. Since Solution4 is wrong, so is this one.
		* Upon further inspection of solution 4, I don't really need to create an array of deltas.
		* When I examine an element in the delta's array, I only look at that element, not the ones
		* in front of it or behind it. So I can calculate the delta as I go. Just iterate the price
		* array and calculate the delta when I move each step forward.
		*/

	public:
		int maxProfit(vector<int> prices)
		{
			if (prices.size() == 1)
				return 0;

			int firstTradeMax = 0;
			int secondTradeMax = 0;
			int twoTradesMax = 0;
			
			int firstDelta = prices[1] - prices[0];
			firstTradeMax = firstDelta > 0 ? firstDelta : 0;
			int accum1 = firstTradeMax, accum2 = 0;

			size_t len = prices.size();
			for (size_t i = 2; i < len; i++)
			{
				int delta = prices[i] - prices[i - 1];
				if (delta + accum1 > 0)
					accum1 += delta;
				else
					accum1 = 0;

				if (delta + accum2 > 0)
				{
					accum2 += delta;
					if (twoTradesMax < firstTradeMax + accum2)
						twoTradesMax = firstTradeMax + accum2;
				}
				else
					accum2 = 0;

				if (firstTradeMax < accum1)
				{
					firstTradeMax = accum1;
					accum2 = 0;
				}
			}
			return max(firstTradeMax, twoTradesMax);
		}
	};



	// 2, 1, 4, 5, 2, 9, 7      11
	// 6  1  3  2  4  7         7
	// still wrong
	class Solution6
	{
		
	public:
		int maxProfit(vector<int> prices)
		{
			if (prices.size() == 1)
				return 0;

			int firstTradeMax = 0;
			int secondTradeMax = 0;
			int twoTradesMax = 0;

			int firstDelta = prices[1] - prices[0];
			firstTradeMax = firstDelta > 0 ? firstDelta : 0;
			int accum1 = firstTradeMax, accum2 = 0;

			size_t len = prices.size();
			for (size_t i = 2; i < len; i++)
			{
				int delta = prices[i] - prices[i - 1];
				if (delta + accum1 > 0)
					accum1 += delta;
				else
					accum1 = 0;

				if (delta + accum2 > 0)
				{
					accum2 += delta;
					if (twoTradesMax + delta < firstTradeMax + accum2)
						twoTradesMax = firstTradeMax + accum2;
					else
						twoTradesMax += delta;
				}
				else
				{
					accum2 = 0;
					if (twoTradesMax + delta < 0)
						twoTradesMax = firstTradeMax;
				}

				if (firstTradeMax < accum1)
				{
					firstTradeMax = accum1;
					accum2 = 0;
				}
			}
			return max(firstTradeMax, twoTradesMax);
		}
	};







	void Test_0123_Best_Time_to_Buy_Sell_Stock_III()
	{
		Solution1 solu;
		string s;
		vector<int> prices;
		while (true)
		{
			s.clear();
			cout << "prices: ";
			getline(cin, s);
			if (s.size() == 0) break;
			stringstream ss(s);
			prices.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(prices));
			cout << solu.maxProfit(prices) << "\n\n";
		}
	}
}