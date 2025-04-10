#include<string>
#include<iostream>
#include<utility>


namespace MemberInitializerListAndStdMove1 {


// This proves that even just for integer, lvalue reference and rvalue reference are different
struct A
{
	A(int& id) :
		_id(id)
	{
		std::cout << "lvalue ref      " << _id << std::endl;
	}

	A(int&& id) :
		_id(id)
	{
		std::cout << "rvalue ref      " << _id << std::endl;
	}

	int _id;
};


// ------------------------------------------------------------ //




struct BookCover
{
	BookCover(int& id) :
		_id(id)
	{
		std::cout << "BookCover lvalue ref      " << _id << std::endl;
	}

	BookCover(int&& id) :
		_id(id)
	{
		std::cout << "BookCover rvalue ref      " << _id << std::endl;
	}


	BookCover(const int& id) :
		_id(id)
	{
		std::cout << "BookCover const lvalue ref      " << _id << std::endl;
	}


	int _id;
};


struct Book
{
	Book(int&& id, std::string&& name) :
		//_cover_info(id),                   // this invokes BookCover(int& id)
		                                     // even if we add const to Book's constructor like this: Book(const int&& id, std::string&& name)
		                                     // it'll invoke BookCover(const int& id)
		//_cover_info(std::move(id)),        // this invokes BookCover(int&& id)
		_cover_info(std::forward<int>(id)),  // this invokes BookCover(int&& id)
		_name(name)
	{

	}


	std::string _name;
	BookCover _cover_info;

};




void Test_MemberInitializerListAndStdMove1()
{
	A obj1 { 4 };

	int t = 9;

	A obj2 { t };


	// ----------------------------- //


	std::string s { "Wuthering Heights" };


	Book b { 19, std::move(s) };


}




}