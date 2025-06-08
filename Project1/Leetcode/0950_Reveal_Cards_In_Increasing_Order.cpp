#include<vector>
#include<iostream>
#include<sstream>
#include<string>
#include<algorithm>

namespace _0950_Reveal_Cards_In_Increasing_Order {

	using namespace std;


	/*
	*     1   2   3   4   5   6   7
	* 
	*                                                7
	*                                            6   7
	*                                    5   7   6   x
	*                            4   6   5   7   x   x
	*                    3   7   4   6   5   x   x   x
	*            2   5   3   7   4   6   x   x   x   x
	*    1   6   2   5   3   7   4   x   x   x   x   x
	* 
	* Just reverse the process.
	* put the last to the front and add one more to the front.
	* In order to minimize dynamic memory allocation, allocate a vector that is large enough.
	* Every time I move an element from the back to the front, an unused spot is left.
	* This chart above illustrate roughly how many spots I need.
	* It's roughly 2 times the length of deck.
	*/

	//accepted 0ms beat 100%
	class Solution
	{
	public:
		vector<int> deckRevealedIncreasing(vector<int>& deck)
		{
			auto len = deck.size();
			if (len == 1)
			{
				return deck;
			}
			sort(deck.begin(), deck.end());

			// constraint: 1 <= deck[i] <= 10^5

			vector<int> a(len * 2, 0);
			auto alen = a.size();

			a[alen - 2] = deck[len - 2];
			a[alen - 1] = deck[len - 1];

			auto effectiveRangeStart = alen - 2;
			auto effectiveRangeEnd = alen - 1;
			
			int i, j;
			for (i = len - 3, j = alen-3; i >= 0; --i)
			{
				a[j--] = a[effectiveRangeEnd];
				--effectiveRangeEnd;
				a[j] = deck[i];
				effectiveRangeStart = j--;
			}

			return vector<int>(a.cbegin() + effectiveRangeStart, a.cbegin() + effectiveRangeEnd + 1);
		}
	};
}
