#include "LibHeaders.h"

namespace _1048_Longest_String_Chain {

	using namespace std;

	// accepted 46ms beat 59%
	// majority is between 36ms and 61ms.
	class Solution
	{
		/*
		* Depth-first-search. Each node is a word, the edges are directional.
		* An edge goes from node1 to node2 means the word in node1 can become the word
		* in node2 by substracting a letter from word in node1.
		*/
		int solve(unordered_set<string>& s, unordered_map<string, int>& m, string& w)
		{
			if (m.contains(w))
				return m[w];
			
			int n = w.size();
			
			if (n == 1)
			{
				m[w] = 1;
				return 1;
			}
			int maxLength = 1;
			string a;
			a.resize(n - 1);
			for (int excludeIndex = 0; excludeIndex < n; ++excludeIndex)
			{
				int aIndex = 0;
				int wIndex = 0;
				for (wIndex = 0; wIndex < excludeIndex; ++wIndex, ++aIndex)
				{
					a[aIndex] = w[wIndex];
				}
				for (wIndex = excludeIndex + 1; wIndex < n; ++wIndex, ++aIndex)
				{
					a[aIndex] = w[wIndex];
				}

				if (s.contains(a))
				{
					int ret = solve(s, m, a);
					maxLength = max(maxLength, ret+1);
				}
			}
			m[w] = maxLength;
			return maxLength;
		}

	public:
		int longestStrChain(vector<string>& words)
		{
			unordered_set<string>s(words.begin(), words.end()); // used to check whether a given string is in the orignal input
			unordered_map<string, int> m; // records the length of the longest chain ending at each string.
			int maxLength = 1;
			for (auto& w : words)
			{
				if (!m.contains(w))
				{
					int len = solve(s, m, w);
					maxLength = max(maxLength, len);
				}
				else
				{
					maxLength = max(maxLength, m[w]);
				}
			}

			return maxLength;
		}
	};


	// accepted 27ms beat 96%
	class Solution2
	{
		/*
		* Depth-first-search. Each node is a word, the edges are directional.
		* An edge goes from node1 to node2 means the word in node1 can become the word
		* in node2 by substracting a letter from word in node1.
		*/
		int solve(unordered_map<string, int>& m, string& w)
		{
			int n = w.size();
			if (n == 1)
			{
				m[w] = 1;
				return 1;
			}
			int maxLength = 1;
			string a;
			a.resize(n - 1);
			for (int excludeIndex = 0; excludeIndex < n; ++excludeIndex)
			{
				int aIndex = 0;
				int wIndex = 0;
				for (wIndex = 0; wIndex < excludeIndex; ++wIndex, ++aIndex)
				{
					a[aIndex] = w[wIndex];
				}
				for (wIndex = excludeIndex + 1; wIndex < n; ++wIndex, ++aIndex)
				{
					a[aIndex] = w[wIndex];
				}

				if (m.contains(a))
				{
					if (m[a] == 0)
					{
						int ret = solve(m, a);
						maxLength = max(maxLength, ret + 1);
					}
					else
					{
						maxLength = max(maxLength, m[a] + 1);
					}
				}
				
			}
			m[w] = maxLength;
			return maxLength;
		}

	public:
		int longestStrChain(vector<string>& words)
		{
			unordered_map<string, int> m; // records the length of the longest chain ending at each string.
			for (auto const& w : words)
			{
				m[w] = 0;
			}
			int maxLength = 1;
			for (auto& w : words)
			{
				if (m[w] == 0)
				{
					int len = solve(m, w);
					maxLength = max(maxLength, len);
				}
				//else
				//{
				//	maxLength = max(maxLength, m[w]);
				//}
			}

			return maxLength;
		}
	};

	void Test_1048_Longest_String_Chain()
	{
		vector<string>words = { "a", "b", "ba", "bca", "bda", "bdca" };
		vector<string>words2 = { "xbc", "pcxbcf", "xb", "cxbc", "pcxbc" };
		Solution2 solu;

		cout << solu.longestStrChain(words2) << "\n\n";
	}
}