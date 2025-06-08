#include<vector>
#include<unordered_set>
#include<iostream>

namespace _0952_Largest_Component_Size_by_Common_Factor {

	using namespace std;

	class Solution
	{
		int gcd(int a, int b)
		{
			if (a == b)
				return a;
			if (a == 0)
				return b;

			if (a > b)
				return gcd(b, a);

			return gcd(b % a, a);
		}

		template<typename Container, typename VisitContainer, typename Number>
		int search(Container const& nums, VisitContainer & visited, Number n)
		{
			auto count = 0;
			auto len = nums.size();
			for (auto i = 0; i < len; ++i)
			{
				if (!visited.contains(nums[i]))
				{
					if (gcd(nums[i], n) > 1)
					{
						visited.insert(nums[i]);
						++count;
						count += search(nums, visited, nums[i]);
					}
				}

			}
			return count;
		}

	public:
		int largestComponentSize(vector<int>const & nums)
		{
			unordered_set<int>visited;

			int maxComponentSize = 0;
			auto len = nums.size();
			for (int i = 0; i < len; ++i)
			{
				if (!visited.contains(nums[i]))
				{
					visited.insert(nums[i]);
					int count = 1 + search(nums, visited, nums[i]);
					if (maxComponentSize < count)
					{
						maxComponentSize = count;
					}
				}
			}
			return maxComponentSize;
		}
	};

	void Test_0952_Largest_Component_Size_by_Common_Factor()
	{
		cout << Solution().largestComponentSize({ 20,50,9,63 }) << "\n";
	}

}