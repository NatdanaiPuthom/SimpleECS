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

	Entity& newEntity = ecs.CreateEntity();
	[[maybe_unused]] EmilTest* newComponent = ecs.AddComponent<EmilTest>(newEntity);

	ECSRegistry::GetInstance()->Destroy();

	return 0;
}
