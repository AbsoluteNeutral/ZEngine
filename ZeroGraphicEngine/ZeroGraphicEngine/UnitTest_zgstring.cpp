#include "stdafx.h"
#include "UnitTest.h"

namespace zg
{
	void simplestringTest_001() 
	{
		string tmp{ "ABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()" };
		string tmp2{ tmp };

		TestLog(1, "Init CTOR, Copy CTOR %s %s", tmp.cstr(), tmp2.cstr());
		TestLog(1, "Front, Back, AT %c %c %c", tmp.front(), tmp.back(), tmp.at(5));

		tmp2 = tmp;
		tmp2 = tmp2;
		TestLog(1, "Copy operator= %s %s", tmp.cstr(), tmp2.cstr());
	}

	void simplestringTest_002()
	{
		string tmp{ "ABCD" };
		tmp += "ASDEI*$R()";
		tmp += "   SSSSS";
		TestLog(1, "operator+= %s", tmp.cstr());

	}
	void simplestringTest_003()
	{
		
	}

	void UnitTest_simplestring(UnitTest* unittest_)
	{
		UnitTestVector& vec = unittest_->list;
		if (unittest_->typeindex != _UTstring)
		{
			unittest_->typeindex = _UTstring;
			vec.clear();

			vec.push_back(simplestringTest_001);
			vec.push_back(simplestringTest_002);
			vec.push_back(simplestringTest_003);
		}
		
		for (auto& obj : vec)
			obj();
	}
}