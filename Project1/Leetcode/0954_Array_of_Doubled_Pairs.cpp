#include<vector>
#include<unordered_set>
#include<algorithm>
#include<iostream>
#include<iomanip>

namespace _0954_Array_of_Doubled_Pairs {

	using namespace std;

	/*
	* This solution doesn't work for this case: 1, 2, 4, 8
	* '2' must pair with '1', '4' must pair with '8'.
	* If '2' is paired with '4', the input would be deemed invalid,
	* whereas this is an obviously valid input.
	* The problem is that when examining a number, if its left neighbor and right
	* neighbor are both present, I have no way to know which side this number
	* should associate itself with.
	*/
	class Solution
	{
	public:
		bool canReorderDoubled(vector<int>const & arr)
		{
			auto len = arr.size();
			unordered_multiset<int>r;
			auto pairsFound = 0;
			bool matchFound = false;
			for (auto const& a : arr)
			{
				matchFound = false;
				if (a % 2 == 0)
				{
					if (r.contains(a / 2))
					{
						r.erase(a / 2);
						++pairsFound;
						matchFound = true;
					}
				}
				if (!matchFound)
				{
					if (r.contains(a * 2))
					{
						r.erase(a * 2);
						++pairsFound;
						matchFound = true;
					}
				}
				if (!matchFound)
				{
					r.insert(a);
				}
			}
			return pairsFound == len / 2;
		}
	};

	// chase the chain in two direction
	// but this fails in test case like this: 1, 2, 2, 4
	// start searching from 1, find a chain of 1->2->4, count is 3, reject.
	//                         16  18  20  22  24  26
	// 2  4  6  8  10  12  14  16  18  20
	// How to determine where is the separation between the two chains

	// accepted 119ms beat 6% memory 89MB beat 5%
	// I thought this would be a better solution than sorting, but it turns out worse
	class Solution2
	{
	public:
		bool canReorderDoubled(vector<int>const& arr)
		{
			unordered_multiset<int>pool1(arr.cbegin(), arr.cend());

			// special handling of 0
			auto temp = pool1.count(0);
			if (temp > 0)
			{
				if (temp % 2 != 0)
					return false;
				else
					pool1.erase(0);
			}

			int i = 0;
			auto len = arr.size();
			typename unordered_multiset<int>::iterator it;
			for (i = 0; i < len; ++i)
			{
				if (pool1.contains(arr[i]))
				{
					int a = arr[i];

					int current;

					if (a > 0)
					{
						
						while (a % 2 == 0)
						{
							a /= 2;
							if (!pool1.contains(a))
							{
								current = 2 * a;

								break;
							}
							if (a == 1)
							{
								current = 1;

								break;
							}
						}
						if (a % 2 == 1 && pool1.contains(a))
							current = a;
						
					}
					else
					{
						a = -a;
						
						while (a % 2 == 0)
						{
							a /= 2;
							if (!pool1.contains(-a))
							{
								current = -2 * a;

								break;
							}
							if (a == 1)
							{
								current = -1;

								break;
							}
						}
						if (a % 2 == 1 && pool1.contains(-a))
							current = -a;
					}
					
					while (true)
					{
						if (pool1.contains(current) && pool1.contains(current * 2))
						{
							it = pool1.find(current);
							pool1.erase(it);
							it = pool1.find(current * 2);
							pool1.erase(it);
							if (pool1.contains(current))
							{
								continue;
							}
							else if (pool1.contains(current * 2))
							{
								current = current * 2;
								continue;
							}
							else if (pool1.contains(current * 4))
							{
								current = current * 4;
								continue;
							}
							else
							{
								break;
							}
						}
						else if (pool1.contains(current) && !pool1.contains(current * 2))
						{
							return false;
						}
					}
				}
			}
			return true;
		}
	};

	/*
	* input: 1, 2, 4, 8, 2, 4, 2, 4   this is a valid input
	* but sorting it yields this result:
	* 1, 2, 2, 2, 4, 4, 4, 8
	* getting the two smallest not always yield a "pair"
	* 
	* even if I make a min-heap, get head of the heap twice, i.e. two smallest numbers
	* these two numbers may not be in a "multiply by 2" relation.
	* Besides this is also O(N*logN), it's not better than just sorting.
	* 
	* another problem:
	* input: 1, 2, 4, 8, 3, 6
	* There are actually two chains
	*/
	// accepted 51ms beat 15%, memory 61MB beat 100%
	class Solution3
	{
	public:
		bool canReorderDoubled(vector<int>& arr) 
		{
			sort(arr.begin(), arr.end());
			auto len = arr.size();
			int i = 0;
			int j = 1;
			bool foundPair = false;
			while (i < len)
			{
				int a = arr[i];
				foundPair = false;
				if (a < 0)
				{
					if ((-a) % 2 != 0)
					{
						// in case this is the input: [-5, -2]
						//-2 == -5 / 2, as if I found a pair, but obviously not

						return false;
					}
					while (j < len)
					{
						if (arr[j] == a / 2)
						{
							arr[i] = 0;
							arr[j] = 0;
							++i;
							++j;
							foundPair = true;
							break;
						}
						else if (arr[j] >= 0 || (arr[j] < 0 && arr[j] > a / 2))
						{
							return false;
						}
						++j;
					}
					if (!foundPair && j == len)
					{
						return false;
					}
				}
				else if (a > 0)
				{
					while (j < len)
					{
						if (arr[j] == a * 2)
						{
							arr[i] = 0;
							arr[j] = 0;
							++i;
							++j;
							foundPair = true;
							break;
						}
						else if (arr[j] > a * 2)
						{
							return false;
						}
						++j;
					}
					if (!foundPair && j == len)
						return false;

				}
				else
				{
					// a == 0
					++i;
					if (i == j)
						++j;
				}
			}
			return true;
		}
	};

	// use counting sort
	// accepted 16ms beat 81%, memory 62MB beat 18% (memory is only worse by a little than Solution3 whose memory beat 100%)
	class Solution4
	{
	public:
		bool canReorderDoubled(vector<int>& arr)
		{
			auto maxIt = max_element(arr.cbegin(), arr.cend());
			auto minIt = min_element(arr.cbegin(), arr.cend());
			if (*maxIt > 0)
			{
				vector<int>v1((*maxIt) + 1, 0);
				for (auto const& a : arr)
				{
					if (a > 0)
					{
						++v1[a];
					}
				}
				int j = 0;

				while (j < v1.size())
				{
					if (v1[j] > 0)
					{
						if (j * 2 < v1.size() && v1[j * 2] > 0)
						{
							--v1[j];
							--v1[j * 2];
						}
						else
						{
							return false;
						}
					}
					else
					{
						++j;
					}
				}
			}
			if (*minIt < 0)
			{
				vector<int>v2((-(*minIt)) + 1, 0);
				for (auto const& a : arr)
				{
					if (a < 0)
					{
						++v2[-a];
					}
				}
				int j = 0;
				while (j < v2.size())
				{
					if (v2[j] > 0)
					{
						if (j * 2 < v2.size() && v2[j * 2] > 0)
						{
							--v2[j];
							--v2[j * 2];
						}
						else
						{
							return false;
						}
					}
					else
					{
						++j;
					}
				}
			}
			return true;
		}
	};

	void Test_0954_Array_of_Doubled_Pairs()
	{
		Solution4 so{};

		vector<int> v{ 4, -2, 2, -4 };
		cout << std::boolalpha << so.canReorderDoubled(v) << "\n"; // true
		 
		v = { 3,1,3, 6 };
		cout << std::boolalpha << so.canReorderDoubled(v) << "\n"; // false

		v = { 2,1,2, 6 };
		cout << std::boolalpha << so.canReorderDoubled(v) << "\n"; // false

		v = { 2,4,0,0,8,1 };
		cout << std::boolalpha << so.canReorderDoubled(v) << "\n"; // true

		v = { 2,1,2,1,1,1,2,2 };
		cout << std::boolalpha << so.canReorderDoubled(v) << "\n"; // true

		v = { 1,2,1,-8,8,-4,4,-4,2,-2 };
		cout << std::boolalpha << so.canReorderDoubled(v) << "\n"; // true

		v = { -6,-3 };
		cout << std::boolalpha << so.canReorderDoubled(v) << "\n"; // true
	}
}