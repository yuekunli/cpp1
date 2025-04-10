#include<iostream>
#include<memory>

namespace ResetUniquePointer1{
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


void f3(std::unique_ptr<A>& refuniptrA)
{
	std::cout << "entering f3" << std::endl;
	A a2 = A{};
	a2.age = 30;
	a2.grade = 40;
	//refuniptrA.reset(&a2);
	std::cout << "exiting f3" << std::endl;
}


void Test_ResetUniquePointer1()
{
	std::unique_ptr<A> pA = std::make_unique<A>();
	pA->age = 10;
	pA->grade = 20;
	pA->show();
	f3(pA);
	//pA->show();
}

}