#include<vector>
#include<string>
#include<array>
#include<iostream>

namespace _0940_Distinct_Subsequence_II {

	using namespace std;

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
					currentCount = currentCount % base;
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