#include<iostream>

namespace DefaultCtorCurlyBracesInitializer1{

struct A
{
	int age;
	int grade;
#if false
	A(int x, int y) : age(x), grade(y)
	{

	}
	A() :age(0), grade(0)
	{

	}
#endif
	void show()
	{
		std::cout << age << "    " << grade << std::endl;
	}

	~A()
	{
		std::cout << "destruct A" << std::endl;
	}
};


void Test_DefaultCtorCurlyBracesInitializer1()
{
	A a = {};

	a.age = 10;
	a.grade = 20;
}

}