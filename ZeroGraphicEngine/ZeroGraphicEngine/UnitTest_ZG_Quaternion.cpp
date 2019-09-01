#include "stdafx.h"
#include "UnitTest.h"

namespace zg
{
	void QuaternionTest_001() 
	{
		Quaternion q{};
		bool getbool = (q.x == 0.0f && q.y == 0.0f && q.z == 0.0f && q.w == 1.0f);
		TestLog(getbool, "Init CTOR");


	}
	void QuaternionTest_002()
	{

	}
	void QuaternionTest_003()
	{

	}

	void UnitTest_ZG_Quaternion(UnitTest* unittest_) 
	{
		UnitTestVector& vec = unittest_->list;
		if (unittest_->typeindex != _UTQuaternion)
		{
			unittest_->typeindex = _UTQuaternion;
			vec.clear();

			vec.push_back(QuaternionTest_001);
			vec.push_back(QuaternionTest_002);
			vec.push_back(QuaternionTest_003);

		}
		
		for (auto& obj : vec)
			obj();
	}
}