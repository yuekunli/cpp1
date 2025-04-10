
#include "VectorSeparateDeclareDefine1.h"
#include "VectorSeparateDeclareDefine1.cpp"


#include "NonTemplClassSeparateDeclDef1.h"

#include<iostream>

// Vector1.cpp  --   full implementation of a template class
// Vector1.h    --   Empty

// VectorSeparateDeclareDefine1.cpp   --  The implementation of a template class
// VectorSeparateDeclareDefine1.h     --  The declaration of a template class

// It proves that for a template class, even if separating declaration and implementation, a different file still can't
// use this class by only including its declaration. The user of this class must include its implementation.

// NonTemplClassSeparateDeclDef1.cpp   --   The implementation of a non-template class
// NonTemplClassSeparateDeclDef1.h     --   The declaration of a non-template class

// It proves that for a non-template class, a different file can use it even if just including the declaration.

// This file doesn't have a corresponding header file, main.cpp doesn't call the test function defined in this file.
// This test function is only used to verify compilation




namespace ClassDeclare1 {

	void Test_ClassDeclare1()
	{
		VectorSeparateDeclareDefine1::Vector<int> v { 6 };

		std::cout << v.size() << std::endl;


		NonTemplClassSeparateDeclDef1::A obj { 9 };

		std::cout << obj.size() << std::endl;
	}
}