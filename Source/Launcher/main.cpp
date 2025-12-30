#include "External/SimpleUtilities/MemoryTracking/MemoryTracker.hpp"
//#include "External/ECS/ECSRegistry.hpp"
//#include "External/ECS/EntityManager.hpp"

#include "Engine/ECS/MemoryPool/MemoryPool.hpp"
#include "Engine/ECS/Components.hpp"

using namespace Simple;

int main()
{
	[[maybe_unused]] MemoryPool emilPool = MemoryPool::CreatePool<EmilTest>();

	/*
	emilPool.PrintMemoryStatus();
	[[maybe_unused]] size_t id1 = emilPool.CreateObject();
	[[maybe_unused]] EmilTest* emilComponent1 = emilPool.GetObjectAtIndex<EmilTest>(id1);
	emilComponent1->value1 = 99;
	emilComponent1->value2 = 100;
	[[maybe_unused]] size_t id2 = emilPool.CreateObject();
	[[maybe_unused]] size_t id3 = emilPool.CreateObject();
	emilPool.PrintMemoryStatus();*/

	return 0;
}
