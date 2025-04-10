#include<iostream>

namespace RefToArrayAsFuncInput1 {

void f2(int(&t)[2])
{
	t[0] = 100;
	t[1] = 200;
}


void Test_RefToArrayAsFuncInput1()
{

	int a[2] = { 50, 60 };

	std::cout << a[0] << "    " << a[1] << std::endl;

	f2(a);

	std::cout << a[0] << "    " << a[1] << std::endl;
}

}