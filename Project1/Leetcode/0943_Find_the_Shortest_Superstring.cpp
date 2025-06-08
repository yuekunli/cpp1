#include<vector>
#include<string>
#include<iostream>
#include<sstream>

namespace _0943_Find_the_Shortest_Superstring {

	using namespace std;

	namespace {
		int overlapLength(string& s1, string& s2)
		{
			/*
			*     s1 :     x   x   x   x   x   x   x   x   x
			*     s2                   Y   Y   Y   Y   Y   Y   Y
			*                         \_____________________/
			*                          max possible match
			* 
			* initially two strings are lined up this way, test if the overlapping part is equal, test each element.
			* if not equal, image s1 is fixed, s2 shift to the right by one spot, so the overlapping is one element less.
			* Again test each overlapping element if they are equal one by one. If not, shift s2 again. Repeat this
			* until at some point, the overlapping part are equal.
			*/
			auto maxPossibleMatchLength = min(s1.size() - 1, s2.size() - 1);
			int matchLen = 0;

			for (int start = s1.size() - maxPossibleMatchLength; start < s1.size(); ++start)
			{
				int i = start;
				int j = 0;
				matchLen = 0;
				while (i < s1.size() && j < s2.size() && s1[i] == s2[j])
				{
					++i;
					++j;
					++matchLen;
				}
				if (matchLen == s1.size() - start)
				{
					return matchLen;
				}
			}
			return 0;
		}
	}


	/*
	* the record structure r2:
	* 
	*  0     1     2     3     4     ...      23     24
	*                          |
	*                           \
	*                          an array of pairs
	*                          [3,6]
	*                          [1,9]
	*                          [10, 7]
	* this means words[3]'s tail and words[6]'s head has overlapping of 4 letters.
	* words[1]'s tail and words[9]'s head has overlapping of 4 letters.
	* words[10]'s tail and words[7]'s head has overlapping of 4 letters.
	* 
	* 
	* 
	*  r:
	* 
	*       0    1    2     3     4     ...
	*   0
	*   1   a    b    c     d     e
	*   2   t    u    v     w     x   
	*   3
	* 
	* if words[1]'s tail and words[2]'s head merge, save the new string as words[1].
	* Now new words[1]'s head is still old words[1]'s head.
	* but new words[1]'s tail becomes old words[2]'s tail.
	* r[1][0], r[1][2], r[1][3], r[1][4] all need update
	* 
	* r[1][0] used to be:
	* 
	* words[1]  ->  x  x  x  x
	*                  Y  Y  Y  Y  Y  Y  <- words[0]
	* 
	* now r[1][0] should be:
	* 
	* words[1]+words[2]  ->   x   x   x   x   u   u   u   u   u   u
	*                                                         Y   Y   Y   Y   Y   Y   <-  words[0]
	* 
	* so essentially r[1][0]'s value should be updated with r[2][0]
	* 
	* Now if next I get a pair [1,5] from r2 structure, the easy scenario is to merge words[1]'s tail with words[5]'s head.
	* But words[1]'s tail has changed, essentially this pair [1,5] becomes an invalid pair.
	* But if I get a pair [7, 1], I can still merge words[7]'s tail with words[1]'s head, because words[1]'s head didn't change.
	* 
	* So I need a few more data structures to record which word's head or tail is still their original head and tail.
	*/

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

			vector<bool>isHeadStillOriginal(len, true);
			vector<bool>isTailStillOriginal(len, true);

			vector<vector<pair<int, int>>> r2(25, vector<pair<int, int>>());

			//vector<vector<int>> r(len, vector<int>(len, -1));  // r[i][j] meaning: words[i]'s tail and words[j]'s head has this many overlapping letters.
			for (int i = 0; i < len; ++i)
			{
				for (int j = 0; j < len; ++j)
				{
					if (i != j)
					{
						int temp = overlapLength(words[i], words[j]);
						//r[i][j] = temp;
						
						r2[temp].emplace_back(i, j);
					}
				}
			}
			int stringsLeft = len;
			for (int i = r2.size() - 1; i >= 0; --i)
			{
				if (r2[i].size() > 0)
				{
					for (int j = 0; j < r2[i].size(); ++j)
					{
						auto [idx1, idx2] = r2[i][j];
						if (isTailStillOriginal[idx1] && isHeadStillOriginal[idx2])
						{
							// overlap length = i
							string s = words[idx1] + words[idx2].substr(i);  // !!! words[idx1] or words[idx2] may not exist any more !!!
							words[idx1] = s;
							isHeadStillOriginal[idx2] = false;
							isTailStillOriginal[idx1] = false;
							//for (int k = 0; k < len; ++k)
							//{
							//	r[idx1][k] = r[idx2][k];
							//}
							if (--stringsLeft == 1)
							{
								return words[idx1];
							}
						}
					}
				}
			}
			return "";
		}
	};

	void Test_0943_Find_the_Shortest_Superstring()
	{
		Solution so;
		vector<string> words;
		string wholeInput;
		while (true)
		{
			words.clear();
			wholeInput.clear();
			cout << "words: \n";
			getline(cin, wholeInput);
			if (wholeInput.size() == 0)
				break;

			stringstream ss(wholeInput);

			copy(istream_iterator<string>(ss), istream_iterator<string>(), back_inserter(words));

			cout << "got input: \n";

			for (auto const& w : words)
			{
				cout << w << ", ";
			}
			cout << "\n";

			cout << so.shortestSuperstring(words);
			cout << "\n";
		}
	}
}