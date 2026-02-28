#include "External/SimpleUtilities/MemoryTracking/MemoryTracker.hpp"

#include "Engine/Debugger/Console/Console.hpp"

#include "Engine/ECS/ECSRegistry.hpp"
#include "Engine/ECS/EntityComponentSystem.hpp"

#include "Engine/ECS/Component/TestComponents.hpp" //Test

using namespace Simple;

int main()
{
	Console console;
	console.Initialize();

	EntityComponentSystem ecs;
	ecs.Initialize();

	[[maybe_unused]] EntityID newEntity1 = ecs.CreateEntity();
	[[maybe_unused]] EntityID newEntity2 = ecs.CreateEntity();

	ecs.AddComponent<EmilTest>(newEntity1);
	//ecs.AddComponent<EmilTest>(newEntity2);

	[[maybe_unused]] Entity& refEntity = ecs.GetEntity(newEntity1);

	ECSRegistry::GetInstance()->Destroy();

	return 0;
}
