#include <iostream>
#include <vector>
#include <random>
#include <functional>
#include <thread>
#include <string>
#include <sstream>

namespace Stock_Trade_Max_Profit {
	
	using namespace std;


    // This is an interview question.
    // This is on top of the most classic stock trade profit problem
    // See 121 Best Time to Buy and Sell Stock

    /*
    * Problem description:
    * 
    * Assuming the list of each day's prices is very long. I need to employ multiple computers
    * to do this job. Each computer is given a subarray of the prices list, what information
    * should each computer return? How to calculate the overall best profit given each computer's
    * output?
    */

    class Solution
    {
        /*
        * The overall best buying and selling prices can co-exist in the same segment, so each computer
        * should definitely compute the max profit within that subarray.
        * If the best buying and selling prices are in two different subarrays, I just need to know
        * the minimum and maximum price of each subarray, when I examine the output of each computer,
        * I just keep track of current minimum price among all subarrays that have been examined so far,
        * and always try the profit obtained by using current minimum price and the maximum price in the
        * next subarray.
        */

        struct SegmentResult
        {
            int min;
            int max;
            int maxProfitInSegment;
        };

    public:

        int globalMaxProfit(vector<SegmentResult>& combined_output)
        {
            int cost = combined_output[0].min;
            int profit = combined_output[0].maxProfitInSegment;

            for (size_t i = 1; i < combined_output.size(); i++)
            {
                profit = max({ profit, combined_output[i].max - cost, combined_output[i].maxProfitInSegment });
                cost = min(cost, combined_output[i].min);
            }
            return profit;
        }

        void computeSegment(vector<int>& prices, size_t start, size_t end, size_t tID, vector<SegmentResult>& segmentResults)
        {
            int seg_min = prices[start];
            int seg_max = prices[start];
            int maxProfit = 0;
            for (size_t i = start + 1; i <= end; i++)
            {
                int profit = prices[i] - seg_min;
                maxProfit = max(profit, maxProfit);
                seg_min = min(prices[i], seg_min);
                seg_max = max(prices[i], seg_max);
            }
            segmentResults[tID] = { seg_min, seg_max, maxProfit };
        }

        int maxProfit(vector<int>& prices)
        {
            size_t n = prices.size();
            int const threadsCount = 4;
            size_t segmentLength = n / 4;
            vector<SegmentResult> segmentResults(threadsCount);
            vector<thread>workers(threadsCount - 1); // new threads (not including main thread)
            size_t previous_end = 0;
            for (size_t i = 0; i < threadsCount - 1; i++)
            {
                size_t start = i == 0 ? 0 : previous_end + 1;
                size_t end = start + segmentLength - 1;
                previous_end = end;
                workers[i] = thread(&Solution::computeSegment, this, std::ref(prices), start, end, i, std::ref(segmentResults));
            }
            computeSegment(prices, previous_end + 1, n - 1, threadsCount - 1, segmentResults);

            for (size_t i = 0; i < threadsCount - 1; i++)
            {
                workers[i].join();
            }

            int answer = globalMaxProfit(segmentResults);
            return answer;
        }
    };


    class BruteForce
    {
    public:
        int maxProfit(vector<int>& prices)
        {
            size_t n = prices.size();
            int maxProfit = 0;
            for (size_t i = 0; i < n; i++)
            {
                for (size_t j = i + 1; j < n; j++)
                {
                    maxProfit = max(prices[j] - prices[i], maxProfit);
                }
            }
            return maxProfit;
        }
    };

    class Test
    {
        random_device rd;
        mt19937 gen;
        uniform_int_distribution<> intDist;

        Solution s2;
        BruteForce bf;
    public:
        Test() :gen(rd()), intDist(1, 30) {}

        bool runOnceSingleThread()
        {
            vector<int> prices(40);
            generate(prices.begin(), prices.end(), std::bind(intDist, std::ref(gen)));
            int bruteForceAnswer = bf.maxProfit(prices);
            int solutionAnswer = s1.maxProfit(prices);
            if (bruteForceAnswer != solutionAnswer)
            {
                cout << "ERROR\n";
                copy(prices.begin(), prices.end(), ostream_iterator<int>(std::cout, "  "));
                cout << "brute force answer: " << bruteForceAnswer << ",  solution answer: " << solutionAnswer << "\n";
                return false;
            }
            return true;
        }

        bool runOnceMultiThread()
        {
            vector<int> prices(100);
            generate(prices.begin(), prices.end(), std::bind(intDist, std::ref(gen)));
            int bruteForceAnswer = bf.maxProfit(prices);
            int solutionAnswer = s2.maxProfit(prices);
            if (bruteForceAnswer != solutionAnswer)
            {
                cout << "ERROR\n";
                copy(prices.begin(), prices.end(), ostream_iterator<int>(std::cout, "  "));
                cout << "\nbrute force answer: " << bruteForceAnswer << ",  solution answer: " << solutionAnswer << "\n";
                return false;
            }
            return true;
        }

        void runSingleThreadTest()
        {
            for (int i = 0; i < 20; i++)
            {
                if (!runOnceSingleThread())
                    return;
            }
            cout << "PASS\n\n";
        }

        void runMultiThreadTest()
        {
            for (int i = 0; i < 20; i++)
            {
                if (!runOnceMultiThread())
                    return;
            }
            cout << "PASS\n\n";
        }
    };


    void Test_Stock_Trade_Max_Profit()
    {
        Test t;

        Solution s2;
        BruteForce bf;

        while (true)
        {
            int option;
            cout << "manual test: 1;  auto test single thread: 2;  auto test multi thread: 3; exit: 0  >  ";
            cin >> option;
            if (option == 1)
            {
                string s;
                cin.ignore();
                cout << "Prices: ";
                getline(cin, s);
                if (s.size() == 0) break;

                vector<int>prices;

                stringstream ss(s);
                copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(prices));
                cout << "brute force: " << bf.maxProfit(prices) << ";  solution1: " << s1.maxProfit(prices) << '\n';
            }
            else if (option == 2)
            {
                t.runSingleThreadTest();
            }
            else if (option == 3)
            {
                t.runMultiThreadTest();
            }
            else if (option == 0) break;
        }
    }
}