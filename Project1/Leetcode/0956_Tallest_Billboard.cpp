#include<vector>
#include<unordered_set>
#include<unordered_map>
#include<algorithm>
#include<iostream>
#include<sstream>

namespace _0956_Tallest_Billboard {

	using namespace std;

	/*
	* back tracking?
	* The action on each elment has three options: assign it to group1, assign it to group2, or drop it.
	*/

	class Solution1
	{
		int search(int sum1, int sum2, int index, vector<int>&rods, vector<int>& rollingSum)
		{
			int diff = abs(sum1 - sum2);

			if (diff > rollingSum[index])
			{
				return 0;
			}
			if (index == 0 && diff == 0)
			{
				return sum1;
			}
			if (index == 0 && diff != rods[0])
			{
				return 0;
			}
			if (index == 0 && diff == rods[0])
			{
				return max(sum1, sum2);
			}
			int result1 = search(sum1 + rods[index], sum2, index - 1, rods, rollingSum);
			int result2 = search(sum1, sum2 + rods[index], index - 1, rods, rollingSum);
			int result3 = search(sum1, sum2, index - 1, rods, rollingSum);

			return max(result1, max(result2, result3));
		}
	public:
		int tallestBillboard(vector<int>& rods)
		{
			sort(rods.begin(), rods.end());
			auto len = rods.size();
			vector<int> rollingSum(len, 0);
			rollingSum[0] = rods[0];
			for (int i = 1; i < len; ++i)
			{
				rollingSum[i] = rollingSum[i - 1] + rods[i];
			}

			return search(0, 0, len-1, rods, rollingSum);
		}
	};

	/*
	* grouping a, b, c three numbers:
	* 
	* [], [a,b,c]
	* [a], [b,c]
	* [b], [a,c]
	* [c], [a,b]
	* 
	* grouping a, b, c, d four numbers:
	* 
	* [], [a,b,c,d]----------+
	* [a], [b,c,d]------+    |
	* [b], [a,c,d]------+----+----+
	* [c], [a,b,d]--+   |    |    |
	* [d], [a,b,c]--+---+----+    |
	* [a,b], [c,d]--+   |         |
	* [a,c], [b,d]------+---------+these two come from [b],[a,c]
	* [a,d], [b,c]------+
	* 
	* there are 4 cases when grouping 3 elements,
	* when the 4th element is added, for each of the 4 cases, putting the new element in either group leads to two scenarios.
	* Therefore, there are 4 * 2 = 8 cases when grouping 4 elements.
	*/

	class Solution2
	{
	public:
		int tallestBillboard(vector<int>& rods)
		{
			auto len = rods.size();

			vector<int> diffs;
			unordered_map<int, int>recordSet;
			diffs.emplace_back(rods[0]);
			recordSet.emplace(rods[0], rods[0]);
			diffs.emplace_back(0);
			recordSet.emplace(0, 0);

			for (int i = 1; i < len; ++i)
			{
				auto currentDiffsSize = diffs.size();

				for (int j = 0; j < currentDiffsSize; ++j)
				{
					// adding this element to the bigger group, diff becomes even bigger
					auto newDiff = diffs[j] + rods[i];
					auto biggerValue = recordSet[diffs[j]];
					auto newBiggerValue = biggerValue + rods[i];
					if ((!recordSet.contains(newDiff)))
					{
						diffs.emplace_back(newDiff);
						recordSet[newDiff] = newBiggerValue;
					}
					else if (recordSet[newDiff] < newBiggerValue)
					{
						recordSet[newDiff] = newBiggerValue;
					}


					// adding this element to the smaller group, diff may become smaller,
					// or the smaller group may become bigger.
					newDiff = abs(diffs[j] - rods[i]);
					newBiggerValue = diffs[j] > rods[i] ? biggerValue : (biggerValue + newDiff);
					if ((!recordSet.contains(newDiff)))
					{
						diffs.emplace_back(newDiff);
						recordSet[newDiff] = newBiggerValue;
					}
					else if (recordSet[newDiff] < newBiggerValue)
					{
						recordSet[newDiff] = newBiggerValue;
					}
				}

				if (!recordSet.contains(rods[i]))
				{
					diffs.emplace_back(rods[i]);
					recordSet.emplace(rods[i], rods[i]);
				}
				else if (recordSet[rods[i]] < rods[i])
				{
					recordSet[rods[i]] = rods[i];
				}
			}
			return recordSet[0];
		}
	};


	// accepted 254ms, beat 24%
	class Solution3
	{
	public:
		int tallestBillboard(vector<int>& rods)
		{
			auto len = rods.size();

			vector<int> diffs;
			unordered_map<int, int>recordSet;
			diffs.emplace_back(rods[0]);
			recordSet.emplace(rods[0], rods[0]);
			diffs.emplace_back(0);
			recordSet.emplace(0, 0);

			vector<int>a;
			unordered_map<int, int>b;

			size_t currentDiffsSize;
			int biggerValue;
			int newDiff;
			int newBiggerValue;

			for (int i = 1; i < len; ++i)
			{
				currentDiffsSize = diffs.size();

				a.clear(); // new diff values or modification-qualified diff values 
				b.clear();
				

				for (int j = 0; j < currentDiffsSize; ++j)
				{
				
					biggerValue = recordSet[diffs[j]];


					// adding this element to the bigger group, diff becomes even bigger
					
					
					newDiff = diffs[j] + rods[i];
					newBiggerValue = biggerValue + rods[i];
					if
					(
						(
							(!recordSet.contains(newDiff)) || recordSet[newDiff] < newBiggerValue
						)
						&&
						(
							(!b.contains(newDiff)) || b[newDiff] < newBiggerValue
						)
					)
					{
						if (!b.contains(newDiff))
						{
							a.emplace_back(newDiff);
						}
						b[newDiff] = newBiggerValue;
					}
					
					

					


					// adding this element to the smaller group, diff may become smaller,
					// or the smaller group may become bigger.
					newDiff = abs(diffs[j] - rods[i]);
					newBiggerValue = diffs[j] > rods[i] ? biggerValue : (biggerValue + newDiff);
					
					if
					(
						(
							(!recordSet.contains(newDiff)) || recordSet[newDiff] < newBiggerValue
						)
						&&
						(
							(!b.contains(newDiff)) || b[newDiff] < newBiggerValue
						)
					)
					{
						if (!b.contains(newDiff))
						{
							a.emplace_back(newDiff);
						}
						b[newDiff] = newBiggerValue;
					}
				}

				// don't need below section
				/*
				newDiff = rods[i];
				newBiggerValue = rods[i];

				
				if
				(
					(
						(!recordSet.contains(newDiff)) || recordSet[newDiff] < newBiggerValue
					)
					&&
					(
						(!b.contains(newDiff)) || b[newDiff] < newBiggerValue
					)
				)
				{
					if (!b.contains(newDiff))
					{
						a.emplace_back(newDiff);
					}
					b[newDiff] = newBiggerValue;
				}
				*/



				for (auto const& p : a)
				{
					if (!recordSet.contains(p))
					{
						diffs.emplace_back(p);
					}
					recordSet[p] = b[p];
				}
			}
			return recordSet[0];
		}
	};


	void Test_0956_Tallest_Billboard()
	{
		Solution3 so;
		while (true)
		{
			cout << "input: ";
			string entireInput;
			getline(cin, entireInput);
			if (entireInput.size() == 0)
			{
				break;
			}
			stringstream ss(entireInput);
			vector<int>input;
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(input));
			cout << so.tallestBillboard(input) << "\n";
		}
	}
}