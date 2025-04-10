#include<iostream>
#include<list>
#include<fstream>
#include<vector>
#include<assert.h>
#include<memory>
#include<functional>


namespace UniquePtrDemo1 {
struct B
{
	virtual void bar()
	{
		std::cout << "B::bar\n";
	}

	virtual ~B() = default;
};

struct D : B
{
	D()
	{
		std::cout << "D::D\n";
	}

	~D()
	{
		std::cout << "D::~D\n";
	}

	void bar() override
	{
		std::cout << "D::bar\n";
	}
};

std::unique_ptr<D> pass_through(std::unique_ptr<D> p)
{
	p->bar();
	return p;
}

void close_file(std::FILE* fp)
{
	std::fclose(fp);
}


struct List
{
	struct Node
	{
		int data;
		std::unique_ptr<Node> next;
		Node(int data)
			: data { data }, next { nullptr }
		{

		}
	};

	List()
		: head { nullptr }
	{

	}

	~List()
	{
		while (head)
		{
			head = std::move(head->next);
		}
	}

	void push(int data)
	{
		auto temp = std::make_unique<Node>(data);
		if (head)
			temp->next = std::move(head);
		head = std::move(temp);
	}

private:
	std::unique_ptr<Node> head;
};


void Test_UniquePtrDemo1()
{
	std::cout << "unique ownership semantics demo\n";
	{
		auto p = std::make_unique<D>();
		auto q = pass_through(std::move(p));
		assert(!p);
		q->bar();
	}

	std::cout << "Runtime polymorphism demo\n";
	{
		std::unique_ptr<B> p = std::make_unique<D>();
		p->bar();

		std::vector<std::unique_ptr<B>> v;
		v.push_back(std::make_unique<D>());
		v.push_back(std::move(p));
		v.emplace_back(new D);
		for (auto& p : v)
		{
			p->bar();
		}
	}

	std::cout << "Custom deleter demo\n";

	std::ofstream("demo.txt") << 'x';
	{
		//std::FILE* fp1;
		//std::fopen_s(fp1, "demo.txt", "r");
#pragma warning(suppress:4996)
		std::unique_ptr<std::FILE, decltype(&close_file)> fp(std::fopen("demo.txt", "r") /*fp1*/, &close_file);

		if (fp)
		{
			std::cout << (char) std::fgetc(fp.get()) << '\n';
		}
	}

	std::cout << "Custom lambda-expression deleter demo\n";
	{
		std::unique_ptr<D, std::function<void(D*)>> p(new D, [](D* ptr)
		{
			std::cout << "destroying from a custom deleter...\n";
			delete ptr;
		});
		p->bar();
	}

	std::cout << "Array form of unique_ptr demo\n";
	{
		std::unique_ptr<D[]> p { new D[3] };
	}

	std::cout << "Linked list demo\n";
	{
		List l;
		for (long n = 0; n != 1'000'000; n++)
		{
			l.push(n);
		}
	}
}

}