#include<vector>
#include<iterator>
#include<iostream>

namespace _1203_Sort_Items_by_Groups_Respecting_Dependencies {

	using namespace std;

	/*
	* If the max group number is 50, when I run into the first element that has group -1,
	* assign group 51 to it. Next one that has group -1 gets group ID 52, so on so forth.
	* 
	* If A belongs to group 10, and B belongs to group 15, and A must be before B, this means
	* group 10 must be before group 15. Following this principle, build a dependency list among groups.
	* Then first topologically sort the groups. Then topologically sort elements in each group.
	*/
	//accepted 18ms beat 98%, memory 41MB beat 99%
	class Solution
	{
		const int NOT_VISITED = 0;
		const int ON_ACTIVE_PATH = 1;
		const int VISITED_NOT_ON_ACTIVE_PATH = 2;
		int n;
		int m;
		bool dfsOneGroup(int k, vector<int>& group, vector<vector<int>>& adj, vector<int>& sortedList, vector<int>& visited)
		{
			visited[k] = ON_ACTIVE_PATH;
			for (auto const& preceedingElem : adj[k])
			{
				if (group[preceedingElem] != group[k])
				{
					continue;
				}
				if (visited[preceedingElem] == NOT_VISITED)
				{
					bool ret = dfsOneGroup(preceedingElem, group, adj, sortedList, visited);
					if (!ret)
					{
						return false;
					}
				}
				else if (visited[preceedingElem] == ON_ACTIVE_PATH)
				{
					return false;
				}
				else
				{
					continue;
				}
			}
			sortedList.emplace_back(k);
			visited[k] = VISITED_NOT_ON_ACTIVE_PATH;
			return true;
		}

		bool dfsGroup(int k, vector<vector<int>>& groupAdj, vector<int>& visited, vector<int>& sortedList)
		{
			visited[k] = ON_ACTIVE_PATH;
			for (auto const& preceedingGroup : groupAdj[k])
			{
				if (visited[preceedingGroup] == NOT_VISITED)
				{
					bool ret = dfsGroup(preceedingGroup, groupAdj, visited, sortedList);
					if (!ret)
					{
						return false;
					}
				}
				else if (visited[preceedingGroup] == VISITED_NOT_ON_ACTIVE_PATH)
				{
					continue;
				}
				else
				{
					// on active path
					return false;
				}
			}
			sortedList.emplace_back(k);
			visited[k] = VISITED_NOT_ON_ACTIVE_PATH;
			return true;
		}
	public:
		vector<int> sortItems(int n, int m, vector<int>& group, vector<vector<int>>& beforeItems)
		{
			this->n = n;
			//this->m = m;

			int newGroupCounter = 0;
			for (int i = 0; i < n; ++i)
			{
				if (group[i] == -1)
				{
					group[i] = m + newGroupCounter;
					++newGroupCounter;
				}
			}

			this->m = m + newGroupCounter;

			vector<vector<int>> groupAdj(this->m, vector<int>());
			for (int i = 0; i < n; ++i)
			{
				int thisElemGroup = group[i];
				auto const& preceedingElems = beforeItems[i];
				for (auto const& preceedingElem : preceedingElems)
				{
					int preceedingGroup = group[preceedingElem];
					if (thisElemGroup != preceedingGroup)
					{
						groupAdj[thisElemGroup].emplace_back(preceedingGroup);
					}
				}
			}

			vector<int> groupSorted;
			vector<int> groupVisitedRecord(this->m, NOT_VISITED);
			for (int i = 0; i < this->m; ++i)
			{
				if (groupVisitedRecord[i] == NOT_VISITED)
				{
					bool ret = dfsGroup(i, groupAdj, groupVisitedRecord, groupSorted);

					if (!ret)
					{
						return vector<int>();
					}
				}
			}

			vector<int> elemsSorted;
			vector<int> elemsOneGroupSorted;
			vector<int> elemsVisitedRecord(n, NOT_VISITED);

			// find all elements for each group
			vector<vector<int>> groupToElem(this->m, vector<int>());
			for (int i = 0; i < n; ++i)
			{
				groupToElem[group[i]].push_back(i);
			}

			for (auto const& g : groupSorted)
			{
				elemsOneGroupSorted.clear();
				for (auto const& e : groupToElem[g])
				{
					if (elemsVisitedRecord[e] == NOT_VISITED)
					{
						bool ret = dfsOneGroup(e, group, beforeItems, elemsOneGroupSorted, elemsVisitedRecord);
						if (!ret)
						{
							return vector<int>();
						}
					}
				}
				
				copy(elemsOneGroupSorted.begin(), elemsOneGroupSorted.end(), back_inserter(elemsSorted));
			}

			return elemsSorted;
		}
	};

	static void test1()
	{
		Solution solu;
		int n = 8;
		int m = 2;
		vector<int> group{ -1, -1, 1, 0, 0, 1, 0, -1 };
		vector<vector<int>> beforeItems = { {}, {6}, {5}, {6}, {3,6}, {}, {}, {} };
		vector<int> ret = solu.sortItems(n, m, group, beforeItems);
		for (auto const& n : ret)
		{
			cout << n << "  ";
		}
		cout << endl << "end" << endl;
	}

	static void test2()
	{
		Solution solu;
		int n = 8;
		int m = 2;
		vector<int> group{ -1, -1, 1, 0, 0, 1, 0, -1 };
		vector<vector<int>> beforeItems = { {}, {6}, {5}, {6}, {3}, {}, {4}, {} };
		vector<int> ret = solu.sortItems(n, m, group, beforeItems);

		for (auto const& n : ret)
		{
			cout << n << "  ";
		}
		cout << endl << "end" << endl;
	}

	static void test3()
	{
		Solution solu;
		int n = 4;
		int m = 1;
		vector<int> group{ -1, 0, 0, -1 };
		vector<vector<int>> beforeItems = { {}, {0}, {1,3}, {2} };
		vector<int> ret = solu.sortItems(n, m, group, beforeItems);

		for (auto const& n : ret)
		{
			cout << n << "  ";
		}
		cout << endl << "end" << endl;
	}

	void Test_1203_Sort_Items_by_Groups_Respecting_Dependencies()
	{
		//test1();
		//test2();
		test3();
	}
}