#include "stdafx.h"
#include "UnitTest.h"

namespace zg
{
	void Vector3Test_001() 
	{
		Vector3 v{};
		bool getbool = (v.x == 0.0f && v.y == 0.0f && v.z == 0.0f);
		TestLog(getbool, "Init Default CTOR to Zero");

		Vector3 v2{ 1.0f, 2.0f };
		getbool = (v2.x == 1.0f && v2.y == 2.0f && v2.z == 0.0f);
		TestLog(getbool, "Init 2nd CTOR");

		Vector3 v3{ 1.0f, 2.0f, 3.0f };
		getbool = (v3.x == 1.0f && v3.y == 2.0f && v3.z == 3.0f);
		TestLog(getbool, "Init 3nd CTOR");

		Vector3 v4{ zg::Vector2{451.0f, 342.0f } };
		getbool = (v4.x == 451.0f && v4.y == 342.0f && v4.z == 0.0f);
		TestLog(getbool, "Init explicit zg::Vector2 CTOR");

		Vector3 v5{ Vector4{ 133.0f, 153.0f, 4.0f, 0.0f } };
		getbool = (v5.x == 133.0f && v5.y == 153.0f && v5.z == 4.0f);
		TestLog(getbool, "Init explicit Vector4 CTOR");

		Vector3 v6{ v5 };
		getbool = (v6 == v5);
		TestLog(getbool, "Copy Ctor ");

		Vector3 v7{ Vector3{v5} };
		getbool = (v6 == v5);
		TestLog(getbool, "Move Ctor ");

		v = v5;
		getbool = (v == v5);
		TestLog(getbool, "copy operator= ");

		v = Vector3{ 1.0f, 203.0f };
		getbool = (v == Vector3{ 1.0f, 203.0f, 0.0f });
		TestLog(getbool, "move operator= ");

	}
	void Vector3Test_002()
	{
		Vector3 v1{};
		TestLog((v1.LengthSq() == 0.0f), "LengthSq(), zero");
		TestLog((v1.Length() == 0.0f), "Length(), zero");

		v1 = { -0.0f, -0.0f, -0.0f };
		TestLog((v1.LengthSq() == 0.0f), "LengthSq(), zero");
		TestLog((v1.Length() == 0.0f), "Length(), zero");

		v1 = { 1.0f, 1.0f, 1.0f };
		TestLog((v1.LengthSq() == 3.0f), "LengthSq()");
		TestLog((v1.Length() == sqrtf(3.0f)), "Length()");

		v1 = { 91.0f, 21.0f, 13.0f };
		TestLog((v1.LengthSq() == 8891.0f), "LengthSq()");
		TestLog((v1.Length() == sqrtf(8891)), "Length()");
	}
	void Vector3Test_003()
	{
		Vector3 v1{ 2, 152, 3 };
		Vector3 v2{ 55, 4, -10 };
		bool getbool = (v1 ^ v2) == Vector3{ -1532, 185, -8352 };
		TestLog(getbool, "Cross Product operator^");
		getbool = (v1.Cross(v2)) == Vector3{ -1532, 185, -8352 };
		TestLog(getbool, "Cross Product function");

		v1 = Vector3{ 0, -1, -0.0f };
		v2 = Vector3{ -1, 0.0f, 0 };
		getbool = (v1 ^ v2) == Vector3{ 0, 0, -1 };
		TestLog(getbool, "Cross Product operator^");
		getbool = (v1.Cross(v2)) == Vector3{ 0, 0, -1 };
		TestLog(getbool, "Cross Product function");
	}
	void Vector3Test_004()
	{
		Vector3 v1{ 0.0f, 0.0f, 0.0f };
		TestLog(v1.IsZero(), "IsZero()");
		v1 = { -0.0f, -0.0f, -0.0f };
		TestLog(v1.IsZero(), "IsZero()");
		v1 = { 0.0f, -0.0f, -0.0f };
		TestLog(v1.IsZero(), "IsZero()");
		v1 = { -0.0f, 0.0f, -0.0f };
		TestLog(v1.IsZero(), "IsZero()");
		v1 = { -0.0f, -0.0f, 0.0f };
		TestLog(v1.IsZero(), "IsZero()");
		v1 = { -0.01f, -0.0f, 0.0f };
		TestLog(!v1.IsZero(), "Not IsZero()");
		v1 = { 91.2f, 4.0f, 0.0f };
		TestLog(!v1.IsZero(), "Not IsZero()");
	}

	void UnitTest_ZG_Vector3(UnitTest* unittest_) 
	{
		UnitTestVector& vec = unittest_->list;
		if (unittest_->typeindex != _UTVector3)
		{
			unittest_->typeindex = _UTVector3;
			vec.clear();

			vec.push_back(Vector3Test_001);
			vec.push_back(Vector3Test_002);
			vec.push_back(Vector3Test_003);
			vec.push_back(Vector3Test_004);

		}
		
		for (auto& obj : vec)
			obj();
	}
}