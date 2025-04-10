#include<iostream>

namespace NewArrayAndPointer1 {


void f1(/*int a[][]*/ int a[][2])  // int a[][]  doesn't work, only the 1st dimension can be left out
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			std::cout << a[i][j] << "  ";
		}
	}
	std::cout << '\n';
}

void f2(int a[])
{
	for (int i = 0; i < 2; i++)
	{
		std::cout << a[i] << "  ";
	}
	std::cout << '\n';
}

void f3(int* a)
{
	for (int i = 0; i < 2; i++)
	{
		std::cout << a[i] << "  ";
	}
	std::cout << '\n';
}

void f4(int** a)
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			std::cout << a[i][j] << "  ";
		}
	}
	std::cout << '\n';
}

void f5(int *a[])
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			std::cout << a[i][j] << "  ";
		}
	}
	std::cout << '\n';
}

void f6(int* a[2])
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			std::cout << a[i][j] << "  ";
		}
	}
	std::cout << '\n';
}

void f7(int(*a)[2])
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			std::cout << a[i][j] << "  ";
		}
	}
	std::cout << '\n';
}

void f8(int(*a)[3])
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			std::cout << a[i][j] << "  ";
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

void Test_NewArrayAndPointer1()
{
	int arr1[2][2] = { {3,4},{5,6} };
	//int arr2[][] = { {3,4},{5,6} };  // this doesn't work, only the 1st dimension can be left out.
	int arr2[][2] = { {3,4},{5,6} };
	f1(arr1);
	f1(arr2);
	//int arr7[][2]; // unknown size
	//int arr3[][][2] = { {{2,3},{4,5}},{{6,7},{8,9}} };  // this doesn't work, only the 1st dimension can be left out.
	int arr3[][2][2] = { {{2,3},{4,5}},{{6,7},{8,9}} };
	
	//int* p1 = new int[2](31);   // array can not use parenthesis initializer
	int* p1 = new int[2] { 31,32 };
	f2(p1);
	std::cout << sizeof(p1)<<'\n';
	int arr5[2] { 24, 28 };
	std::cout << sizeof(arr5) << '\n';

	//int arr4[] = new int[2] { 13,15 };  // cannot convert from int* to int[]
	//f2(arr4);

	int arr6[2] { 90, 92 };
	f3(arr6);

	//int** p2 = new int[2][2] { {35, 36},{37, 38} };  // cannot convert from int(*)[2] to int**

	auto p2 = new int[2][2] { {35, 36},{37, 38} }; 
	auto p4 = new int[][2] { {35, 36},{37, 38} };
	//auto p3 = new int[][2];  // array size must be specified,  
	// with initialization, compiler can deduce the 1st dimension, this applies to array declaration and new expression

	int(*p5)[2] = new int[2][2];  // this is how to replace that "auto"



	//f4(p2);  // cannot convert from int(*)[2] to int**
	//f5(p2);  // cannot convert from int(*)[2] to int*[]
	//f6(p2);  // cannot convert from int(*)[2] to int*[]
	f7(p2);
	f7(p5);

	delete[] p2;
	delete[] p4;
	delete[] p5;


	int a1 = 15, a2 = 16, a3 = 17, a4 = 18;
	auto p6 = new int[2][3] { {1,2,3},{4,5,6} };
	f8(p6);

	int(*p7)[3] = new int[2][3];
	// (*p7)[3]  this feels like 3 pointers, but it's actually unknown number of pointers, each of which points to an array of 3 elements. 
	// the "unknown number" is decided by the following new expression
	p7[0][0] = a1;
	p7[0][1] = a2;
	p7[0][2] = a3;
	p7[1][0] = a4;
	p7[1][1] = a1;
	p7[1][2] = a2;
	f8(p7);
	//p7[0] = &a1;  // this doesn't work
	//p7[1] = &a2;
	//std::cout << *p7[0] << "    " << *p7[1] << '\n';

	auto p8 = new int[2][3][4];
	int(*p9)[3][4] = new int[2][3][4];
	//f8(p8);


	delete[] p6;
	delete[] p7;
	delete[] p8;

}



}