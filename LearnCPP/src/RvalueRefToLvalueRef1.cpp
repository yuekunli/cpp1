#include<iostream>

namespace RvalueRefToLvalueRef1 {


void f(int& t)
{
	std::cout << t << std::endl;
	t = 30;
	std::cout << t << std::endl;
}

void Test1()
{
	int&& x = 20;
	std::cout << x << std::endl;
	x = 24;
	std::cout << x << std::endl;   // this int variable has a name (albeit an alias) and can be modified, doesn't that just make it a lvalue?
	f(x);
	std::cout << x << std::endl;
}


void Test_RvalueRefToLvalueRef1()
{
	Test1();
}


}