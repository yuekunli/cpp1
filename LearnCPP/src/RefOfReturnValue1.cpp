#include<iostream>

namespace RefOfReturnValue1 {


int f(int a)
{
	static int b = 10;

	b += a;

	return b;
}


int globalv = 80;

int& bar(int a)
{
	//return (10 + a);  // this won't compile, it's the same error as int& r = f(3)
	                  // initial value of reference to non-const must be an lvalue
	                  // basically if I want to return "the reference to something" from this function, 
	                  // that "something" must last longer than this function
	                  // that "something" can be a global variable or a static variable defined inside this function

	globalv += a;

	return globalv;


}


// assign the return value to a reference

void Test_1()
{

	//int & r = f(3);  // this won't compile
	                // because the return value of a function is a rvalue.
	                // the initial value of a reference should be an lvalue
	                // formal error message: initial value of reference to non-const must be an lvalue
	                // follow-up question: what is a reference to a const?

	int const& r = f(3);  // This is the answer to the follow-up question, this can compile

	std::cout << r << std::endl;

	//r = 20;

	int x = f(6);

	std::cout << x << std::endl;

}


// the return value *is* a reference

void Test_2()
{
	int x = bar(5);

	std::cout << x << std::endl;

	x = 40;  // this doesn't change globalv

	std::cout << x << std::endl;
	std::cout << globalv << std::endl;

	int& t = bar(2);

	std::cout << t << std::endl;

	t = 40;  // this changes globalv

	std::cout << t << std::endl;
	std::cout << globalv << std::endl;

	// generally speaking, it's impossible to take the address of a reference
	// but I can take the address of the "thing" whose reference is returned by function bar.
	// it seems as if I'm taking the address of a reference, but I'm acutally taking the address of that "thing" whose reference is returned by bar

	int* p = &bar(7);
	std::cout << *p << std::endl;  // note that the last experiment changed globalv to 40
	std::cout << globalv << std::endl;

	*p = 50;
	std::cout << *p << std::endl;
	std::cout << globalv << std::endl;


}



void Test_RefOfReturnValue1()
{
	//Test_1();

	Test_2();
}




}