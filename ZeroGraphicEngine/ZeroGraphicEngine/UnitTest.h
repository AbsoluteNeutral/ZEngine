#ifndef _UNITSTEST
#define _UNITSTEST

#include <typeinfo>
#include "ZGMath.h"
#include "zgstring.h"
#include "zglist.h"

namespace zg 
{
	typedef std::vector<void(*)()> UnitTestVector;
	class ZG_API UnitTest;

	void UnitTest_ZG_Vector2(UnitTest* unittest_);
	void UnitTest_ZG_Vector3(UnitTest* unittest_);
	void UnitTest_ZG_Vector4(UnitTest* unittest_);
	void UnitTest_ZG_Quaternion(UnitTest* unittest_);

	void UnitTest_simplestring(UnitTest* unittest_);
	void UnitTest_list(UnitTest* unittest_);

	enum _ENUM_UNITTEST {
		_UTVector3 = 0,
		_UTQuaternion,
		_UTstring,
		_UTlist,

		_UTNone
	};

	class ZG_API UnitTest
	{
	public:
		UnitTest();
		~UnitTest();
	
		void Run(_ENUM_UNITTEST whichtest_) 
		{
			switch (whichtest_) {
			case _UTVector3:
				UnitTest_ZG_Vector3(this);
				break;
			case _UTQuaternion:
				UnitTest_ZG_Quaternion(this);
				break;
			case _UTstring:
				UnitTest_simplestring(this);
				break;
			case _UTlist:
				UnitTest_list(this);
				break;
			}
		}
		void RunAll() 
		{
			//UnitTest_ZG_Vector3(this);
			//UnitTest_ZG_Quaternion(this);
			//UnitTest_simplestring(this);
			UnitTest_list(this);
		}
		void Clear();
	
		_ENUM_UNITTEST typeindex;
		UnitTestVector list;
	};
	ZG_API extern UnitTest UnitTestSite;

} // namespace zg



#endif //_UNITSTEST
