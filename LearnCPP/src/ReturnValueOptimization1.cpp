#include<utility>
#include<iostream>

namespace ReturnValueOptimization1 {



struct A
{
	A(int n) :
		_v(n)
	{
		std::cout << "0" << std::endl;
	}

	A(A& input) :
		_v(input._v)
	{
		std::cout << "1" << std::endl;
	}

	A(A&& input) :
		_v(input._v)
	{
		std::cout << "2" << std::endl;
	}

	int _v;

};


A foo()
{
	A a { 10 };
	return a;
}

A&& bar()
{
	A a { 20 };
	return std::move(a);
}


void Test_ReturnValueOptimization1()
{


	A a1 = foo();

	A a2 = bar();

	// both these invokes A's move constructor even though function foo doesn't specify that it returns an rvalue reference.


}



}