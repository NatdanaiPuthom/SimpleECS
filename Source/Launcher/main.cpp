#include "External/SimpleUtilities/MemoryTracking/MemoryTracker.hpp"

#include "Engine/ECS/MemoryPool/MemoryPool.hpp"
#include "Engine/ECS/Component/TestComponents.hpp"
#include "Engine/Debugger/Console/Console.hpp"

#include "Engine/ECS/EntityManager.hpp"

using namespace Simple;

int main()
{
	Console console;
	console.Init();

	[[maybe_unused]]TypeErasureComponent emilTest = ECSRegistry::GetInstance()->GetTypeErasureComponent<EmilTest>();	
	[[maybe_unused]] const auto& it = ECSRegistry::GetInstance()->GetRegisteredComponents();

	EntityManager entityManager;
	entityManager.AddComponent<EmilTest>();

	EmilTest emilCopyStruct;
	emilCopyStruct.value1 = 99;
	emilCopyStruct.value2 = 199;

	MemoryPool pool(emilTest.identity, 8);

	size_t index1 = pool.CreateObject();
	[[maybe_unused]] EmilTest* emilStruct1 = pool.GetObjectAtIndex<EmilTest>(index1);

	size_t index2 = pool.CreateObject(&emilCopyStruct);
	[[maybe_unused]] EmilTest* emilStruct2 = pool.GetObjectAtIndex<EmilTest>(index2);

	ECSRegistry::GetInstance()->Destroy();

	return 0;
}
