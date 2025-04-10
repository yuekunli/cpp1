#include "../LibHeaders.h"

namespace BuiltInAlgo1 {

/*=============================================================*/
/*          Non-modifying sequence operations                  */
/*=============================================================*/

void Test_allof_noneof_anyof()
{
	std::vector<int> v(10, 2);
	std::partial_sum(v.cbegin(), v.cend(), v.begin());
	std::cout << "Among the number: ";
	std::copy(v.cbegin(), v.cend(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl;

	if (std::all_of(v.cbegin(), v.cend(), [](int i)
	{
		return i % 2 == 0;
	}))
	{
		std::cout << "All numbers are even\n";
	}

	if (std::none_of(v.cbegin(), v.cend(), std::bind(std::modulus<int>(), std::placeholders::_1, 2)))
	{
		std::cout << "None of them are odd\n";
	}

	struct DivisibleBy
	{
		const int d;
		DivisibleBy(int n) :
			d(n)
		{

		}
		bool operator()(int n)const
		{
			return n % d == 0;
		}
	};
	if (std::any_of(v.cbegin(), v.cend(), DivisibleBy(7)))
	{
		std::cout << "At least one number is divisible by 7\n";
	}
}




struct Sum1
{
	void operator()(int n)
	{
		sum += n;
	}
	int sum { 0 };
};

void Test_for_each()
{
	std::vector<int> nums { 3,4,2,8,15,267 };

	//auto print = [](const int& n)   // this works too
	std::function<void(const int& n)>print = [](const int&n)  // print is a function object
	{
		std::cout << " " << n;
	};

	std::cout << "before:";
	std::for_each(nums.cbegin(), nums.cend(), print);
	std::cout << "\n";

	std::for_each(nums.begin(), nums.end(), [](int& n)
	{
		n++;
	});

	Sum1 s = std::for_each(nums.begin(), nums.end(), Sum1());

	std::cout << "after: ";
	std::for_each(nums.cbegin(), nums.cend(), print);
	std::cout << '\n';
	std::cout << "sum: " << s.sum << '\n';
}


void Test_for_each_n()
{
	std::vector<int> ns { 1,2,3,4,5 };

	for (auto n : ns)
	{
		std::cout << n << ", ";
	}

	std::cout << std::endl;

	std::for_each_n(ns.begin(), 3, [](auto& n)
	{
		n *= 2;
	});

	for (int& n : ns)
	{
		std::cout << n++ << ", ";
	}
	std::cout << std::endl;

	for (int& n : ns)
	{
		std::cout << n << ", ";
	}
	std::cout << std::endl;


	// first 3 numbers time 2 again, take the return value which should be the iterator that points at the 4th number.

	typename std::vector<int>::iterator it = std::for_each_n(ns.begin(), 3, [](auto& n)
	{
		n *= 2;
	});


	// last 2 numbers time 10

	std::for_each(it, ns.end(), [](int& n)    // here "it" is an iterator (not a const_iterator), 2nd arg must not be ns.cend(), 
		                                      // because that results in passing in (interator, const_iterator, function_object)
		                                      // there is no overload of for_each matching this sequence of arguments.
	{
		n *= 10;
	});


	// re-examine the vector

	for (int n : ns)
	{
		std::cout << n << ", ";
	}
	std::cout << std::endl;

}




void Test_count_if()
{
	std::vector<int> v { 1,2,3,4,4,3,7,8,9,10 };

	int target1 = 3;
	int target2 = 5;

	auto num_items1 = std::count(v.begin(), v.end(), target1);

	typename std::iterator_traits<typename std::vector<int>::iterator>::difference_type num_items2 = std::count(v.begin(), v.end(), target2);
	// see this line how to expand that "auto"


	std::cout << "number: " << target1 << " count: " << num_items1 << std::endl;
	std::cout << "number: " << target2 << " count: " << num_items2 << std::endl;

	auto num_items3 = std::count_if(v.begin(), v.end(), [](int i)
	{
		return i % 3 == 0;
	});

	std::cout << "number divisible by 3: " << num_items3 << std::endl;
}


std::string mirror_ends(const std::string& in)
{
	return std::string(in.begin(), std::mismatch(in.begin(), in.end(), in.rbegin()).first);
}

std::string mirror_ends2(std::string in)
{
	std::pair<typename std::string::iterator, typename std::string::reverse_iterator> it_pair = std::mismatch(in.begin(), in.end(), in.rbegin());
	std::string ret = { in.begin(), it_pair.first };

	return ret;
}

std::string mirror_ends3(std::string in)
{
	using iterator = typename std::string::iterator;
	using r_iterator = typename std::string::reverse_iterator;
	std::pair<iterator,r_iterator> it_pair = std::mismatch(in.begin(), in.end(), in.rbegin());
	std::string ret = { in.begin(), it_pair.first };

	return ret;
}



void Test_mismatch()
{
	std::cout << mirror_ends("abXYZba") << std::endl
		<< mirror_ends2("abca") << std::endl
		<< mirror_ends3("aba") << std::endl;
}


void Test_find_findif_findifnot()
{
	int n1 = 3;
	int n2 = 5;

	std::vector<int> v { 0,1,2,3,4 };

	auto result1 = std::find(std::begin(v), std::end(v), n1);
	typename std::vector<int>::iterator result2 = std::find(v.begin(), v.end(), n2);
	auto result3 = std::find_if(std::begin(v), std::end(v), [](int i)
	{
		return (i > 0) && (i % 2 == 0);
	});

	if (result1 != std::end(v))
	{
		std::cout << "v contains: " << n1 << std::endl;
	}
	else
	{
		std::cout << "v does not contain: " << n1 << std::endl;
	}


	if (result2 != v.end())
	{
		std::cout << "v contains " << n2 << std::endl;
	}
	else
	{
		std::cout << "v does not contain: " << n2 << std::endl;
	}

	if (result3 != v.end())
	{
		std::cout << "v contains a positive even number: " << *result3 << std::endl;
	}
	else
	{
		std::cout << "v does not contain positive even numbers" << std::endl;
	}

}


void Test_find_end()
{
	std::vector<int> v { 1,2,3,4,1,2,3,4,1,2,3,4 };
	typename std::vector<int>::iterator result;  // omitting "typename" also works

	std::vector<int> t1 { 1,2,3 };

	result = std::find_end(v.begin(), v.end(), t1.begin(), t1.end());

	if (result == v.end())
	{
		std::cout << "sequence not found\n";
	}
	else
	{
		std::cout << "last occurrence is at: " << std::distance(v.begin(), result) << "\n";
	}

	std::vector<int> t2 { 4,5,6 };

	result = std::find_end(v.begin(), v.end(), t2.begin(), t2.end());
	if (result == v.end())
	{
		std::cout << "sequence not found\n";
	}
	else
	{
		std::cout << "last occurrence is at: "
			<< std::distance(v.begin(), result) << "\n";
	}
}


static auto print_sequence1 = [](auto const id, auto const& seq)
{
	std::cout << id << "{ ";
	
	//for (int i {}; auto const& e : seq)  // this needs C++20, this part "auto const& e : seq" is actually OK
	//{
	//	std::cout << (i++ ? ", " : "") << e;
	//}

	int i = 0;
	for (auto const& e : seq)
	{
		std::cout << (i++ ? ", ":"") << e;
	}

	std::cout << " }\n";
};

void Test_find_first_of()
{
	const std::vector<int> v { 0,2,3,25,5 };
	const auto t1 = { 3, 19,10,2 };
	const auto t2 = { 1,6,7,9 };

	auto find_any_of = [](const auto& v, const auto& t)
	{
		const auto result = std::find_first_of(v.begin(), v.end(), t.begin(), t.end());
		if (result == v.end())
		{
			std::cout << "No elements of v are equal to any element of ";
			print_sequence1("t = ", t);
		}
		else
		{
			std::cout << "Found a match (" << *result << ") at position " << std::distance(v.begin(), result);
			print_sequence1(", where t = ", t);
		}
	};

	print_sequence1("v = ", v);
	find_any_of(v, t1);
	find_any_of(v, t2);
}



void Test_adjacent_find()
{
	std::vector<int> v1 { 0, 1, 2, 3, 40, 40, 41, 41, 5 };

	auto i1 = std::adjacent_find(v1.begin(), v1.end());

	if (i1 == v1.end())
	{
		std::cout << "No matching adjacent elements" << std::endl;
	}
	else
	{
		std::cout << "The first adjacent pair of equal elements is at " << std::distance(v1.begin(), i1) << ", *i1 = " << *i1 << std::endl;
	}

	auto i2 = std::adjacent_find(v1.begin(), v1.end(), std::greater<int>{});
	if (i2 == v1.end())
	{
		std::cout << "The entire vector is sorted in ascending order" << std::endl;
	}
	else
	{
		std::cout << "The last element in the non-decreasing subsequence is at " << std::distance(v1.begin(), i2) << ", *i2 = " << *i2 << std::endl;
	}
}



template<typename Container>
static bool contains1(const Container& c, const std::string& s)
{
	return std::search(c.begin(), c.end(), s.begin(), s.end()) != c.end();
}

void Test_search()
{
	std::string str = "why wast time learning, when ignorance is instantaneous?";
	std::cout << std::boolalpha << contains1(str, "learning") << '\n' << contains1(str, "lemming") << std::endl;

	std::vector<char> vec(str.begin(), str.end());
	std::cout << contains1(vec, "learning") << std::endl << contains1(vec, "leaning") << std::endl;

	constexpr std::string_view haystack =
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed "
		"do eiusmod tempor incididunt ut labore et dolore magna alique";
	const std::string needle { "pisci" };

	if (/*auto*/ typename std::string_view::iterator it = std::search(haystack.begin(), haystack.end(), std::boyer_moore_searcher(needle.begin(), needle.end()));
		it != haystack.end())
	{
		std::cout << "The string " << quoted(needle, '\'') << " found at offset " << it - haystack.begin() << std::endl;
	}
	else
	{
		std::cout << "The string " << std::quoted(needle) << " not found" << std::endl;
	}
}


template<class Container, class Size, class T>
static bool consecutive_values1(const Container& c, Size count, const T& v)
{
	return std::search_n(std::begin(c), std::end(c), count, v) != std::end(c);
}

void Test_search_n()
{
	const char seq[] = "1001010100010101001010101";

	std::cout << std::boolalpha;
	std::cout << "Has 4 consecutive zeros: " << consecutive_values1(seq, 4, '0') << std::endl;
	std::cout << "Has 3 consecutive zeros: " << consecutive_values1(seq, 3, '0') << std::endl;
}



/*=============================================================*/
/*              Modifying sequence operations                  */
/*=============================================================*/

void Test_copy_copyif()
{
	std::vector<int> from_vector(10);
	std::iota(from_vector.begin(), from_vector.end(), 0);

	std::vector<int> to_vector;
	std::copy(from_vector.begin(), from_vector.end(), std::back_inserter(to_vector));

	std::vector<int> to_vector2;
	to_vector2.resize(from_vector.size());
	std::copy(from_vector.begin(), from_vector.end(), to_vector2.begin());

	std::vector<int> to_vector3 = from_vector;

	std::cout << "to vector contains: ";
	std::copy(to_vector.begin(), to_vector.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl;

	std::cout << "odd numbers in to_vector are: ";
	std::copy_if(to_vector2.begin(), to_vector2.end(), std::ostream_iterator<int>(std::cout, " "), [](int x)
	{
		return x % 2 != 0;
	});
	std::cout << std::endl;

	std::cout << "to vector contains these multiples of 3:";
	std::cout << std::endl;

	typedef std::back_insert_iterator<std::vector<int>> bii_type_shortcut;  // When I give "vector" to "back_insert_iterator", I must de-abstract "vector" by giving it "int".
	to_vector3.clear();
	std::copy_if<typename std::vector<int>::iterator, /*std::back_insert_iterator<std::vector<int>>*/bii_type_shortcut, bool(*)(int)>(from_vector.begin(), from_vector.end(), std::back_inserter(to_vector3),
		[](int x)
	{
		return x % 3 == 0;
	});
	for (int x : to_vector3)
	{
		std::cout << x << ' ';
	}
	std::cout << '\n';
}




void Test_copy_n()
{
	std::string in = "1234567890";
	std::string out;

	std::copy_n<typename std::string::iterator, int, std::back_insert_iterator<std::string>>(in.begin(), 4, std::back_inserter(out));
	std::cout << out << '\n';

	std::vector<int> v_in(128);
	std::iota(v_in.begin(), v_in.end(), 1);
	std::vector<int> v_out(v_in.size());

	std::copy_n(v_in.begin(), 100, v_out.begin());
	std::cout << std::accumulate(v_out.begin(), v_out.end(), 0) << '\n';
}


void Test_copy_backward()
{
	std::vector<int> from_vector;
	for (int i = 0; i < 10; i++)
	{
		from_vector.push_back(i);
	}
	
	std::vector<int> to_vector(15);
	std::copy_backward(from_vector.begin(), from_vector.end(), to_vector.end());

	std::cout << "to_vector contains: ";
	for (auto i : to_vector)
	{
		std::cout << i << ' ';
	}
}


void f1(int n)
{
	std::this_thread::sleep_for(std::chrono::seconds(10-2*n));  // thread 1 will sleep for the longest.
	std::cout << "thread" << n << " ended" << std::endl;        // this line for thread 3 will be printed at earliest time
}

void Test_move()
{
	std::vector<std::thread> v;
	v.emplace_back(f1, 1);
	v.emplace_back(f1, 2);
	v.emplace_back(f1, 3);
	std::list<std::thread> l;

	std::move(v.begin(), v.end(), std::back_inserter(l));
	int i = 1;
	for (auto& t : l)
	{
		t.join();    // this call to "join" is blocking, even though thread 3 ends first, 
		             // it will wait for thread 1's finish. When the main thread is ready 
		             // to join thread 2 and 3, there won't be wait time because they would've already finished
		std::cout << "join thread " << i << std::endl;
		i++;
	}
}



using container1 = std::vector<std::string>;

static void print3(std::string_view comment, const container1& src, const container1& dst = {})
{
	auto prn = [](std::string_view name, const container1& c)
	{
		std::cout << name;
		for (const auto& s : c)
		{
			std::cout << (s.empty() ? "-" : /*s.data()*/s) << ' ';
		}
		std::cout << '\n';
	};
	std::cout << comment << '\n';
	prn("src: ", src);
	if (dst.empty())
	{
		return;
	}
	prn("dst: ", dst);
}

void Test_move_n()
{
	container1 src { "foo", "bar", "baz" };
	container1 dst { "qux", "quux", "quuz", "corge" };
	print3("Non-overlaping case; before move_backward:", src, dst);

	std::move_backward(src.begin(), src.end(), dst.end());
	print3("After:", src, dst);

	src = { "snap", "crackle", "pop", "lock", "drop" };
	print3("Overlapping case; before move_backward:", src);
	std::move_backward(src.begin(), std::next(src.begin(), 3), src.end());
	print3("After:", src);
}


void Test_fill()
{
	std::vector<int> v { 0,1,2,3,4,5,6,7,8,9 };
	std::fill(v.begin(), v.end(), -1);
	for (auto elem : v)
	{
		std::cout << elem << ' ';
	}
	std::cout << std::endl;
}


void Test_fill_n()
{
	std::vector<int> v1 { 0,1,2,3,4,5,6,7,8,9 };

	std::fill_n(v1.begin(), 5, -1);

	std::copy(begin(v1), end(v1), std::ostream_iterator<int>(std::cout, " "));
	std::cout << '\n';
}


template<typename T>
struct plus5
{
	T operator()(const T& lhs, const T& rhs)
	{
		return lhs + rhs + 5;
	}
};


void Test_transform()
{
	std::string s("hello");

	std::transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c)->unsigned char
	{
		return std::toupper(c);
	});


	std::vector<std::size_t>ordinals;

	std::transform(s.begin(), s.end(), std::back_inserter(ordinals),
		[](unsigned char c)->std::size_t
	{
		return c;
	});


	std::cout << s << ':';

	for (std::size_t ord : ordinals)
	{
		std::cout << ' ' << ord;
	}

	std::transform(ordinals.cbegin(), ordinals.cend(),  // first range 
                   ordinals.cbegin(),                   // second range
                   ordinals.begin(),                    // destination range
                   std::plus<std::size_t> {}); // any of these works too: std::plus{},  std::plus<>{},  std::plus<void>{}
	std::cout << std::endl;

	for (auto ord : ordinals)
	{
		std::cout << ord << ' ';
	}

	std::cout << std::endl;


	std::transform(ordinals.cbegin(), ordinals.cbegin()+3,//ordinals.cend(),  // first range 
		ordinals.cbegin(),                   // second range
		ordinals.begin(),                    // destination range
		plus5<std::size_t> {});

	std::cout << std::endl;

	for (auto ord : ordinals)
	{
		std::cout << ord << ' ';
	}

	std::cout << std::endl;


}


static int func2()
{
	static int i;
	return ++i;
}

void Test_generate()
{
	std::vector<int> v(5);
	auto print = [&]  // capture everything by reference?
	{
		std::cout << "v: ";
		for (auto iv:v)
		{
			std::cout << iv << ' ';
		}
		std::cout << '\n';
	};

	std::generate(v.begin(), v.end(), func2);
	print();

	std::generate(v.begin(), v.end(), [n = 0]()mutable{return n++; });
	//std::cout << "v: " << v << std::endl;  // not every sequence type container can just pass its container object to ostream's operator<<, only std::string can do.
	print();
}

void Test_generate_n()
{
	std::mt19937 rng;
	//std::generate_n<std::ostream_iterator<std::mt19937::result_type>, int, std::mt19937&>(std::ostream_iterator<std::mt19937::result_type>(std::cout, " "), 5, std::ref(rng));
	std::generate_n<>(std::ostream_iterator<std::mt19937::result_type>(std::cout, " "), 5, std::ref(rng));
	std::cout << '\n';
}


void Test_remove()
{
	std::string str1 = "Text with some   spaces";
	
	auto noSpaceEnd = std::remove(str1.begin(), str1.end(), ' ');

	//std::cout << std::string_view(str1.begin(), noSpaceEnd) << " size: " << str1.size() << std::endl;
	             // this way of creating string_view is only in C++20

	std::cout << std::string_view(str1.data(), std::distance(str1.begin(), noSpaceEnd)) << " size: " << str1.size() << std::endl;


	str1.erase(noSpaceEnd, str1.end());

	std::cout << str1 << " size: " << str1.size() << std::endl;

	std::string str2 = "Text\n with\tsome \t  whitespaces\n\n";

	str2.erase(std::remove_if<typename std::string::iterator, int(*)(unsigned char)>(str2.begin(), str2.end(), [](unsigned char x)
	{                                                      // this must be int instead of bool
		return std::isspace(x);  // the return value of this API is int, it matters if I want to specifify template parameters i.e. the lambda's prototype
	}), str2.end());

	std::cout << str2 << std::endl;
}


void Test_remove_copy_if()
{
	std::string str = "#Return #Value #Optimization";
	std::cout << "before: " << std::quoted(str) << std::endl;

	std::cout << "after:  \"";
	std::remove_copy(str.begin(), str.end(), std::ostream_iterator<char>(std::cout), '#');
	std::cout << "\"\n";
}



void Test_replace_if()
{
	std::array<int, 10> s { 5,7,4,2,8,6,1,9,0,3 };

	std::replace(s.begin(), s.end(), 8, 88);

	for (int a : s)
	{
		std::cout << a << " ";
	}
	std::cout << std::endl;
	std::replace_if(s.begin(), s.end(), std::bind(std::less<int>{}, std::placeholders::_1, 5), 55);
	for (int a : s)
	{
		std::cout << a << ' ';
	}
	std::cout << std::endl;

	auto f1 = [](int x)
	{
		return x == 9;
	};

	std::replace_if(s.begin(), s.end(), f1, 99);

	auto p1 = [](std::array<int, 10>& a)
	{
		for (int x : a)
		{
			std::cout << x << ' ';
		}
		std::cout << std::endl;
	};

	p1(s);
}


void Test_replace_copy_if()
{
	std::vector<int> v { 5,7,4,2,8,6,1,9,0,3 };
	std::replace_copy_if(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "),
		[](int x)
	{
		return x > 5;
	}, 99);
	std::cout << std::endl;
}



class A1
{
	int id {};

	friend void swap(A1& lhs, A1& rhs)
	{
		std::cout << "swap(" << lhs << ", " << rhs << ")\n";
		std::swap(lhs.id, rhs.id);
	}

	friend std::ostream& operator<<(std::ostream& os, A1 const& a);  // I have to declare it here

public:
	A1(int i) : id(i)
	{
	}
	A1(A1 const&) = delete;
	A1& operator=(A1 const&) = delete;
};


std::ostream& operator<<(std::ostream& os, A1 const& a)
{
	return os << "A1::id=" << a.id;
}

void Test_swap()
{
	int a = 5, b = 3;
	std::cout << a << ' ' << b << std::endl;
	std::swap(a, b);
	std::cout << a << ' ' << b << std::endl;

	A1 p { 6 }, q { 9 };
	std::cout << p << ' ' << q << std::endl;
	swap(p, q); // in this namespace
	std::cout << p << ' ' << q << std::endl;
}



auto print5 = [](auto comment, auto const& seq)
{
	std::cout << comment;
	for (const auto& e : seq)
	{
		std::cout << e << ' ';
	}
	std::cout << std::endl;
};

void Test_swap_ranges()
{
	std::vector<char> v = { 'a', 'b', 'c', 'd', 'e' };
	std::list<char> l = { '1', '2', '3', '4', '5' };

	print5("Before swap_ranges:\n" "v: ", v);
	print5("l: ", l);

	std::swap_ranges(v.begin(), v.begin() + 3, l.begin());

	print5("After swap_ranges:\n" "v: ", v);
	print5("l: ", l);
}

template <class ForwardIt>
void selection_sort1(ForwardIt begin, ForwardIt end)
{
	for (ForwardIt i = begin; i != end; i++)
	{
		std::iter_swap(i, std::min_element(i, end));
	}
}

void Test_iter_swap()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(-9, 9);
	std::vector<int> v;
	std::generate_n(std::back_inserter(v), 20, std::bind(dist, std::ref(gen)));

	std::cout << "Before sort: " << std::showpos;
	for (auto e : v)
	{
		std::cout << e << ' ';
	}

	selection_sort1(v.begin(), v.end());
	std::cout << "\nAfter sort : ";
	for (auto e : v)
	{
		std::cout << e << ' ';
	}
	std::cout << '\n';
}


void Test_reverse()
{
	std::vector<int> v { 1,2,3 };
	std::reverse(v.begin(), v.end());
	for (auto e : v)
	{
		std::cout << e;
	}
	std::cout << std::endl;
	int a[] = { 4,5,6,7 };
	std::reverse(std::begin(a), std::end(a));
	for (auto e : a)
	{
		std::cout << e;
	}
	std::cout << std::endl;
}


void Test_reverse_copy()
{
	auto print = [](std::vector<int> const& v)
	{
		for (auto const& value : v)
		{
			std::cout << value << ' ';
		}
		std::cout << '\t';
	};

	std::vector<int> v { 1,2,3 };
	print(v);

	std::vector<int> destination(3);

	std::reverse_copy(std::begin(v), std::end(v), std::begin(destination));
	print(destination);

	std::reverse_copy(std::rbegin(v), std::rend(v), std::begin(destination));
	print(destination);
}


auto print6 = [](auto const& remark, auto const& v)
{
	std::cout << remark;
	for (int const & n : v)
	{
		std::cout << n << ' ';
	}
	std::cout << std::endl;
};

void Test_rotate()
{
	std::vector<int> v { 2,4,2,0,5,10,7,3,7,1 };

	print6("before sort:\t\t", v);

	/*  this works
	for (auto i = v.begin(); i != v.end(); i++)
	{
		std::rotate(std::upper_bound(v.begin(), i, *i), i, i + 1);
	}
	*/
    
	typedef std::reverse_iterator<std::vector<int>::iterator> rev_iter;
	
	for (auto i = v.begin() + 1; i != v.end(); i++)
	{
		std::rotate(rev_iter(i+1), rev_iter(i), rev_iter(std::upper_bound(v.begin(), i, *i)));
		
	}
	
	// this works
    if (false) {
	for (auto i = v.begin() + 1; i != v.end(); i++)
	{
		auto upperBound = std::upper_bound(v.begin(), i, *i);
		auto iToEnd = std::distance(i, v.end());
		auto upperToI = std::distance(upperBound, i);

		std::rotate(v.rbegin()+(iToEnd-1), v.rbegin() + (iToEnd), v.rbegin() + (iToEnd+upperToI));
		            /*        I        */  /*         e       */  /*  one hop to the left of U*/

		// rotate between I to U (counting backward), right rotate once is enough,
		// all I need to to move 'I' all the way to the left-most position
		// but everything has to be represented in the form of reverse iterator
		// so I have to add offsets to rbegin()

		//                 +-- upperBound
		//                 |           +-- i
		//                 |           |   +-- end
		//                 |           |   |
		// x x x x x x x x U a b c d e I x _
		//               |           | |
		//               |           | +-- first
		//               |           +-- mid
		//               +-- last
		//
		// x x x x x x x x I U a b c d e x _

	}
    }


	print6("after sort:\t\t", v);

	std::rotate(v.begin(), v.begin() + 1, v.end());
	print6("simple rotate left:\t", v);

	std::rotate(v.rbegin(), v.rbegin() + 1, v.rend());
	print6("simple rotate right:\t", v);

	//std::rotate(v.end() - 1, v.end() - 2, v.begin());    // this doesn't work
	//print6("simple rotate right:\t", v);

	//std::rotate(rev_iter(v.end()-1), rev_iter(v.end()-2), rev_iter(v.begin()));
	std::rotate(rev_iter(v.end()), rev_iter(v.end()-1), rev_iter(v.begin()));
	print6("simple rotate right:\t", v);
}



void Test_rotate_copy()
{
	std::vector<int> src = { 1,2,3,4,5 };
	std::vector<int> dest(src.size());
	auto pivot = std::find(src.begin(), src.end(), 3);

	std::rotate_copy(src.begin(), pivot, src.end(), dest.begin());
	for (int i : dest)
	{
		std::cout << i << ' ';
	}
	std::cout << '\n';
	pivot = std::find(dest.begin(), dest.end(), 1);
	std::rotate_copy(dest.begin(), pivot, dest.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << std::endl;
}



#if 0
struct S1
{
	int value { 0 };
	bool specified_state { true };

	S1(int v = 0) : value(v)
	{
	}

	S1(S1 const& rhs) = default;
	S1(S1&& rhs)
	{
		*this = std::move(rhs);  //  this calls the move assign operator just a few lines below
	}
	S1& operator=(S1 const& rhs) = default;
	S1& operator=(S1&& rhs)
	{
		if (this != &rhs)
		{
			value = rhs.value;
			specified_state = rhs.specified_state;
			rhs.specified_state = false;
		}
		return *this;
	}
};


template<typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T>const& v)
{
	for (auto const& s : v)
	{
		if constexpr (std::is_same<std::remove_cv_t<T>, S1>{})
		{
			s.specified_state ? os << s.value << ' ' : os << "? ";
		}
		else if constexpr (std::is_same<std::remove_cv_t<T>, std::string>{})
		{
			os << std::setvbuf(3) << quoted(s) << ' ';
		}
		else
		{
			os << s << ' ';
		}
	}
	return os;
}

void Test_shift()
{
	std::cout << std::left;
	std::vector<S1> a { 1,2,3,4,5,6,7 };
	std::vector<int> b { 1,2,3,4,5,6,7 };
	std::vector<std::string> c { "a", "b", "c", "d", "e", "f", "g" };
	std::cout << a << " " << b << " " << c << std::endl;

	std::shift_left(begin(a), end(a), 3);
	std::shift_left(begin(b), end(b), 3);
	std::shift_left(begin(c), end(c), 3);
	std::cout << a << " " << b << " " << c << std::endl;

	std::shift_right(begin(a), end(a), 2);
	std::shift_right(begin(b), end(b), 2);
	std::shift_right(begin(c), end(c), 2);
	std::cout << a << " " << b << " " << c << std::endl;

	std::shift_left(begin(a), end(a), 3);
	std::shift_left(begin(b), end(b), 3);
	std::shift_left(begin(c), end(c), 3);
	std::cout << a << " " << b << " " << c << std::endl;
}

#endif


void Test_random_shuffle()
{
	std::vector<int> v = { 1,2,3,4,5,6,7,8,9,10 };
	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(v.begin(), v.end(), g);

	std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));

	std::cout << std::endl;

}


void Test_sample()
{
	std::string in = "hgfedcba", out;
	std::sample(in.begin(), in.end(), std::back_inserter(out), 5, std::mt19937 { std::random_device{}() });
	std::cout << "five random letters out of " << in << " : " << out << std::endl;
}


void Test_unique()
{
	std::vector<int>v { 1,2,1,1,3,3,3,4,5,4 };
	auto print = [&](int id)
	{
		std::cout << "@" << id << ": ";
		for (int i : v)
		{
			std::cout << i << ' ';
		}
		std::cout << std::endl;
	};

	print(1);

	auto last = std::unique(v.begin(), v.end());

	v.erase(last, v.end());
	print(2);

	std::sort(v.begin(), v.end());
	print(3);

	last = std::unique(v.begin(), v.end());
	v.erase(last, v.end());
	print(4);
}

void Test_unique_copy()
{
	std::string s1 = "The     string    with many      spaces!";
	std::cout << "before: " << s1 << std::endl;

	std::string s2;
	std::unique_copy(s1.begin(), s1.end(), std::back_inserter(s2), [](char c1, char c2)
	{
		return c1 == ' ' && c2 == ' ';
	});
	std::cout << "after:  " << s2 << std::endl;
}



/*=============================================================*/
/*                    Partitioning operations                  */
/*=============================================================*/

static bool is_even2(int i)
{
	return i % 2 == 0;
}

//static bool (*is_even3)(int i)
//{
//	return i % 2 == 0;
//}

void Test_is_partitioned()
{
	int const L = 9;
	std::array<int, L> v = { 1,2,3,4,5,6,7,8,9 };

	auto is_even = [](int i)
	{
		return i % 2 == 0;
	};

	std::cout.setf(std::ios_base::boolalpha);

	std::cout << std::is_partitioned(v.begin(), v.end(), is_even) << ' ';

	std::partition(v.begin(), v.end(), is_even2);

	std::cout << std::is_partitioned(v.begin(), v.end(), is_even2) << ' ';

	std::reverse(v.begin(), v.end());

	std::cout << std::endl;
	std::copy(v.cbegin(), v.cend(), std::ostream_iterator<int>(std::cout, " | ")); 
	std::cout << std::endl;

	std::cout << std::is_partitioned(v.cbegin(), v.cend(), is_even) << ' ';

	std::cout << std::is_partitioned(v.cbegin(), v.cend(), std::not_fn(is_even)) << ' ';
	
	std::cout << std::is_partitioned(v.crbegin(), v.crend(), is_even) << std::endl;
	
}


template<class ForwardIt>
void quicksort(ForwardIt first, ForwardIt last)
{
	if (first == last)
		return;

	auto pivot = *std::next(first, std::distance(first, last) / 2);
	ForwardIt middle1 = std::partition(first, last, [pivot](auto const& em)
	{
		return em < pivot;
	});
	ForwardIt middle2 = std::partition(middle1, last, [pivot](auto const& em)
	{
		return !(pivot < em);
	});
	quicksort(first, middle1);
	quicksort(middle2, last);
}

void Test_partition()
{
	std::vector<int> v { 0,1,2,3,4,5,6,7,8,9 };
	std::cout << "Original vector:\n    ";
	for (int e : v)
		std::cout << e << ' ';

	auto it = std::partition(v.begin(), v.end(), [](int i)
	{
		return i % 2 == 0;
	});

	std::cout << "\nPartitioned vector:\n    ";
	std::copy(std::begin(v), it, std::ostream_iterator<int>(std::cout, " "));
	std::cout << " * " " ";
	std::copy(it, std::end(v), std::ostream_iterator<int>(std::cout, " "));

	std::forward_list<int> fl = { 1,30,-4,3,5,-4,1,6,-8,2,-5,64,1,92 };
	std::cout << "\nUnsorted list:\n    ";
	for (int n : fl)
		std::cout << n << ' ';

	std::cout << std::endl;

	quicksort(std::begin(fl), std::end(fl));
	std::cout << "Sorted using quicksort:\n    ";
	for (int fi : fl)
		std::cout << fi << ' ';

	std::cout << std::endl;
}



void Test_partition_copy()
{
	int arr[10] = { 1,2,3,4,5,6,7,8,9,10 };
	int true_arr[5] = { 0 };
	int false_arr[5] = { 0 };

	std::partition_copy(std::begin(arr), std::end(arr), std::begin(true_arr), std::begin(false_arr), [](int i)
	{
		return i > 5;
	});

	std::cout << "true_arr: ";
	for (int const& x : true_arr)
	{
		std::cout << x << ' ';
	}
	std::cout << std::endl;

	std::cout << "false_arr: ";
	for (int const& x : false_arr)
	{
		std::cout << x << ' ';
	}
	std::cout << std::endl;
}


struct A2
{
public:
	int a;
	char b;
	A2() = delete;
	A2(int aa, char bb) : a(aa), b(bb)
	{
	}
	A2(A2 const& a2) // copy constructor must have the "const" in its parameter list?
	{
		a = a2.a;
		b = a2.b;
	}
};

void Test_stable_partition()
{
	std::vector<A2> v { {17, 'p'},{6,'a'},{3, 'b'},{13, 'q'},{6,'c'},{17,'r'} };
	std::stable_partition(v.begin(), v.end(), [](A2& a2)
	{
		return a2.a < 10;
	});
	for (A2 const& a2 : v)
	{
		std::cout << "{" << a2.a << "," << a2.b << "}" << ' ';
	}
	std::cout << std::endl;
}




auto print7 = [](auto remark, auto first, auto last)
{
	for (std::cout << remark; first != last; std::cout << *first++ << ' ')
	{

	}
	std::cout << std::endl;
};

void Test_partition_point()
{
	std::array v = { 1,2,3,4,5,6,7,8,9 };

	auto is_even = [](int i)
	{
		return i % 2 == 0;
	};

	//std::partition(v.begin(), v.end(), is_even);
	std::stable_partition(v.begin(), v.end(), is_even);

	print7("After partitioning, v: ", v.cbegin(), v.cend());

	auto const pp = std::partition_point(v.cbegin(), v.cend(), is_even);
	auto const i = std::distance(v.cbegin(), pp);

	std::cout << "Partition point is at " << i << "; v[" << i << "] = " << *pp << '\n';

	print7("First partition (all even elements): ", v.cbegin(), pp);
	print7("Second partition (all odd elements): ", pp, v.cend());
}


/*=============================================================*/
/*                    Sorting operations                       */
/*=============================================================*/

void Test_is_sorted()
{
	int digits[] = { 3,1,4,1,5 };

	for (auto& i : digits)
	{
		std::cout << i << ' ';
	}
	std::cout << ": is_sorted: " << std::boolalpha << std::is_sorted(std::begin(digits), std::end(digits)) << '\n';
	std::sort(std::begin(digits), std::end(digits));
	for (auto& i : digits)
	{
		std::cout << i << ' ';
	}
	std::cout << ": is_sorted: " << std::is_sorted(std::cbegin(digits), std::cend(digits)) << '\n';
}



void Test_is_sorted_until()
{
	std::random_device rd;
	std::mt19937 g { rd() };
	int const N = 6;
	int nums[N] = { 3,1,4,1,5,9 };

	int const min_sorted_size = 4;

	//int sorted_size = 0;
	typename std::iterator_traits<decltype(std::begin(nums))>::difference_type sorted_size; 
													// this difference_type is actually 64 bit int
													// and std::distance returns a value of this difference_type
													// so if I define "sorted_size" as int, I'm going to truncate when
													// assigning the return value of std::distance to "sorted_size"

	do
	{
		std::shuffle(nums, nums + N, g);
		int* sorted_end = std::is_sorted_until(nums, nums + N);
		sorted_size = std::distance(nums, sorted_end);

		for (auto const& i : nums)
		{
			std::cout << i << ' ';
		}
		std::cout << " : " << sorted_size << " initial sorted elements\n";
	} while (sorted_size < min_sorted_size);
}



void Test_is_sorted_until2()
{
	std::random_device rd;
	std::mt19937 g { rd() };
	int nums[6] = { 3,1,4,1,5,9 };

	int const min_sorted_size = 4;
	//int sorted_size = 0;
	typename std::iterator_traits<decltype(std::begin(nums))>::difference_type sorted_size;

	do
	{
		std::shuffle(std::begin(nums), std::end(nums), g);
		auto sorted_end = std::is_sorted_until(std::begin(nums), std::end(nums));
		sorted_size = std::distance(std::begin(nums), sorted_end);

		for (auto const& i : nums)
		{
			std::cout << i << ' ';
		}
		std::cout << " : " << sorted_size << " initial sorted elements\n";
	} while (sorted_size < min_sorted_size);
}


void Test_sort()
{
	std::array<int, 10> s = { 5,7,4,2,8,6,1,9,0,3 };
	std::function<void(std::string_view const&)> print = [&s](std::string_view const & remark)
	{
		for (auto a : s)
		{
			std::cout << a << ' ';
		}
		std::cout << ": " << remark << '\n';
	};

	std::sort(s.begin(), s.end());
	print("sorted with the default operator<");

	std::sort(s.begin(), s.end(), std::greater<int>{});
	print("sorted with the standard library compare function object");

	struct customLess
	{
		bool operator()(int a, int b) const
		{
			return a < b;
		}
	} ;

	std::sort(s.begin(), s.end(), customLess {});
	print("sorted with a custom function object");

	std::sort(s.begin(), s.end(), [](int a, int b)
	{
		return a > b;
	});
	print("sorted with a lambda expression");
}


static auto print4 = [](auto const& s, int middle)
{
	for (int a : s)
	{
		std::cout << a << ' ';
	}
	std::cout << '\n';
	if (middle > 0)
	{
		while (middle-- > 0)
		{
			std::cout << "--";
		}
		std::cout << '^';
	}
	else if (middle < 0)
	{
		for (auto i = s.size() + middle; --i; std::cout << "  ");
		for (std::cout << '^'; middle++ < 0; std::cout << "--");
	}
	std::cout << '\n';
};


void Test_partial_sort()
{
	std::array<int, 10>s { 5,7,4,2,8,6,1,9,0,3 };
	print4(s, 0);
	std::partial_sort(s.begin(), s.begin() + 3, s.end());
	print4(s, 3);
	std::partial_sort(s.rbegin(), s.rbegin() + 4, s.rend());
	print4(s, -4);
	std::partial_sort(s.rbegin(), s.rbegin() + 5, s.rend(), std::greater<int>{});
	print4(s, -5);
}


void Test_partial_sort_copy()
{
	auto const v0 = { 4,2,5,1,3 };
	std::vector<int> v1 { 10,11,12 };
	std::vector<int> v2 { 10,11,12,13,14,15,16 };
	typename std::vector<int>::iterator it;

	it = std::partial_sort_copy(v0.begin(), v0.end(), v1.begin(), v1.end());

	std::cout << "Writing to the smaller vector in ascending order gives: ";
	for (int& a : v1)
	{
		std::cout << a << ' ';
	}
	std::cout << std::endl;

	if (it == v1.end())
	{
		std::cout << "The return value is the end iterator\n";
	}

	it = std::partial_sort_copy(v0.begin(), v0.end(), v2.begin(), v2.end(), std::greater<int>{});

	std::cout << "Writing to the larger vector in descending order gives: ";
	for (int const& a : v2)
	{
		std::cout << a << ' ';
	}
	std::cout << '\n' << "The return value is the iterator to " << *it << std::endl;
}

struct Employee
{
	int age;
	std::string name;
};
static bool operator<(Employee const& lhs, Employee const& rhs)
{
	return lhs.age < rhs.age;
}

void Test_stable_sort()
{
	std::vector<Employee> v = { {108, "Zaphod"}, {32, "Arthur"}, {108, "Ford"} };
	std::stable_sort(v.begin(), v.end());
	for (const Employee& e : v)
	{
		std::cout << e.age << ", " << e.name << '\n';
	}
}


static void print8(const std::vector<int>& vec)
{
	std::cout << "v= {";
	for (int const& i : vec)
	{
		std::cout << i << ", ";
	}
	std::cout << "}\n";
}

void Test_nth_element()
{
	std::vector<int> v { 5,10,6,4,3,2,6,7,9,3 };
	print8(v);

	auto m = v.begin() + v.size() / 2;
	std::nth_element(v.begin(), m, v.end());
	if (v.size() % 2 == 0)
	{
		std::cout << "\nThe median is " <<  static_cast<float>(*m + *(m-1))/2 << '\n';
	}
	else
	{
		std::cout << "\nThe median is " << *m << '\n';
	}
	assert(std::accumulate(v.begin(), m, 0) < std::accumulate(m, v.end(), 0));
	print8(v);

	std::nth_element(v.begin(), v.begin() + 1, v.end(), std::greater<int>{});
	std::cout << "\nThe second largest element is " << v[1] << '\n';
	std::cout << "The largest element is " << v[0] << '\n';
	print8(v);
}

/*=============================================================*/
/*          Binary search operations (on sorted ranges)        */
/*=============================================================*/

struct S1
{
	double price;
};

void Test_lower_bound()
{
	std::vector<int> const data = { 1,2,3,4,5,5,6 };
	for (int i = 0; i < 8; i++)
	{
		auto lower = std::lower_bound(data.begin(), data.end(), i);

		std::cout << i << " <= ";
		lower != data.end() ? std::cout << *lower << " at index " << std::distance(data.begin(), lower) : std::cout << "[not found]";
		std::cout << std::endl;
	}


	std::vector<S1>prices = { {100.0}, {101.5}, {102.5}, {102.5}, {107.3} };

	double targets[2] = { 102.5, 110.2 };

	for (double to_find : /*{102.5, 110.2}*/ targets)
	{
		auto p = std::lower_bound(prices.begin(), prices.end(), to_find, [](S1 const& p, double value)
		{
			return p.price < value;
		});
		p != prices.end() ?
			std::cout << p->price << " at index " << p - prices.begin()
			: std::cout << to_find << " not found";
		std::cout << std::endl;
	}
}

void Test_lower_bound_comp_function_arg_order()
{
	std::vector<int> a{ 1,9,7,3,5 };
	std::vector<int> indexes{ 0,1,2,3,4 };
	sort(indexes.begin(), indexes.end(), [&](int const& i1, int const& i2) {return a[i1] < a[i2]; });
	auto it = lower_bound(indexes.begin(), indexes.end(), 6,
		[&](int const& i, int const& t)
		{
			std::cout << "compare " << i << " and " << t << '\n';   // verify that the 2nd argument in Comp functional object in "lower_bound" function is always the target.
			return a[i] < t;
		});

	std::cout << *it << "\n\n";
}

void Test_upper_bound()
{
	std::vector<int> const data = { 1,2,4,5,5,6 };
	for (int i = 0; i < 7; i++)
	{
		auto upper = std::upper_bound(data.begin(), data.end(), i);

		std::cout << i << " < ";
		upper != data.end() ?
			std::cout << *upper << " at index " << std::distance(data.begin(), upper)
			: std::cout << "not found";
		std::cout << std::endl;
	}

	std::vector<S1> prices = { {100.0}, {101.4}, {102.5}, {102.5}, {107.3} };
	for (double to_find : {102.5, 110.2})
	{
		auto p = std::upper_bound(prices.begin(), prices.end(), to_find, [](double v, S1 const& p)
		{
			return v < p.price;
		});
		p != prices.end() ?
			std::cout << p->price << " at index " << p - prices.begin() :
			std::cout << to_find << " not found";
		std::cout << std::endl;
	}
}

void Test_binary_search()
{
	std::vector<int> haystack { 1,3,4,5,9 };
	std::vector<int> needles { 1,2,3 };

	for (auto const& n : needles)
	{
		std::cout << "Searching for " << n << std::endl;
		if (std::binary_search(haystack.begin(), haystack.end(), n))
		{
			std::cout << "Found " << n << std::endl;
		}
		else
		{
			std::cout << "no dice!\n";
		}
	}
}

struct S2
{
	int number;
	char name;
	bool operator<(const S2& s)const
	{
		return number < s.number;
	}
};

void Test_equal_range()
{
	std::vector<S2> const vec = { {1,'A'}, {2,'B'}, {2,'C'}, {2, 'D'}, {4,'G'}, {3,'F'} }; // in order to use equal_range, a fully sorted range is not an absolutely necessary
	S2 const value = { 2, '?' };

	std::cout << "Compare using S::operation<()";

	auto const p = std::equal_range(vec.begin(), vec.end(), value);

	for (auto i = p.first; i != p.second; ++i)
	{
		std::cout << i->name << ' ';
	}

	std::cout << "\nUsing heterogeneous comparison: ";

	struct S3
	{
		bool operator()(S2 const& s, int i) const
		{
			return s.number < i;
		}
		bool operator()(int i, S2 const& s) const
		{
			return i < s.number;
		}
	};
	auto const p2 = std::equal_range(vec.begin(), vec.end(), 2, S3());

	for (auto i = p2.first; i != p2.second; i++)
	{
		std::cout << i->name << ' ';
	}
}


void Test_equal_range2()
{
	std::vector<int> v1 = { 4,4,5,5,6,6,7,7 };

	auto result1 = std::equal_range(v1.begin(), v1.end(), 3);
	auto result2 = std::equal_range(v1.begin(), v1.end(), 4);
	auto result3 = std::equal_range(v1.begin(), v1.end(), 7);
	auto result4 = std::equal_range(v1.begin(), v1.end(), 8);

	//auto print = [&v1](std::pair < std::vector<int>::iterator, std::vector<int>::iterator > const& r)  // this line works
	auto print = [&v1](decltype(result1) const & r)  // surprisingly, this works too
	{
		if (r.first != v1.end())
		{
			std::cout << "first: index: " << r.first - v1.begin() << "   " << "value: " << *(r.first) << '\n';
		}
		else
		{
			std::cout << "first is at end\n";
		}
		if (r.second != v1.end())
		{
			std::cout << "second: index: " << r.second - v1.begin() << "    " << "value: " << *(r.second) << '\n';
		}
		else
		{
			std::cout << "second is at end\n";
		}
		std::cout << "-------------\n";
	};

	print(result1);
	print(result2);
	print(result3);
	print(result4);
}


/*=============================================================*/
/*          Other operations on sorted range                   */
/*=============================================================*/


auto print1 = [](auto const rem, auto const& v)
{
	std::cout << rem;
	std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << '\n';
};

void Test_merge()
{
	std::random_device rd;
	std::mt19937 mt(rd());  // rd has operator() defined
	std::uniform_int_distribution<> dis(0, 9);

	std::vector<int> v1(10), v2(10);
	std::generate(v1.begin(), v1.end(), std::bind(dis, std::ref(mt)));
	std::generate(v2.begin(), v2.end(), std::bind(dis, std::ref(mt)));

	print1("Originally:\nv1:  ", v1);
	print1("V2:  ", v2);

	std::sort(v1.begin(), v1.end());
	std::sort(v2.begin(), v2.end());

	print1("After sorting:\nv1:  ", v1);
	print1("v2:  ", v2);

	std::vector<int> dst;
	std::merge(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(dst));

	print1("After merging:\ndst:  ", dst);

	dst.clear();
	
	print1("After clearing:\ndst:  ", dst);
	
	//dst.reserve(v1.size() + v2.size());
	dst.resize(v1.size() + v2.size());
	std::merge(v1.begin(), v1.end(), v2.begin(), v2.end(), dst.begin());

	print1("After merging:\ndst:  ", dst);
}

#if 0
static void print2(const auto& in1, const auto& in2, auto first, auto last)
{
	std::cout << "{ ";
	for (const auto& e : in1)
	{
		std::cout << e << ' ';
	}
	std::cout << "} +\n{ ";
	for (const auto& e : in2)
	{
		std::cout << e << ' ';
	}
	std::cout << "} = \n{ ";
	while (!(first == last))
	{
		std::cout << *first++ << ' ';
	}
	std::cout << "}\n\n";
}

void Test_range_merge()
{
	std::vector<int> in1, in2, out;

	in1 = { 1, 2, 3, 4, 5 };
	in2 = { 3, 4, 5, 6, 7 };
	out.resize(in1.size() + in2.size());
	const auto ret = std::ranges::merge(in1, in2, out.begin());
	print2(in1, in2, out.begin(), ret.out);

	in1 = { 1, 2, 3, 4, 5, 5, 5 };
	in2 = { 3, 4, 5, 6, 7 };
	out.clear();
	out.reserve(in1.size() + in2.size());
	std::ranges::merge(in1, in2, std::back_inserter(out));
	print2(in1, in2, out.cbegin(), out.cend());
}
#endif

template<class Iter>
static void merge_sort1(Iter first, Iter last)
{
	if (last - first > 1)
	{
		Iter middle = first + (last - first) / 2;
		merge_sort1(first, middle);
		merge_sort1(middle, last);
		std::inplace_merge(first, middle, last);
	}
}

template<class Iter>
static void merge_sort2(Iter first, Iter last)
{
	if (last - first > 1)
	{
		Iter middle = first + (last - first) / 2;
		merge_sort2(first, middle);
		merge_sort2(middle, last);
		std::inplace_merge(first, middle, last, std::greater<typename std::iterator_traits<Iter>::value_type>());
	}
}


void Test_inplace_merge()
{
	std::vector<int> v { 8, 2, -2, 0, 11, 11, 1, 7, 3 };
	merge_sort1<typename std::vector<int>::iterator>(v.begin(), v.end());   // this part <typename std::vector<int>::iterator> is not necessary, compiler can deduce

	for (int n : v)
	{
		std::cout << n << ' ';  // If I use apostraphy here, I must make sure there is one and only one character in between
	}
	std::cout << std::endl;

	std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
	std::cout << '\n';

	for (auto it = v.begin(); it != v.end(); it++)
	{
		std::cout << *it << ' ';
	}
	std::cout << '\n';

	for (typename std::vector<int>::iterator it = v.begin(); it != v.end(); it++)
	{
		std::cout << *it << ' ';
	}
	std::cout << '\n';


	merge_sort2(v.begin(), v.end());

	for (auto it = v.begin(); it != v.end(); it++)
	{
		std::cout << *it << ' ';
	}
	std::cout << '\n';

}

/*=============================================================*/
/*                 Set operations  (on sorted ranges)          */
/*=============================================================*/


template<class Os, class Co>Os& operator<<(Os& os, Co const& v)
{
	for (auto & i : v)
	{
		os << i << ' ';
	}
	return os << '\t';
}

void Test_include()
{
	auto const v1 = { 'a', 'b', 'c', 'f', 'h', 'x' },
		v2 = { 'a', 'b', 'c' },
		v3 = { 'a', 'c' },
		v4 = { 'a', 'a', 'b' },
		v5 = { 'g' },
		v6 = { 'a', 'c', 'g' },
		v7 = { 'A', 'B', 'C' };

	std::cout << typeid(v1).name() << "\n\n\n";


	auto no_case = [](char a, char b)
	{
		return std::tolower(a) < std::tolower(b);
	};

	std::cout <<
		v1 << "\nincludes:\n" << std::boolalpha
		<< v2 << ": " << std::includes(v1.begin(), v1.end(), v2.begin(), v2.end()) << '\n'
		<< v3 << ": " << std::includes(v1.begin(), v1.end(), v3.begin(), v3.end()) << '\n'
		<< v4 << ": " << std::includes(v1.begin(), v1.end(), v4.begin(), v4.end()) << '\n'
		<< v5 << ": " << std::includes(v1.begin(), v1.end(), v5.begin(), v5.end()) << '\n'
		<< v6 << ": " << std::includes(v1.begin(), v1.end(), v6.begin(), v6.end()) << '\n'
		<< v7 << ": " << std::includes(v1.begin(), v1.end(), v7.begin(), v7.end(), no_case) << " (case-insensitive)\n";
}



auto print9 = [](auto const& v, std::string_view end = "")
{
	std::cout << "{ ";
	for (auto& i : v)
		std::cout << i << ' ';
	std::cout << "} " << end;
};

struct Order
{
	int order_id;
	friend std::ostream& operator<<(std::ostream& os, Order const& ord)
	{
		return os << ord.order_id << ',';
	}
};

void Test_set_difference()
{
	std::vector<int> const v1 { 1,2,5,5,5,9 };
	std::vector<int> const v2 { 2,5,7 };
	std::vector<int> diff;

	std::set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), std::inserter(diff, diff.begin()));

	print9(v1, "\t");
	print9(v2, "= ");
	print9(diff, "\n");

	std::vector<Order> old_orders { {1},{2},{5},{9} };
	std::vector<Order> new_orders { {2},{5},{7} };
	std::vector<Order> cut_orders;

	std::set_difference(old_orders.begin(), old_orders.end(), new_orders.begin(), new_orders.end(), std::back_inserter(cut_orders), [](auto& a, auto& b)
	{
		return a.order_id < b.order_id;
	});

	std::cout << "old orders = ";
	print9(old_orders, "\n");
	std::cout << "new orders = ";
	print9(new_orders, "\n");
	std::cout << "cut orders = ";
	print9(cut_orders, "\n");

}

void Test_set_intersection()
{
	std::vector<int> v1 { 1,2,3,4,5,6,7,8 };
	
	//std::vector<int> v2 { 5,7,9,10 };

	//std::array<int, 4> v2 { 10,5,9,7 };

	std::list<int> v2 { 10,5,9,7 };

	std::sort(v1.begin(), v1.end());
	//std::sort(v2.begin(), v2.end());

	v2.sort();  // if v2 is a list

	std::vector<int> v_intersect;

	std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(v_intersect));

	for (int& n : v_intersect)
	{
		std::cout << n << ' ';
	}
	std::cout << '\n';
}


void Test_set_symmetric_difference()
{
	std::vector<int> v1 { 1,2,3,4,5,6,7,7,7,8 };
	std::vector<int> v2 { 5,5,5,7,9,10 };
	std::sort(v1.begin(), v1.end());
	std::sort(v2.begin(), v2.end());

	std::list<int> v_sym_diff;

	std::set_symmetric_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(v_sym_diff));

	for (int& n : v_sym_diff)
	{
		std::cout << n << ' ';
	}
	std::cout << '\n';
}

void Test_set_union()
{
	{
		std::vector<int> v1 = { 1,2,3,4,5 };
		std::vector<int> v2 = { 3,4,5,6,7 };
		std::vector<int> dest1;

		std::set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(dest1));

		for (auto const& i : dest1)
			std::cout << i << ' ';
		std::cout << '\n';
	}

	{
		std::vector<int> v1 = { 1,2,3,4,5,5,5 };
		std::vector<int> v2 = { 3,4,5,6,7 };

		std::vector<int> dest1;
		std::set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(dest1));

		for (auto const& i : dest1)
			std::cout << i << ' ';
		std::cout << '\n';
	}
}

/*=============================================================*/
/*                 Heap operations                             */
/*=============================================================*/

static bool has_single_bit(unsigned int x)
{
	return x != 0 && ((x & (x-1)) == 0);
}

void Test_is_heap()
{
	std::vector<int> v { 3,1,4,1,5,9,2,6,5,3,5,8,9,7,9 };
	std::cout << "initially, v: \n";
	for (auto const& i : v)
		std::cout << i << ' ';
	std::cout << '\n';
	if (!std::is_heap(v.begin(), v.end()))
	{
		std::cout << "making heap...\n";
		std::make_heap(v.begin(), v.end());
	}
	std::cout << "after make_heap, v: \n";
	auto t { 1U };
	for (auto& i:v)
		std::cout << i << (has_single_bit(++t) ? " | " : " ");  // has_single_bit is a C++20 feature
	std::cout << '\n';
}

void Test_is_heap_until()
{
	std::vector<int> v { 3,1,4,1,5,9 };
	std::make_heap(v.begin(), v.end());
	v.push_back(2);
	v.push_back(6);

	auto heap_end = std::is_heap_until(v.begin(), v.end());

	std::cout << "all of v: ";
	for (auto& i : v)
		std::cout << i << ' ';
	std::cout << '\n';

	std::cout << "only heap: ";
	for (auto i = v.begin(); i != heap_end; i++)
		std::cout << *i << ' ';
	std::cout << '\n';
}



static void print10(std::string_view text, std::vector<int> const& v = {})
{
	std::cout << text << ": ";
	for (auto const& e : v)
		std::cout << e << ' ';
	std::cout << '\n';
}

void Test_make_heap()
{
	print10("Max heap");

	std::vector<int> v { 3,2,4,1,5,9 };
	print10("initially, v", v);

	std::make_heap(v.begin(), v.end());
	print10("after make heap, v", v);

	std::pop_heap(v.begin(), v.end());
	print10("after pop heap, v", v);

	auto top = v.back();
	v.pop_back();
	print10("former top element", { top });
	print10("after removing the former top element, v", v);

	print10("\nMin heap");

	std::vector<int>v1 { 3,2,4,1,5,9 };
	print10("initially, v1", v1);

	std::make_heap(v1.begin(), v1.end(), std::greater<int>{});
	print10("after make heap, v1", v1);

	std::pop_heap(v1.begin(), v1.end(), std::greater<int>{});
	print10("after pop heap, v1", v1);

	auto top1 = v1.back();
	v1.pop_back();
	print10("former top element", { top1 });
	print10("after removing the former top element, v1", v1);
}

void Test_push_heap()
{
	std::vector<int> v { 3,1,4,1,5,9 };
	std::make_heap(v.begin(), v.end());
	std::cout << "heap v: ";
	for (auto const& i : v)
		std::cout << i << ' ';
	std::cout << '\n';
	v.emplace_back(6);
	std::cout << "before push_heap: ";
	for (auto const& i : v)
		std::cout << i << ' ';
	std::cout << '\n';
	std::push_heap(v.begin(), v.end());
	std::cout << "after push_heap: ";
	for (auto const& i : v)
		std::cout << i << ' ';
	std::cout << '\n';
}

void Test_pop_heap()
{
	std::vector<int> v { 3,1,4,1,5,9 };

	std::make_heap(v.begin(), v.end());

	std::cout << "heap v: ";
	for (auto& i : v)
		std::cout << i << ' ';
	std::cout << '\n';

	std::pop_heap(v.begin(), v.end());

	std::cout << "after pop_heap: ";
	for (auto& i : v)
		std::cout << i << ' ';
	std::cout << '\n';

	int largest = v.back();
	v.pop_back();
	std::cout << "largest element: " << largest << '\n';

	std::cout << "heap without largest: ";
	for (auto& i : v)
		std::cout << i << ' ';
	std::cout << '\n';
}


void Test_heap_sort()
{
	std::array<int, 6> v = { 3,1,4,1,5,9 };

	std::make_heap(v.begin(), v.end());

	std::cout << "heap:  ";
	for (const auto& i : v)
	{
		std::cout << i << ' ';
	}

	std::cout << std::endl;

	std::sort_heap(v.begin(), v.end());

	std::cout << "ascendingly sorted:  ";

	for (auto i : v)
	{
		std::cout << i << ' ';
	}

	std::cout << std::endl;


	std::array<int, 14> k = { 3, 9, 4, 10, 67, 23, 33, 71, 8, 42, 75, 91, 27, 28 };

	std::make_heap(k.begin(), k.end(), std::greater<int>{});

	std::sort_heap(k.begin(), k.end(), std::greater<int>{});

	std::cout << "descendingly sorted:  ";

	for (int& i : k)
	{
		std::cout << i << "  ";
	}

	std::cout << std::endl;

}


/*=============================================================*/
/*             Minimum/maximum operations                      */
/*=============================================================*/

void Test_max()
{
	std::cout << "longest of \"foo\", \"bar\", and \"hello\" is \""
		<< std::max({ "foo", "bar", "hello" }, [](std::string_view const& s1, std::string_view const& s2)
	{
		return s1.size() < s2.size();
	})
		<< "\"\n";


	std::cout << "longest of \"foo\", \"bar\", and \"hello\" is "
		<< std::quoted(std::max({ "fo", "bar", "hello" }, [](std::string_view const& s1, std::string_view const& s2)
	{
		return s1.size() > s2.size();
	}))
		<< "\n";


	std::cout << "longest of \"foo\", \"bar\", and \"hello\" is "
		<< std::quoted(std::max({ "fo", "bar", "hello", "catch", "punch"}, [](std::string_view const& s1, std::string_view const& s2)
	{
		return s1.size() <= s2.size();   // This causes run time assertion.  
	}))
		<< "\n";
}


static bool abs_compare(int a, int b)
{
	return std::abs(a) < std::abs(b);
}


void Test_max_element()
{
	std::vector<int> v { 3,1,-14,1,5,9 };
	std::vector<int>::iterator ret;

	ret = std::max_element(v.begin(), v.end());
	std::cout << "max element at: " << std::distance(v.begin(), ret) << '\n';

	ret = std::max_element(v.begin(), v.end(), abs_compare);
	std::cout << "max absolute element at: " << std::distance(v.begin(), ret) << '\n';
}

void Test_minmax_element()
{
	auto const v = { 3,9,1,4,1,2,5,9 };

	const auto [min, max] = std::minmax_element(v.begin(), v.end());

	std::cout << "min = "<<"v["<<std::distance(v.begin(), min)<<"] = " << *min << ",  max = "<< "v["<<std::distance(v.begin(), max)<<"] = " << *max << '\n';
}


void Test_clamp()
{
	std::cout << " raw    clamped to int8_t   clamper to uint8_t\n";
	for (int const& v : { -129, -128, -1, 0, 42, 127, 128, 255, 256 })
	{
		std::cout << std::setw(04) << v
			<< std::setw(20) << std::clamp<int>(v, INT8_MIN, INT8_MAX)
			<< std::setw(21) << std::clamp<unsigned int>(v, 0, UINT8_MAX) << '\n';
	}

	std::cout << "\n\n\n";

	signed char x = -10;

	std::cout << x << "    " << (unsigned char) x << "    " << (unsigned int) x << "\n\n";

	std::cout << 0 << " ~ " << 255 << "  " << std::clamp<unsigned char>((unsigned char) x, 0, UINT8_MAX)<<"\n\n\n";

	unsigned int y = UINT8_MAX;
	std::cout << y << "\n\n\n";

}


/*=============================================================*/
/*                  Comparison operations                      */
/*=============================================================*/

static bool is_palindrome(const std::string_view& s)
{
	return std::equal(s.begin(), s.begin() + s.size() / 2, s.rbegin());
}

void Test_equal()
{
	std::cout << std::quoted("radar") << " is " << (is_palindrome("radar") ? "" : "not") << " palindrome\n";
	std::cout << std::quoted("hello") << " is " << (is_palindrome("hello") ? "" : "not") << " palindrome\n";
}


void Test_lexi_comp()
{
	std::vector<char> v1 { 'a', 'b','c','d' };
	std::vector<char> v2 { 'a','b','c','d' };

	std::mt19937 g { std::random_device{}() };
	while (!std::lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end()))
	{
		for (auto& c : v1)
			std::cout << c << ' ';
		std::cout << ">= ";
		for (auto& c : v2)
			std::cout << c << ' ';
		std::cout << '\n';

		std::shuffle(v1.begin(), v1.end(), g);
		std::shuffle(v2.begin(), v2.end(), g);
	}

	for (auto& c : v1)
		std::cout << c << ' ';
	std::cout << "< ";
	for (auto& c : v2)
		std::cout << c << ' ';
	std::cout << '\n';

}

/*=============================================================*/
/*                  Permutation operations                     */
/*=============================================================*/


template <typename Os, typename V>
static Os& operator<=(Os& os, V const& v)
{
	os << "{ ";
	for (auto const& e : v)
		os << e << ' ';
	return os << "}";
}

void Test_is_permu()
{
	auto const v1 = { 1,2,3,4,5 };
	auto const v2 = { 3,5,4,1,2 };
	auto const v3 = { 3,5,4,1,1 };

	std::cout <= v2;
	std::cout << " is a permutation of ";
	std::cout <= v1;
	std::cout << ": " << std::boolalpha << std::is_permutation(v1.begin(), v1.end(), v2.begin()) << '\n';

	((std::cout <= v3) << " is a permutation of " <= v1)
		<< ": " << std::boolalpha << std::is_permutation(v1.begin(), v1.end(), v3.begin()) << '\n';

}


void Test_next_permu()
{
	std::string s = "aba";
	std::sort(s.begin(), s.end());

	do
	{
		std::cout << s << '\n';
	} while (std::next_permutation(s.begin(), s.end()));
}



void Test_prev_permu()
{
	std::string s = "abc";
	std::sort(s.begin(), s.end(), std::greater<>{});

	do
	{
		std::cout << s << '\n';
	} while (std::prev_permutation(s.begin(), s.end()));
	std::cout << '\n';
}



/*=============================================================*/
/*                      Numeric operations                     */
/*=============================================================*/

void Test_iota()
{
	std::list<int> l(10);
	std::iota(l.begin(), l.end(), -4);

	std::vector<std::list<int>::iterator> v(l.size());
	std::iota(v.begin(), v.end(), l.begin());

	std::shuffle(v.begin(), v.end(), std::mt19937 { std::random_device{}() });

	std::cout << "Contents of the list: ";
	for (auto& n : l)
		std::cout << n << ' ';
	std::cout << '\n';

	std::cout << "Contents of the list, shuffled: ";
	for (auto& n : v)
		std::cout << *n << ' ';
	std::cout << '\n';
}

void Test_accumulate()
{
	std::vector<int> v { 1,2,3,4,5,6,7,8,9,10 };

	int sum = std::accumulate(v.begin(), v.end(), 0);

	int product = std::accumulate(v.begin(), v.end(), 1, std::multiplies<int>{});

	auto dash_fold = [](std::string a, int b)
	{
		return std::move(a) + '-' + std::to_string(b);
	};

	std::string s = std::accumulate(std::next(v.begin()), v.end(), std::to_string(v[0]), dash_fold);

	std::string rs = std::accumulate(std::next(v.rbegin()), v.rend(), std::to_string(v.back()), dash_fold);

	std::cout << std::left << std::setw(40) << "sum: " << sum << '\n'
		<< std::setw(40) << "product: " << product << '\n'
		<< std::setw(40) << "dash-separated string: " << s << '\n'
		<< std::setw(40) << "dash-separated string(right-folded): " << rs << '\n';

	for (auto& n : v)  // vector itself is not modified
		std::cout << n << ' ';
	std::cout << '\n';
}


void Test_accumulate_return_type()
{
	std::vector<int> v{ 2'147'483'000, 2'147'483'000 };

	long long total1 = accumulate(v.begin(), v.end(), 0); // this is going to overflow and total1 will become a negative number
	long long total2 = accumulate<typename std::vector<int>::iterator, long long>(v.begin(), v.end(), 0ll);
	long long total3 = 0;
	for (size_t i = 0; i < v.size(); i++)
	{
		total3 += static_cast<long long>(v[i]);
	}
	std::cout << total1 << "  " << total2 << "  " << total3 << '\n';
}


void Test_inner_product()
{
	std::vector<int> a { 0,1,2,3,4 };
	std::vector<int> b { 5,4,2,3,1 };

	int r1 = std::inner_product(a.begin(), a.end(), b.begin(), 0);
	std::cout <<std::left<<std::setw(50) << "Inner product of a and b: " << r1 << '\n';

	int r2 = std::inner_product(a.begin(), a.end(), b.begin(), 0, std::plus<int>{}, std::equal_to<int>{});

	std::cout << std::left << std::setw(50) << "Number of pairwise matches between a and b: " << r2 << '\n';
}


auto print11 = [](auto comment, auto const& seq)
{
	std::cout << std::left << std::setw(15) << comment;
	for (const auto& n : seq)
		std::cout << std::right << std::setw(3) << n << ' ';
	std::cout << '\n';
};

void Test_adjacent_difference()
{
	std::vector v { 4,6,9,13,18,19,19,15,10 };
	print11("Initially, v = ", v);

	std::adjacent_difference(v.begin(), v.end(), v.begin());
	print11("Modified, v = ", v);

	std::array<int, 10> a { 1 };
	print11("Initially, a = ", a);
	std::adjacent_difference(std::begin(a), std::prev(std::end(a)), std::next(std::begin(a)), std::plus<int>{});
	print11("Fibonacci, a = ", a);
}


void Test_partial_sum()
{
	std::vector<int> v { 2,2,2,2,2,2,2,2,2,2 };

	std::cout << std::left << std::setw(20) << "The first 10 even numbers are: ";
	std::partial_sum(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));

	std::cout << '\n';

	std::partial_sum(v.begin(), v.end(), v.begin(), std::multiplies<int>{});
	std::cout << std::left << std::setw(20) << "The first 10 powers of 2 are: ";
	for (auto& n : v)
		std::cout <<std::right<<std::setfill('!')<<std::setw(8) << n;
	std::cout << '\n';
}

void Test_reduce()
{
	auto eval = [](auto fun)
	{
		auto const t1 = std::chrono::high_resolution_clock::now();
		auto const [name, result] = fun();
		auto const t2 = std::chrono::high_resolution_clock::now();
		const std::chrono::duration<double, std::milli> ms = t2 - t1;
		std::cout  
			<< std::left << std::setw(25) << name 
			<< " result " << std::left << std::setw(12) << std::fixed << std::setprecision(1) << result 
			<< " took " << std::left << std::setw(7) << ms.count() << " ms\n";
	};

	{
		std::vector<double> const v(100'000'007, 0.1);

		eval([&v]
		{
			return std::pair { "std::accumulate (double)", std::accumulate(v.cbegin(), v.cend(), 0.0) };
		});

		eval([&v]
		{
			return std::pair { "std::reduce (seq, double)", std::reduce(std::execution::seq, v.cbegin(), v.cend()) };
		});

		eval([&v]
		{
			return std::pair { "std::reduce (par, double)", std::reduce(std::execution::par, v.cbegin(), v.cend()) };
		});
	}
	{
		const std::vector<long> v(100'000'007, 1);

		eval([&v]
		{
			return std::pair { "std::accumulate(long)", std::accumulate(v.cbegin(), v.cend(), 0) };
		});

		eval([&v]
		{
			return std::pair { "std::reduce(seq, long)", std::reduce(std::execution::seq, v.cbegin(), v.cend()) };
		});

		eval([&v]
		{
			return std::pair { "std::reduce(par, long)", std::reduce(std::execution::par, v.cbegin(), v.cend()) };
		});
	}
}

void Test_exclusive_inclusive_scan()
{
	std::vector<int> data { 3,1,4,1,5,9,2,6 };

	std::cout << std::left << std::setw(15) << "exclusive sum: ";
	std::exclusive_scan(data.begin(), data.end(), std::ostream_iterator<int>(std::cout, " "), 0);

	std::cout << std::left << std::setw(15) << "\ninclusive sum: ";
	std::inclusive_scan(data.begin(), data.end(), std::ostream_iterator<int>(std::cout, " "));

	std::cout << std::left << std::setw(15) << "\n\nexclusive product: ";
	std::exclusive_scan(data.begin(), data.end(), std::ostream_iterator<int>(std::cout, " "), 1, std::multiplies<int>{});

	std::cout << std::left << std::setw(15) << "\ninclusive product: ";
	std::inclusive_scan(data.begin(), data.end(), std::ostream_iterator<int>(std::cout, " "), std::multiplies<int>{}, 1);
}


void Test_transform_reduce()
{
	std::vector<double> xvalues(10007, 1.0), yvalues(10007, 1.0);

	double result = std::transform_reduce(std::execution::par, xvalues.begin(), xvalues.end(), yvalues.begin(), 0.0);

	std::cout << result << '\n';

	auto rop = [](auto const& a, auto const& b)
	{
		return a + b + 0.02;
	};

	auto top = [](auto const& a, auto const& b)
	{
		return a + b + 0.02;
	};

	double result1 = std::transform_reduce(std::execution::par, xvalues.begin(), xvalues.end(), yvalues.begin(), 0.0, rop, top);

	std::cout << result1 << '\n';
}

void Test_tramsform_exclusive_inclusive_scan()
{
	std::vector<int> data { 3,1,4,1,5,9,2,6 };

	auto times10 = [](int x)
	{
		return x * 10;
	};

	std::cout << std::left << std::setw(30) << "10 times exclusive sum: ";
	std::transform_exclusive_scan(data.begin(), data.end(), std::ostream_iterator<int>(std::cout, " "), 0, std::plus<int>{}, times10);

	std::cout << std::left << std::setw(30) << "\n10 times inclusive sum: ";
	std::transform_inclusive_scan(data.begin(), data.end(), std::ostream_iterator<int>(std::cout, " "), std::plus<int>{}, times10, 77);
}


/*=============================================================*/
/*          Operations on uninitialized memory                 */
/*=============================================================*/

void Test_uninitialized_copy()
{
	char const* v[] = { "this", "is", "an", "example" };

	auto sz = std::size(v);

	if (void* pbuf = std::calloc(sz, sizeof(std::string)))
	{
		try
		{
			auto first = static_cast<std::string*>(pbuf);
			auto last = std::uninitialized_copy(std::begin(v), std::end(v), first);

			for (auto it = first; it != last; it++)
			{
				std::cout << *it << "_";
			}
			std::cout << '\n';

			std::destroy(first, last);
		}
		catch (...)
		{
		}
		std::free(pbuf);
	}
}



#if 0

#define _SILENCE_CXX17_TEMPORARY_BUFFER_DEPRECATION_WARNING
#pragma warning(disable : 4996)
void Test_uninitialized_copy_n()
{
	std::vector<std::string> v = { "This", "is", "an", "example" };

	std::string* p;
	std::size_t sz;
	std::tie(p, sz) = std::get_temporary_buffer<std::string>(v.size());
	sz = std::min(sz, v.size());

	std::uninitialized_copy_n(v.begin(), sz, p);

	for (std::string* i = p; i != p + sz; i++)
	{
		std::cout << *i << ' ';
		i->~basic_string<char>();
	}
	std::return_temporary_buffer(p);
}


void Test_uninitialized_copy_n2()
{
	std::vector<std::string> v = { "This", "is", "an", "example" };

	std::string* p;
	std::size_t sz;
	std::tie(p, sz) = std::get_temporary_buffer<std::string>(v.size());
	sz = std::min(sz, v.size());

	std::string* last = std::uninitialized_copy_n(v.begin(), sz, p);

	for (std::string* i = p; i != last; i++)
	{
		std::cout << *i << ' ';
		//i->~basic_string<char>();
	}
	std::destroy(p, last);
	std::return_temporary_buffer(p);
}



void Test_uninitialized_fill()
{
	std::string* p;
	std::size_t sz;
	std::tie(p, sz) = std::get_temporary_buffer<std::string>(4);

	std::uninitialized_fill(p, p + sz, "Example");

	for (std::string* i = p; i != p + sz; i++)
	{
		std::cout << *i << '\n';
		i->~basic_string<char>();
	}
	std::return_temporary_buffer(p);
}

void Test_uninitialized_fill_n()
{
	std::string* p;
	std::size_t sz;
	std::tie(p, sz) = std::get_temporary_buffer<std::string>(4);
	std::uninitialized_fill_n(p, sz, "Example");

	for (std::string* i = p; i != p + sz; i++)
	{
		std::cout << *i << '\n';
		i->~basic_string<char>();
	}
	std::return_temporary_buffer(p);
}



#endif





static void print13(std::string_view remark, std::string* first, std::string* last)
{
	for (std::cout << std::left << std::setw(22) << remark; first != last; first++)
		std::cout << std::left << std::setw(17) << std::quoted(*first);
	std::cout << '\n';
}

void Test_uninitialized_move()
{
	std::string in[] { "Home", "Work!" };

	print13("initially, in:", std::begin(in), std::end(in));

	if (constexpr auto sz = std::size(in); void* out = std::calloc(sz, sizeof(std::string)))
	{
		try
		{
			auto first { static_cast<std::string*>(out) };
			auto last { first + sz };
			std::uninitialized_move(std::begin(in), std::end(in), first);

			print13("after move, in:", std::begin(in), std::end(in));
			print13("after move, out:", first, last);

			std::destroy(first, last);
		}
		catch (...)
		{
			std::cout << "Exception!\n";
		}
		std::free(out);
	}
}


void Test_uninitialized_move_n()
{
	std::string in[] { "One", "Definition", "Rule" };
	print13("initially, in:", std::begin(in), std::end(in));

	if (constexpr auto sz = std::size(in); void* out = std::calloc(sz, sizeof(std::string)))
	{
		try
		{
			auto first {static_cast<std::string*>(out)};
			auto last { first + sz };
			std::uninitialized_move_n(std::begin(in), sz, first);

			print13("after move, in:", std::begin(in), std::end(in));
			print13("after move, out:", first, last);

			std::destroy(first, last);
		}
		catch (...)
		{
			std::cout << "Exception\n";
		}
		std::free(out);
	}
}


void Test_uninitialized_default_construct_construct_n()
{
	struct S
	{
		std::string m { "Default value" };
		S()
		{
			m = "default constructor";
		}
	};

	constexpr int n { 3 };

	alignas(alignof(S)) unsigned char mem[n * sizeof(S)];
	alignas(alignof(S)) unsigned char mem2[n * sizeof(S)];

	try
	{
		auto first { reinterpret_cast<S*>(mem) };
		auto last { first + n };

		std::uninitialized_default_construct(first, last);

		for (auto it { first }; it != last; it++)
		{
			std::cout << it->m << '\n';
		}
		std::destroy(first, last);

		//----------------------------------------------------
		
		S* first2 = reinterpret_cast<S*>(mem2);
		
		S* last2 = std::uninitialized_default_construct_n(first2, n);

		for (S*& it = first2; it != last2; it++)
			std::cout << it->m << '\n';

		std::destroy(first2, last2);

	}
	catch (...)
	{
		std::cout << "Exception!\n";
	}

	int v[] { 1,2,3,4 };
	const int original[] { 1,2,3,4 };

	std::uninitialized_default_construct(std::begin(v), std::end(v));  // if the destination is not uninitialized, it won't treat it as if it's uninitialized.

	for (int const& i : v)
		std::cout << i << ' ';
	std::cout << '\n';

	std::cout << (std::memcmp(v, original, sizeof(v)) == 0 ? "Unmodified\n" : "Modified\n");

	std::uninitialized_default_construct_n(std::begin(v), std::size(v));  // if the destination is not uninitialized, it won't treat it as if it's uninitialized.

	for (int const& i : v)
		std::cout << i << ' ';
	std::cout << '\n';

	std::cout << (std::memcmp(v, original, sizeof(v)) == 0 ? "Unmodified\n" : "Modified\n");
}

void Test_uninitialized_value_construct_construct_n()
{
	struct S
	{
		std::string m { "Default value" };
		S()
		{
			m = "Default constructor";
		}
	};

	constexpr int n { 3 };
	alignas(alignof(S)) unsigned char mem[n * sizeof(S)];

	auto first { reinterpret_cast<S*>(mem) };
	auto last { first + n };

	std::uninitialized_value_construct(first, last);

	for (S*& it = first; it != last; it++)
		std::cout << it->m << '\n';

	std::destroy(first, last);


	alignas(alignof(S)) unsigned char mem2[n * sizeof(S)];

	S* head = reinterpret_cast<S*>(mem2);

	S* tail = std::uninitialized_value_construct_n(head, n);

	for (S*& it = head; it != tail; it++)
		std::cout << it->m << '\n';
	
	std::destroy(head, tail);



	int v[] { 1,2,3,4 };
	
	for (int& i : v)
		std::cout << i << ' ';
	std::cout << '\n';

	std::uninitialized_value_construct(std::begin(v), std::end(v));

	for (int& i : v)
		std::cout << i << ' ';
	std::cout << '\n';

	v[0] = 1; v[1] = 2; v[2] = 3; v[3] = 4;
	std::uninitialized_value_construct_n(std::begin(v), std::size(v));
	for (int& i : v)
		std::cout << i << ' ';
	std::cout << '\n';
}


void Test_destroy()
{
	struct S
	{
		int value;
		~S()
		{
			std::cout << value << "  destructed\n";
			// Note that adding this output line makes S non-trivially destructible
		}
	};

	alignas(alignof(S)) unsigned char buf[sizeof(S) * 8];

	for (int i = 0; i < 8; i++)
		new(buf + sizeof(S) * i)S { i };

	auto ptr = std::launder(reinterpret_cast<S*>(buf));

	// there is an optimization for debug code, 
	// if the struct/class is trivially destructible, destroy won't call their destructor

	std::destroy(ptr, ptr + 8);    // must not use "buf" and "buf+8"

	//std::destroy_n(ptr, 8);

	//for (int i = 0; i < 8; i++)
	//	std::destroy_at(ptr + i);

	/*
	S* it = ptr;
	while (it != ptr + 8)
	{
		it->~S();
		it++;
	}
	*/
}

#if 0
void Test_construct_at()  // C++20
{
	struct S
	{
		int x;
		float y;
		double z;

		S(int x, float y, double z) :x(x), y(y), z(z)
		{
			std::cout << "S::S();\n";
		}
		~S()
		{
			std::cout<<"S::~S();\n"
		}
		void print() const
		{
			std::cout << "S { x=" << x << "; y= " << y << "; z= " << z << "; };\n";
		}
	};

	alignas(S) unsigned char storage[sizeof(S)];

	S* ptr = std::construct_at(reinterpret_cast<S*>(storage), 42, 2.718, 3.14);
	ptr->print();

	std::destroy_at(ptr);
}
#endif




}