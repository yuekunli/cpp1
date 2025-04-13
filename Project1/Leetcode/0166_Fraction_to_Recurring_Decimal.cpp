#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<unordered_map>

namespace _0166_Fraction_to_Recurring_Decimal {

	using namespace std;

	// accepted 0ms beats 100%
	class Solution1
	{
	public:

		/*
		* 32-bit signed integer range: -2,147,483,648 -- 2,147,483,647
		* 
		* If I assign 2,147,483,648 to a 32-bit signed integer, it will wrap around and the variable actually becomes -2,147,483,648
		* 
		* "long" and "int" are both 32-bit !!!  Only "long long" is 64-bit
		* 
		* corner cases:
		*     -1 / -2147483648
		*         If I want to convert these two numbers to positive numbers, I need 64-bit signed integers
		*         I convert both to positive, the division becomes 1 / 2,147,483,648
		*         of course, the first many digits are all zero in the result, when the divider can finally divide 2,147,483,648, the divider would be:
		*         10,000,000,000   --- this is 10 billion, maximum of unsigned 32-bit integer is about 4 billion (roughly double of 2,147,483,648). So I need 64-bit.
		*          2,147,483,648
		* 
		*     -2147483648 / 1
		*         after converting to positive, the division result is 2147483648, which means "quo" is that.
		*         and this is beyond signed 32-bit integer's range, so "quo" can't just be "int"
		*/

		string fractionToDecimal(int numerator, int denominator)
		{
			if (numerator == 0)
			{
				return "0";
			}

			bool isNegative = ((numerator < 0 && denominator > 0) || (numerator > 0 && denominator < 0));

			auto n = static_cast<long long>(numerator);
			auto d = static_cast<long long>(denominator);

			n = n < 0 ? (-1LL * n) : n;
			d = d < 0 ? (-1LL * d) : d;

			auto quo = n / d;   
			auto rem = n % d;

			stringstream ss{};

			ss << quo;

			if (rem != 0)
			{
				ss << '.';
			}
			else
			{
				return isNegative ? ( "-" + ss.str()) : ss.str();
			}

			unordered_map<decltype(rem), unsigned int> occurrence;

			// guaranteed the result string's length is within 10^4
			// so the occurring position or possible remains have at most 10^4 different possibilities
			// so it's OK the value type in the unordered_map is "int"
			// but since "unsigned int" is as wide as "int", and positions can't be negative, 
			// so I'm using "unsigned int" as the value type
			
			unsigned int i = 1;
			unsigned int repeatStart = 0;

			while (rem != 0)
			{
				// when starting to calculate the decimal part, it is already carrying a remain.
				// for example, if the calculation carries a remain of 5,
				// the quotient corresponding to 5 is at the position of 1 (first number after the dot).
				if (occurrence.contains(rem))
				{
					repeatStart = occurrence[rem];
					break;
				}
				else
				{
					occurrence[rem] = i;
				}

				n = rem * 10LL;
				quo = n / d;
				rem = n % d;
				ss << quo;
				if (rem == 0)
				{
					break;
				}
				++i;
			}

			if (repeatStart != 0)
			{
				string temp = ss.str();
				unsigned int k = 0;

				while (temp[k] != '.')
				{
					++k;
				}

				// if 'k' is 3, dot's index is 3, dot is the 4th character.
				// such as 105.8269..
				// if "repeatStart" is 4, that means repetition starts at '9' in the above example.
				// there are 3 decimals not in the repeating section.

			
				string temp2 = temp.substr(0, (k+1) + (repeatStart-1));

				string temp3 = temp.substr((k+1) + (repeatStart-1));
				return isNegative ? ( "-" +  temp2 + "(" + temp3 + ")") : (temp2 + "(" + temp3 + ")");
			}
			else
			{
				return isNegative ? ("-" + ss.str()) : ss.str();
			}
		}
	};

	void Test_0166_Franction_to_Recurring_Decimal()
	{
		int option;
		int numerator;
		int denominator;
		Solution1 solu;
		while (true)
		{
			cout << "1: test;  0: exit;  >  ";
			cin >> option;

			if (option == 1)
			{

				cout << "numerator: ";
				cin >> numerator;

				cout << "denominator: ";
				cin >> denominator;

				string answer = solu.fractionToDecimal(numerator, denominator);
				cout << "answer: " << answer << "\n\n";
			}
			else
			{
				return;
			}
		}
	}
}