#include "UnitTest/pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:

		TEST_METHOD(Construct)
		{
			Assert::AreEqual(1, 1);
		}
	};
}
