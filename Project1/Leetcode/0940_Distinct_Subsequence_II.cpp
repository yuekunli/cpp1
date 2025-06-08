#include<vector>
#include<string>
#include<array>
#include<iostream>

namespace _0940_Distinct_Subsequence_II {

	using namespace std;

	/*
	*        a     x     x     x     x     x     x
	*             \_______________________________/
	*                for example I know this segment gives me 100 distinct subsequences
	*    
	*    add a letter 'a', if the tail segment doesn't have another 'a', the case is easy.
	*    3 categories:
	*    (1). no 'a', the 100 subsequences from the tail
	*    (2). 'a' and the 100 subsequences from the tail
	*    (3). just 'a'
	*    So the total distinct subsequences are 100 + 100 + 1.
	* 
	*      (1)                           (2)
	*       a    x    x    x    x    x    a    x    x    x    x    x
	*           \_____________________/       \_____________________/
	*           call this segment 1            call this segment 2
	*          \____________________________________________________/
	*                      call this segment 3
	* 
	* a(2) is the first 'a' searching from left to right in segment 3.
	* 
	* assume segment 2 has 100 distinct subsequences.
	* assume segment 3 has 150 distinct subsequences.
	* 
	* If I use the easy case formula, the number of distinct subsequence after adding a(1) is 2 * 150 + 1 = 301.
	* Assume one of the subsequences produced by segment 2 is "bc".
	* "bc" is also a distinct subsequence produced by segment 3. (don't select anything from segment 1 and a(2)).
	* a(2) + "bc" is obviously a subsequence produced by segment 3.
	* 
	* Now add a(1), "a(1)" + "bc" should be a "new" subsequence, but "a(2)"+"bc" is already a subsequence produced
	* segment 3. They are duplicate, I can only count once.
	* 
	* Let's see those 3 categories in the easy scenario:
	* (1). no a(1), just distinct subsequences from segment 3  --- include "bc" and "a(2)bc"
	* (2). a(1) + every distinct subsequences from segment 3   --- include "a(1)bc" which is duplicate of "a(2)bc"
	* (3). just a(1)   --- duplicate of just a(2)
	* 
	* conclude:
	* a(2) + every distinct subsequence in segment 2  ==  a(1) + every distinct subsequence in segment 2
	* a(2) == a(1)
	* 
	* So the number of duplicated subsequences is number of distinct subsequences in segment#2 + 1
	* 
	* overall distinct subsequences:     150    *    2    +    1    -    (   100    +    1  )
	*                                   \_______________________/             |          |
	*                                      easy scenario                      |         just a letter 'a'
	*                                                           distinct of segment 2
	* 
	* Additional question:
	* Assume that, if I just look at segment 1 independently, "ef" is a distinct subsequence, if I just look at
	* segment 2 independently, "ef" is also a distinct subsequence.
	* a(2) + "ef" (from segment 2) == a(1) + "ef" (from segment 1) == a(1) + "ef" (from segment 2)
	* Does this cause more duplications?
	* No it doesn't.
	* 
	* If segment1 has "ef" and segment2 has "ef", overall distinct subsequences in segment 3 will only count "ef" once.
	* Let me list out all the distinct subsequences in segment 3:
	* ...
	* ef (no matter which segment is comes from)
	* a(2)ef  (no matter how I pick the last distinct subseq, i.e. "ef", "aef" with "ef" from segment2 is a distinct subseq)
	* ...
	* 
	* as long as there is 1 "ef" in this list, a(1)+ef is a duplicate of a(2)ef.
	* 
	* Do I need to worry about more letter 'a' in segment 2?
	* No I don't.
	* Note that a(2) is the *first* letter 'a' I see when I search 'a' from left to right in segment 3.
	* So there is no letter 'a' in segment 1, there can be more 'a' in segment 2.
	* But all need to know is the number of distinct subsequences produced by segment 2.
	*         (1)                 (2)                 (3)
	* 	       a    x    x    x    a    x    x    x    a    x    x    x
	*             \____________/       \___________/       \___________/
	*              segment 1            segment 2           segment 3
	*             \_____________________________________________________/
	*                                segment 4
	*                                  \_______________________________/
	*                                           segment 5
	* 
	* assume there is "ef" in segment 2 and there is also "ef" in segment 3, does this cause more duplication?
	* No it doesn't.
	* No matter where "ef" is, it's counted only once when computing distinct subsequences in segment 4.
	* If there is a "ef" after a(2), then "aef" is a subseq of segment 4.
	* Even if "ef" is in segment3, that conflict is already resolved when adding a(2) to segment 5.
	* "aef" is only counted once when computing distinct subsequences of segment 4.
	* Overall effect is that there is exactly 1 "ef" in the list of distinct subseq of segment 4,
	* and there is exactly 1 "aef" in the list of distinct subseq of segment 4, if "ef" is behind a(2).
	* Only 1 duplication when adding a(1).
	* --------------------------------------------------------------------------------------------------
	* Implementation:
	* I only need an array of 26 elements for book-keeping.
	* The first element means, when I last time saw a letter 'a', the number of distinct subsequences in the segment
	* just to the right of that letter 'a'.
	* 
	* If the right-most letter of the input string is, for example, 't'. (t is the 20th letter in the alphabet).
	* array[19] should be initialize with 0. The segment to the right of 't' is nothing, its distinct subseq is also 0.
	*      (1)            (2)
	*       t  x  x  x  x  t
	*                        \_____/
	*                         this is empty
	* 
	* to the right of 't' is empty.
	* Duplications are, as analyzed above, :
	*  t(1) + distinct subseq of the segment to the right of t(2)
	* and just a letter 't'.
	* 
	* In fact, in this case, the only duplication is just the letter 't'.
	* 
	* so if I set array[19] to 0, the duplication's calculation is just right: 0 + 1 = 1
	* 
	*/
	//accepted 0ms beat 100%
	class Solution
	{
	public:
		int distinctSubseqII(string s)
		{
			auto len = s.size();
			if (len == 1)
			{
				return 1;
			}
			int base = 1'000'000'007;

			array<int, 26> subseqCount;
		
			for (int i = 0; i < 26; ++i)
			{
				subseqCount[i] = -1;
			}
			
			char c = s[len - 1];
			char v = c - 'a';
			subseqCount[v] = 0;

			c = s[len - 2];
			v = c - 'a';
			subseqCount[v] = 1;
			
			int currentCount;
			if (s[len - 2] == s[len - 1])
			{
				currentCount = 2;
			}
			else
			{
				currentCount = 3;
			}

			for (int i = len - 3; i >= 0; --i)
			{
				c = s[i];
				v = c - 'a';
				if (subseqCount[v] == -1)
				{
					subseqCount[v] = currentCount;
					currentCount = currentCount * 2 + 1;
					currentCount = currentCount % base;
				}
				else
				{
					int temp = subseqCount[v];
					subseqCount[v] = currentCount;
					currentCount = currentCount * 2 + 1;
					while (currentCount < (temp + 1))
					{
						currentCount += base;
					}
					currentCount = currentCount - (temp + 1);
					currentCount = currentCount % base; // after take modulus, currentCount can be a very small number, it can become smaller than a previous result, which will become "temp" in the future.
				}
			}
			return currentCount;
		}
	};

	void Test_0940_Distinct_Subsequences_II()
	{
		int option;
		string s;
		Solution so;
		while (true)
		{
			cout << "1: manual test; 0: exit > ";
			cin >> option;
			if (option == 0)
			{
				return;
			}
			cout << "string: ";
			s.clear();
			cin >> s;
			cout << so.distinctSubseqII(s) << "\n";
		}
	}
}