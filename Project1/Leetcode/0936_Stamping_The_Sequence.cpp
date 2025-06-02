#include<vector>
#include<deque>
#include<string>
#include<iostream>


namespace _0936_Stamping_The_Sequence {

	using namespace std;

	namespace {
		string::size_type stampLen;
		string::size_type targetLen;

		void findMatchingFromBeginning(string& stamp, string& target, int start, int& matchLength)
		{
			matchLength = 0;
			int i = 0, j = start;
			while (i < stampLen && j < targetLen)
			{
				if (stamp[i] == target[j])
				{
					++i;
					++j;
					++matchLength;
				}
				else
				{
					break;
				}
			}
		}

		/*
		*  target :    a     b     c     d     e     f     g     h     i     j     k
		*                                                      \_/
		*                                                  \_____/
		*                                            \___________/
		*                                      \_________________/
		*                                \_______________________/
		*                          \_____________________________/
		*                   ...__________________________________/
		*
		* test if a segment of "target" (ending at 'h') matches a suffix of stamp.
		* In another word, test if 'h' matches the very last letter of stamp,
		* test if 'g' matches the 2nd last letter of stamp, if 'f' matches the 3rd last, ...
		*/
		void findMatchingFromEnd(string& stamp, string& target, int start, int& matchLength)
		{
			matchLength = 0;
			int i = stampLen - 1;
			int j = start;
			while (i > 0 && j > 0)
			{
				if (stamp[i] == target[j])
				{
					--i;
					--j;
					++matchLength;
				}
				else
				{
					break;
				}
			}
		}

		void findMatchingStampSuffix(string& stamp, string& target, int start, int& matchLength)
		{
			matchLength = 0;
			int possibleMatchLength = 0;
			int k = 0, i, j;
			for (k = 0; k < stampLen; ++k)
			{
				i = k;
				j = start;
				possibleMatchLength = 0;
				while (i < stampLen && j < targetLen && stamp[i] == target[j])
				{
					++i;
					++j;
					++possibleMatchLength;
				}
				if (possibleMatchLength == (stampLen - k))
				{
					matchLength = possibleMatchLength;
					return;
				}
			}
		}

		void findMatchingStampSuffix2(string& stamp, string& target, int start, int& matchLength)
		{
			matchLength = 0;
			int k = 0, i, j;
			for (k = stampLen - 1; k > 0; --k)
			{
				i = k;
				j = start;

				while (i < stampLen && j < targetLen && stamp[i] == target[j])
				{
					++i;
					++j;
				}
				if (i == stampLen)
				{
					matchLength = stampLen - k;
					return;
				}
			}
		}

		void findMatchingInMiddle(string& stamp, string& target, int start, int& stampMatchSegmentStart, int& matchLength)
		{
			matchLength = 0;

			int possibleMatchLength = 0;
			int k = 0; // starting index of a segment of the stamp, this segment matches the segment in target.

			int i, j;

			for (k = 0; k < stampLen; ++k)
			{
				// if at some point, the distance between 'k' and the end of stamp is already shorter
				// than a "matchLength" found earlier, I won't be able to find a longer "matchLength"
				// for this point on.
				// 0 1 2 3 4 5 6 7
				//           |
				//           k          stamp length = 8, k = 5, the max possible match length from this point on is 3
				// if the best match length I found earlier is 4, there is no point keep comparing.
				// but if the previous best match is 3, and this 3-letter ending segment is a match,
				// should I use the middle match or this suffix match? I should use this suffix match,
				// because if this is a suffix match, that gives more flexibility to the next match.
				// if this is a suffix match, the next match can be any. if this is a pure middle match, the next must be prefix.

				if (stampLen - k < matchLength) // use < instead of <=, if it's equal, keep searching, in hope to find suffix match
				{
					return;
				}

				i = k;
				j = start;
				possibleMatchLength = 0;
				while (i < stampLen && j < targetLen && stamp[i] == target[j])
				{
					++i;
					++j;
					++possibleMatchLength;
				}
				if (possibleMatchLength >= matchLength) // using >= instead of >, in equal, update it, in hope to find suffix match.
				{
					matchLength = possibleMatchLength;
					stampMatchSegmentStart = k;
					if (matchLength == stampLen)
					{
						return;
					}
				}
			}
		}

		struct Record
		{
			int targetStartIndex;
			int stampStartIndex;
			int matchLength;
			int matchType; // 0: tail needs cover, 1: tail is fully shown
			Record(int ti, int si, int ml, int mt)
				:targetStartIndex(ti), stampStartIndex(si), matchLength(ml), matchType(mt)
			{}
		};

		bool isPrefix(string& stamp, string& s1, string& s2)
		{
			int i = 0;
			int j = 0;
			while (i < stampLen && j < s1.size() && stamp[i] == s1[j])
			{
				++i;
				++j;
			}
			if (j != s1.size())
			{
				return false;
			}
			else
			{
				j = 0;
				while (i < stampLen && j < s2.size() && stamp[i] == s2[j])
				{
					++i;
					++j;
				}
				if (j != s2.size())
				{
					return false;
				}
			}
			return true;
		}
	}

	/*
	*
	*                   |-----------|                      |-----------|
	*          |-----------|                     |-----------|
	*       |-----------|                 |-----------|
	* |-----------|              |-----------|
	*
	*
	*
	*
	*                   |-----------|   |-----------|
	*          |-----------|              |-----------|
	*       |-----------|                   |-----------|
	* |-----------|               |-----------|
	*
	*
	* There are only 3 types of segments:
	*
	* prefix + prefix + prefix + ... + prefix
	*
	* a middle part
	*
	* suffix + suffix + ... + suffix
	*
	* if stamps is abcdefg
	*
	* type 1: abc + abcd + a + abcdefg + ab + abcde + ...
	* type 2: cde
	* type 3: cfg + fg + cdefg + g + bcdefg + ...
	*
	* From the beginning of the target, there must be a type 1 segment.
	* From the end of the target, there must be a type 3 segment.
	* somewhere in the middle of target, they may or may not be a type 2 segment
	* There can't be two consecutive type 2 segments
	* for example, I can't have ...bcef...
	* because if I first put the stamp in place to create "bc", then if I line up stamp with "ef",
	* the 2nd stamping action is going to overwrite "bc".
	*
	* how to deal with a full stamp? A full stamp can be seen as a prefix or a suffix.
	*
	* for example:
	* stamp: abcdef
	* target:    ab   abc   cde   abcdef  def  f
	*          \_________/ \___/ \______________/
	* I can identify the first segment as type 1, then I can identify the 2nd segment as type 2.
	* But for the beginning of the 3rd segment, it's easy to identify it as type 1, but it's part of a bigger type 3 segment.
	* Or maybe it's OK to identify the full stamp as a type 1, just identify the last 4 letters as a type 3 segment.
	* In another word, a full stamp can be seen as a prefix and suffix is not a dilemma, it's actually a benefit.
	* I can treat a full stamp either way and not affect the overall result.
	*/

	/*
	* The key idea of these solutions is that, if at any point, I find a prefix match, and it's not a full stamp match,
	* for example, the stamp is "abcdef", and I find a segment "abcd", there *must* be another prefix match right after this.
	* In another word, in order to create "abcd", I line up the stamp and press, but what I pressed into the stamp is "abcdef".
	* The tail, "ef", must be covered by another press. This "abcd" can't be followed by a pure middle part match.
	* such as "abcd" + "bcd". Because in order to create a pure middle part match, I must use a pressing sequence like this:
	* |-------------|        |-------------|
	*            |-------------|
	* This means the segment to the left of the middle part match must be a suffix match (can be a full stamp match, because
	* full stamp match is also a suffix match, but must not be a pure prefix match).
	* In order to have "abcd", there must be something that "covers" "ef":
	*            |-------------| <-- this cover must start with the beginning of that stamp, so it must be a prefix match.
	* a  b  c  d  e  f
	* The only scenario I can have a pure middle part match is when the previous matching segment ends with the end of the stamp,
	* in another word, the previous match is a suffix match.
	* 
	* When a segment can be either a prefix match or pure middle match or suffix match, it matters how to decide that:
	*/
	// stamp: a b c d a
	// 
	//                   if I treat this 'a' as a pure prefix match, then this target is intractable
	//                   it must be treated as a suffix, so that the next letter 'c' can be an "any" type match
	//                         |
	// target:  a  b  c  d  a  a  c  a  b  c  d  a
	//                      a  b  c  d  a
	//             a  b  c  d  a
    //                               a  b  c  d  a
	//          a  b  c  d  a
	
	// Solution1 and Solution2 are primarily the same. Solution2 tries to simplify and streamline Solution1,
	// but both solutions fail to consider reconciliation.

	class Solution1
	{
	public:
		vector<int> movesToStamp(string stamp, string target)
		{
			vector<int> prefixMatchStampingIndex;
			vector<int> middleMatchStampingIndex;
			vector<int> suffixMatchStampingIndex;

			targetLen = target.size();
			stampLen = stamp.size();
			
			int ml; // length of a segment in stamp that matches a segment in target
			int ms; // starting index of a segment in stamp that matches a segment in target

			int previousMatchType = 0; // 0: match prefix, 1: match in middle (must not be a suffix), 2: match suffix
			int matchTypeToFind = 0;

			int j = 0;

			while (j < targetLen)
			{
				if (matchTypeToFind == 0)
				{
					findMatchingFromBeginning(stamp, target, j, ml);
					
					if (ml != 0) // a good case, a prefix of stamp matches this part of target.
					{
						prefixMatchStampingIndex.push_back(j);
						j += ml;
						previousMatchType = 0;
					}
					else if (j == 0) // there is no match, and this is the very beginning of target
					{
						return {};
					}
					else
					{
						matchTypeToFind = 1;
					}
				}
				else if (matchTypeToFind == 1)
				{
					findMatchingInMiddle(stamp, target, j, ms, ml);
					if (ml == 0)
					{
						// this part of "target" doesn't match anything
						// a very simple example can be that the letter pointed at by 'j' in the target doesn't even exist in stamp
						return {};
					}

					if (stampLen - ms > targetLen - j)
					{
						// at any time, the stamp's placement must not overrun the target
						// stamp: abcde
						// target: abcdeb
						// the final answer must not be [4, 0]
						// the last 'b' in target can't be created in any way.
						return {};
					}
					
					if (previousMatchType == 1 && ms != 0 && ms + ml != stampLen)
					{
						// previous match is a pure middle part match, and this match is neither a prefix nor a suffix,
						// in another word, this match is also pure middle match, these two matches interfer
						return {};
					}

					//            0   1   2   3   4   5   6
					//   target:  a   b   c   d   e   f   g
					//                            j
					//   stamp:           x   x   x   x   x
					//                    0   1   2   3   4
					// 
					//
					if (ms == 0)
					{
						previousMatchType = 0;
						prefixMatchStampingIndex.push_back(j);
					} 
					else if (ms + ml == stampLen)
					{
						previousMatchType = 2;
						suffixMatchStampingIndex.push_back(j - ms);
					}
					else
					{
						previousMatchType = 1;
						middleMatchStampingIndex.push_back(j - ms);
						matchTypeToFind = 0;
					}
					j += ml;
				}
			}
			
			vector<int>answer(middleMatchStampingIndex.cbegin(), middleMatchStampingIndex.cend());
			answer.insert(answer.end(), suffixMatchStampingIndex.crbegin(), suffixMatchStampingIndex.crend());
			answer.insert(answer.end(), prefixMatchStampingIndex.cbegin(), prefixMatchStampingIndex.cend());

			return answer;
		}
	};

	class Solution2
	{
	public:
		vector<int> movesToStamp(string stamp, string target)
		{
			vector<int> prefixMatchStampingIndex;
			vector<int> middleAndSuffixMatchStampingIndex;

			targetLen = target.size();
			stampLen = stamp.size();

			int ml; // length of a segment in stamp that matches a segment in target
			int ms; // starting index of a segment in stamp that matches a segment in target

			int matchTypeToFind = 0; // 0: match prefix, 1: any type

			int j = 0;

			while (j < targetLen)
			{
				if (matchTypeToFind == 0) // this is a strong demand, if the demand is for this iteration to find a prefix match, then this iteration *must* find one
				{
					findMatchingFromBeginning(stamp, target, j, ml);

					if (ml != 0) // a good case, a prefix of stamp matches this part of target.
					{
						prefixMatchStampingIndex.push_back(j);
						j += ml;
						if (ml != stampLen) // need another prefix to cover a suffix of this stamp
						{
							matchTypeToFind = 0;
						}
						else
						{
							// this is a full stamp, don't need another prefix to cover this stamp
							// the next segment can be any of the 3 types of matches.
							matchTypeToFind = 1;
						}
					}
					else
					{
						// if previous match dictates that this match must be prefix match
						// however I didn't find one, the target can't be created
						// This covers the case of j == 0 (i.e. the very beginning of target)
						// if there is no prefix match at the very beginning of target, target can't be created.
						return {};
					}
				}
				else if (matchTypeToFind == 1)
				{
					findMatchingInMiddle(stamp, target, j, ms, ml);
					if (ml == 0)
					{
						// this part of "target" doesn't match anything
						// a very simple example can be that the letter pointed at by 'j' in the target doesn't even exist in stamp
						return {};
					}

					if (stampLen - ms > targetLen - j)
					{
						// at any time, the stamp's placement must not overrun the target
						// stamp: abcde
						// target: abcdeb
						// the final answer must not be [4, 0]
						// the last 'b' in target can't be created in any way.
						return {};
					}

					//            0   1   2   3   4   5   6
					//   target:  a   b   c   d   e   f   g
					//                            |   |
					//                            j
					//                            |   |
					//   stamp:           x   x   x   x   x
					//                    0   1   2   3   4
					// 
					//   target[4,5] matches stamp[2,3]
					//   stamp[0] should line up to target[j-2]

					if (ms == 0)
					{
						prefixMatchStampingIndex.push_back(j);
					}
					else if (ms + ml == stampLen)
					{
						middleAndSuffixMatchStampingIndex.push_back(j - ms);
					}
					else
					{
						middleAndSuffixMatchStampingIndex.push_back(j - ms);
					}
					if (ms + ml != stampLen)
					{
						// not a suffix match, tail must be covered, next match must be prefix match
						matchTypeToFind = 0;
					}
					else
					{
						matchTypeToFind = 1;
					}
					j += ml;
				}
			}
			vector<int>answer(middleAndSuffixMatchStampingIndex.crbegin(), middleAndSuffixMatchStampingIndex.crend());
			answer.insert(answer.end(), prefixMatchStampingIndex.cbegin(), prefixMatchStampingIndex.cend());

			return answer;
		}
	};


	/*
	* stamp:  aab
	* target: aaab
	* 
	* This target can be created by stamping at [0, 1]
	* 
	*   a a b
	* a a b
	* 
	* But if I break the target into "aa" + "ab", this becomes an intractable target.
	* When searching for prefix match, if I do so greedily, I would identify the first two letters as a prefix,
	* and because it's not a full match, I must find a "cover" after it, which means the next segment must also be prefix match,
	* but obviously "ab" is not a prefix match.
	* Breaking the target into "a" + "aab" is the correct solution, but how can I know if I should use the first letter solely or
	* the first two letters as the first prefix match?
	* There is no way to know apriori. I can only try to greedily find the prefix match, if the next segment isn't a prefix, I need
	* to use a reconciliation process. i.e. shorten the previous prefix match.
	* x  x  x  x  x  x  x  x  x  x  x  x  x  x
	* |-----------------------------|  |
	*    this is a prefix              but this isn't the start of another prefix
	* |----------------------|   |
	*    shorten this prefix     see if this is the start of another prefix.
	* 
	* If previous match is a pure middle part match, and I fail to find a prefix match next, should I also reconcile? -- Yes
	* 
	// stamp: aabxyzaab
	// 
	//         |----------------|-------|----------------|
	// target: a a b x y z a a b x y z a a a b x y z a a b
	//                     a a b x y z a a b
	//         a a b x y z a a b
	//                                   a a b x y z a a b
	//
	// correct target break-down:              aabxyzaab + xyza  + aabxyzaab
	// wrong break-down:                       aabxyzaab + xyzaa +  abxyzaab
	* 
	*/

	// How to do reconciliation?
	// 
	// If reconciliation only happens when previous segment is a prefix match, I can use the kpm algorithm, 
	// 
	// x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x
	//                |                 |  |
	//          previous match start    |  |
	//                  previous match end |
	//                                     |
	//                                     j points at here
	// 
	// now 'j' doesn't start another prefix, previous segment match length is 7.
	// 
	// x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x
	//                |-----------------|
	//                0  1  2  3  4  5  6
	//                         |--------|
	//                         0  1  2  3
	// 
	// a prefix [0,6] of the stamp, match another prefix [0,3] in the right-adjusted fashion
	// However, if previous match is a pure middle match or suffix match, KPM algorithm is not applicable.
	//
	// Only reconciling in the previous match is not enough:
	// stamp: ffebb
	// target fffeffebbb
	// correct break-down: f   +   ffe   +   ffebb   +   b
	//                     ff  +   f     +   e...  <- greedy prefix search is going to yield this break-down
	// I can't find a prefix that starts with 'e', so I reconcile to the 2nd segment, but that is not enough,
	// obviously I need to abandon the 2nd segment and reconcile into the 1st segment.
	//
	// I only need to reconcile when my goal is to find a prefix match.
	// If previous match is a suffix match, then I fail to find an "any" type match, if I reconcile,
	// previous match becomes a pure middle part match, now my goal becomes finding a prefix match.
	// a b c d e f g h i
	//       |---|
	// for example "def" is a suffix match, 'g' doesn't start another prefix.
	// I reconcile by 1, "de" is a pure middle match, next to find a prefix, let's say "fgh" is a prefix.
	// Then how could I fail to find an "any" type match for "gh"?

	// this solution is incorrect in the reconciliation process because it only reconcile 
	// to the beginning of the previous matching segment. What needs to be done is to reconcile
	// back more than 1 segment.
	class Solution3
	{
	public:
		vector<int> movesToStamp(string stamp, string target)
		{
			vector<int> prefixMatchStampingIndex;
			vector<int> middleAndSuffixMatchStampingIndex;

			targetLen = target.size();
			stampLen = stamp.size();

			int ml;
			int ms;
			int pml = 1; // match length of the previous prefix match

			int matchTypeToFind = 0; // 0: match prefix, 1: any type

			int j = 0;

			while (j < targetLen)
			{
				if (matchTypeToFind == 0)
				{
					findMatchingFromBeginning(stamp, target, j, ml);

					if (ml != 0)
					{
						prefixMatchStampingIndex.push_back(j);
						j += ml;
						pml = ml;
						if (ml != stampLen)
						{
							matchTypeToFind = 0;
						}
						else
						{
							matchTypeToFind = 1;
						}
					}
					else
					{
						// stamp: ffebb
						// target fffeffebbb
						// ff    +     f      +      e
						//          \____/
						//          this is the
						//         so-called "previous" segment
						//
						// 'j' is pointing at 'e' right now.
						// "f" is the "previous" segment, the idea is to reconcile until the *previous* segment has only 1 letter left
						// but in this case, the *previous* segment only has 1 letter, the moment I want to reconcile,
						// I see "pml" equal to 1, I would return empty vector immediately.

						if (pml != 1) // brute force reconciliation, shorten the previous match one by one
						{
							--j;
							--pml;
						}
						else
						{
							return {};
						}
					}
				}
				else if (matchTypeToFind == 1)
				{
					findMatchingInMiddle(stamp, target, j, ms, ml);
					if (ml == 0)
					{
						return {};
					}

					if (stampLen - ms > targetLen - j)
					{
						return {};
					}

					if (ms == 0)
					{
						prefixMatchStampingIndex.push_back(j);
					}
					else if (ms + ml == stampLen)
					{
						middleAndSuffixMatchStampingIndex.push_back(j - ms);
					}
					else
					{
						middleAndSuffixMatchStampingIndex.push_back(j - ms);
					}
					if (ms + ml != stampLen)
					{
						matchTypeToFind = 0;
					}
					else
					{
						matchTypeToFind = 1;
					}
					j += ml;
					pml = ml;
				}
			}
			vector<int>answer(middleAndSuffixMatchStampingIndex.crbegin(), middleAndSuffixMatchStampingIndex.crend());
			answer.insert(answer.end(), prefixMatchStampingIndex.cbegin(), prefixMatchStampingIndex.cend());

			return answer;
		}
	};

	// this solution is incomplete, when reconcile, it's too complex to decide
	// how far back I should erase those already recored results.
	class Solution4
	{
	public:
		vector<int> movesToStamp(string stamp, string target)
		{
			deque<Record> r;

			targetLen = target.size();
			stampLen = stamp.size();

			int ml;
			int ms;
			int pml = 1; // match length of the previous prefix match

			int matchTypeToFind = 0; // 0: match prefix, 1: any type

			int j = 0;

			while (j < targetLen)
			{
				if (matchTypeToFind == 0)
				{
					findMatchingFromBeginning(stamp, target, j, ml);

					if (ml != 0)
					{
						r.emplace_back(j, 0);
						j += ml;
						pml = ml;
						if (ml != stampLen)
						{
							matchTypeToFind = 0;
						}
						else
						{
							matchTypeToFind = 1;
						}
					}
					else
					{
						if (pml != 1) // brute force reconciliation, shorten the previous match one by one
						{
							--j;
							--pml;
						}
						else if (r.size() >= 2)
						{
							auto [a1, b] = *(r.crbegin());
							auto [a2, b] = *(r.crbegin() + 1);
							pml = a1 - a2 - 1;
							
							r.erase(r.cend() - 1);

							j -= 2;

							// !!! ERASING JUST ONE PREVIOUS MATCH IS NOT ENOUGH  !!!

							//        0  1  2  3  4  5  6  7  8  9
							//                 a        a
							// target x  x  x  x  x  x  x  x  x  x  x
							// stamp 
							//                                   j
							// 
							// vector 'r': [ ..., ..., [3,.], [6,.]]
							// 
							// target[3,4,5] matches something
							// target[6,7,8] matches something
							// I can't find another prefix match from '9',
							// reconcile, try 'j' at 8, and 7, still failure,
							// now the [6,7,8] segment has 1 left,
							// next I'll try 'j' at '5'.
							// so 'j' needs to move back by 2 steps.
							// the match length of the [3,4,5] segment is 3,
							// but when I try 'j' at '5', the match length of this segment
							// is already decremented by 1.
							// what if previous match is only a 1-letter match?
							// stamp: ffebb
							// target: fffeffebbb
							// ff + f + e
							// 
						}
						else
						{
							return {};
						}
					}
				}
				else if (matchTypeToFind == 1)
				{
					findMatchingInMiddle(stamp, target, j, ms, ml);
					if (ml == 0)
					{
						return {};
					}

					if (stampLen - ms > targetLen - j)
					{
						return {};
					}

					r.emplace_back(j, ms);

					if (ms + ml != stampLen)
					{
						matchTypeToFind = 0;
					}
					else
					{
						matchTypeToFind = 1;
					}
					j += ml;
					pml = ml;
				}
			}
			return {};
		}
	};

	namespace
	{

#if 0
		bool combine(string& stamp, Record& a, string s)
		{
			if (a.matchType == 1)
			{
				return false;
			}
			int aEndIndex = a.stampStartIndex + a.matchLength;
			if (stamp[aEndIndex] == c)
			{
				++a.matchLength;
				if (a.stampStartIndex + a.matchLength == stampLen)
				{
					a.matchType == 1;
				}
				return true;
			}
			else
			{
				return false;
			}
		}
#endif
	}


#if 0
	class solution5
	{
	public:
		vector<int>movesToStamp(string stamp, string target)
		{
			deque<Record> r;
			if (target[0] == stamp[0])
			{
				r.emplace_back(0, 0, 1, 0);
			}
			else
			{
				return {};
			}
			int i = 1;
			while (r.size() != 0)
			{
				auto a = r.back();
				if (a.matchType == 0)
				{
					if (target[i] == stamp[0])
					{
						r.emplace_back(i, 0, 1, 0);
						++i;
					}
					else
					{
						bool ret = combine(stamp, r.back(), target[i]);
						if (ret)
						{
							++i;
						}
						else
						{
							return {};
						}
					}
				}
				else
				{
					auto b = stamp.find(target[i]);
					if (b == stamp.size() - 1)
					{
						r.emplace_back(i, b, 1, 1);
					}
					else
					{
						r.emplace_back(i, b, 1, 0);
					}
					++i;
				}
			}

			if (r.size() == 0)
			{
				return {};
			}

			// construct the answer
		}
	};
#endif



	void Test_0936_Stamping_The_Sequence()
	{
		int choice;
		Solution3 s;
		while (true)
		{
			cout << "1: manual test, 0: exit \n";
			cin >> choice;

			if (choice == 0)
				return;

			if (choice == 1)
			{
				string stamp;
				string target;

				cout << "stamp: \n";
				cin >> stamp;
				cout << "got stamp string: " << stamp << ", size: " << stamp.size() << "\n";

				cout << "target: \n";
				cin >> target;
				cout << "got target string: " << target << ", size: " << target.size() << "\n";

				vector<int> answer = s.movesToStamp(stamp, target);

				for (auto const& i : answer)
				{
					cout << i << ",  ";
				}
				cout << '\n';
			}
		}
	}
}