#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<algorithm>
#include<numeric>

namespace _0833_Find_And_Replace_in_String {

	using namespace std;

	// Some important notes about the input:
	// 
	// 1.
	// the indices can be given in non-sorted order
	// 
	// 2.
	// the targeted substring can go over the original string's end
	// for example, this is one of the leetcode test cases:
	// s = abcde
	// indices = 2, 2, 3
	// sources = "cde", "cdef", "dk"
	// targets = "fe", "f", "xyz"

	// a   b   c   d   e
	//         2
	//         |
	//         c   d   e   f
	// the original string doesn't even have 4 letters starting at index '2'.

	class Solution
	{
	public:
		string findReplaceString(string s, vector<int>& indices, vector<string>& sources, vector<string>& targets)
		{
			size_t k = indices.size();
			
			vector<int>indicesOfIndices(k, 0);

			/*
			* indices:            13       52        8         44          38          21
			* indicesOfIndices:    0        1        2          3           4           5    <-- initialized with iota
			* 
			* indicesOfIndices:    2(8)     0(13)    5(21)      4(38)       3(44)       1(52)
			* 
			* indicesOfIndices[2] ---> 5       the 3rd smallest index is the one in cell#5
			* indices[5]          ---> 21      "21" is the one in cell#5
			* "21" is the index into the original string
			*/


			iota(indicesOfIndices.begin(), indicesOfIndices.end(), 0);
			sort(indicesOfIndices.begin(), indicesOfIndices.end(),
				[&](int const& a, int const& b)
				{
					return indices[a] < indices[b];
				});
			
			int offset = 0;
			for (size_t i = 0; i < k; i++)
			{
				int m = indicesOfIndices[i];
				int startIndex = indices[m] + offset;
				if (equal(sources[m].begin(), sources[m].end(), s.begin() + startIndex))
				{
					size_t sourceLength = sources[m].size();
					s.replace(s.begin() + startIndex, s.begin() + startIndex + sourceLength, targets[m].begin(), targets[m].end());
					offset += (int)targets[m].size() - (int)sourceLength;
				}
			}
			return s;
		}
	};


	// accepted 0ms, beats 100%
	// this solution doesn't sort the indices
	// Each character in the original string is mapped to a Record struct in a new array
	// +---------------------+
	// |  origianl char      |
	// |  is still valid     | 
	// | ptr to replacement  |
	// +---------------------+
	//
	//  for example, input 's' = abcde
	//  a    b    c    d    e
	//           \______/
	//              |
	//            a target matches this substring
	//            set "valid" to false in both characters' Record struct
	//            set the replacement to c's Record
	// 
	// The record array becomes:
	// a     b    false     false     e
	//              |
	//               \
	//                xyz
	//
	class Solution2
	{
	public:
		struct Record
		{
			char original_char;
			bool valid;
			char const* newAddition;
			Record() : original_char(-1), valid(false), newAddition(nullptr) {}
			Record(char _original_char) : original_char(_original_char), valid(true), newAddition(nullptr) {}
			Record(Record const& rhs) : original_char(rhs.original_char), valid(rhs.valid), newAddition(rhs.newAddition) {}
			Record(Record&& rhs) : original_char(rhs.original_char), valid(rhs.valid), newAddition(rhs.newAddition)
			{
				rhs.newAddition = nullptr;
			}
			~Record() = default;
		};
		string findReplaceString(string s, vector<int>& indices, vector<string>& source, vector<string>& targets)
		{
			size_t targeted_substring_count = indices.size();
			size_t original_string_size = s.size();
			size_t n = original_string_size;
			vector<Record> original(original_string_size, Record{});

			for (int i = 0; i < original_string_size; ++i)
			{
				original[i].original_char = s[i];
				original[i].valid = true;
			}
			
			for (int i = 0; i < targeted_substring_count; ++i)
			{
				size_t substring_len = source[i].size();
				bool match = true;
				for (int j = indices[i], p = 0; p < substring_len; ++j, ++p)
				{
					if (j >= original_string_size)
					{
						match = false;
						break;
					}
					if (original[j].original_char != source[i][p])
					{
						match = false;
						break;
					}
				}
				if (match)
				{
					n = n + (targets[i].size() - source[i].size());
					for (int j = indices[i], p = 0; p < substring_len; ++j, ++p)
					{
						original[j].valid = false;
					}
					int j = indices[i];
					original[j].newAddition = targets[i].c_str();
				}
			}
			
			string ret(n, 0);
			for (int j = 0, p = 0; j < original_string_size; ++j)
			{
				if (original[j].valid)
				{
					ret[p] = original[j].original_char;
					++p;
				}
				else
				{
					if (original[j].newAddition != nullptr)
					{
						char const* t = original[j].newAddition;
						while (*t != '\0')
						{
							ret[p] = *t;
							++p;
							++t;
						}
					}
				}
			}
			return ret;
		}
	};


	void Test_0833_Find_And_Replace_in_String()
	{
		Solution2 solu;
		string tmp; // the entire input is temporarily stored in this string
		vector<string>tmp2; // basically break the entire input by whitespace and each part is stored in the vector

		// for example if a problem has these inputs:
		// original string "vmokgggqzp"
		// indices: 3, 5, 1
		// sources: "kg", "ggq", "mo"
		// targets: "s", "so", "bfr"

		// put these on one line then press enter:
		// vmokgggqzp 3 5 1 kg ggq mo s so bfr

		string s;
		vector<int>indices;
		vector<string>sources;
		vector<string>targets;
		while (true)
		{
			tmp.clear();
			cout << "all inputs: ";
			getline(cin, tmp);
			if (tmp.size() == 0) break;

			stringstream ss(tmp);
			tmp2.clear();
			copy(istream_iterator<string>(ss), istream_iterator<string>(), back_inserter(tmp2));

			size_t sz = tmp2.size();
			size_t k = (sz - 1) / 3; // minus 1 is because the very first element is the original input string
			indices.clear();
			sources.clear();
			targets.clear();
			s = tmp2[0];
			int tmp2Index = 1;
			for (int i = 0; i < k; i++)
			{
				indices.emplace_back(stoi(tmp2[tmp2Index]));
				tmp2Index++;
			}

			for (int i = 0; i < k; i++)
			{
				sources.emplace_back(tmp2[tmp2Index]);
				tmp2Index++;
			}

			for (int i = 0; i < k; i++)
			{
				targets.emplace_back(tmp2[tmp2Index]);
				tmp2Index++;
			}

			cout << solu.findReplaceString(s, indices, sources, targets) << "\n\n";
		}
	}
}