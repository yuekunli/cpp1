#include<vector>
#include<string>
#include<iostream>
#include<set>
#include<iterator>


namespace SetLowerBound1 {


struct Building
{
public:


	// a constructor with such signature: Building(int id, std::string name) will always match the call site's invocation
	// no matter what call site passes in. (No matter if call site passes in lvalues or rvalues)



	Building(int id, std::string& name):
		_id(id),
		_name(name)
	{
		std::cout << "Using copy constructor" << std::endl;
	}


	Building(int id, std::string&& name) :
		_id(id),
		_name(std::move(name))   // Do I need to std::move the name here? 
		                         // looks like I don't need to if I put const in the signature like this const std::string&& name
	{
		std::cout << "Using move constructor" << std::endl;
	}


	int get_id() const
	{
		return _id;
	}

	std::string get_name() const
	{
		return _name;
	}

private:
	int _id;
	std::string _name;

};



struct CustomCmp
{
	bool operator() (const Building& b1, const Building& b2) const
	{
		return b1.get_id() < b2.get_id();
	}

	bool operator() (const Building& b1, int id) const
	{
		return b1.get_id() < id;
	}

	bool operator() (int id, const Building& b2) const
	{
		return id < b2.get_id();
	}

	

	//typedef int is_transparent ;   // this also works

	using is_transparent = void;

};


void Test_SetLowerBound1()
{
	std::set<Building, CustomCmp> campus { Building(1, std::string("office")), 
		Building(2, std::string("supply")), Building(3, std::string("cafe")), 
		Building(4, std::string("showcase")), Building(5, std::string("museum")) };

	std::set<Building, CustomCmp>::iterator it = campus.lower_bound<int>(2);

	//std::set<Building, CustomCmp>::iterator it = campus.find(2);

	std::cout << it->get_name() << std::endl;
}



}