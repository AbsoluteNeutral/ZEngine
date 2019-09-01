#include "stdafx.h"
#include "UnitTest.h"

namespace zg 
{
	ZG_API UnitTest UnitTestSite{};

	UnitTest::UnitTest()
		:typeindex(_UTNone)
		, list{}
	{}

	UnitTest::~UnitTest()
	{
		typeindex = _UTNone;
		Clear();
	}

	void UnitTest::Clear() {
		list.clear();
	}
}