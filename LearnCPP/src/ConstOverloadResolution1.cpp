#include<iostream>

namespace ConstOverloadResolution1 {


class A
{
public:
	A(int input) :
		val(input)
	{

	}

	int Get()
	{
		std::cout << "non-const" << std::endl;
		return val;
	}


	//int& Get()     // this doesn't compile: cannot overload functions distinguished by return type only
	//{
	//	return val;
	//}


	int Get() const
	{
		std::cout << "const" << std::endl;
		return val;
	}


#if 0
	// a const function promises not to modify any data member inside this function
	// what if I don't modify any data member but return reference to some data member
	// so that the caller of this function can modify the data member via the reference?
	int& Get2() const
	{
		std::cout << "Get2" << std::endl;
		return val;    // cannot convert from 'const int' to 'int&'
		               // we say the function is const, so the return value is automatically regarded as a const
	}
#endif


private:
	int val;
};




void Test_ConstOverloadResolution1()
{
	A a { 10 };

	std::cout << a.Get() << std::endl;

	const A a2 { 20 };

	std::cout << a2.Get() << std::endl;

#if 0
	a.Get2() = 30;
	std::cout << a.Get() << std::endl;
#endif
}







}