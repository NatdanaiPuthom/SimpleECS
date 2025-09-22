#include "External/SimpleUtilities/MemoryTracking/MemoryTracker.hpp"
#include "MemoryPool/MemoryPool.hpp"
#include <typeinfo>

struct EmilTest
{
	int value1 = 0;
	int value2 = 1;
};

int main()
{
	Simple::MemoryPool memoryPool(sizeof(EmilTest), alignof(EmilTest), typeid(EmilTest).hash_code(), 1);
	memoryPool.PrintMemoryStatus();
	return 0;
}