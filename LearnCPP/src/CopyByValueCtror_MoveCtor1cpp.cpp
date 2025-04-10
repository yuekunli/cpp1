#include<iostream>

namespace CopyByValueCtor_MoveCtor1 {

class A
{
public:
	A()
	{
		std::cout << "default" << std::endl;
	}


	//A(A a){	}  // compiler won't even allow a constructor that takes a copy of the object.


	A(A& a)
	{
		std::cout << "1" << std::endl;
	}

	A(A&& a)
	{
		std::cout << "2" << std::endl;
	}
};



void f1(A a)
{
	std::cout << "3" << std::endl;
}

#if 0
void f1(A&& a)
{
	std::cout << "4" << std::endl;
}
#endif


void Test_CopyByValueCtor_MoveCtor1()
{

	A obj1;

	std::cout << "---------" << std::endl;

	f1(obj1);    

	std::cout << "---------" << std::endl;

	f1(std::move(obj1));

}


}