#include<vector>
#include<algorithm>
namespace _0948_Bag_of_Tokens {
	using namespace std;

	/*
	* a fixed strategy, greedy algorithm
	* sort the tokens, after sorting, assume these are teh tokens:
	* a   b   c   d   e   f   g   h
	* play the low value tokens until current power is not enought to play any more.
	* Now I have gather some scores.
	* Spend 1 score to get the highest token, play the current lowest token to get that
	* score back, but the diff between the highest token and the lowest token become my
	* power accumulation. repeat such process, my score remain the same but my power
	* keeps growing, until at some point my power is high enough to play current lowest
	* token then do that, keep spending power on lowest tokens until it can continue such.
	* then go back to spending 1 score on highest token then play lowest token.
	*/
	// accepted 0ms beat 100%
	class Soltuion
	{
		int bagOfTokensScore(vector<int>& tokens, int power)
		{
			if (tokens.size() == 0)
				return 0;

			sort(tokens.begin(), tokens.end());
			auto it1 = lower_bound(tokens.cbegin(), tokens.cend(), power);

			if (power < tokens[0])
			{
				return 0;
			}

			int i = 0;
			auto score = 0;
			while (i < tokens.size() && power >= tokens[i])
			{
				power -= tokens[i];
				++score;
				++i;
			}

			auto lowIndex = i;
			auto hiIndex = tokens.size() - 1;

			while (true)
			{
				if (lowIndex + 1 >= hiIndex)
					break;

				power += (tokens[hiIndex--] - tokens[lowIndex++]);

				// at some point the power may even be high enough to play
				// multiple lowest tokens, that's why this next line is a while loop
				// instead of just checking 1 token.
				while (lowIndex < tokens.size() && power >= tokens[lowIndex])
				{
					power -= tokens[lowIndex++];
					++score;
				}
			}
			return score;
		}
	};
}