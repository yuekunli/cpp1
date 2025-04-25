#include<iostream>
#include<string>
#include<sstream>
#include<vector>

namespace _0188_Best_Time_Buy_Sell_Stock_IV {

	using namespace std;

	/*
	* If k is '2', I can do "buy sell buy sell". A buy action and a sell action together are called 1 transaction
	*/



	// The common reason that Solution1 ~ Solution7 are all slow is because within 1 recurrsion frame,
	// there is looping. i.e. there is either "for" loop or "while" loop within the "solve" function.
	// Or, for example, Solution6, one scenario doesn't set any value in records array, 
	// so that scenario is repeatedly solved.
	// 
	// The reason I had to either use loop or had trouble setting values in records array is becasue in this scenario:
	//     there is an outstanding share, it was bought at price 'x', and I'm standing on a high price point, 
	//     with 'k' transactions allowed, what is the best I can do? 
	// 
	// At first glance the best solution for this subproblem depends on the buying price of that outstanding share. 
	// So if the best result turns out to be selling that share and gather the best profit from the rest of
	// the prices array, then the formula is:
	// 
	//      current_price - x + result from the rest. 
	// 
	// I can't set this value in the records array because 'x' here is a varying value. 
	// Next time I'm in the same scenario, 'x' can be different, if I just read from the record array, 
	// wouldn't that produce a wrong answer? Yes it would.
	// 
	// The key is that the best action I can take when in that scenario doesn't depend on the buying price
	// of that outstanding share. Transform that formula a little:
	//      (current_price + result_from_the_rest) - x
	//      \____________________________________/
	//        what action I take decides the value of this part
	// 
	// No matter what 'x' is, whem I'm in such situation, I should always take the best action and the
	// result within those parantheses is the same. What I really should set in the records array is the value
	// of the part inside parantheses.
	// 

	// By studying the optimal solution provided by leetcode, it looks that converting the original prices array
	// to a lows-highs array doesn't save that much time (maybe even causes some slow down).
	// And it also doesn't quite matter how the lows-highs array is built, whether to create a new array or modify the input
	// prices array in-place doesn't seem to matter that much.

	// time limit exceeded
	class Solution1
	{
		size_t n;
		int solve(vector<int>& prices, size_t index, int k, vector<vector<int>>& r, int previousBuyPrice)
		{
			if (index == n)
				return 0;
			if (k == 0)
				return 0;

			if (previousBuyPrice == -1 && r[index][k] != -1)
				return r[index][k];

			int maxProfit = 0;
			if (previousBuyPrice >= 0)
			{
				for (size_t i = index; i < n; i++)
				{
					int profit = prices[i] - previousBuyPrice + solve(prices, i+1, k - 1, r, -1);
					maxProfit = max(maxProfit, profit);
				}
				return maxProfit;
			}
			else
			{
				for (size_t i = index; i < n; i++)
				{
					int profit = solve(prices, i + 1, k, r, prices[i]); // check the possibility of buying at each index
					maxProfit = max(maxProfit, profit);

					profit = solve(prices, i + 1, k, r, -1);// check the possibility of not doing anything at each index and looking at the rest of the array with a "fresh" start
					maxProfit = max(maxProfit, profit);
				}
				r[index][k] = maxProfit;
				return maxProfit;
			}
		}

	public:
		int maxProfit(int k, vector<int>& prices)
		{
			n = prices.size();
			vector<vector<int>>records(n, vector<int>(k + 1, -1));
			return solve(prices, 0, k, records, -1);
		}
	};

	// still too slow
	class Solution2
	{
		size_t n;
		int solve(vector<int>& prices, vector<vector<vector<int>>>& recordWithStockBought, vector<vector<int>>&recordNoStock, size_t index, size_t previousBuyIndex, int k)
		{
			if (index == n)
				return 0;
			if (k == 0)
				return 0;

			if (previousBuyIndex == -1 && recordNoStock[index][k] != -1)
				return recordNoStock[index][k];

			if (previousBuyIndex > -1 && recordWithStockBought[index][previousBuyIndex][k] != -1)
				return recordWithStockBought[index][previousBuyIndex][k];

			int maxProfit = 0;
			if (previousBuyIndex >= 0)
			{
				for (size_t i = index; i < n; i++)
				{
					int profit = prices[i] - prices[previousBuyIndex] + solve(prices, recordWithStockBought, recordNoStock, i + 1, -1, k - 1);
					maxProfit = max(maxProfit, profit);
				}
				recordWithStockBought[index][previousBuyIndex][k] = maxProfit;
				return maxProfit;
			}
			else
			{
				for (size_t i = index; i < n; i++)
				{
					int profit = solve(prices, recordWithStockBought, recordNoStock, i + 1, i, k); // check the possibility of buying at each index
					maxProfit = max(maxProfit, profit);

					profit = solve(prices, recordWithStockBought, recordNoStock, i+1, -1, k);// check the possibility of not doing anything at each index and looking at the rest of the array with a "fresh" start
					maxProfit = max(maxProfit, profit);
				}
				recordNoStock[index][k] = maxProfit;
				return maxProfit;
			}
		}

	public:
		int maxProfit(int k, vector<int>& prices)
		{
			n = prices.size();
			vector<vector<int>>recordNoStock(n, vector<int>(k + 1, -1));
			vector<vector<vector<int>>>recordWithStockBought(n, vector<vector<int>>(n, vector<int>(k + 1, -1)));
			return solve(prices, recordWithStockBought, recordNoStock, 0, -1, k);
		}
	};


	class Solution3
	{
		size_t n;
		int solve(vector<int>& prices, vector<vector<vector<int>>>& recordWithStockBought, vector<vector<int>>& recordNoStock, size_t index, size_t previousBuyIndex, int k)
		{
			if (index == n)
				return 0;
			if (k == 0)
				return 0;

			if (previousBuyIndex == -1 && recordNoStock[index][k] != -1)
				return recordNoStock[index][k];

			if (previousBuyIndex > -1 && recordWithStockBought[index][previousBuyIndex][k] != -1)
				return recordWithStockBought[index][previousBuyIndex][k];

			int maxProfit = 0;
			if (previousBuyIndex >= 0)
			{
				for (size_t i = index; i < n; i++)
				{
					if ((i < n - 1 && prices[previousBuyIndex] < prices[i] && prices[i] >  prices[i + 1]) || (i == n - 1 && prices[previousBuyIndex] < prices[i]))
						              // only sell if i's price is higher than (i+1)'s price, otherwise, hold on and sell later
						              // actually if i's price is lower than (i-1)'s price, I shouldn't trying selling at i either, should've sold earlier.
					{
						int profit = prices[i] - prices[previousBuyIndex] + solve(prices, recordWithStockBought, recordNoStock, i + 1, -1, k - 1);
						maxProfit = max(maxProfit, profit);
					}
				}
				recordWithStockBought[index][previousBuyIndex][k] = maxProfit;
				return maxProfit;
			}
			else
			{
				for (size_t i = index; i < n; i++)
				{
					int profit = 0;

					if (i < n - 1 && prices[i] < prices[i + 1]) // don't buy if i's prices is not even lower than (i+1)'s price
					{
						profit = solve(prices, recordWithStockBought, recordNoStock, i + 1, i, k);
						maxProfit = max(maxProfit, profit);
					}
					maxProfit = max(maxProfit, profit);
				}
				recordNoStock[index][k] = maxProfit;
				return maxProfit;
			}
		}

	public:
		int maxProfit(int k, vector<int>& prices)
		{
			n = prices.size();
			vector<vector<int>>recordNoStock(n, vector<int>(k + 1, -1));
			vector<vector<vector<int>>>recordWithStockBought(n, vector<vector<int>>(n, vector<int>(k + 1, -1)));
			return solve(prices, recordWithStockBought, recordNoStock, 0, -1, k);
		}
	};



	class Solution4
	{
		/*
		* If at some point, I have 3 transactions left (k=3), and the "lowsHighs" array has 2 pairs of (low, high) combination,
		* I should stop trying because a better solution must have been reached when I had 3 pairs of (low, high) combination.
		* But be careful when the lowsHighs array doesn't even have that many pairs to begin with.
		* For example: prices: [2, 4, 1];  k = 2.  This prices array is converted to a lowsHighs array like this: [2, 4]
		* There is only 1 pair and k = 2.
		* But if there are 5 pairs in total, k= 3, I picked 1 pair, and skipped 2 pairs, now I'm looking at 2 final pairs,
		* but I just need 2 more pairs. 
		*/

		size_t n2;
		int originalK;
		int solve(vector<int>& lowsHighs, vector<vector<int>>& r1, vector<vector<vector<int>>>& r2, size_t index, size_t buyIndex, int k)
		{
			if (index == n2) return 0;
			if (k == 0) return 0;

			if (buyIndex == -1 && r1[index][k] != -1)
				return r1[index][k];
			if (buyIndex > -1 && r2[index][buyIndex][k] != -1)
				return r2[index][buyIndex][k];

			int maxProfit = 0;
			if (buyIndex == -1) // no previous buy
			{
				for (size_t i = index; i < n2; i++)
				{
					if (i % 2 == 0) // I'm looking at a low point (low points have indices: 0, 2, 4, 6, ....)
					{
						if (n2/2 < originalK || (n2 - i) / 2 >= k) // There are more transactions allowed than I can do, so buy in of course
						{
							int profit = solve(lowsHighs, r1, r2, i + 1, i, k);
							maxProfit = max(maxProfit, profit);
						}
						// Why do I not consider when there are limited transactions allowed?
					}
				}
				r1[index][k] = maxProfit;
				return maxProfit;
			}
			else
			{
				for (size_t i = index; i < n2; i++)
				{
					if (i % 2 == 1 && lowsHighs[i] > lowsHighs[buyIndex])
					{
						int profit = lowsHighs[i] - lowsHighs[buyIndex] + solve(lowsHighs, r1, r2, i + 1, -1, k - 1);
						maxProfit = max(maxProfit, profit);
					}
				}
				r2[index][buyIndex][k] = maxProfit;
				return maxProfit;
			}
		}
	public:
		int maxProfit(int k, vector<int>& prices)
		{
			originalK = k;
			size_t n = prices.size();
			if (n < 2) return 0;

			vector<int>lowsHighs;
			bool isUp;
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
				else
				{
					// the entire array is flat
					return 0;
				}
			}

			for (; i < n; i++)
			{
				if (isUp)
				{
					if (prices[i - 1] > prices[i])
					{
						lowsHighs.push_back(prices[i - 1]);
						isUp = false;
					}
				}
				else
				{
					if (prices[i-1] < prices[i])
					{
						lowsHighs.push_back(prices[i - 1]);
						isUp = true;
					}
				}
			}
			if (isUp)
				lowsHighs.push_back(prices[i - 1]);
			
			n2 = lowsHighs.size();
			vector<vector<int>>r1(n2, vector<int>(k + 1, -1));
			vector<vector<vector<int>>>r2(n2, vector<vector<int>>(n2, vector<int>(k + 1, -1)));
			return solve(lowsHighs, r1, r2, 0, -1, k);
		}
	};


	class Solution5
	{
		/*
		* The big improvement from Solution4 to Solution5 is that
		* I don't really need to remember where I bought the stock.
		*/

		size_t n2;
		int originalK;
		int solve(vector<int>& lowsHighs, vector<vector<vector<int>>>& r, size_t index, int k, bool bought)
		{
			if (index == n2) return 0;
			if (k == 0) return 0;

			if ((!bought) && r[index][k][0] != -1)
				return r[index][k][0];
			if (bought && r[index][k][1] != -1)
				return r[index][k][1];

			int maxProfit = 0;
			if (!bought)
			{
				for (size_t i = index; i < n2; i++)
				{
					if (i % 2 == 0) // only try buying at a low point
					{
						if (n2 / 2 < originalK || (n2 - i) / 2 >= k)
						{
							int profit = solve(lowsHighs, r, i + 1, k, true) - lowsHighs[i]; // a call to "solve" with "true" as the last arg will return a result including the sell price
							maxProfit = max(maxProfit, profit);
						}
					}
				}
				r[index][k][0] = maxProfit;
				return maxProfit;
			}
			else  // bought stock before
			{
				for (size_t i = index; i < n2; i++)
				{
					if (i % 2 == 1)
					{
						int profit = lowsHighs[i] + solve(lowsHighs, r, i + 1, k - 1, false); // return the sell price and the optimal solution of the rest of the problem. The caller of this can add the sell price to its own optimal solution
						maxProfit = max(maxProfit, profit);
					}
				}
				r[index][k][1] = maxProfit;
				return maxProfit;
			}
		}
	public:
		int maxProfit(int k, vector<int>& prices)
		{
			originalK = k;
			size_t n = prices.size();
			if (n < 2) return 0;

			vector<int>lowsHighs;
			bool isUp;
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
				while (i < n && prices[i] == prices[0])
					i++;
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
				else
				{
					// entire prices array is flat.
					return 0;
				}
			}

			for (; i < n; i++)
			{
				if (isUp)
				{
					if (prices[i - 1] > prices[i])
					{
						lowsHighs.push_back(prices[i - 1]);
						isUp = false;
					}
				}
				else
				{
					if (prices[i - 1] < prices[i])
					{
						lowsHighs.push_back(prices[i - 1]);
						isUp = true;
					}
				}
			}
			if (isUp)
				lowsHighs.push_back(prices[i - 1]);

			n2 = lowsHighs.size();
			
			vector<vector<vector<int>>>r(n2, vector<vector<int>>(k+1, vector<int>(2, -1)));
			return solve(lowsHighs, r, 0, k, false);
		}
	};


	class Solution6
	{
		size_t lowHighCounts;
		int originalK;

		int solve(vector<int>& lowsHighs, vector<vector<int>>& r, size_t index, int k, int previousBuyPrice)
		{
			if (index == lowHighCounts)
				return 0;

			if (k == 0)
				return 0;

			if (previousBuyPrice == -1) // didn't buy before
			{
				if (index % 2 == 0) // this is a low point
				{
					if (r[index][k] != -1) // has record
					{
						return r[index][k];
					}
					else // no record
					{
						int remainingPairs = (lowHighCounts - index) / 2;
						if (remainingPairs == k)
						{
							// number of remaining low-high pairs is less than or equal to the number of transactions allowed
							// take all the possible profit
							// which means, I must buy here and sell at the very next high price
							int profit = lowsHighs[index+1] - lowsHighs[index];
							if (index != lowHighCounts - 2) // this is not the last low-high pair
							{
								profit += solve(lowsHighs, r, index + 2 /*go direct to next low price*/, k - 1, -1);
							}
							r[index][k] = profit;
							return profit;
						}
						else // for example, there are 200 pairs left, but I can only make 50 transactions amoung them.
						{
							int profitBuyHere = 0;
							int profitNotBuyHere = 0;

							profitBuyHere = solve(lowsHighs, r, index + 1, k, lowsHighs[index]);
							
							if (index != lowHighCounts - 2)
							{
								profitNotBuyHere = solve(lowsHighs, r, index + 2, k, -1);
							}

							int maxProfit = max(profitBuyHere, profitNotBuyHere);
							r[index][k] = maxProfit;
							return maxProfit;
						}
					}
				}
				else // this is a high point, of cource don't buy at this price
				{
					// it should not happen that this is the very last high price point, and there is still transactions allowed.
					// I'm basically wasting transaction
					if (index != lowHighCounts-1) // but to be safe, do this boundry check
					{
						return solve(lowsHighs, r, index + 1, k, -1);
					}
					else
					{
						return 0;
					}
				}
			}
			else // I bought before
			{
				if (index % 2 == 0) // this is a low price, of course don't sell
				{
					return solve(lowsHighs, r, index + 1, k, previousBuyPrice);
				}
				else // this is high price, I have two options (1) sell here (2) sell at some later high price
				{
					int immediateProfitSellHere = 0;
					int profitNotSellHere = 0;

					int profitAfterSellHere = 0;

					if (previousBuyPrice < lowsHighs[index])
					{
						immediateProfitSellHere = lowsHighs[index] - previousBuyPrice;
					}

					if (index != lowHighCounts - 1)
					{
						profitNotSellHere = solve(lowsHighs, r, index + 2, k, previousBuyPrice);
					}
					
					if (index != lowHighCounts - 1)
					{
						profitAfterSellHere = solve(lowsHighs, r, index + 1, k-1, -1);
					}
					
					// !!!! Attention, there seems no loop in "solve", but this solution is still slow
					// !!!!===========================================================================================
					// the bottleneck of this solution is that this is not setting any value in records 2-D array here.
					// !!!!===========================================================================================

					return max(immediateProfitSellHere+profitAfterSellHere, profitNotSellHere);
				}
			}
		}

	public:
		int maxProfit(int k, vector<int>& prices)
		{
			originalK = k;
			size_t n = prices.size();
			if (n < 2) return 0;

			vector<int>lowsHighs;
			bool isUp;
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
				while (i < n && prices[i] == prices[0])
					i++;
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
				else
				{
					return 0;
				}
			}

			for (; i < n; i++)
			{
				if (isUp)
				{
					if (prices[i - 1] > prices[i])
					{
						lowsHighs.push_back(prices[i - 1]);
						isUp = false;
					}
				}
				else
				{
					if (prices[i - 1] < prices[i])
					{
						lowsHighs.push_back(prices[i - 1]);
						isUp = true;
					}
				}
			}
			if (isUp)
				lowsHighs.push_back(prices[i - 1]);

			lowHighCounts = lowsHighs.size();

			vector<vector<int>>r(lowHighCounts, vector<int>(k + 1, -1));
			return solve(lowsHighs, r, 0, k, -1);
		}
	};




	class Solution7
	{
		size_t lowHighCounts;
		int originalK;

		int solve(vector<int>& lowsHighs, vector<vector<int>>& r, size_t index, int k, int previousBuyPrice)
		{
			if (index == lowHighCounts)
				return 0;

			if (k == 0)
				return 0;

			int indexInRecord = index / 2;


			if (r[indexInRecord][k] != -1) // has record
			{
				return r[indexInRecord][k];
			}
			else // no record
			{
				int remainingPairs = (lowHighCounts - index) / 2;
				if (k > remainingPairs)
				{
					k = remainingPairs;
				}
				if (remainingPairs == k)
				{
					// number of remaining low-high pairs is less than or equal to the number of transactions allowed
					// take all the possible profit
					// which means, I must buy here and sell at the very next high price
					int profit = lowsHighs[index + 1] - lowsHighs[index];
					if (index != lowHighCounts - 2) // this is not the last low-high pair
					{
						profit += solve(lowsHighs, r, index + 2 /*go direct to next low price*/, k - 1, -1);
					}
					r[indexInRecord][k] = profit;
					return profit;
				}
				else // for example, there are 200 pairs left, but I can only make 50 transactions amoung them.
				{
					int maxProfitBuyHere = 0;

					int i = index + 1;
					int viableImmediateProfit = 0;
					while ( i < lowHighCounts  &&   ((lowHighCounts - i - 1) / 2 >=  k - 1)  )
					{
						int p = lowsHighs[i] - lowsHighs[index];
						if (p > viableImmediateProfit)
						{
							viableImmediateProfit = p;
							int profit = p + solve(lowsHighs, r, i + 1, k - 1, -1);
							maxProfitBuyHere = max(maxProfitBuyHere, profit);
						}
						i += 2;
					}

					int maxProfitNotBuyHere = 0;
					if (index != lowHighCounts - 2)
					{
						maxProfitNotBuyHere = solve(lowsHighs, r, index + 2, k, -1);
					}

					r[indexInRecord][k] = max(maxProfitBuyHere, maxProfitNotBuyHere);

					return r[indexInRecord][k];
				}
			}			
		}

	public:
		int maxProfit(int k, vector<int>& prices)
		{
			originalK = k;
			size_t n = prices.size();
			if (n < 2) return 0;
			int j = 0;
			
			bool isUp;
			int i;
			if (prices[0] > prices[1])
			{
				isUp = false;
				i = 1;
			}
			else if (prices[0] < prices[1])
			{
				++j;
				isUp = true;
				i = 1;
			}
			else
			{
				i = 1;
				while (i < n && prices[i] == prices[0])
					i++;
				if (i < n)
				{
					if (prices[i - 1] < prices[i])
					{
						prices[j] = prices[i - 1];
						++j;
						isUp = true;
					}
					else
						isUp = false;
				}
				else
				{
					return 0;
				}
			}

			for (; i < n; i++)
			{
				if (isUp)
				{
					if (prices[i - 1] > prices[i])
					{
						prices[j] = prices[i - 1];
						++j;
						isUp = false;
					}
				}
				else
				{
					if (prices[i - 1] < prices[i])
					{
						prices[j] = prices[i - 1];
						++j;
						isUp = true;
					}
				}
			}
			if (isUp)
			{
				prices[j] = prices[i - 1];
				++j;
			}

			lowHighCounts = j;
			if (lowHighCounts / 2 < k)
			{
				k = lowHighCounts / 2;
			}
			vector<vector<int>>r(lowHighCounts/2, vector<int>(k + 1, -1));
			return solve(prices, r, 0, k, -1);
		}
	};








	// accepted but not ideal 
	class Solution8
	{
		size_t n2;
		int originalK;
		int solve(vector<int>& lowsHighs, vector<vector<vector<int>>>& r, size_t index, int k, bool bought)
		{
			if (index == n2) return 0;
			if (k == 0) return 0;

			if (!bought && r[index][k][0] != -1)
				return r[index][k][0];
			if (bought && r[index][k][1] != -1)
				return r[index][k][1];

			int maxProfit = 0;
			if (!bought)
			{
				size_t i = index;
				int profit = 0;
				// It feels that the handling here is not clean, as to whether I should try buying or moving on or both. but this is accepted.
				// the next solution makes this part cleaner.
				if (i % 2 == 0) // only try buying at a low point
				{
					if (n2 / 2 < originalK || (n2 - i) / 2 >= k)
					{
						profit = solve(lowsHighs, r, i + 1, k, true) - lowsHighs[i]; // a call to "solve" with "true" as the last arg will return a result including the sell price
						maxProfit = max(maxProfit, profit);
					}
				}
				profit = solve(lowsHighs, r, i + 1, k, false); // looking at a high point and I didn't buy stock before, so just solve the rest of the array
				maxProfit = max(maxProfit, profit);
				r[index][k][0] = maxProfit;
				return maxProfit;
			}
			else  // bought stock before
			{
				size_t i = index;
				int profit = 0;
				if (i % 2 == 1)
				{
					// suppose I sell at this high point, return the price of this high point and the best result for the rest of the array
					int profit = lowsHighs[i] + solve(lowsHighs, r, i + 1, k - 1, false); // return the sell price and the optimal solution of the rest of the problem. The caller of this can add the sell price to its own optimal solution
					maxProfit = max(maxProfit, profit);
				}
				profit = solve(lowsHighs, r, i + 1, k, true);
				maxProfit = max(maxProfit, profit);
				r[index][k][1] = maxProfit;
				return maxProfit;
			}
		}
	public:
		int maxProfit(int k, vector<int>& prices)
		{
			originalK = k;
			size_t n = prices.size();
			if (n < 2) return 0;

			vector<int>lowsHighs;
			bool isUp;
			if (prices[0] > prices[1])
				isUp = false;
			else if (prices[0] < prices[1])
			{
				lowsHighs.push_back(prices[0]);
				isUp = true;
			}
			else
			{
				int i = 1;
				while (i < n && prices[i] == prices[0])
					i++;
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

			int i;
			for (i = 2; i < n; i++)
			{
				if (isUp)
				{
					if (prices[i - 1] > prices[i])
					{
						lowsHighs.push_back(prices[i - 1]);
						isUp = false;
					}
				}
				else
				{
					if (prices[i - 1] < prices[i])
					{
						lowsHighs.push_back(prices[i - 1]);
						isUp = true;
					}
				}
			}
			if (isUp)
				lowsHighs.push_back(prices[i - 1]);

			n2 = lowsHighs.size();

			vector<vector<vector<int>>>r(n2, vector<vector<int>>(k + 1, vector<int>(2, -1)));
			return solve(lowsHighs, r, 0, k, false);
		}
	};



	class Solution9  // cleaner way to build the lowsHighs array
	{
		size_t lowHighCounts;
		int totalTransactionsAllowed;

		int solve(vector<int>& lowsHighs, vector<vector<vector<int>>>& r, size_t index, int k, bool bought)
		{
			if (index == lowHighCounts) return 0;
			if (k == 0) return 0;

			if (!bought && r[index][k][0] != -1)
				return r[index][k][0];
			if (bought && r[index][k][1] != -1)
				return r[index][k][1];

			int maxProfit = 0;
			if (!bought)
			{
				int profit = 0;
				if (index % 2 == 0) // only try buying at a low point
				{
					if (lowHighCounts / 2 < totalTransactionsAllowed || (lowHighCounts - index) / 2 <= k)
					{
						// I have more transaction quota than the trades left. So just buy at every low and sell at every high, maximize the profit for this subarray.
						for (size_t i = index + 1; i < lowHighCounts; i = i + 2)
						{
							maxProfit += (lowsHighs[i] - lowsHighs[i - 1]);
						}
					}
					else
					{
						// I don't have that many transactions quota left, I need to try both strategies.
						{// buy
							profit = solve(lowsHighs, r, index + 1, k, true) - lowsHighs[index]; // a call to "solve" with "true" as the last arg will return a result including the sell price
							maxProfit = max(maxProfit, profit);
						}

						{// not buy
							profit = solve(lowsHighs, r, index + 1, k, false); // looking at a high point and I didn't buy stock before, so just solve the rest of the array
							maxProfit = max(maxProfit, profit);
						}
					}
				}
				else
				{
					// I'm looking at a high point, of course I should not buy, so just move on.
					profit = solve(lowsHighs, r, index + 1, k, false);
					maxProfit = max(maxProfit, profit);
				}

				r[index][k][0] = maxProfit;
				return maxProfit;
			}
			else  // bought stock before
			{
				int profit = 0;
				if (index % 2 == 1)
				{
					// suppose I sell at this high point, return the price of this high point and the best result for the rest of the array
					int profit = lowsHighs[index] + solve(lowsHighs, r, index + 1, k - 1, false); // return the sell price and the optimal solution of the rest of the problem. The caller of this can add the sell price to its own optimal solution
					maxProfit = max(maxProfit, profit);

					profit = solve(lowsHighs, r, index + 1, k, true);
					maxProfit = max(maxProfit, profit);
				}
				else
				{
					// looking at a low point, don't sell, move on.
					profit = solve(lowsHighs, r, index + 1, k, true);
					maxProfit = max(maxProfit, profit);
				}

				r[index][k][1] = maxProfit;
				return maxProfit;
			}
		}
	public:
		int maxProfit(int k, vector<int>& prices)
		{
			vector<int>lowsHighs;
			bool isUp;

			totalTransactionsAllowed = k;
			if (prices.size() < 2) return 0;

			lowsHighs.push_back(prices[0]);
			isUp = false; // I assume I start with a down trend, this way I won't record prices[0] if the prices array really starts with a down trend, which is what I want.
			// basically, if I assume I start with a down trend, I want to catch the first low point, if I assume I start with an up trend, I want to catch the first high point.
			size_t n = prices.size();
			for (size_t i = 1; i < n; i++)
			{
				if (isUp)
				{
					if (prices[i - 1] <= prices[i])
						*lowsHighs.rbegin() = prices[i];
					else
					{
						lowsHighs.push_back(prices[i]);
						isUp = false;
					}
				}
				else
				{
					if (prices[i - 1] >= prices[i])
						*lowsHighs.rbegin() = prices[i];
					else
					{
						lowsHighs.push_back(prices[i]);
						isUp = true;
					}
				}
			}
			if (!isUp)
			{
				// the prices array ends with a down trend, I should've recorded the last prices as a low, but I don't need that.
				lowsHighs.erase(lowsHighs.end() - 1);
			}

			lowHighCounts = lowsHighs.size();

			vector<vector<vector<int>>>r(lowHighCounts, vector<vector<int>>(k + 1, vector<int>(2, -1)));
			return solve(lowsHighs, r, 0, k, false);
		}
	};





	// accepted 3ms beats 82%, memory 16MB beats 67%
	class Solution10
	{
		size_t lowHighCounts;

		int solve(vector<int>& lowsHighs, vector<vector<int>>& r, int index, int k, int previousBuyPrice)
		{
			if (index == lowHighCounts)
			{
				return 0;
			}
			if (k == 0)
			{
				return 0;
			}
			if (index % 2 == 0)
			{
				// this is a low price point.
				// It's guaranteed that I don't have an outstanding share.
				// If I buy at some previous low price point, the recurssion makes sure the next frame lands on a high price point,
				// if that high price point decides not to sell, it moves forward by two elements in the lowsHighs array, which
				// makes sure the next frame still lands on high price point.

				if (r[index][k] != -1)
				{
					return r[index][k];
				}
				else
				{
					if ((lowHighCounts - index) / 2 == k)
					{
						r[index][k] = lowsHighs[index + 1] - lowsHighs[index] + solve(lowsHighs, r, index + 2, k - 1, -1);
						return r[index][k];
					}
					else
					{
						int profitBuyHere = 0;
						int profitNotBuyHere = 0;

						profitBuyHere = solve(lowsHighs, r, index + 1, k, lowsHighs[index]) - lowsHighs[index];

						profitNotBuyHere = solve(lowsHighs, r, index + 2, k, -1);

						r[index][k] = max(profitBuyHere, profitNotBuyHere);
						return r[index][k];
					}
				}
			}
			else
			{
				// high price point
				// it's guaranteed that there is an outstanding share.
				// if I bought at some previously low price point, the recurssion moves forward by 1 element in the lowsHighs array
				// so the next frame of the recurrsion lands on a high price point, if I don't buy here, I move forward by 2
				// elements, which again guarantees the next frame lands on a high price point.

				if (r[index][k] != -1)
				{
					return r[index][k];
				}
				else
				{
					int positiveAccumulationIfSellHere = 0;
					int positiveAccumulationIfNotSellHere = 0;
					positiveAccumulationIfSellHere = lowsHighs[index] + solve(lowsHighs, r, index + 1, k - 1, -1);
					if (index != lowHighCounts - 1)
					{
						positiveAccumulationIfNotSellHere = solve(lowsHighs, r, index + 2, k, previousBuyPrice);
					}
					r[index][k] = max(positiveAccumulationIfSellHere, positiveAccumulationIfNotSellHere);
					return r[index][k];
				}
			}
		}

	public:
		int maxProfit(int k, vector<int>& prices)
		{
			size_t n = prices.size();
			if (n < 2) return 0;
			int j = 0;

			bool isUp;
			int i;
			if (prices[0] > prices[1])
			{
				isUp = false;
				i = 1;
			}
			else if (prices[0] < prices[1])
			{
				++j;
				isUp = true;
				i = 1;
			}
			else
			{
				i = 1;
				while (i < n && prices[i] == prices[0])
					i++;
				if (i < n)
				{
					if (prices[i - 1] < prices[i])
					{
						prices[j] = prices[i - 1];
						++j;
						isUp = true;
					}
					else
						isUp = false;
				}
				else
				{
					return 0;
				}
			}

			for (; i < n; i++)
			{
				if (isUp)
				{
					if (prices[i - 1] > prices[i])
					{
						prices[j] = prices[i - 1];
						++j;
						isUp = false;
					}
				}
				else
				{
					if (prices[i - 1] < prices[i])
					{
						prices[j] = prices[i - 1];
						++j;
						isUp = true;
					}
				}
			}
			if (isUp)
			{
				prices[j] = prices[i - 1];
				++j;
			}

			lowHighCounts = j;
			if (lowHighCounts / 2 < k)
			{
				k = lowHighCounts / 2;
			}
			vector<vector<int>>r(lowHighCounts, vector<int>(k + 1, -1));
			return solve(prices, r, 0, k, -1);
		}
	};





	void Test_0188_Best_Time_Buy_Sell_Stock_IV()
	{
		Solution6 s;
		string s1;
		int k;
		vector<int>prices;

		while (true)
		{
			s1.clear();
			cout << "prices: ";
			getline(cin, s1);
			if (s1.size() == 0) break;
			stringstream ss(s1);
			prices.clear();
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(prices));
			cout << "k: ";
			cin >> k;
			cin.ignore();
			cout << s.maxProfit(k, prices) << "\n\n";
		}
	}
}