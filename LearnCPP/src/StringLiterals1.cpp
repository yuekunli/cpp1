#include<iostream>
#include<string>

namespace StringLiterals1 {




void Test_StringLiterals1()
{
	using namespace std::string_literals;

	std::string s1 = "abc\0\0def";
	std::string s2 = "abc\0\0def"s;

	std::string s3 = "abc\4\mdef"s;

	std::string s4 = "abc⌘♜def";

	std::cout <<s1.size()<<"  "<< "s1:  " << s1 << std::endl;
	std::cout <<s2.size()<<"  "<< "s2:  " << s2 << std::endl;
	std::cout <<s3.size()<<"  "<< "s3:  " << s3 << std::endl;
	std::cout << s4.size() << "  " << "s4:  " << s4 << std::endl;

	for (char i : s1)
	{
		std::cout << (unsigned short) i << "  ";
	}
	std::cout << std::endl;

	for (char i : s2)
	{
		std::cout << (unsigned short) i << "  ";
	}
	std::cout << std::endl;

	for (char i : s3)
	{
		std::cout << (unsigned short) i << "  ";
	}
	std::cout << std::endl;

	for (char i : s4)
	{
		std::cout << (unsigned short) i << "  ";
	}
	std::cout << std::endl;


	char u1 = '\u004d';

	std::cout << u1 << std::endl;


}
}