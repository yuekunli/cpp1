
namespace ConstPointer1 {

void Test_ConstPointer1()
{
	int a1 = 90;
	int a2 = 80;

	const int* p = &a1;
	p = &a2;
}
}