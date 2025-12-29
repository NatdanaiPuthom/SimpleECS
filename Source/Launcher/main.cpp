#include "External/SimpleUtilities/MemoryTracking/MemoryTracker.hpp"
#include "ECS/MemoryPool/MemoryPool.hpp"
#include "ECS/Components.hpp"
#include "ECS/ECSRegistry.hpp"
#include "ECS/EntityManager.hpp"

using namespace Simple;

int main()
{
	{
		[[maybe_unused]] MemoryPool emilPool = MemoryPool::CreatePool<EmilTest>();
		emilPool.PrintMemoryStatus();
		[[maybe_unused]] size_t id1 = emilPool.CreateObject();
		[[maybe_unused]] EmilTest* emilComponent1 = emilPool.GetObjectAtIndex<EmilTest>(id1);
		emilComponent1->value1 = 99;
		emilComponent1->value2 = 100;
		[[maybe_unused]] size_t id2 = emilPool.CreateObject();
		[[maybe_unused]] size_t id3 = emilPool.CreateObject();
		emilPool.PrintMemoryStatus();
	}


	//[[maybe_unused]] MemoryPool memoryPool(sizeof(EmilTest), alignof(EmilTest), typeid(EmilTest).hash_code(), 1);
	//memoryPool.PrintMemoryStatus();

	//[[maybe_unused]] bool success1 = ECSRegistry::GetInstance()->Register<EmilTest>();

	//EntityManager entityManager;

	//Entity& entity1 = entityManager.CreateEntity();
	//entity1;

	//Entity& entityCopy = entityManager.GetEntity(0);
	//entityCopy;

	////entityManager.AddComponent<EmilTest>()

	//ECSRegistry::GetInstance()->Destroy();

	return 0;
}
