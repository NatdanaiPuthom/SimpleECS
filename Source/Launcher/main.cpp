#include "External/SimpleUtilities/MemoryTracking/MemoryTracker.hpp"

#include "Engine/Debugger/Console/Console.hpp"

#include "Engine/ECS/ECSRegistry.hpp"
#include "Engine/ECS/EntityComponentSystem.hpp"

#include "Engine/ECS/Component/TestComponents.hpp"

using namespace Simple;

int main()
{
	Console console;
	console.Initialize();

	EntityComponentSystem ecs;
	ecs.Initialize();

	[[maybe_unused]] Entity& newEntity1 = ecs.CreateEntity();
	[[maybe_unused]] Entity& newEntity2 = ecs.CreateEntity();
	[[maybe_unused]] Entity& newEntity3 = ecs.CreateEntity();
	[[maybe_unused]] Entity& newEntity4 = ecs.CreateEntity();

	//ecs.DestroyEntity(newEntity2);

	//[[maybe_unused]] EmilTest* newComponent = ecs.AddComponent<EmilTest>(newEntity1);

	std::bitset<64> a;
	a.set(1, true);

	ECSRegistry::GetInstance()->Destroy();

	return 0;
}
