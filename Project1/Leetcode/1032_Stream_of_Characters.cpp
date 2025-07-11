#include<iostream>
#include<iomanip>
#include<string>
#include<vector>
#include<algorithm>


namespace _1032_Stream_of_Characters {

	using namespace std;

	class StreamChecker
	{
		/*
		*  (1). Sort the list of words
		*  (2). Run KMP preprocessing on every word, so that I know the length of prefix that match the suffix of any prefix for any word
		*  (3). I get the first query, because the list of words is sorted. Find the words whose first letter match this first query
		*       Put these words in a "working set"
		*  (4). I get the second query, check the 2nd letter of each word in the "working set". 
		* 
		* Every record in the "working set" contains two pieces of information:
		* (1). the index of the word in the sorted list.
		* (2). the index of the word that has so far muched previous queries.
		* If the next query doesn't match the next letter of a word, use the KMP information to reconcile the matching index. If it reconciles
		* all the way back to not matching anything, remove this record from the "working set".
		*/

		struct Record
		{
			int pIndex{ 0 };
			int matchIndex{ -1 };
		};

		vector<Record>* workSet{ nullptr };

		vector<vector<int>> prefixMatchLen;

		vector<string>patterns;

	public:

		StreamChecker(vector<string>& strs)
		{
			size_t len = strs.size();

			prefixMatchLen.resize(len);

			patterns = strs;
			sort(patterns.begin(), patterns.end());

			int index = 0;
			for (string& s : patterns)
			{
				kmpPreprocess(s, prefixMatchLen[index]);
				index++;
			}
			workSet = new vector<Record>;
		}

		~StreamChecker()
		{
			if (workSet != nullptr)
				delete workSet;
		}

		void kmpPreprocess(string& s, vector<int>& pfxLen)
		{
			size_t len = s.size();
			pfxLen.resize(len);
			int i = 1, k = 0;
			while (i < len)
			{
				if (s[i] == s[k])
				{
					k++;
					pfxLen[i] = k;
					i++;
					continue;
				}
				if (k == 0)
				{
					pfxLen[i] = 0;
					i++;
					continue;
				}
				k = pfxLen[k - 1];
			}
		}

		bool query(char c)
		{
			bool foundMatch = false;
			if (workSet->size() > 0)
			{
				vector<Record>* ws2 = new vector<Record>;
				for (auto i = workSet->begin(); i != workSet->end(); i++)
				{
					string& pat = patterns[i->pIndex];
					int idx = i->matchIndex;
					if (c == pat[idx + 1])
					{
						if (idx + 1 == pat.size() - 1)
						{
							// found match!
							foundMatch = true;
							int k = prefixMatchLen[i->pIndex][idx + 1];
							if (k > 0)
							{
								ws2->push_back({ i->pIndex, k - 1 });
							}
						}
						else
						{
							ws2->push_back({ i->pIndex, idx + 1 });
						}
					}
					else
					{
						// reconcil
						int k = prefixMatchLen[i->pIndex][idx];
						while (k > 0)
						{
							if (c == pat[k])
							{
								ws2->push_back({ i->pIndex, k });
								break;
							}
							k = prefixMatchLen[i->pIndex][k - 1];
						}
					}
				}
				delete workSet;
				workSet = ws2;
			}

			auto iter1 = lower_bound(patterns.begin(), patterns.end(), string(1, c));
			if (iter1 != patterns.end() and c == (*iter1)[0])
			{
				if (c != 'z')
				{
					auto iter2 = lower_bound(patterns.begin(), patterns.end(), string(1, c + 1));
					for (auto i = iter1; i < iter2; i++)
					{
						if (i->size() == 1)
							foundMatch = true;
						workSet->push_back({ int(distance(patterns.begin(), i)), 0 });
					}
				}
				else
				{
					for (auto i = iter1; i < patterns.end(); i++)
					{
						if (i->size() == 1)
							foundMatch = true;
						workSet->push_back({ int(distance(patterns.begin(), i)), 0 });
					}
				}
			}
			return foundMatch;
		}
	};

	// accepted, with destructor 504ms beat 6%
	// without destructor 41ms beat 65%
	/*
	* Improvement idea:
	* right now this trie has one and only one letter in each node,
	* the idea for improvement is to make a split only when necessary
	*                       a
	*                     /   \
	*                   bce    tr
	*                  /   \
	*                 fw    g
	*                /  \
	*               x    y
	* 
	* This trie represent 4 words: xwfecba, ywfecba, gecba, rta
	* In another word, implement it in the way Cisco ACL radix trie is implemented.
	* 
	* This saves memory, but may not save time.
	*/
	class StreamChecker2
	{
		struct Node
		{
			vector<Node*> nextLevel = vector<Node*> (26, nullptr);
			bool isEnd{ false }; // is it the end of a word? (actually the start of the word because I build reverse trie
		};

		Node* root;
		string streamSoFar;

		void AddToTrie(string const & w)
		{
			Node* n = root;
			auto len = w.size();
			for (int i = len - 1; i >= 0; --i)
			{
				const char& c = w[i];
				Node* nextNode = n->nextLevel[c - 'a'];
				if (nextNode == nullptr)
				{
					nextNode = new Node();
					n->nextLevel[c - 'a'] = nextNode;
				}
				n = nextNode;
			}
			n->isEnd = true;
		}

		bool searchTrie()
		{
			Node* n = root;
			for (auto it = rbegin(streamSoFar); it != rend(streamSoFar); it = next(it))
			{
				Node* nextNode = n->nextLevel[*it - 'a'];
				
				if (nextNode == nullptr)
					return false;
				
				if (nextNode->isEnd)
					return true;

				n = nextNode;
			}
			return false;
		}

		void destroyTrie(Node* root)
		{
			Node* n = root;
			for (Node*& nextNode : n->nextLevel)
			{
				if (nextNode != nullptr)
					destroyTrie(nextNode);
			}
			delete n;
		}

	public:
		StreamChecker2(vector<string>& words)
		{
			root = new Node();

			for (string& w : words)
			{
				//reverse(w.begin(), w.end());
				AddToTrie(w);
			}
		}

		~StreamChecker2()
		{
			destroyTrie(root);
		}

		bool query(char letter)
		{
			streamSoFar.append(string(1,letter));
			return searchTrie();
		}
	};


	/*
	* The idea is to make a 2-level trie.
	* first level has the last letter of each word
	* second level has the second last letter of each word
	* 
	*                          +----...----+
	*                          |  e ...    |
	*                          +----...----+
	*                          /   | ...|   \
	*                            +-..-+
	*                            |  t |
	*                            +----+
	*                               |
	*                               an vector of words (all the words ending with "et" are saved here)
	* The hope is that a 2-level trie can distribute all the "words" relatively evenly to each bucket,
	* So that each vector of "words" on the 3rd level won't be too many.
	*/       
	// accepted but slow
	class StreamChecker3
	{
	private:
		vector<bool> singleLetterWords;
		vector<vector<vector<string*>>>r;
	public:
		StreamChecker3(vector<string>& words)
			:r(26, vector<vector<string*>>(26, vector<string*>())),
			singleLetterWords(26, false)
		{
			for (auto & w : words)
			{
				if (w.size() == 1)
				{
					singleLetterWords[w[0] - 'a'] = true;
				}
				else
				{
					char const & last1 = w[w.size() - 1];
					char const& last2 = w[w.size() - 2];

					r[last1 - 'a'][last2 - 'a'].emplace_back(&w);
				}
			}
		}
		string streamSoFar;
		bool query(char letter)
		{
			if (singleLetterWords[letter - 'a'])
			{
				return true;
			}
			else
			{
				if (streamSoFar.size() == 0)
				{
					streamSoFar.append(std::initializer_list<char>{letter});
					return false;
				}
				streamSoFar.append(std::initializer_list<char>{letter});
				char const& last1 = streamSoFar[streamSoFar.size() - 1];
				char const& last2 = streamSoFar[streamSoFar.size() - 2];

				auto const& arrayOfWords = r[last1 - 'a'][last2 - 'a'];
				for (auto const& w : arrayOfWords)
				{
					auto len = w->size();
					if (len == 2)
					{
						return true;
					}
					else
					{
						bool found = true;
						int i,j;
						for (i = len - 3, j = streamSoFar.size() - 3; i >= 0 && j >= 0; --i, --j)
						{
							if ((*w)[i] != streamSoFar[j])
							{
								found = false;
								break;
							}
						}
						if (found && (i == -1))
						{
							return true;
						}
					}					
				}
				return false;
			}
		}
	};

	void Test_1032_Stream_of_Characters()
	{
		vector<string> words{ "cd", "f", "kl" };
		StreamChecker3* obj = new StreamChecker3(words);
		char letters[] = "abcdefghijkl";
		for (int i = 0; i < size(letters)-1; i++)  
			// there are 12 characters in "letters", but its size (or sizeof) is 13, the character in #12 (0-index) cell is '\0'
		{
			cout << "i: " << i << "  " << letters[i] << "  " << boolalpha << obj->query(letters[i]) << '\n';
		}
		delete obj;
	}

}