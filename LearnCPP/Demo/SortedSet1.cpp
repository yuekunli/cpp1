#include<set>
#include<functional>
#include<iostream>
#include<chrono>

namespace SortedSet1 {



// ================
// test emplace
// ================


class Dew
{
private:
	int a;
	int b;
	int c;

public:
	Dew(int _a, int _b, int _c)
		:a(_a), b(_b), c(_c)
	{

	}

	bool operator<(const Dew& other) const
	{
		if (a < other.a)
			return true;
		if (a == other.a && b < other.b)
			return true;
		return (a == other.a && b == other.b && c < other.c);
	}
};


const int nof_operations = 120;

size_t set_emplace()
{
	std::set<Dew>set;
	for (int i = 0; i < nof_operations; i++)
	{
		for (int j = 0; j < nof_operations; j++)
		{
			for (int k = 0; k < nof_operations; k++)
			{
				set.emplace(i, j, k);
			}
		}
	}
	return set.size();
}


size_t set_insert()
{
	std::set<Dew>set;
	for (int i = 0; i < nof_operations; i++)
	{
		for (int j = 0; j < nof_operations; j++)
		{
			for (int k = 0; k < nof_operations; k++)
			{
				set.insert(Dew(i, j, k));
			}
		}
	}
	return set.size();
}

void timeit(std::function<size_t()> set_test, std::string what = "")
{
	auto start = std::chrono::system_clock::now();
	size_t setsize = set_test();
	auto stop = std::chrono::system_clock::now();
	std::chrono::duration<double, std::milli> time = stop - start;

	std::cout << std::fixed << std::setprecision(2) << time.count() << "  ms for " << what << "  ---   " << setsize  << '\n';

}


void Test1()
{
	timeit(set_insert, "insert");
	timeit(set_emplace, "emplace");
	timeit(set_insert, "insert");
	timeit(set_emplace, "emplace");
}


// =========================
// test emplace_hint
// =========================


const int count = 100500;

size_t set_emplace2()
{
	std::set<int>set;
	for (int i = 0; i < count; i++)
	{
		set.emplace(i);
	}
	return set.size();
}

size_t set_emplace_hint()
{
	std::set<int> set;

	auto it = set.begin();

	for (int i = 0; i < count; i++)
	{
		set.emplace_hint(it, i);
		it = set.end();
	}
	return set.size();
}

size_t set_emplace_hint_wrong()
{
	std::set<int> set;

	auto it = set.begin();
	for (int i = count; i > 0; i--)
	{
		set.emplace_hint(it, i);
		it = set.end();
	}
	return set.size();
}


size_t set_emplace_hint_corrected()
{
	std::set<int> set;
	auto it = set.begin();
	for (int i = count; i > 0; i--)
	{
		set.emplace_hint(it, i);
		it = set.begin();
	}
	return set.size();
}


size_t set_emplace_hint_closest()
{
	std::set<int> set;
	auto it = set.begin();
	for (int i = 0; i < count; i++)
	{
		it = set.emplace_hint(it, i);
	}

	// this runs fast in fact. but documentation (en.cppreference.com) says "amortized contant if the
	// new element is inserted *just before* the hint". In this test, the new element is inserted just after the hint.
	// 
	// Perhaps, "set" compares the to-be-inserted element with the hint,
	// if the to-be-inserted is greater, we go to the hint's successor, if the to-be-inserted is still greater,
	// we keep going to the successor until we find the position to insert.
	// 
	// If the to-be-inserted is less than the hint, we go to hint's predecessor and repeat until we find proper position.
	// "hint" basically serves as the start point for searching.
	//
	// However, documentation also says "logarithmic in the size of the container in general". If it searches from the hint
	// one-by-one, doesn't it make it O(n) in general?


	return set.size();
}


void Test2()
{
	timeit(set_emplace2, "no hint");
	timeit(set_emplace_hint, "emplace with correct hint");
	timeit(set_emplace_hint_wrong, "emplace with wrong hint");
	timeit(set_emplace_hint_corrected, "corrected emplace");
	timeit(set_emplace_hint_closest, "emplace using returned iterator");
}



// ===================================
// my example to test emplace_hint
// (not timed test, just simple functionality)
// ===================================

struct Alpha
{
public:
	int a;
	int b;
	Alpha(int&& a1, int&& b1)
		:a(0), b(0)
	{
		a = a1;
		b = b1;
	}

	bool operator<(const Alpha& obj2) const  // Attention!!! There must be two "const"s, 
		                                     // because when the "set" compares two Alpha objects, 
		                                     // it will look for an implementation of operator< that takes const reference 
		                                     // and doesn't modify passed in object
	{
		return a < obj2.a;
	}
};

void Test3()
{
	std::set<Alpha> alphaSet;

	int step = 10;
	for (int i = 1; i <= 5; i++)
	{
		alphaSet.emplace((i*step), 1);
	}

	std::cout << "initial set:" << std::endl;

	//for (auto it = alphaSet.begin(); it != alphaSet.end(); it++)  // this is also correct
	//{
	//	std::cout << it->a << " , " << it->b << std::endl;
	//}

	for (const Alpha& obj : alphaSet)
	{
		std::cout << obj.a << " , " << obj.b << std::endl;
	}


	auto it2 = alphaSet.find(Alpha{ 40,1 });

	std::cout << "insert 70,2" << std::endl;
	auto it3 = alphaSet.emplace_hint(it2, 70, 2);

	//std::cout << "insert 35,2" << std::endl;
	//auto it3 = alphaSet.emplace_hint(it2, 35, 2);


	std::cout << "result:  " << it3->a << " , " << it3->b << std::endl;


	std::cout << "final set:" << std::endl;
	for (auto it = alphaSet.begin(); it != alphaSet.end(); it++)
	{
		std::cout << it->a << " , " << it->b << std::endl;
	}
}


void Test_SortedSet1()
{
	//Test1();
	//Test2();
	Test3();

	std::cout << std::endl;
}

}