#include<iostream>


namespace NamespaceScope1 {



namespace N1 {

int a = 10;

namespace N2 {

int a = 20;

namespace N3 {

void f()
{
	std::cout << a << std::endl;
	std::cout << N1::a << std::endl;
	//std::cout << ::a << std::endl;  // global scope doesn't have definition for "a", and compiler won't look for it in sub-spaces
}

}

}

}


void TestNamespaceScope1()
{
	N1::N2::N3::f();
}





}