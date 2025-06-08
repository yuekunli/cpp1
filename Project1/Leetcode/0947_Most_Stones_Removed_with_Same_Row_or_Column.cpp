#include<vector>
#include<map>
#include<unordered_map>
#include<unordered_set>

namespace _0947_Most_Stones_Removed_with_Same_Row_or_Column {

	using namespace std;

	/*
	* all the points that are connected by either vertical or horizontal lines are reduced down to 1
	* Essentially this is to compute connected components of a graph
	*/

	//accepted 30ms beats 37%
	class Solution
	{
		template<typename Map1, typename Map2, typename Set1, typename Point>
		void search(Map1& xKeyMap, Map2& yKeyMap, Set1& visited, Point&& p)
		{
			for (auto const& y : xKeyMap[p.first])
			{
				if (!visited.contains(pair<int, int>(p.first, y)))
				{
					visited.emplace(p.first, y);
					search(xKeyMap, yKeyMap, visited, Point(p.first, y));
				}
			}
			for (auto const& x : yKeyMap[p.second])
			{
				if (!visited.contains(pair<int, int>(x, p.second)))
				{
					visited.emplace(x, p.second);
					search(xKeyMap, yKeyMap, visited, Point(x, p.second));
				}
			}
		}
	public:
		int removeStones(vector<vector<int>>& stones)
		{
			unordered_map<int, vector<int>>xCoorKeyMap;
			unordered_map<int, vector<int>>yCoorKeyMap;

			auto pairHash = [](auto const& a) {return ((size_t)a.first) * 10'000 + (size_t)a.second; };
			auto pairComp = [](auto const& a, auto const& b) {return a.first == b.first && a.second == b.second; };
			unordered_set < pair<int, int>, decltype(pairHash), decltype(pairComp) > visited(stones.size(), pairHash, pairComp);

			for (auto const& p : stones)
			{
				xCoorKeyMap[p[0]].push_back(p[1]);
				yCoorKeyMap[p[1]].push_back(p[0]);
			}

			int stonesLeft = 0;

			auto total = stones.size();

			for (auto const& p : stones)
			{
				if (!visited.contains(pair<int, int>(p[0], p[1])))
				{
					++stonesLeft;
					visited.emplace(p[0], p[1]);
					search(xCoorKeyMap, yCoorKeyMap, visited, pair<int, int>(p[0], p[1]));
				}
			}
			return total - stonesLeft;
		}
	};

	class Solution2
	{
	public:
		int removeStones(vector<vector<int>>& stones)
		{

		}
	};
}