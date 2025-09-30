#include<unordered_map>
#include<unordered_set>
#include<vector>
#include<string>
#include<iostream>
#include<numeric>
#include<functional>
#include<bitset>
#include<deque>

#pragma warning(disable: 4267 4244)

namespace _0126_Word_Ladder_II {

using namespace std;

/*
* Each word can have 1 to 5 letters.
* There are up to 500 words in the dictionary
* *all* the shortest transformation sequence
*/

class Solution
{
	unordered_map<string, vector<string>> m;  // key: one word;  value: all words that differ by 1 letter from this word
	vector<string> v[2];  // 2 sequential containers used in breadth-first-search
	vector<vector<string>> answer;
	int pathLen = 0; // length of shortest path

	int diffLetterCount(string& s1, string& s2)
	{
		return inner_product(s1.begin(), s1.end(), s2.begin(), 0, plus<int>{}, 
			[](auto& a, auto& b)
			{
				return a == b ? 0 : 1;
			});
	}

	void buildAnswer(string& beginWord, string& endWord, string& currentWord, vector<string>& currentPath, int currentLen)
	{
		if (currentLen == pathLen)
		{
			if (currentWord == endWord)
			{
				answer.push_back(vector<string>{});
				copy(currentPath.begin(), currentPath.end(), back_inserter(*(answer.rbegin())));
			}
			return;
		}

		for (auto& nextWord : m[currentWord])
		{
			currentPath.push_back(nextWord);
			buildAnswer(beginWord, endWord, nextWord, currentPath, currentLen + 1);
			currentPath.erase(currentPath.cend()-1);
		}
		return;
	}

	/*
	* q1: beginWord    q2: empty
	* 
	* examine q1, set currentPathLength to 0, fill q2, clear q1
	* 
	* q1: empty   q2: word1, word2, word3,...
	* 
	* examine q2, set currentPathLength to 1, fill q1, clear q2
	* 
	* q1: word6, word7, word8,...   q2: empty
	* 
	* examine q1, set currentPathLength to 2, found endWord
	* 
	* So a shortest path would be like this:  begin  -->  word2  --> end
	* I call this a path of length of 2, because 2 hops.
	*/

	/*
	* I build the adjacency list and run the breadth-first-search simultaneously.
	* Make sure I don't visit the same node twice.
	* for example dictionary: {"abc", "dbc", "ebc"} beginWord = "abc"
	* q1: "abc", q2: empty
	* q1: empty, q2: "dbc", "ebc"
	* Now when I examine "dbc", I need to make sure I don't put "ebc" back in q1, because "ebc" is 1 letter different from "dbc".
	* If I do put "ebc" back in q1, does that affect me looking for the *shortest* path?
	* If the shortest path does go through "ebc", it's probably OK because I'm about to examine "ebc"
	* right after I'm done with "dbc". But it wastes time.
	*/
public:
	vector<vector<string>> findLadders(string beginWord, string endWord, vector<string>& wordList)
	{
		int currentPathLen = -1;
		int qswitch = 0;
		vector<string>* q = &v[qswitch];
		q->push_back(beginWord);
		bool found = false;
		while (q->size() > 0)
		{
			currentPathLen++;
			qswitch = (qswitch + 1) % 2;
			vector<string>* nextQ = &v[qswitch];
			for (auto& w : *q)
			{
				if (w == endWord)
				{
					found = true;
					pathLen = currentPathLen;
					break;   // If I found, I break, but I didn't build an adjacency list fully, is that OK when I build *all* shortest paths?
					// it should be OK, because all the adjacencies I need to reach this level (in terms of breadth-first-search level) have been built.
				}

				vector<string>& adj = m[w];
				for (auto& a : wordList)
				{
					if (diffLetterCount(w, a) == 1)
					{
						//if (find(adj.begin(), adj.end(), a) == adj.end())  // I don't need this check, if I only examine a word once, of course its adj list would be empty
						adj.push_back(a);
						if (!m.contains(a))
						{
							nextQ->push_back(a);
							m.insert({ a, vector<string>() });
						}
					}
				}
			}
			if (found)
				break;
			q->clear();
			q = nextQ;
		}
		if (found)
		{
			vector<string> currentPath { beginWord };

			buildAnswer(beginWord, endWord, beginWord, currentPath, 0);
			return answer;
		}
		return vector<vector<string>>{};
	}
};




class Solution2
{
	/*
	* build an adjancency list at the beginning.
	*/

	unordered_map<string, vector<string>> m;
	vector<string> v[2];
	vector<vector<string>> answer;
	int pathLen = 0;

	int diffLetterCount(string& s1, string& s2)
	{
		return inner_product(s1.begin(), s1.end(), s2.begin(), 0, plus<int>{},
			[](auto& a, auto& b)
			{
				return a == b ? 0 : 1;
			});
	}

	void buildAnswer(string& beginWord, string& endWord, string& currentWord, vector<string>& currentPath, int currentLen)
	{
		if (currentLen == pathLen)
		{
			if (currentWord == endWord)
			{
				answer.push_back(vector<string>{});
				copy(currentPath.begin(), currentPath.end(), back_inserter(*(answer.rbegin())));
			}
			return;
		}

		for (auto& nextWord : m[currentWord])
		{
			currentPath.push_back(nextWord);
			buildAnswer(beginWord, endWord, nextWord, currentPath, currentLen + 1);
			currentPath.erase(currentPath.cend() - 1);
		}
		return;
	}

	void buildAdjacency(string& beginword, vector<string>& wordList)
	{
		for (size_t i = 0; i < wordList.size(); i++)
		{
			string& w1 = wordList[i];
			for (size_t j = i+1; j < wordList.size(); j++)
			{
				string& w2 = wordList[j];
				if (diffLetterCount(w1, w2) == 1)
				{
					m[w1].push_back(w2);
					m[w2].push_back(w1);
				}
			}
		}
		if (!m.contains(beginword))
		{
			for (size_t i = 0; i < wordList.size(); i++)
			{
				string& w1 = wordList[i];
				if (diffLetterCount(w1, beginword) == 1)
				{
					m[beginword].push_back(w1);
				}
			}
		}
	}

public:
	vector<vector<string>> findLadders(string beginWord, string endWord, vector<string>& wordList)
	{
		buildAdjacency(beginWord, wordList);
		int currentPathLen = -1;
		int qswitch = 0;
		vector<string>* q = &v[qswitch];
		q->push_back(beginWord);
		bool found = false;
		unordered_set<string>seen;
		while (q->size() > 0)
		{
			currentPathLen++;
			qswitch = (qswitch + 1) % 2;
			vector<string>* nextQ = &v[qswitch];
			for (auto& w : *q)
			{
				if (w == endWord)
				{
					found = true;
					pathLen = currentPathLen;
					break;
				}

				vector<string>& adj = m[w];
				for (auto& a : adj)
				{
					if (seen.count(a) == 0)
					{
						nextQ->push_back(a);
						seen.insert(a);
					}
				}
			}
			if (found)
				break;
			q->clear();
			q = nextQ;

		}
		if (found)
		{
			vector<string> currentPath{ beginWord };

			buildAnswer(beginWord, endWord, beginWord, currentPath, 0);
			return answer;
		}
		return vector<vector<string>>{};
	}
};




class Solution3
{
	/*
	* previous solutions waste time on building the answers.
	* When I look for the answer, I already build up a lot of information.
	* I should not abandon them and re-run depth-first-search to find
	* the answers again.
	* So when I run the breadth-first-search to look for the answer,
	* I build a tree-like structure. Each node on a level keeps a pointer
	* to a node on the previous level that is the parent.
	* So when I see the end word, I can trace back to the root which is the
	* begin word by following the parent link.
	*/

	vector<vector<string>> answers;

	int diffLetterCount(string& s1, string& s2)
	{
		return inner_product(s1.begin(), s1.end(), s2.begin(), 0, plus<int>{},
			[](auto& a, auto& b)
			{
				return a == b ? 0 : 1;
			});
	}

	struct A
	{
		string word;
		struct A* parent;
		A(string& _word, struct A* _parent) :word(_word), parent(_parent) {}
	};

	vector<vector<A>> bfsTree;

	void buildAnswers(vector<A>& q, string endword)
	{
		for (size_t i = 0; i < q.size(); i++)
		{
			if (q[i].word == endword)
			{
				vector<string> oneAnswer;
				oneAnswer.push_back(q[i].word);
				A* p = q[i].parent;
				while (p != nullptr)
				{
					oneAnswer.push_back(p->word);
					p = p->parent;
				}
				reverse(oneAnswer.begin(), oneAnswer.end());
				answers.push_back(oneAnswer);
			}
		}
	}

public:

	vector<vector<string>> findLadders(string beginWord, string endWord, vector<string>& wordList)
	{
		bfsTree.push_back(vector<A>{});
		size_t currentLevelIndex = 0;
		bfsTree[0].emplace_back(beginWord, nullptr);

		unordered_set<string>seen;
		seen.insert(beginWord);

		while (bfsTree[currentLevelIndex].size() > 0)
		{
			bfsTree.push_back(vector<A>{});
			vector<A>& nextQ = bfsTree[bfsTree.size()-1];
			
			for (auto& a : bfsTree[currentLevelIndex])
			{
				if (a.word == endWord)
				{
					buildAnswers(bfsTree[currentLevelIndex], endWord);
					return answers;
				}
				
				for (auto& w : wordList)  // this is searching adjacent words in the entire word list, which is slow.
				{
					if (seen.count(w) == 0&& diffLetterCount(w, a.word) == 1)
					{
						nextQ.emplace_back(w, &a);
					}
				}
			}
			for (auto& b : nextQ) // this must be outside the previous for loop, because the same word can appear on the same level multiple times, but not on different levels
				seen.insert(b.word);
			currentLevelIndex++;
		}
		return vector<vector<string>>{};
	}
};



class Solution4
{
	/*
	* build an adjancency list at the beginning and keep the parent link in the
	* breadth-first-search tree. but this still has the problem of processing the
	* same node on the same level multiple times.
	*/

public:
	unordered_map<string, vector<string>> m;

	vector<vector<string>> answers;

	int diffLetterCount(string& s1, string& s2)
	{
		return inner_product(s1.begin(), s1.end(), s2.begin(), 0, plus<int>{},
			[](auto& a, auto& b)
			{
				return a == b ? 0 : 1;
			});
	}

	struct A
	{
		string word;
		struct A* parent;
		A(string& _word, struct A* _parent) :word(_word), parent(_parent) {}
	};

	vector<vector<A>> bfsTree;


	void buildAdjacency(string& beginword, vector<string>& wordList)
	{
		for (size_t i = 0; i < wordList.size(); i++)
		{
			string& w1 = wordList[i];
			for (size_t j = i + 1; j < wordList.size(); j++)
			{
				string& w2 = wordList[j];
				if (diffLetterCount(w1, w2) == 1)
				{
					m[w1].push_back(w2);
					m[w2].push_back(w1);
				}
			}
		}
		if (m.count(beginword) == 0)
		{
			for (size_t i = 0; i < wordList.size(); i++)
			{
				string& w1 = wordList[i];
				if (diffLetterCount(w1, beginword) == 1)
				{
					m[beginword].push_back(w1);
				}
			}
		}
	}

	void buildAnswers(vector<A>& q, string endword)
	{
		for (size_t i = 0; i < q.size(); i++)
		{
			if (q[i].word == endword)
			{
				vector<string> oneAnswer;
				oneAnswer.push_back(q[i].word);
				A* p = q[i].parent;
				while (p != nullptr)
				{
					oneAnswer.push_back(p->word);
					p = p->parent;
				}
				reverse(oneAnswer.begin(), oneAnswer.end());
				answers.push_back(oneAnswer);
			}
		}
	}
	vector<vector<string>> findLadders(string beginWord, string endWord, vector<string>& wordList)
	{
		buildAdjacency(beginWord, wordList);
		bfsTree.push_back(vector<A>{});
		size_t currentLevelIndex = 0;
		bfsTree[0].emplace_back(beginWord, nullptr);

		unordered_set<string>seen;
		seen.insert(beginWord);

		while (bfsTree[currentLevelIndex].size() > 0)
		{
			bfsTree.push_back(vector<A>{});
			vector<A>& nextQ = bfsTree[bfsTree.size() - 1];

			for (auto& a : bfsTree[currentLevelIndex])
			{
				if (a.word == endWord)
				{
					buildAnswers(bfsTree[currentLevelIndex], endWord);
					return answers;
				}

				for (auto& w : m[a.word])
				{
					if (!seen.contains(w))
					{
						nextQ.emplace_back(w, &a);
					}
				}
			}
			for (auto& b : nextQ)
				seen.insert(b.word);
			currentLevelIndex++;
		}
		return vector<vector<string>>{};
	}
};

/*
* begin = red;   end = tax
* list: ted, tex, red, tax, tad, den, rex, pee
* 
*                  red
*              /          \
*          ted             rex
*        /   \             |
*      tex(1) tad          tex(2)
*      |       |            |
*     tax     tax          tax
* 
*  The same word can appear on the same level more than once.
*  
*  If for example, I'm looking at word 'a' at level 'x'.
*  word 'b' and 'a' has 1 character different, but 'b' already
*  appeared before level 'x'. I should not put 'b' on level x+1.
*  If 'p' is also on level 'x', and I examined 'p' before I did 'a'.
*  and 'b' and 'p' are also 1 character different, 'b' is already
*  put on level x+1.
* 
* 
*      p       .....       a               <-- level x
*      |
*      b       .....                       <-- level x+1
* 
*  Logically, I should put 'b' on level x+1 one more time as 'a''s descendent.
*  
* 
* If 'b' appeared before level x, if 'b' eventully leads to end word, putting 'b'
* under 'a' is pointless, because the path a-->b-->...--> end must be longer than
* the path that leads to the 'b' that is on previous level.
* 
* But 'b' on the same level appears multiple times is possible, that is just
* going to create more eligible answers.
* 
* But practically, I must not put another 'b' on the same level, because processing
* it more than once is a waste of time. The more descendants 'b' has, the more time is wasted.
*           p   ......    a
*           |             |
*           b             b
*          /|\           /|\
* just think about this, if 'b' has a huge subtree, the deeper I go, the more duplications I have
* 
* Therefore, on a level, I need to remember what words have appeared, and what their
* parents are. For example, I just put one instance of 'b' on level x+1, but I
* remember that 'b''s parent can be 'p' or 'a'.
* 
* So each level is a hash map.
*/

class Solution5
{

	vector<unordered_map<string, vector<string>>> bfsTree;

	/*
	* vector: unordered_map<string, vector<string>>,  unordered_map<string, vector<string>>,  ....
	*         \___________________________________/                  |            |
	*                 one level of bfs tree                          |            the parents of this element
	*                                                             an element on a certain level
	*                                a
	*                              /   \
	*                             b      c
	*                            /  \   /  \
	*                           d    e  f   d
	* 
	* By using unordered_map, I reduce the copies of 'd' on the 3rd level, because 'd' will be the key
	* of the unordered_map, but the value of 'd' in the unordered_map will be a vector comprising 'b' and 'c',
	* I end up increasing the copies of 'b' and 'c'.
	* I have to use pointers instead of hard copy of words to represents child-to-parent relation
	*/


	unordered_map<string, vector<string>> m; // adjacency list
public:
	
	vector<vector<string>> answers;

	int diffLetterCount(string& s1, string& s2)
	{
		return inner_product(s1.begin(), s1.end(), s2.begin(), 0, plus<int>{},
			[](auto& a, auto& b)
			{
				return a == b ? 0 : 1;
			});
	}

	void buildAdjacency(string& beginword, vector<string>& wordList)
	{
		for (size_t i = 0; i < wordList.size(); i++)
		{
			string& w1 = wordList[i];
			for (size_t j = i + 1; j < wordList.size(); j++)
			{
				string& w2 = wordList[j];
				if (diffLetterCount(w1, w2) == 1)
				{
					m[w1].push_back(w2);
					m[w2].push_back(w1);
				}
			}
		}
		if (m.count(beginword) == 0)
		{
			for (size_t i = 0; i < wordList.size(); i++)
			{
				string& w1 = wordList[i];
				if (diffLetterCount(w1, beginword) == 1)
				{
					m[beginword].push_back(w1);
				}
			}
		}
	}

	void buildAnswersRecur(string word, size_t treeLevel, vector<string>&oneAnswer) // building an answer is similar to a depth-first-search.
	{
		oneAnswer.push_back(word);
		if (treeLevel == 0)
		{
			size_t len = oneAnswer.size();
			answers.push_back(vector<string>(len));
			copy(oneAnswer.rbegin(), oneAnswer.rend(), answers[answers.size() - 1].begin());
		}
		else
		{
			vector<string>& parents = bfsTree[treeLevel][word];
			for (auto const& w : parents)
			{
				buildAnswersRecur(w, treeLevel - 1, oneAnswer);
			}
		}
		oneAnswer.erase(oneAnswer.end() - 1);
	}

	void buildAnswers(string endword, size_t treeLevel)
	{
		vector<string> oneAnswer;
		buildAnswersRecur(endword, treeLevel, oneAnswer);
	}
	vector<vector<string>> findLadders(string beginWord, string endWord, vector<string>& wordList)
	{
		buildAdjacency(beginWord, wordList);
		bfsTree.push_back(unordered_map<string, vector<string>>{});
		size_t currentLevelIndex = 0;
		bfsTree[0][beginWord] = vector<string>{};

		unordered_set<string>seen;
		seen.insert(beginWord);

		while (bfsTree[currentLevelIndex].size() > 0)
		{
			bfsTree.push_back(unordered_map<string, vector<string>>{});

			auto& nextQ = bfsTree[bfsTree.size() - 1];

			for (auto& a : bfsTree[currentLevelIndex]) // I already have the current level full, and each level is in a map
			{
				if (a.first == endWord) // I can quickly check if end word is in current level 
				{  // I don't need to check end word while filling in next level
					buildAnswers(endWord, currentLevelIndex);
					return answers;
				}
				// if end word really is in current level, the half filled next level would be a waste.
				for (auto& w : m[a.first])
				{
					if (seen.count(w) == 0)
					{
						nextQ[w].push_back(a.first);
					}
				}
			}
			for (auto& b : nextQ)
				seen.insert(b.first);
			currentLevelIndex++;
		}
		return vector<vector<string>>{};
	}
};



class Solution6
{
	/*
	* Avoid copying strings, use index of words in the very original wordlist.
	*/

	/*
	* bfs:
	*      +----------------------------------+
	*      |    5 ->  [7, 15, 28]             |   <== each level of bfs is a map
	*      |                                  |
	*      +----------------------------------+
	* 
	* on a certain level, a key-value pair is 5 -> [7, 15, 28]
	* key:   5:           '5' belongs to this level (a string whose index in the original word list is 5)
	* value: [7, 15, 28]:  these 3 numbers belong to the previous level. They all have a link to '5' from previous level to this level.
	*/
	vector<unordered_map<size_t, vector<size_t>>> bfs;
	vector<vector<size_t>> m;

	int beginWordIndex = -1;
	int endWordIndex = -1;

	vector<vector<string>> answers;

	int diffLetterCount(string& s1, string& s2)
	{
		return inner_product(s1.begin(), s1.end(), s2.begin(), 0, plus<int>{},
			[](auto& a, auto& b)
			{
				return a == b ? 0 : 1;
			});
	}

	void buildAdjancency(string& beginword, string& endword, vector<string>& wordList)
	{
		m = vector<vector<size_t>>(wordList.size());

		for (size_t i = 0; i < wordList.size(); i++)
		{
			string& w1 = wordList[i];
			if (beginWordIndex == -1 && diffLetterCount(beginword, w1) == 0)
				beginWordIndex = i;
			if (endWordIndex == -1 && diffLetterCount(endword, w1) == 0)
				endWordIndex = i;

			for (size_t j = i + 1; j < wordList.size(); j++)
			{
				string& w2 = wordList[j];
				if (diffLetterCount(w1, w2) == 1)
				{
					m[i].push_back(j);
					m[j].push_back(i);
				}
			}
		}
	}

	void buildAnswersRecur(size_t currentLevelIndex, size_t currentWordIndex, vector<string>& wordList, vector<string>& oneAnswer, string& beginword)
	{
		oneAnswer.push_back(wordList[currentWordIndex]);
		if (currentLevelIndex == 0)
		{
			oneAnswer.push_back(beginword);
			size_t len = oneAnswer.size();
			answers.push_back(vector<string>(len));
			copy(oneAnswer.rbegin(), oneAnswer.rend(), answers[answers.size() - 1].begin());
			oneAnswer.erase(oneAnswer.end() - 1);
		}
		else
		{
			vector<size_t>& parents = bfs[currentLevelIndex][currentWordIndex];
			for (size_t const& a : parents)
			{
				buildAnswersRecur(currentLevelIndex - 1, a, wordList, oneAnswer, beginword);
			}
		}
		oneAnswer.erase(oneAnswer.end() - 1);
	}

	void buildAnswers(size_t currentLevelIndex, vector<string>& wordList, string& beginword)
	{
		vector<string> oneAnswer;
		buildAnswersRecur(currentLevelIndex, endWordIndex, wordList, oneAnswer, beginword);
	}

public:

	vector<vector<string>> findLadders(string beginword, string endword, vector<string>& wordList)
	{
		buildAdjancency(beginword, endword, wordList);
		vector<bool>seen = vector<bool>(wordList.size(), false);
		bfs.push_back(unordered_map<size_t, vector<size_t>>{});
		size_t currentLevelIndex = 0;

		if (beginWordIndex == -1)
		{
			for (size_t i = 0; i < wordList.size(); i++)
			{
				if (diffLetterCount(beginword, wordList[i]) == 1)
					bfs[0][i].push_back(-1);
			}
		}
		else
		{
			for (size_t& i : m[beginWordIndex])
				bfs[0][i].push_back(beginWordIndex);
		}

		while (bfs[currentLevelIndex].size() > 0)
		{
			if (bfs[currentLevelIndex].count(endWordIndex) != 0)
			{
				buildAnswers(currentLevelIndex, wordList, beginword);
				return answers;
			}

			bfs.push_back(unordered_map<size_t, vector<size_t>>{});

			unordered_map<size_t, vector<size_t>>& nextLevel = *bfs.rbegin();
			for (auto& a : bfs[currentLevelIndex]) // 'a' is an entry in the unordered_map, so it's a pair
			{
				for (auto& w : m[a.first])
				{
					if (!seen[w])
					{
						nextLevel[w].push_back(a.first);
					}
				}
			}
			for (auto& b : nextLevel)
				seen[b.first] = true;
			currentLevelIndex++;
		}
		return vector<vector<string>>{};
	}
};


/*
* bfs: each level is a vector of pairs [ (), (), (), ...]
* begin: hit
* end: cog
* wordlist: [hot, dot, dog, lot, log, cog]
*             0    1    2    3    4    5
* 
*                   hit
*                    |
*                   hot
*                  /   \
*               dot      lot
*                |        |
*               dog      log
*                |        |
*               cog      cog
* 
*            [ (0, 0) ]         hot's index in wordlist is 0
*          [(0, 1), (0,3)]    [ (0: comes from element #0 on previous level, 1: "dot"), (0: element #0 on previous, 3: "log")]
*          [(0, 2), (1,4)]    [ (0: comes from element #0 on previous, i.e. "dot", 2: "dog"), (1: element #1 on previous, i.e. "lot", 4: "log")]
*          [(0, 5), (1,5)]    [ (0: comes form element #0 on previous, i.e. "dog", 5: "cog"), (1: element #1 on previous, i.e. "log", 5: "cog")]
* 
* Basically, the first number in each pair tells with element on the previous level is the parent, the second number is the index of the word in the original word list
* 
* This solution doesn't not eliminate duplicates on any certain levels.
* If on level 'x', there are 3 different words (example: "mbc", "apc", "abw"), all of which descend to word "abc", 
* then on level 'x+1', word "abc" appears 3 times, even though each one of these 3 appearences points to a different parent.
* If "abc" has large number of descends, then all those descends appear 3 times on level 'x+2', such duplicative effect can cascade.
* Therefore, this solution is not as good as Solution6. Furthermore this solution doesn't build adjacency list at the beginning.
*/
class Solution7
{
	unsigned char isNeighbor(string& w1, string& w2)
	{
		size_t len = w1.size();
		unsigned char diffCount = 0;
		for (size_t i = 0; i < len; ++i)
		{
			if (w1[i] != w2[i] && diffCount == 1)
			{
				return 2;
			}
			else if (w1[i] != w2[i])
			{
				diffCount = 1;
			}
		}
		return diffCount;
	}

	vector<vector<string>> buildAnswers(
		vector<vector<pair<unsigned, unsigned>>>& bfsRecord,
		vector<string>& wordList,
		string& beginword,
		unsigned endwordIndex, 
		unsigned answersCount, 
		unsigned wordsCountInOneAnswer)
	{
		vector<vector<string>> answer(answersCount, vector<string>(wordsCountInOneAnswer, ""));
		unsigned answerIndex = 0;
		size_t levels = bfsRecord.size();
		auto const& bottomLevel = bfsRecord[levels-1];
		size_t bottomLevelLength = bottomLevel.size();
		for (unsigned i = 0; i < bottomLevelLength; ++i)
		{
			if (bottomLevel[i].second == endwordIndex)
			{
				unsigned wordIndexInOneAnswer = wordsCountInOneAnswer - 1;
				
				int levelIndex = levels - 1;
				unsigned pairIndexInLevel = i;

				while (levelIndex >= 0)
				{
					auto const& indexInWordList = bfsRecord[levelIndex][pairIndexInLevel].second;
					answer[answerIndex][wordIndexInOneAnswer] = wordList[indexInWordList];
					
					pairIndexInLevel = bfsRecord[levelIndex][pairIndexInLevel].first;
					--levelIndex;
					--wordIndexInOneAnswer;
				}
				answer[answerIndex][wordIndexInOneAnswer] = beginword;
				++answerIndex;
			}
		}
		return answer;
	}
public:
	vector<vector<string>> findLadders(string beginword, string endword, vector<string>& wordList)
	{
		vector<unsigned int>beginwordNeighbors;
		int beginwordIndex = -1;
		int endwordIndex = -1; // endword is not guaranteed to be in wordlist either.
		size_t wordListLen = wordList.size();
		
		unsigned char a;
		for (unsigned int i = 0; i < wordListLen; ++i)
		{
			a = isNeighbor(beginword, wordList[i]);
			if (a == 0)
			{
				beginwordIndex = (int)i;
			}
			else if (a == 1)
			{
				beginwordNeighbors.emplace_back(i);
			}
			if (isNeighbor(endword, wordList[i]) == 0)
			{
				endwordIndex = (int)i;
			}
		}
		if (endwordIndex == -1)
		{
			return vector<vector<string>>();
		}

		bitset<500> isVisited;
		vector<vector<pair<unsigned, unsigned>>>bfsRecords;
		if (beginwordIndex != -1) // if begin word is in the word list, first exclude it from candidates pool.
		{
			isVisited[beginwordIndex] = true;
		}
		bfsRecords.emplace_back(vector<pair<unsigned, unsigned>>());
		
		size_t len = beginwordNeighbors.size();
		for (unsigned i = 0; i < len; ++i)
		{
			if (beginwordNeighbors[i] == (unsigned)endwordIndex)  // endword could be just 1 letter away from bgeinword, if that is the case, there'll be just one correct sequence.
			{
				return vector<vector<string>>(1, vector<string>{beginword, endword});
			}
			bfsRecords[0].emplace_back(0, beginwordNeighbors[i]);
			isVisited[beginwordNeighbors[i]] = true;
		}

		//vector<pair<unsigned, unsigned>>* p = &(bfsRecords[0]); 
		//vector<pair<unsigned, unsigned>>* next = nullptr;
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// Attention! I can't use a pointer to keep track of the levels of the BFS tree.
		// bfsRecords keeps calling emplace_back, it's going to migrate the whole structure
		// to new address space, pointers become invalid.

		unsigned currentLevelIndex = 0;
		unsigned nextLevelIndex = 1;
		
		unsigned found = 0;
		
		unsigned ladderRungs = 2;
		// How many words in one correct sequence? help building answers. example 4 rungs: beginword -> word1 -> word2 -> endword
		// even if I run the while loop just once, this is equivalent of having 3 words in each correct sequence
		// beginword -> direct neighbor of beginword -> a word added in the 1st iteration of this while loop

		while (bfsRecords[currentLevelIndex].size() > 0)
		{
			++ladderRungs;
			bfsRecords.emplace_back(vector<pair<unsigned, unsigned>>());
			
			len = bfsRecords[currentLevelIndex].size();
			for (unsigned i = 0; i < len; ++i)
			{
				auto const& e = bfsRecords[currentLevelIndex][i];
				auto const& f = e.second;
				for (unsigned j = 0; j < wordListLen; ++j)
				{
					if (!isVisited[j] && isNeighbor(wordList[f], wordList[j]) == 1)
					{
						bfsRecords[nextLevelIndex].emplace_back(i, j);
						if (j == (unsigned)endwordIndex)
						{
							++found;
							break;
							// I'm looking for the neighbors of an element on current level,
							// once I find the target endword, all other neighbors don't matter any more
							// I should move on to the next element on current level.
						}	
					}
				} // looking for neighbors of a single element on the previous level
			}// iterating through previous level
			if (found > 0)
			{
				return buildAnswers(bfsRecords, wordList, beginword, endwordIndex, found, ladderRungs);
			}
			if (bfsRecords[nextLevelIndex].size() == 0)
			{
				return vector<vector<string>>();
			}
			for (auto const& u : bfsRecords[nextLevelIndex])
			{
				isVisited[u.second] = true;
			}
			++currentLevelIndex;
			++nextLevelIndex;
		}
		return vector<vector<string>>();
	}
};

// accepted 3ms beat 98%, memory 12.39MB beat 99.98%
/*
* Requirements have "shortest" path, naturally, breadth-first-search.
* The chanllenge is to reconstruct all answers.
* BFS is basically a hierarchical progression, from the starting node, ndoes are added to
* the hierarchy layer after layer based on their distance to the starting node.
* Comments of previous solutions already explained the reason for having the same node
* multiple times on the same level.
* Previous solution all try to keep the "parent" node information on the BFS hierarchy.
* The BFS hierarchy is dynamically allocated, too many information being put on it can
* lead to large scale memory blocks migration which slows it down.
* Must put such "parent" information on the stack.
* For example, assuming there are 100 words in the wordlist
* "parentsMap":
*  vector of 100 elements, each element is a pair.
*  0  ( true/false, [ ...... ] )
*  1
*  .
*  50 ( true/false, [ 13, 29, 7, 35, 16 ] )
*  .
*  99
* 
*  the bool value in the pair tells me if that word has been visited.
*  the vector (the 2nd member of the pair) tells the parents of this element
* 
* Another thing this solution does better than previous ones is that instead of
* my accustomed two-queue-toggle approach to run BFS, this solution uses textbook classic one bi-directional queue approach.
* In two-queue-toggle approach, when either queue is being examined, it keeps shrinking all the way to zero. According to dynamic vector algorithm,
* such shrinking may cause re-allocation.
* but in one-bidirectional queue approach, dequeuing and enqueuing happen alternatively, the length of the queue is relatively stable,
* which reduces re-allocation and migration of all elements in the queue.
*/
class Solution8
{
	const unsigned int SENTINAL = 1'000'000;

	unsigned char isNeighbor(string& w1, string& w2)
	{
		size_t len = w1.size();
		unsigned char diffCount = 0;
		for (size_t i = 0; i < len; ++i)
		{
			if (w1[i] != w2[i] && diffCount == 1)
			{
				return 2;
			}
			else if (w1[i] != w2[i])
			{
				diffCount = 1;
			}
		}
		return diffCount;
	}

	void buildAnswersRecursive(
		vector<string>& wordlist,
		string& beginword,
		int beginwordIndex,
		string& endword,
		int endwordIndex,
		vector<pair<bool, vector<unsigned int>>>& parentsMap,
		vector<vector<string>>& answers,
		vector<string>& oneAnswer,
		unsigned previousWordIndexInWordlist,
		unsigned wordIndexInSeq)
	{
		if (wordIndexInSeq == 0)
		{
			// beginword should be already pre-filled, so just put this "oneAnswer" onto "answers"
			//oneAnswer[0] = beginword;
			answers.emplace_back(oneAnswer);
			//oneAnswer[0] = "";
			return;
		}

		unsigned a = parentsMap[previousWordIndexInWordlist].second.size();
		for (unsigned i = 0; i < a; ++i)
		{
			unsigned thisWordIndexInWordlist = parentsMap[previousWordIndexInWordlist].second[i];
			oneAnswer[wordIndexInSeq] = wordlist[thisWordIndexInWordlist];
			buildAnswersRecursive(wordlist, beginword, beginwordIndex, endword, endwordIndex, parentsMap, answers, oneAnswer, thisWordIndexInWordlist, wordIndexInSeq - 1);
		}
	}

	vector<vector<string>> buildAnswers(
		vector<string>& wordlist, 
		string& beginword, 
		int beginwordIndex, 
		string& endword, 
		int endwordIndex, 
		vector<pair<bool, vector<unsigned int>>>& parentsMap,
		unsigned eachAnswerLength)
	{
		vector<vector<string>>answers;
		
		// find the length of one answer (bfs stage should be able to find the length of each answer)
		/*
		unsigned b = endwordIndex;	
		unsigned len = 1;
		while (b != SENTINAL)
		{
			++len;
			b = parentsMap[b].second[0];
		}
		if (beginwordIndex != -1)
		{
			--len;
		}
		vector<string>oneAnswer(len, "");
		*/

		vector<string>oneAnswer(eachAnswerLength, "");
		// I can pre-fill beginword and endword
		oneAnswer[0] = beginword;
		oneAnswer[eachAnswerLength - 1] = endword;
		buildAnswersRecursive(wordlist, beginword, beginwordIndex, endword, endwordIndex, parentsMap, answers, oneAnswer, endwordIndex, eachAnswerLength - 2);
		return answers;
	}
public:
	vector<vector<string>> findLadders(string& beginword, string& endword, vector<string>& wordlist)
	{
		size_t len = wordlist.size();
		vector<pair<bool, vector<unsigned int>>> parentsMap(len, pair<bool, vector<unsigned>>(false, vector<unsigned>()));
		deque<unsigned> bfs;
		int endwordIndex = -1;
		int beginwordIndex = -1;
		vector<unsigned> beginwordNeighbors;
		bool found = false;

		// find beginword's and endword's indices in the wordlist
		// and find all direct neighbors of beginword
		for (unsigned i = 0; i < len; ++i)
		{
			auto a = isNeighbor(beginword, wordlist[i]);
			if (a == 0)
			{
				beginwordIndex = i;
			}
			else if (a == 1)
			{
				beginwordNeighbors.push_back(i);
			}
			if (endword == wordlist[i])
			{
				endwordIndex = i;
			}
		}

		if (endwordIndex == -1) // endword isn't in the word list, there won't be any answers
		{
			return vector<vector<string>>();
		}

		size_t len2 = beginwordNeighbors.size();
		if (len2 == 0)
		{
			// beginword has no neighbor, there is no way for it to go anywhere
			return vector<vector<string>>();
		}

		// check if beginword and endword are direct neighbors
		if (isNeighbor(beginword, endword) == 1)
		{
			return vector<vector<string>>(1, vector<string>{beginword, endword});
		}
		
		// if beginword is in word list, close its parents vector
		if (beginwordIndex != -1)
		{
			parentsMap[beginwordIndex].first = true;
			parentsMap[beginwordIndex].second.push_back(SENTINAL);
		}

		// put beginword's direct neighbors in the bfs deque, close their parents vector
		for (unsigned i = 0; i < len2; ++i)
		{
			bfs.push_back(beginwordNeighbors[i]);
			parentsMap[beginwordNeighbors[i]].second.push_back(beginwordIndex == -1 ? SENTINAL : beginwordIndex);
			parentsMap[beginwordNeighbors[i]].first = true;
		}
		// don't forget to put a sentinal after beginword's direct neighbors
		bfs.push_back(SENTINAL);

		unsigned eachAnswerLength = 2;

		while (!bfs.empty())
		{
			auto a = bfs.front();
			bfs.pop_front();
			if (a == SENTINAL) // just finished examining a layer in BFS hierarchy
			{
				if (found)
				{
					// if found is true, the layer above "endword" in the bfs hierarchy has been examined, and endword's parents have all been identified
					// I now have everything I need to build answers
					return buildAnswers(wordlist, beginword, beginwordIndex, endword, endwordIndex, parentsMap, eachAnswerLength);
				}
				if (bfs.size() > 0) // this means there is a next layer, and I just filled up this next layer, elements on this next layer need to be marked as "already visited"
				{
					for (auto c : bfs)
					{
						parentsMap[c].first = true;
					}
					bfs.push_back(SENTINAL); // put a SENTIANL to the end of this new layer
					++eachAnswerLength;
				}
				else
				{
					return vector<vector<string>>();
				}
			}
			//else if (a == endwordIndex) // probably don't need this "if", I'll never actually get "endword" from the deque, BFS stops when I reach the layer above endword
			//{
			//	return buildAnswers(wordlist, beginword, beginwordIndex, endword, endwordIndex, parentsMap, eachAnswerLength);
			//}
			else // this is an intermediate node in the BFS hierarchy
			{
				auto b = isNeighbor(endword, wordlist[a]);
				if (b == 1) // this element is a neighbor of endword, this layer is the one direct above endword
				{
					if (parentsMap[endwordIndex].second.size() == 0)
					{
						bfs.push_back(endwordIndex);
						found = true;
						++eachAnswerLength;
					}
					parentsMap[endwordIndex].second.push_back(a);
					continue;
				}
				if (b == 2 && found)
				{
					// if endword is already found, and this word is not a neighbor of endword, it's pointless to put its neighbors in the deque
					// the task now becomes to find all the words on the previous BFS level that is a direct neighbor of endword
					continue;
				}
				for (unsigned i = 0; i < len; ++i)
				{
					if (parentsMap[i].first == false)
					{
						b = isNeighbor(wordlist[i], wordlist[a]);
						if (b == 1)
						{
							if (parentsMap[i].second.size() == 0)
							{
								bfs.push_back(i);
							}
							parentsMap[i].second.push_back(a);
						}
					}
				}
			}
		}
		return vector<vector<string>>();
	}
};


static void printAnswer(vector<vector<string>>const& a)
{
	for (auto& path : a)
	{
		for (auto& w : path)
			cout << w << "   ";

		cout << '\n';
	}
	cout << '\n';
}

static void Test1()
{
	string beginWord = "hit";
	string endWord = "cog";
	vector<string> wordList = {"hot", "dot", "dog", "lot", "log", "cog"};

	Solution8 s;
	vector<vector<string>>const& answer = s.findLadders(beginWord, endWord, wordList);
	printAnswer(answer);

}

static void Test2()
{
	string beginWord = "hit", endWord = "cog";
	vector<string>wordList = { "hot", "dot", "dog", "lot", "log" };

	Solution8 s;
	vector<vector<string>>const& answer = s.findLadders(beginWord, endWord, wordList);
	printAnswer(answer);
}

static void Test3()
{
	string beginWord = "dog", endWord = "dot";
	vector<string> wordList = { "cog", "bot", "dag", "dod", "bog", "dot"};
	Solution8 s;
	vector<vector<string>>const& answer = s.findLadders(beginWord, endWord, wordList);
	printAnswer(answer);
}

static void Test4()
{
	string beginWord = "red", endWord = "tax";
	vector<string> wordList = { "ted","tex","red","tax","tad","den","rex","pee" };
	Solution8 s;
	vector<vector<string>>const& answer = s.findLadders(beginWord, endWord, wordList);
	printAnswer(answer);
}

static void Test5()
{
	string beginWord = "hot", endWord = "dog";
	vector<string> wordList = { "hot", "dog", "dot"};
	Solution8 s;
	vector<vector<string>>const& answer = s.findLadders(beginWord, endWord, wordList);
	printAnswer(answer);
}

static void Test6()
{
	string beginWord = "hot", endWord = "dog";
	vector<string> wordList = { "hot","cog","dog","tot","hog","hop","pot","dot" };
	Solution8 s;
	vector<vector<string>>const& answer = s.findLadders(beginWord, endWord, wordList);
	printAnswer(answer);
}

static void Test7()
{
	string beginWord = "aaaaa", endWord = "ggggg";
	vector<string> wordList = { "aaaaa", "caaaa", "cbaaa", "daaaa", "dbaaa", 
		"eaaaa", "ebaaa", "faaaa", "fbaaa", "gaaaa", "gbaaa", "haaaa", 
		"hbaaa", "iaaaa", "ibaaa", "jaaaa", "jbaaa", "kaaaa", "kbaaa", 
		"laaaa", "lbaaa", "maaaa", "mbaaa", "naaaa", "nbaaa", "oaaaa", 
		"obaaa", "paaaa", "pbaaa", "bbaaa", "bbcaa", "bbcba", "bbdaa", 
		"bbdba", "bbeaa", "bbeba", "bbfaa", "bbfba", "bbgaa", "bbgba", 
		"bbhaa", "bbhba", "bbiaa", "bbiba", "bbjaa", "bbjba", "bbkaa", 
		"bbkba", "bblaa", "bblba", "bbmaa", "bbmba", "bbnaa", "bbnba", 
		"bboaa", "bboba", "bbpaa", "bbpba", "bbbba", "abbba", "acbba", 
		"dbbba", "dcbba", "ebbba", "ecbba", "fbbba", "fcbba", "gbbba", 
		"gcbba", "hbbba", "hcbba", "ibbba", "icbba", "jbbba", "jcbba", 
		"kbbba", "kcbba", "lbbba", "lcbba", "mbbba", "mcbba", "nbbba", 
		"ncbba", "obbba", "ocbba", "pbbba", "pcbba", "ccbba", "ccaba", 
		"ccaca", "ccdba", "ccdca", "cceba", "cceca", "ccfba", "ccfca", 
		"ccgba", "ccgca", "cchba", "cchca", "cciba", "ccica", "ccjba", 
		"ccjca", "cckba", "cckca", "cclba", "cclca", "ccmba", "ccmca", 
		"ccnba", "ccnca", "ccoba", "ccoca", "ccpba", "ccpca", "cccca", 
		"accca", "adcca", "bccca", "bdcca", "eccca", "edcca", "fccca", 
		"fdcca", "gccca", "gdcca", "hccca", "hdcca", "iccca", "idcca", 
		"jccca", "jdcca", "kccca", "kdcca", "lccca", "ldcca", "mccca", 
		"mdcca", "nccca", "ndcca", "occca", "odcca", "pccca", "pdcca", 
		"ddcca", "ddaca", "ddada", "ddbca", "ddbda", "ddeca", "ddeda", 
		"ddfca", "ddfda", "ddgca", "ddgda", "ddhca", "ddhda", "ddica", 
		"ddida", "ddjca", "ddjda", "ddkca", "ddkda", "ddlca", "ddlda", 
		"ddmca", "ddmda", "ddnca", "ddnda", "ddoca", "ddoda", "ddpca", 
		"ddpda", "dddda", "addda", "aedda", "bddda", "bedda", "cddda", 
		"cedda", "fddda", "fedda", "gddda", "gedda", "hddda", "hedda", 
		"iddda", "iedda", "jddda", "jedda", "kddda", "kedda", "lddda", 
		"ledda", "mddda", "medda", "nddda", "nedda", "oddda", "oedda", 
		"pddda", "pedda", "eedda", "eeada", "eeaea", "eebda", "eebea", 
		"eecda", "eecea", "eefda", "eefea", "eegda", "eegea", "eehda", 
		"eehea", "eeida", "eeiea", "eejda", "eejea", "eekda", "eekea", 
		"eelda", "eelea", "eemda", "eemea", "eenda", "eenea", "eeoda", 
		"eeoea", "eepda", "eepea", "eeeea", "ggggg", "agggg", "ahggg", 
		"bgggg", "bhggg", "cgggg", "chggg", "dgggg", "dhggg", "egggg", 
		"ehggg", "fgggg", "fhggg", "igggg", "ihggg", "jgggg", "jhggg", 
		"kgggg", "khggg", "lgggg", "lhggg", "mgggg", "mhggg", "ngggg", 
		"nhggg", "ogggg", "ohggg", "pgggg", "phggg", "hhggg", "hhagg", 
		"hhahg", "hhbgg", "hhbhg", "hhcgg", "hhchg", "hhdgg", "hhdhg", 
		"hhegg", "hhehg", "hhfgg", "hhfhg", "hhigg", "hhihg", "hhjgg", 
		"hhjhg", "hhkgg", "hhkhg", "hhlgg", "hhlhg", "hhmgg", "hhmhg", 
		"hhngg", "hhnhg", "hhogg", "hhohg", "hhpgg", "hhphg", "hhhhg", 
		"ahhhg", "aihhg", "bhhhg", "bihhg", "chhhg", "cihhg", "dhhhg", 
		"dihhg", "ehhhg", "eihhg", "fhhhg", "fihhg", "ghhhg", "gihhg", 
		"jhhhg", "jihhg", "khhhg", "kihhg", "lhhhg", "lihhg", "mhhhg", 
		"mihhg", "nhhhg", "nihhg", "ohhhg", "oihhg", "phhhg", "pihhg", 
		"iihhg", "iiahg", "iiaig", "iibhg", "iibig", "iichg", "iicig", 
		"iidhg", "iidig", "iiehg", "iieig", "iifhg", "iifig", "iighg", 
		"iigig", "iijhg", "iijig", "iikhg", "iikig", "iilhg", "iilig", 
		"iimhg", "iimig", "iinhg", "iinig", "iiohg", "iioig", "iiphg", 
		"iipig", "iiiig", "aiiig", "ajiig", "biiig", "bjiig", "ciiig", 
		"cjiig", "diiig", "djiig", "eiiig", "ejiig", "fiiig", "fjiig", 
		"giiig", "gjiig", "hiiig", "hjiig", "kiiig", "kjiig", "liiig", 
		"ljiig", "miiig", "mjiig", "niiig", "njiig", "oiiig", "ojiig", 
		"piiig", "pjiig", "jjiig", "jjaig", "jjajg", "jjbig", "jjbjg", 
		"jjcig", "jjcjg", "jjdig", "jjdjg", "jjeig", "jjejg", "jjfig", 
		"jjfjg", "jjgig", "jjgjg", "jjhig", "jjhjg", "jjkig", "jjkjg", 
		"jjlig", "jjljg", "jjmig", "jjmjg", "jjnig", "jjnjg", "jjoig", 
		"jjojg", "jjpig", "jjpjg", "jjjjg", "ajjjg", "akjjg", "bjjjg", 
		"bkjjg", "cjjjg", "ckjjg", "djjjg", "dkjjg", "ejjjg", "ekjjg", 
		"fjjjg", "fkjjg", "gjjjg", "gkjjg", "hjjjg", "hkjjg", "ijjjg", 
		"ikjjg", "ljjjg", "lkjjg", "mjjjg", "mkjjg", "njjjg", "nkjjg", 
		"ojjjg", "okjjg", "pjjjg", "pkjjg", "kkjjg", "kkajg", "kkakg", 
		"kkbjg", "kkbkg", "kkcjg", "kkckg", "kkdjg", "kkdkg", "kkejg", 
		"kkekg", "kkfjg", "kkfkg", "kkgjg", "kkgkg", "kkhjg", "kkhkg", 
		"kkijg", "kkikg", "kkljg", "kklkg", "kkmjg", "kkmkg", "kknjg", 
		"kknkg", "kkojg", "kkokg", "kkpjg", "kkpkg", "kkkkg", "ggggx", 
		"gggxx", "ggxxx", "gxxxx", "xxxxx", "xxxxy", "xxxyy", "xxyyy", 
		"xyyyy", "yyyyy", "yyyyw", "yyyww", "yywww", "ywwww", "wwwww", 
		"wwvww", "wvvww", "vvvww", "vvvwz", "avvwz", "aavwz", "aaawz", 
		"aaaaz" };
	Solution8 s;
	vector<vector<string>>const& answer = s.findLadders(beginWord, endWord, wordList);
	printAnswer(answer);
}

void Test_0126_Word_Ladder_II()
{
	int test;
	function<void(void)> tests[]={Test1, Test2, Test3, Test4, Test5, Test6, Test7};
	while (true)
	{
		cout << "which test to run?  (0 to exit) ";
		cin >> test;
		if (test == 0) break;
		tests[test-1]();
	}
}

}