#include "External/SimpleUtilities/MemoryTracking/MemoryTracker.hpp"
#include "ECS/MemoryPool/MemoryPool.hpp"
#include "ECS/Components.hpp"
#include "ECS/ECSRegistry.hpp"
#include "ECS/EntityManager.hpp"

using namespace Simple;

int main()
{
	MemoryPool memoryPool(sizeof(EmilTest), alignof(EmilTest), typeid(EmilTest).hash_code(), 1);
	memoryPool.PrintMemoryStatus();

	bool success1 = ECSRegistry::GetInstance()->Register<EmilTest>();
	success1;

	EntityManager entityManager;

	Entity& entity1 = entityManager.CreateEntity();
	entity1;

	Entity& entityCopy = entityManager.GetEntity(0);
	entityCopy;

	//entityManager.AddComponent<EmilTest>()

	ECSRegistry::GetInstance()->Destroy();

	return 0;
}