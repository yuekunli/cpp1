#include<vector>

namespace _0121_Best_Time_to_Buy_and_Sell_Stock {

	using namespace std;

    // accepted 80ms beats 5%  (Is this solution really that bad?)
    class Solution1
    {
        /*
        * for example the input is this:
        * a  b  c  d  e  f  g  h  i  j  k  m  n  p  q  r  s  t
        *                |                 |
        *               buy               sell
        * If the answer is to buy at 'f' and sell at 'm', then 'f' must be the smallest
        * number in range [a, m], and 'm' must be the biggest in range [f, t].
        * If 'c' is smaller than 'f', why don't I buy at 'c'. If 'q' is bigger than 'm',
        * why don't I sell at 'q'.
        *
        * Examine the numbers one by one, keep track of the smallest among all numbers
        * that have been examined (the emphasis is "have been examined).
        * Try each number as a selling price, this smallest number amoung the numbers
        * that have been examined must be the buying point.
        * keep track of what the max profit is.
        */

    public:
        int maxProfit(vector<int>& prices)
        {
            auto n = prices.size();
            auto i = 0;
            int minPriceSoFar = prices[0];
            int maxProfit = 0;
            for (i = 1; i < n; i++)
            {
                maxProfit = max(maxProfit, (prices[i] - minPriceSoFar));
                minPriceSoFar = min(minPriceSoFar, prices[i]);
            }
            return maxProfit;
        }
    };

    /*
    * This is the maximum sum subarray solution.
    * Turn the prices array into prices diffs array.
    * p1      p2       p3       p4       p5
    * p2-p1   p3-p2    p4-p3    p5-p4    X
    * 
    * If buying at p2 and selling at p4 is the optimal solution
    * than (diff[1] + diff[2]) is the max sum of a subarray of the diffs array
    * diff[1] + diff[2] = p3-p2 + p4-p3 = p4 - p2
    */
    // accepted 0ms beats 100% (is this really that good?)
    class Solution2
    {
    public:
        int maxProfit(vector<int>& prices)
        {
            vector<int> diff = vector<int>(prices.size(), 0);

            auto n = prices.size();
            for (int i = 0; i < n - 1; ++i)
            {
                diff[i] = prices[i + 1] - prices[i];
            }
            int maxProfit = 0;
            int currentRunningSum = 0;
            for (int i = 0; i < n - 1; ++i)
            {
                if (currentRunningSum + diff[i] < 0)
                {
                    currentRunningSum = 0;
                }
                else
                {
                    currentRunningSum += diff[i];
                    if (currentRunningSum > maxProfit)
                    {
                        maxProfit = currentRunningSum;
                    }
                }
            }
            return maxProfit;
        }
    };
}