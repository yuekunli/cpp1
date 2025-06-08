#include<vector>
#include<string>
#include<array>
#include<sstream>

namespace _0949_Largest_Time_for_Given_Digits {

	using namespace std;

	/*
	* Note that I can't greedily pursue the latest time stamp.
	* A greedy algorithm would assign the first digit a 2 if there is a 2.
	* In this example:
	* [2, 0, 6, 6]
	* 
	* If I assign 2 to the first digit, this input becomes an invalid input.
	* However, this input has a solution which is "06:26"
	*/
	// accepted 0ms beat 100%
	class Solution2
	{
	public:
		string largestTimeFromDigits(vector<int>& arr)
		{
			array<int, 4> result{ { 0,0,0,0 } };
			vector<char> count(10, 0);
			bool isResetCount = false;
			bool isFound = false;
			for (auto const& a : arr)
			{
				++count[a];
			}
			if (count[2] > 0)
			{
				result[0] = 2;
				--count[2];
				int i;
				for (i = 3; i >= 0; --i)
				{
					if (count[i] > 0)
					{
						result[1] = i;
						--count[i];
						break;
					}
				}
				if (i == -1)
				{
					isResetCount = true;
					goto CHECK_1;
				}

				for (i = 5; i >= 0; --i)
				{
					if (count[i] > 0)
					{
						result[2] = i;
						--count[i];
						break;
					}
				}
				if (i == -1)
				{
					isResetCount = true;
					goto CHECK_1;
				}

				for (i = 9; i >= 0; --i)
				{
					if (count[i] > 0)
					{
						result[3] = i;
						--count[i];
						break;
					}
				}
				stringstream ss;
				ss << result[0] << result[1] << ":" << result[2] << result[3];
				return ss.str();
			}

		CHECK_1:
			if (isResetCount)
			{
				count = vector<char>(10, 0);
				for (auto const& a : arr)
				{
					++count[a];
				}
				isResetCount = false;
			}

			if (count[1] > 0)
			{
				result[0] = 1;
				--count[1];
				int i;
				for (i = 9; i >= 0; --i)
				{
					if (count[i] > 0)
					{
						result[1] = i;
						--count[i];
						break;
					}
				}
				for (i = 5; i >= 0; --i)
				{
					if (count[i] > 0)
					{
						result[2] = i;
						--count[i];
						break;
					}
				}
				if (i == -1)
				{
					isResetCount = true;
					goto CHECK_0;
				}

				for (i = 9; i >= 0; --i)
				{
					if (count[i] > 0)
					{
						result[3] = i;
						--count[i];
						break;
					}
				}
				stringstream ss;
				ss << result[0] << result[1] << ":" << result[2] << result[3];
				return ss.str();
			}
		CHECK_0:
			if (isResetCount)
			{
				count = vector<char>(10, 0);
				for (auto const& a : arr)
				{
					++count[a];
				}
				isResetCount = false;
			}

			if (count[0] > 0)
			{
				result[0] = 0;
				--count[0];
				int i;
				for (i = 9; i >= 0; --i)
				{
					if (count[i] > 0)
					{
						result[1] = i;
						--count[i];
						break;
					}
				}
				for (i = 5; i >= 0; --i)
				{
					if (count[i] > 0)
					{
						result[2] = i;
						--count[i];
						break;
					}
				}
				if (i == -1)
					return "";

				for (i = 9; i >= 0; --i)
				{
					if (count[i] > 0)
					{
						result[3] = i;
						--count[i];
						break;
					}
				}
			}
			else
			{
				return "";
			}
			stringstream ss;
			ss << result[0] << result[1] << ":" << result[2] << result[3];
			return ss.str();
		}
	};
}

