#include "UnitTest/pch.h"
#include "CppUnitTest.h"

#include "Engine/ECS/MemoryPool/MemoryPool.hpp"
#include "Engine/ECS/Component.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Simple;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:

		TEST_METHOD(Constructor)
		{
			const int expectedValue = 100;

			struct Dummy : Component
			{
				Dummy() : value(expectedValue) {}

				int value = 12;
			};


			MemoryPool pool = MemoryPool::CreatePool<Dummy>();
			size_t id = pool.CreateObject();

			Dummy* dummy = pool.GetObjectAtIndex<Dummy>(id);

			Assert::AreEqual(expectedValue, dummy->value);
		}

		TEST_METHOD(Destructor)
		{
			bool destructorIsCalled = false;
			const bool expectedValue = true;

			struct Dummy : Component
			{
				bool* flag = nullptr;

				~Dummy()
				{
					*flag = true;
				}
			};

			{
				MemoryPool pool = MemoryPool::CreatePool<Dummy>();
				const size_t id = pool.CreateObject();

				Dummy* dummy = pool.GetObjectAtIndex<Dummy>(id);
				dummy->flag = &destructorIsCalled;
				*dummy->flag = true;
			}

			Assert::AreEqual(expectedValue, destructorIsCalled);
		}
	};
}
