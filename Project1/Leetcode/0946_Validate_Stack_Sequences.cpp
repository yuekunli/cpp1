#include<vector>
#include<iostream>
#include<sstream>
#include<string>
#include<iomanip>

namespace _0946_Validate_Stack_Sequences {

	using namespace std;

	// accepted 0ms, beat 100% (almost all accepted solutions are in the 0ms group)
	class Solution
	{
	public:
		bool validateStackSequences(vector<int>& pushed, vector<int>& popped)
		{
			int i = 0;
			while (pushed[i] != popped[0])
			{
				++i;
			}
			int nextPush = i + 1;
			auto len = popped.size();
			int j = 0;
			while(j < len)
			{
				// think about this scenario, I push a few numbers.
				// then the head of the stack keeps matching "popped",
				// in another word, I keep popping elements out of the stack,
				// during these pop actions, I didn't push one more.
				// eventually every element is popped, the stack is empty.
				// right now i == -1, next iteration, when I check if the
				// element pointed by 'j' in popped matches the element pointed
				// by 'i' in pushed, 'i' is -1, indexing overflow.
				// Therefore, need to make sure i is >= 0.
				if (i >= 0 && popped[j] == pushed[i])
				{
					--i;
					++j;
				}
				else
				{
					if (nextPush >= len)
					{
						return false;
					}
					pushed[++i] = pushed[nextPush++];
				}
			}
			return true;
		}
	};

	void Test_0946_Validate_Stack_Sequences()
	{
		Solution so;
		while (true)
		{
			cout << "pushed array: ";
			string pushedWholeInput;
			getline(cin, pushedWholeInput);
			if (pushedWholeInput.size() == 0)
				return;

			cout << "popped array: ";
			string poppedWholeInput;
			getline(cin, poppedWholeInput);
			if (poppedWholeInput.size() == 0)
				return;

			vector<int> pushed;
			stringstream ss(pushedWholeInput);
			copy(istream_iterator<int>(ss), istream_iterator<int>(), back_inserter(pushed));

			vector<int> popped;
			stringstream ss2(poppedWholeInput);
			copy(istream_iterator<int>(ss2), istream_iterator<int>(), back_inserter(popped));

			cout << std::boolalpha << so.validateStackSequences(pushed, popped) << "\n";
		}
	}
}