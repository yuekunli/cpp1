#include<iostream>
#include<string>
#include<unordered_map>
#include<algorithm>

namespace _1044_Longest_Duplicate_Substring {

	using namespace std;

	class Solution1
	{
		/*
		* For example the string is "a b c d e f g h i j k l m n p q r s t"
		* For example the longest duplicate substring is "defg" which repeats itself at "klmn"
		* If I run the KMP string matching preprocessing on this string "d e f g h i j k l m n p q r s t"
		* I'll find a matching situation like this:
		* d e f g h i j k l m n p q r s t
		*               d e f g
		* which means when I stand on 'n', it matches a prefix of length 4.
		* After running the pre-processing, I'll find a longest match.
		* But how do I know I should cut off "abc" and then run the pre-processing?
		* I don't know, so I have to cut off the letters one by one and run the pre-processing every time I cut off a letter.
		*/

	public:

		string longestDupSubstring(string s)
		{
			size_t len = s.size();

			int* matchPfxLen = new int[len];
			for (int i = 0; i < len; i++)
			{
				matchPfxLen[i] = 0;
			}

			int longestMatch = 0;
			int startForLongestMatch = 0;
			
			for (int start = 0; start <= len - 2; start++)
			{
				int i = start + 1;
				int k = start;
				int matchingLen = 0;
			
				// if the longest match is 4-element long so far, the length of [start, end] has to be at least
				// 6 in order to have 2 5-element substrings and if they match, they can surpass the longest match so far.
				// 0 1 2 3 4 5 6 7 8 9
				//       |
				//      start        total length = 10, when start is 3, length of [start,end] is 7 =  10 - 3
				if (len - start < longestMatch + 2)
					break;

				while (i < len)
				{
					if (s[i] == s[k])
					{
						matchingLen++;
						matchPfxLen[i] = matchingLen;

						if (matchingLen > longestMatch)
						{
							longestMatch = matchingLen;
							startForLongestMatch = start;
						}

						i++; k++;
					}
					else
					{
						/*
						*   0  1  2  3  4  5  6  7  8  9
						*         |           2  3  X
						*                        2
						* 
						* start = 2, when i = 7, matchingLen = 2
						* when i = 8, s[8] doesn't match s[4]
						* if the matchingLen for when i = 3 is 1
						* next k to try is 3
						* 
						*/
						if (k != start)
						{
							matchingLen = matchPfxLen[k - 1];
							k = matchingLen + start;
						}
						else
							i++;
					}
				}

			}

			delete[] matchPfxLen;

			if (longestMatch > 0)
				return s.substr(startForLongestMatch, longestMatch);
			else
				return "";
		}
	};


	class Solution2 //accepted
	{
		/*
		* if I already found that there are two 5-element substrings that match,
		* there must also be 4-element substrings that match, also be 3-element substrings that match, so on so forth.
		* if the total length is 10, the maximum length of substrings that can match is 9.
		* So I know there are 5-element substrings that match, I should try to look for 8-element substrings, if that fails
		* I try between 5 and 8. This is a binary search approach.
		*/

		/*
		* What is Rabin-Karp algorithm?
		* Turn each substring into a number, each digit multiply 26^x, then modulus a big prime such as 1'000'000'007
		* 
		*/

	public:

		bool compareTwoStrings(string&s, size_t s1, size_t s2, int length)
		{
			return equal(s.begin()+s1, s.begin()+s1+length, s.begin()+s2);
		}

		string findDupSubstringForCertainLength(string& s, int subStringLength)
		{
			size_t fullLength = s.size();
			unordered_map<long long, size_t> seen;
			int modBase = 1'000'000'007;
			//int highestPositionWeight = (static_cast<int>(pow(26, subStringLength - 1))) % modBase;

			long long highestPositionWeight = 1;
			for (int i = 1; i < subStringLength; i++)
			{
				highestPositionWeight *= 26;
				highestPositionWeight %= modBase;
			}

			// total length is 10, index 0 to 9, substring length 4, start can be at 6, 
			long long hash = 0;
			for (size_t start = 0; start <= fullLength - subStringLength; start++)
			{
				if (start == 0)
				{
					for (int i = 0; i < subStringLength; i++)
					{
						hash *= 26;
						hash %= modBase;
						hash += s[i] - 'a';
						hash %= modBase;
					}
				}
				else
				{
					// Note the example on "Introduction to Algorithms  -- Cormen, Leiserson, ..." page 992
					// this part "old hash - (high digit * high position weight)" should especially be applied with the modulu operation
					int c = s[start - 1] - 'a';
					hash -= c * highestPositionWeight;  // this cat cause "hash" to become negative
					while (hash < 0)
						hash += modBase;
					hash *= 26;
					hash %= modBase;
					hash += s[start + subStringLength - 1] - 'a';
					hash %= modBase;
				}
				if (seen.find(hash) != seen.end())
				{
					size_t startOfAnother = seen[hash];
					if (compareTwoStrings(s, startOfAnother, start, subStringLength)) 
						// if I use "26" as the coefficient and *don't* do modulus operation, I don't need to do a char-by-char compare
						// it's just like for example, if two decimal numbers are equal, they have to have every digit equal.
						// but if I do do modulus operation, the char-by-char compare is necessary
					{
						return s.substr(start, subStringLength);
					}
				}
				else
				{
					// there is a flaw!
					// what if two different substrings both produce the same hash
					// and I just found a third substring that produces the same hash again!
					// I need to compare the third substring with each of the previous two
					// so the map should really be unordered_map<long, vector<int>>
					// it needs to store all the substrings producing the same hash.
					// The original Rabin-Karp algorithm is to compare each substring of a string
					// against a pattern, so it doesn't have this problem.
					seen[hash] = start;
				}
			}
			return "";
		}

		string longestDupSubstring(string s)
		{
			size_t len = s.size();

			int attemptLengthLowerLimit = 1;
			int attemptLengthUpperLimit = (int)len-1;

			string answer;  // return *any* such longest duplicated substring

			while (attemptLengthLowerLimit <= attemptLengthUpperLimit)
			{
				int attemptLength = (attemptLengthLowerLimit + attemptLengthUpperLimit) / 2;   //  a + (b-a) / 2  =  a + b/2 - a/2  =  b/2 + a/2  =  (a + b)/2 
				string tempAnswer = findDupSubstringForCertainLength(s, attemptLength);
				if (tempAnswer != "")
				{
					attemptLengthLowerLimit = attemptLength + 1;
					answer = tempAnswer;
				}
				else
					attemptLengthUpperLimit = attemptLength - 1;
			}
			return answer;
		}
	};


	/*
	* This is Leetcode's best solution, but this doesn't quite work.
	* This statement:  if (p == n)
	* It takes many many iterations still p is not equal to n.
	* In another word, I can't achieve "every suffix substring has a unique rank" in an efficient way.
	*/
	class Solution3
	{
		using ll = long long int;
	public:
		//const static int N = 3e4 + 5; // max string length, max value rank
		const static int N = 100;
		ll n, m, p, ans;
		ll psum[N], p_rk[N], rk[N], sa[N], id[N], ht[N];
		string s;

		void cal_sa() {
			m = 100;
			for (int i = 1; i <= n; ++i) {
				int x = s[i - 1];
				rk[i] = x; // Use ASCII code as rank for the first time
				++psum[x];
			}
			for (int i = 1; i <= m; ++i) 
				psum[i] += psum[i - 1];
			for (int i = n; i >= 1; --i) 
				sa[psum[rk[i]]--] = i;

			for (int j = 1; ; j <<= 1, m = p) { // j: length
				int k = 1; // rank index
				for (int i = n; i + j > n; --i) 
					id[k++] = i;
				for (int i = 1; i <= n; ++i) if (sa[i] > j) id[k++] = sa[i] - j;
				memset(psum, 0, sizeof psum);
				for (int i = 1; i <= n; ++i) ++psum[rk[id[i]]];
				for (int i = 1; i <= m; ++i) psum[i] += psum[i - 1];
				for (int i = n; i >= 1; --i) sa[psum[rk[id[i]]]--] = id[i];
				memcpy(p_rk, rk, sizeof rk);
				auto cmp = [&](int x, int y, int len) {
					return p_rk[x] == p_rk[y] && p_rk[x + len] == p_rk[y + len];
				};
				p = 0;
				for (int i = 1; i <= n; ++i) {
					if (cmp(sa[i], sa[i - 1], j)) {
						rk[sa[i]] = p;
					}
					else {
						rk[sa[i]] = ++p;
					}
				}
				if (p == n) {
					for (int i = 1; i <= n; ++i) sa[rk[i]] = i;
					break;
				}
			}
		}

		void cal_ht() {
			for (int i = 1, k = 0; i <= n; ++i) {
				if (k) --k;
				while (rk[i] > 1 && s[i - 1 + k] == s[sa[rk[i] - 1] - 1 + k]) ++k;
				ht[rk[i]] = k;
			}
		}
		string longestDupSubstring(string& s) {
			memset(psum, 0, sizeof(psum));
			memset(p_rk, 0, sizeof(p_rk));
			memset(rk, 0, sizeof(rk));
			memset(sa, 0, sizeof(sa));
			memset(id, 0, sizeof(id));
			memset(ht, 0, sizeof(ht));
			n = s.size();
			//this->s = move(s);
			this->s = s;
			cal_sa();
			cal_ht();
			int j = 0;
			int mx = 0;
			for (int i = 1; i <= n; ++i) {
				if (ht[i] > mx) {
					mx = ht[i];
					j = i;
				}
			}
			if (j) return this->s.substr(sa[j] - 1, mx);
			return "";
		}
	};

	

	// abcabxabcd
	/*
	* The 3 suffixes starting with 'a':
	* abcabxabcd
	* abxabcd
	* abcd
	* 
	* if I just put them in the order of their appearence in the original string,
	* then this is the order. 
	* Now if I count each one's matching length against previous one, I get
	* abcabxabcd
	* abxabcd     2 letters match against abcabxabcd
 	* abcd        2 letters match against abxabcd
	* 
	* However if they are sorted in lexicographical order:
	* abcd
	* abcabxabcd    3 letters match against abcd
	* abxabcd       2 letters match against abcabxabcd
	* 
	* Therefore they must be sorted.
	*/
	/*
	* time limit exceeded
	* The failed test case has a very long input string, every letter in the string is the same.
	* In such case, the sorting step needs to sort all suffix substrings,
	* and every two suffix substrings need to be compared down to the last letter.
	*/
	class Solution4
	{
	private:
		string s1;
		vector<int>initialKeyOccurrence=vector<int>(26, 0);
		vector<int>rankToIndex;
		vector<int>indexToKey;
		vector<int>matchLengthRecord;
		int n;

		void sortSuffixSubstrings()
		{
			rankToIndex = vector<int>(n, 0);
			indexToKey = vector<int>(n, 0);
			for (int i = 0; i < n; ++i)
			{
				char x = s1[i];
				x = x - 'a';
				++initialKeyOccurrence[x];
			}
			for (int i = 1; i < 26; ++i)
			{
				initialKeyOccurrence[i] += initialKeyOccurrence[i - 1];
			}

			for (int i = n - 1; i >= 0; --i)
			{
				char x = s1[i];
				x = x - 'a';
				rankToIndex[initialKeyOccurrence[x]-1] = i;
				--initialKeyOccurrence[x];
			}

			auto cmp = [this](int const& a, int const& b)
			{
				int i, j;
				for (i = a, j = b; i < n && j < n; ++i, ++j)
				{
					if (s1[i] < s1[j])
						return true;
					if (s1[i] > s1[j])
						return false;
				}
				if (i == n && j < n)
					return true;
				else
					return false;
			};

			int i = 0;
			while (i < n)
			{
				int start = i, j = i;
				char x = s1[rankToIndex[start]];
				while (j < n && s1[rankToIndex[j]] == x)
				{
					++j;
				}
				--j;
				// start to j has the same initial letter
				/*
				for (int t = j, p = start; t >= start; --t, ++p)
				{
					rankToIndexFinal[p] = rankToIndex[t];
					indexToKey[rankToIndexFinal[p]] = p;
				}
				*/
				sort(rankToIndex.begin() + start, rankToIndex.begin() + j + 1, cmp);
				i = j + 1;
			}

			for (i = 0; i < n; ++i)
			{
				indexToKey[rankToIndex[i]] = i;
			}
		}
		
		void countMatches()
		{
			matchLengthRecord = vector<int>(n, 0);
			int matchLength = 0;
			for (int i = 0; i < n; ++i)
			{
				if (matchLength > 0)
				{
					--matchLength;
				}
				if (indexToKey[i] > 0)
				{
					int currentSuffixStartingIndex = i;
					int precedeRankedSuffixStartingIndex = rankToIndex[indexToKey[i] - 1];
					while (precedeRankedSuffixStartingIndex+matchLength < n && s1[precedeRankedSuffixStartingIndex + matchLength] == s1[i + matchLength])
					{
						++matchLength;
					}
					matchLengthRecord[indexToKey[i]] = matchLength;
				}
			}
		}

	public:
		string longestDupSubstring(string& s)
		{
			n = s.size();
			//s1 = std::move(s);
			s1 = s;
			sortSuffixSubstrings();
			countMatches();
			int maxMatchLength = 0;
			int rankOfSuffixWithMaxMatch = 0;
			for (int i = 0; i < n; ++i)
			{
				if (matchLengthRecord[i] > maxMatchLength)
				{
					maxMatchLength = matchLengthRecord[i];
					rankOfSuffixWithMaxMatch = i;
				}
			}
			if (maxMatchLength > 0)
			{
				string result = s.substr(rankToIndex[rankOfSuffixWithMaxMatch], maxMatchLength);
				return result;
			}
			else
			{
				return "";
			}
		}

		void reset()
		{
			s1.clear();
			//copy(initialKeyOccurrence.begin(), initialKeyOccurrence.end(), 0);
			//copy(rankToIndex.begin(), rankToIndex.end(), 0);
			//copy(indexToKey.begin(), indexToKey.end(), 0);
			//copy(matchLengthRecord.begin(), matchLengthRecord.end(), 0);
			fill(initialKeyOccurrence.begin(), initialKeyOccurrence.end(), 0);
			rankToIndex.clear();
			indexToKey.clear();
			matchLengthRecord.clear();
			n = 0;
		}
	};

	/*
	* Ukkonen suffix tree
	*/
	class Solution5
	{
		struct TreeNode
		{
			struct TreeNode* children[26];
			struct TreeNode* sibling;
			int start;
			int end;
			bool isLeaf;
			int suffixStartingIndex;

			TreeNode() :
				sibling(nullptr),
				start(-1),
				end(-1),
				isLeaf(false),
				suffixStartingIndex(0)
			{
				memset(children, 0, sizeof(children));
			}
		};

		class UkkSuffixTree
		{
		private:

			string input;
			TreeNode root;

			TreeNode* currentNode;
			int probe; // currentChildIndexInChildrenArray
			int matchLength;
			int extensionToDoCount;
			TreeNode* lastNewIntermediateNode;
			int leafEndIdx; 

			void buildTree()
			{
				size_t sz = input.size();
				for (int i = 0; i < sz; ++i)
				{
					addOneChar(i);
				}
			}

			int segmentLength(TreeNode* probe)
			{
				if (probe->isLeaf)
				{
					return leafEndIdx - probe->start + 1;
				}
				else
				{
					return probe->end - probe->start + 1;
				}
			}

			void addOneChar(int i)
			{
				lastNewIntermediateNode = nullptr;
				++extensionToDoCount;
				leafEndIdx = i;

				char c = input[i] - 'a';

				while (extensionToDoCount > 0)
				{
					if (matchLength == 0)
					{
						if (currentNode->children[c] == nullptr)
						{
							TreeNode* newLeaf = new TreeNode();

							newLeaf->start = i;

							newLeaf->isLeaf = true;
							
							newLeaf->suffixStartingIndex = i - extensionToDoCount + 1;

							currentNode->children[c] = newLeaf;
							--extensionToDoCount;

							if (lastNewIntermediateNode != nullptr)
							{
								lastNewIntermediateNode->sibling = currentNode;
							}
						}
						else
						{
							probe = c;
							matchLength = 1;
							// need to see if I need to move down to the next tier to set currentNode
							int underneathSegmentLength = segmentLength(currentNode->children[probe]);
							if (underneathSegmentLength == matchLength)
							{
								currentNode = currentNode->children[probe];
								matchLength = 0;
								probe = -1;
							}
							break;
						}
					}
					else
					{
						TreeNode* n = currentNode->children[probe];
						char nextCharInPath = input[n->start + matchLength];
						if (nextCharInPath == input[i])
						{
							++matchLength;
							int underneathSegmentLength = segmentLength(n);
							if (underneathSegmentLength == matchLength)
							{
								currentNode = n;
								matchLength = 0;
								probe = -1;
							}
							break;
						}
						else
						{
							TreeNode* newIntermediate = new TreeNode();
							newIntermediate->start = n->start;
							n->start = n->start + matchLength;
							newIntermediate->end = n->start - 1;
							newIntermediate->isLeaf = false;

							TreeNode* newLeaf = new TreeNode();
							newLeaf->start = i;
							newLeaf->isLeaf = true;
							newLeaf->suffixStartingIndex = i - extensionToDoCount + 1;
							
							currentNode->children[probe] = newIntermediate;

							newIntermediate->children[nextCharInPath - 'a'] = n;
							newIntermediate->children[c] = newLeaf;

							if (lastNewIntermediateNode != nullptr)
							{
								lastNewIntermediateNode->sibling = newIntermediate;
							}
							lastNewIntermediateNode = newIntermediate;
						}
						--extensionToDoCount;
					}

					if (extensionToDoCount == 0)
					{
						probe = -1;
						matchLength = 0; // in fact, matchLength right now should already be 0
					}
					else
					{
						if (currentNode == &root)
						{
							if (matchLength > 1)
							{
								// need to keep spinning on root
								char nextCharInPath = input[i - extensionToDoCount + 1];
								probe = nextCharInPath - 'a';
							}
							--matchLength;
						}
						else
						{
							currentNode = currentNode->sibling;
						}
					}
				}
			}



		public:

			
			UkkSuffixTree(string const& s) :
				input(s),
				root(),
				currentNode(&root),
				probe(-1),
				matchLength(0),
				extensionToDoCount(0),
				lastNewIntermediateNode(nullptr),
				leafEndIdx(0)
			{
				buildTree();
			}

			void dfsTraverse3(TreeNode* n, int& end, int& maxLength, int accumulatedLength)
			{
				int currentNodeLength;
				if (n->start == -1 && n->end == -1)
				{
					currentNodeLength = 0; // n is root
				}
				else
				{
					currentNodeLength = n->end - n->start + 1;
				}
				if (currentNodeLength + accumulatedLength > maxLength)
				{
					maxLength = currentNodeLength + accumulatedLength;
					end = n->end;
				}
				for (int i = 0; i < 26; ++i)
				{
					TreeNode* child = n->children[i];
					if (child != nullptr && !(child->isLeaf))
					{
						dfsTraverse3(child, end, maxLength, accumulatedLength + currentNodeLength);
					}
				}
			}

			string findLongestDuplicateSubstring()
			{
				int maxLength = 0;
				int end = 0;

				dfsTraverse3(&root, end, maxLength, 0);
				if (maxLength > 0)
				{
					int start = end - maxLength + 1;
					return string{ input.substr(start, maxLength) };
				}
				else
				{
					return "";
				}
			}			
		};

		string longestDupSubstring(string s)
		{
			UkkSuffixTree uksuft{ s };
			return uksuft.findLongestDuplicateSubstring();
		}
	};



	void Test_1044_Longest_Duplicate_Substring()
	{
		Solution2 so2;
		//Solution3 so3;
		Solution4 so4;

		while (true)
		{
			string s;
			cout << "string: ";
			getline(cin, s);
			if (s.size() == 0) break;

			cout << so2.longestDupSubstring(s) << endl;
			//cout << so3.longestDupSubstring(s) << endl;
			cout << so4.longestDupSubstring(s) << endl;
			so4.reset();
		}
	}
}

