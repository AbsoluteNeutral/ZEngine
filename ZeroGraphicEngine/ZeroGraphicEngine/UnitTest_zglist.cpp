#include "stdafx.h"
#include "UnitTest.h"

namespace zg
{
	void listTest_001() 
	{
		TestLog(1, "");
		list<int> tmp{};
		tmp.push_back(1);	tmp.print();
		tmp.push_back(2);	tmp.print();
		tmp.push_back(3);	tmp.print();
		tmp.push_back(4);	tmp.print();
		tmp.pop_back();		tmp.print();
		tmp.pop_back();		tmp.print();
		tmp.pop_back();		tmp.print();

		tmp.push_back(1);	tmp.print();
		tmp.push_back(2);	tmp.print();
		tmp.push_back(3);	tmp.print();
		tmp.push_back(4);	tmp.print();
		Logg("Front Value: %d", tmp.front());
		tmp.reverse();		tmp.print();
		Logg("Front Value: %d", tmp.front());
		tmp.clear();		tmp.print();
	}

	void listTest_002()
	{
		TestLog(1, "");
		list<int> tmp{};
		tmp.push_front(1);	tmp.print();
		tmp.push_front(2);	tmp.print();
		tmp.push_front(3);	tmp.print();
		tmp.push_front(4);	tmp.print();
		tmp.pop_front();	tmp.print();
		tmp.pop_front();	tmp.print();
		tmp.pop_front();	tmp.print();
	}

	void listTest_003()
	{
		TestLog(1, "");	
		list<int> tmp{};
	}

	void UnitTest_list(UnitTest* unittest_)
	{
		UnitTestVector& vec = unittest_->list;
		if (unittest_->typeindex != _UTlist)
		{
			unittest_->typeindex = _UTlist;
			vec.clear();

			vec.push_back(listTest_001);
			vec.push_back(listTest_002);
			vec.push_back(listTest_003);
		}
		
		for (auto& obj : vec)
			obj();
	}
}