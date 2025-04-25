#include<vector>

namespace _0122_Best_Time_to_Buy_and_Sell_Stock_II {

	using namespace std;

	/*
	* Break the prices array into a series of consecutive trends:
	* 
	*                      *
	*           *       *     *   *
	* *       *   *  *               *
	*   *   *                           *
	*     *
	*
	* There are 5 trends in this array: down, up, down, up, down
	* Buy at the bottom of each downward trend and sell at the top of each upward trend
	*
	* If the prices start in a upward trend, the first day's price can be viewed as the
	* bottom of a virtual downward trend preceding this array.
	*/
	// accepted 0ms beats 100%
	class Solution
	{
		int maxProfit(vector<int>& prices)
		{
			if (prices.size() == 1)
			{
				return 0;
			}

			bool isHolding = false;
			int totalProfit = 0;
			int i = 1;
			auto n = prices.size();
			int buyPrice;

			while (true)
			{
				if (!isHolding)
				{
					while (i < n && prices[i - 1] >= prices[i])
					{
						++i;
					}

					if (i == n)
					{
						// the last trend of the prices array is downward, no more change to make money
						return totalProfit;
					}

					// i-1 is the bottom of the downward trend

					buyPrice = prices[i - 1];
					isHolding = true;
				}
				else
				{
					while (i < n && prices[i - 1] <= prices[i])
					{
						++i;
					}

					totalProfit += (prices[i - 1] - buyPrice);
					isHolding = false;

					if (i == n)
					{
						return totalProfit;
					}
				}
			}
		}
	};
}