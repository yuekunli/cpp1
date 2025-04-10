#include<Windows.h>
#include<ActiveDS.h>
#include<AdsHlp.h>  // must put windows.h and activeds.h before adshlp.h
#include<guiddef.h>

#include<string>
namespace ADS_TEST {

	void test1()
	{
		std::wstring a = L"test-test";
		IID* b = NULL;
		void* p = (void*)malloc(30);

		//IADs* pObject;

		b = (IID*)malloc(sizeof(IID));

		HRESULT hr = ADsGetObject(L"LADP://CN=jeffsmith", *b, &p);

	}

}