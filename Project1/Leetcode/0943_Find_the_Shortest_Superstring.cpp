#include<vector>
#include<string>

namespace _0943_Find_the_Shortest_Superstring {

	using namespace std;

	namespace {
		int overlapLength(string& s1, string& s2)
		{
			auto testLen = min(s1.size() - 1, s2.size() - 1);
			int i = s1.size() - testLen;
			int j = 0;
			int matchLen = 0;
			while (i < s1.size() && j < s2.size() && s1[i] == s2[j])
			{
				++i;
				++j;
				++matchLen;
			}
			return matchLen;
		}
	}

	class Solution
	{
	public:
		string shortestSuperstring(vector<string>& words)
		{
			auto len = words.size();

			if (len == 1)
			{
				return words[0];
			}

			vector<vector<pair<int, int>>> r2(25, vector<pair<int, int>>());

			vector<vector<int>> r(len, vector<int>(len, -1));
			for (int i = 0; i < len; ++i)
			{
				for (int j = 0; j < len; ++j)
				{
					if (i != j)
					{
						int temp = overlapLength(words[i], words[j]);
						r[i][j] = temp;
						if (temp > 0)
							r2[temp].emplace_back(i, j);
					}
				}
			}
			for (int i = r2.size() - 1; i > 0; --i)
			{
				if (r2[i].size() > 0)
				{
					for (int j = 0; j < r2[i].size(); ++j)
					{
						auto [idx1, idx2] = r2[i][j];
						// overlap length = i
						string s = words[idx1] + words[idx2].substr(i);
						words[idx1] = s;
						for (int k = 0; k < len; ++k)
						{
							r[idx1][k] = r[idx2][k];
						}
					}
				}
			}
		}
	};
}