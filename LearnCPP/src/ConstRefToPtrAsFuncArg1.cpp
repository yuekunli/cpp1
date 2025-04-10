#include<iostream>
#include<memory>

namespace ConstRefToPtrAsFuncArg1{

void f1(int& /*const*/ a)  // the const in front of "a" is redundant, it means "a" will always refer to the same variable
{
	a = 30;
}

void f2(const int& a) // if 'a' refers x, we pledge not to change x's value via a
{
	//a = 30;  // compile error
}

void f3(int* const b)   // this means if b points to x, b will always point to x, but we can use b to change x's value
{
	*b = 190;
}

void f4(const int* b)   // if b points to x, we pledge not to change x's value via b
{
	//*b = 190;  // compile error
}


struct A
{
	A()  // this is default constructor
	{
		std::cout << "init a struct A" << std::endl;
		data = 100;
	}

	void change_value()
	{
		data = 200;
	}

	void show_value()
	{
		std::cout << data << std::endl;
	}

	int data;
};


struct B
{
	B(A& inputA) : objA(inputA)   // this member initializer list is necessary
	{
		std::cout << "init a struct B" << std::endl;
		//objA = inputA;   // this won't work, it must use member initializer list
	}

	A& objA;
};


struct C
{
	C()
	{
		std::cout << "init a struct C" << std::endl;
		//objA = A();  // we can initialize objA here, don't need to put in in member initializer list
		               // we can even leave objA un-initialized in the constructor
		               // fundamental reason is that objA is a concrete object, instead of a reference, A's default constructor can be used here.
	}

	A objA;
};

void f5(const std::shared_ptr<A>& psA)
{
	psA->show_value();  
	psA->change_value();  // the memory storing the object of A is not part of the memory storing shared_ptr object
	psA->show_value();
}


void f6(const B& rfB)
{
	rfB.objA.change_value();
	rfB.objA.show_value();
}

void f7( /*const*/ C& rfC)  // adding that const in front of C& causes compile error
{
	rfC.objA.change_value();
	rfC.objA.show_value();
}


void Test_ConstRefToPtrAsFuncArg1()
{
	int y = 40;
	std::cout << y << std::endl;
	f1(y);
	//f3(&y);
	std::cout << y << std::endl;

	std::cout << "test shared_ptr" << std::endl;
	std::shared_ptr<A> psA = std::make_shared<A>();
	f5(psA);

	std::cout << "--------1---------" << std::endl;
	A a1 = A();

	B b(a1);
	f6(b);

	std::cout << "--------2---------" << std::endl;
	C c = C();
	c.objA = a1;  // we can assign objA here even if C's constructor doesn't initialize objA
	f7(c);

}

}