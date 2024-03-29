#include "LibHeaders.h"

namespace CallFunctionInChildClass1 {

	class House;

	class Building
	{
	public:
		Building(int a, int b)
			:data_a(a),
			data_b(b)
		{
		}
		void GetInfo()
		{
			std::cout << "Building Info data_a  " << data_a << "       data_b  " << data_b << "\n";
		}
		void Func1()
		{
			std::cout << "Inside Func1\n";
			GetInfo();   // this calls Building's GetInfo function, and it doesn't call House's GetInfo afterward

			//House::GetInfo();  // this is compile error, this only legal if GetInfo is a static function in class House

			//((House*) this)->GetInfo(); // compile error: undefined type House. We're calling House's member function,
										// only declaring class House (on line 5) is not enough
										// We have to just declare Func1 here and move the implementation of Func1 to somewhere down below House's definition.

			//static_cast<House*>(this)->GetInfo(); // compile error: cannot convert from Building to House
			//reinterpret_cast<House*>(this)->GetInfo(); // compile error: use of undefined type. I think the cast is OK, but House's GetInfo is unknown at this point.
		}

		void Func2();

	private:
		int data_a;
		int data_b;
	};

	class House : public Building
	{
	public:
		House(int a, int b) :
			Building(a, b),
			hdata_a(a),
			hdata_b(b)
		{
		}
		void GetInfo()
		{
			std::cout << "House Info hdata_a  " << hdata_a << "      hdata_b  " << hdata_b << "\n";
		}
	private:
		int hdata_a;
		int hdata_b;
	};



	void Building::Func2()
	{
		((House*) this)->GetInfo(); // This works, this is forced type cast, compiler doesn't care whether "this" can really be converted to House*
	}



	void Test_CallFunctionInChildClass1()
	{
		House houseABC = House(19, 53);
		houseABC.Func1();
		houseABC.Func2();

		Building B1 { 51, 61 };
		B1.Func2();  // this still compiles fine, it will force convert "this" to House*, we end up getting garbage for hdata_a and hdata_b
	}

}