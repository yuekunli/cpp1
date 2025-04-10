#include<vector>
#include<iostream>
#include<iomanip>


namespace DigitDynamicProgramming {

using namespace std;

// how many numbers are there in the range a to b, where digit d occurs exactly k times in x?

class Solution
{
public:

	vector<int> num;
	int b, d, k;
	int dp[12][12][2]; // no particular reason for size 12, if the input is a signed integer, it won't be too long

	// flag: 0: previous digits are all equal to those of the upper limit
	//       1: the first distinct digit between the built sequence and the upper limit is less than that of the upper limit
	//           upper limit: 7 6 3 9 0 5 7 3 6 4 2
	// sequence built so far: 7 6 3 9 0 3 ....
	//                                  |
	//                                 first different digit is here, it's given a digit that is strictly less then upper limit's
	//                                 the rest of the digits can be assigned (as long as don't exceed k times target digit) with 0 to 9
	int call(int pos, int cnt, int flag)
	{
		if (cnt > k)
			return 0;

		if (pos == num.size())
		{
			if (cnt == k)
				return 1;
			return 0;
		}

		if (dp[pos][cnt][flag] != -1)
			return dp[pos][cnt][flag];

		int res = 0;
		int LMT;

		if (flag == 0)
		{
			LMT = num[pos];
		}
		else
		{
			LMT = 9;
		}

		for (int dgt = 0; dgt <= LMT; dgt++)
		{
			int newFlag = flag;
			int newCnt = cnt;
			if (flag == 0 && dgt < LMT)
				newFlag = 1;
			if (dgt == d)
				newCnt++;
			if (newCnt <= k)
				res += call(pos + 1, newCnt, newFlag);
		}

		return dp[pos][cnt][flag] = res;
	}

	int solve(int upperLimit, int targetDigit, int kTimes)
	{
		b = upperLimit; d = targetDigit; k = kTimes;
		num.clear();
		while (b > 0)
		{
			num.push_back(b % 10);
			b /= 10;
		}
		reverse(num.begin(), num.end()); // move most siginificant to the left

		memset(dp, -1, sizeof(dp));
		int res = call(0, 0, 0);
		return res;
	}

};

void Test_DigitDynamicProgramming()
{

	int lo, hi, d, k;
	cin >> lo >> hi >> d >> k;

	Solution s;

	int res = s.solve(hi, d, k) - s.solve(lo - 1, d, k);
	cout << res << '\n';
}

}