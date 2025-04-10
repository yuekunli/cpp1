#include "../LibHeaders.h"

namespace ContainerLib1 {

/*------------- vector -------------*/


void Test_vector_basic_apis()
{
	std::vector<int> v;
	std::cout << "capacity      size      distance\n";

	auto print = [](std::vector<int>& v)
	{
		std::cout << std::left << std::setw(8) << v.capacity() 
			<< std::left << std::setw(8) << v.size() 
			<< std::left << std::setw(8) << std::distance(v.begin(), v.end())
			<<'\n';
	};

	print(v);

	v.reserve(10);

	print(v);

	v.resize(20);

	print(v);

	std::vector<int> v2(30);
	print(v2);
	
}


/*------------- sequential container in general -------------*/

template<typename Container>
static void removeEveryOtherItem(Container& c)
{
	//auto itr = c.begin();   // this works

	//Container::iterator itr = c.begin();  // this doesn't compile

	typename Container::iterator itr = c.begin();  // this works


	while (itr != c.end())
	{
		itr = c.erase(itr);   // erase returns the iterator next to the one that is just deleted
		if (itr != c.end())
		{
			itr++;
		}
	}
}


void Test_RemoveEveryOtherInSeqContainer()
{
	std::list<int> lst{ 1,2,3,4,5,6,7,8,9,10 };

	for (int& i : lst)
	{
		std::cout << i << "  ";
	}

	std::cout << std::endl;

	removeEveryOtherItem(lst);

	for (int& i : lst)
	{
		std::cout << i << "  ";
	}

	std::cout << std::endl;

}


/*------------- set -------------*/

void Test_set_lower_bound()
{
	std::set<int, std::less<>> s1 = { {1}, {2}, {3} };

	auto p = s1.lower_bound(2.5);
	// because std::less::is_transparent is defined, 
	// I can pass a float number to lower_bound whereas the elements in the set belong to type "int"

	std::cout << *p << '\n';

}

// verify "set" data structure is ordered
void Test_set_is_ordered()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<> dis(1, 30);

	std::set<int> s;

	std::cout << s.size() << '\n';

	for (int i = 0; i < 10; i++)
	{
		int n = dis(mt);
		std::cout << n << "  ";
		s.emplace(n);
	}
	std::cout << '\n';

	std::cout << s.size() << '\n';

	std::copy(s.begin(), s.end(), std::ostream_iterator<int>(std::cout, "  ")); // it's in sorted order

}

/*------------- unordered_set -------------*/


void Test_unordered_set1()
{
	struct pair_hash
	{
		std::size_t operator() (const std::pair<int, int>& v) const
		{
			return v.first * 10 + v.second;
		}
	};

	std::unordered_set<std::pair<int, int>, pair_hash> s;

	s.emplace(1, 2);

	std::vector<std::pair<int, int>> v { {3,4},{5,6} };

	s.insert(v[0]);
	s.insert(v[1]);

	s.erase(v[0]);
	//std::pair<int, int> a = std::make_pair<int, int>(v[1].first, v[2].second);
	//s.erase(a);

	//s.erase(std::make_pair<int, int>(v[1].first, v[2].second));
	s.erase({ v[1].first, v[1].second });

}


// Can the std::hash be specialized for a simple struct?
void Test_unordered_set_hash_function_specialization()  
{
	struct A
	{
		int s;
		int t;
	};
	struct Comp
	{
		bool operator()(A const& a, A const& b) const
		{
			return a.s == b.s && a.t == b.t;
		}

	};
	struct AHash
	{
		size_t operator()(A const& a) const   // the "const" at the end is necessary
		{
			return a.s * 100 + a.t;
		}
	};

	A q[3] = { {3, 9},{13, 61},{55, 27} };

	//std::unordered_set < A, std::hash<A>, Comp > s; // doesn't quite work
	std::unordered_set<A, AHash, Comp> s;
	// must not be std::unordered_set<A, AHash(), Comp()> s
	// I need "types" inside angle brackets not instances of those types
	s.insert(q[0]);
	s.insert(q[1]);
	s.insert(q[2]);
	std::cout << s.count(q[2]) << "\n";
}



/*------------- map -------------*/


void Test_ordered_map()
{
	std::map<int, char>m;
	m[1] = 'a';
	m[5] = 'b';

	auto it = m.lower_bound(3);
	it->second = 'x';   // verify that the iterator (returned by lower_bound in "map") points to a pair and the values of the pair are references.
	//it->first = 4;  // the "first" of the pair is the key in the map, I can't modify it.

	for (auto& p : m)
	{
		std::cout << p.first << "  " << p.second << '\n';
	}
	std::cout << "\n\n";
}



/*------------- stack -------------*/

class BrainHackInterpreter
{
	std::map<unsigned, unsigned> 
		open_brackets,     // key: open bracket position, value: close bracket position 
		close_brackets;    // key: close bracket position, value: open bracket position
	unsigned program_pos_ { 0 };
	std::array<std::uint8_t, 8> data_;
	int data_pos_ { 0 };

	void collect_brackets_positions(std::string_view const program)
	{
		std::stack<unsigned> brackets_stack;

		for (auto pos { 0U }; pos != program.length(); pos++)
		{
			char const c { program[pos] };
			if ('[' == c)
			{
				brackets_stack.push(pos);
			}
			else if (']' == c)
			{
				if (brackets_stack.empty())
				{
					throw std::runtime_error("brackets [] do not match");
				}
				else
				{
					open_brackets[brackets_stack.top()] = pos;
					close_brackets[pos] = brackets_stack.top();
					brackets_stack.pop();
				}
			}
		}

		if (!brackets_stack.empty())
		{
			throw std::runtime_error("brackets [] do not match");
		}
	}

	void check_data_pos(int pos)
	{
		if (pos < 0 or pos >= static_cast<int>(data_.size()))
			throw std::out_of_range("data pointer out of bound");
	}

public:

	BrainHackInterpreter(std::string_view const program)
	{
		collect_brackets_positions(program);
		data_.fill(0);

		for (; program_pos_ < program.length(); program_pos_++)
		{
			switch (program[program_pos_])
			{
				case '<':
					check_data_pos(--data_pos_);
					break;
				case '>':
					check_data_pos(++data_pos_);
					break;
				case '-':
					--data_[data_pos_];
					break;
				case '+':
					++data_[data_pos_];
					break;
				case '.':
					std::cout << data_[data_pos_];
					break;
				case ',':
					std::cin >> data_[data_pos_];
					break;
				case '[' :
					if (data_[data_pos_] == 0)
						program_pos_ = open_brackets[program_pos_];
					break;
				case ']' :
					if (data_[data_pos_] != 0)
						program_pos_ = close_brackets[program_pos_];
					break;
			}
		}
	}
};


void Test_stack()
{
	BrainHackInterpreter test {
		"++++++++[>++>>++>++++>++++<<<<<-]>[<+++>>+++<-]>[<+"
		"+>>>+<<-]<[>+>+<<-]>>>--------.<<+++++++++.<<----.>"
		">>>>.<<<------.>..++.<++.+.-.>.<.>----.<--.++.>>>+."
	};
}



void Test_stack_emplace()
{
	struct S
	{
		int id;
		S(int i, double d, std::string s) : id(i)
		{
			std::cout << "S::S(" << i << ", " << d << ", \"" << s << "\");\n";
		}
	};

	std::stack<S> adaptor;

	const S& s = adaptor.emplace(42, 3.14, "C++");

	std::cout << "id = " << s.id << '\n';
}


/*--------- Priority Queue --------*/


struct Event
{
	int priority {};
	char data { ' ' };

	friend bool operator<(Event const& lhs, Event const& rhs)
	{
		return lhs.priority < rhs.priority;
	}

	friend std::ostream& operator<<(std::ostream& os, Event const& e)
	{
		return os << "{ " << e.priority << ", '" << e.data << "' } ";
	}
};

void Test_priority_queue()
{
	std::priority_queue<Event> events;

	std::cout << "Fill the events queue:\n";

	//for (auto const e : { Event { 6, 'L' }, { 8,'I' }, { 9,'S' }, { 1,'T' }, { 5,'E' }, { 3,'N' } })
	std::vector<Event> init { { 6, 'L' }, { 8,'I' }, { 9,'S' }, { 1,'T' }, { 5,'E' }, { 3,'N' } };
	for (auto const & e: init)
	{
		std::cout << e << ' ';
		events.push(e);
	}

	std::cout << "\nProcess events:\n";

	for (; !events.empty(); events.pop())
	{
		Event const& e = events.top();
		std::cout << e << ' ';
	}

	std::cout << '\n';
}











}