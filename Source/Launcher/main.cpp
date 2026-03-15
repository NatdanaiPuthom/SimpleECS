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

	[[maybe_unused]] EntityID newEntity0 = ecs.CreateEntity();
	[[maybe_unused]] EntityID newEntity1 = ecs.CreateEntity();
	[[maybe_unused]] EntityID newEntity2 = ecs.CreateEntity();
	[[maybe_unused]] EntityID newEntity3 = ecs.CreateEntity();
	[[maybe_unused]] EntityID newEntity4 = ecs.CreateEntity();

	ecs.AddComponent<EmilTest>(newEntity0);
	ecs.AddComponent<EmilTest>(newEntity1);
	ecs.AddComponent<EmilTest>(newEntity2);

	[[maybe_unused]] Entity& refEntity1 = ecs.GetEntity(newEntity0);
	[[maybe_unused]] Entity& refEntity2 = ecs.GetEntity(newEntity1);
	[[maybe_unused]] Entity& refEntity3 = ecs.GetEntity(newEntity2);
	[[maybe_unused]] Entity& refEntity4 = ecs.GetEntity(newEntity3);
	[[maybe_unused]] Entity& refEntity5 = ecs.GetEntity(newEntity4);

	ecs.myComponentIndexToEntityID;
	ecs.myEntityIDToComponentIndex;
	ecs.myEntityIDToEntityData;
	ecs.mySignatureToEntities;
	ecs.myComponents;

	[[maybe_unused]] bool success0 = ecs.RemoveComponent<EmilTest>(newEntity0);
	[[maybe_unused]] bool success1 = ecs.RemoveComponent<EmilTest>(newEntity1);

	[[maybe_unused]] bool destroyEntity = ecs.DestroyEntity(newEntity2);


	ECSRegistry::GetInstance()->Destroy();

	return 0;
}
