#include "EntityComponentSystem.hpp"
#include "ECS/ECSRegistry.hpp"

namespace Simple
{
	EntityComponentSystem::EntityComponentSystem() 
		: myNextEntityID(0)
	{
	}

	EntityComponentSystem::~EntityComponentSystem()
	{
		myComponents.clear();
	}

	bool EntityComponentSystem::Initialize()
	{
		const auto& allRegisteredComponents = ECSRegistry::GetInstance()->GetRegisteredComponents();

		for (const auto& [hash, comp] : allRegisteredComponents)
		{
			ComponentTypeIdentity identity = comp.identity;
			myComponents.emplace_back(identity, 1);
		}

		return true;
	}

	Entity& EntityComponentSystem::CreateEntity()
	{
		const ComponentSignature signature;

		auto& entities = mySignatureToEntities[signature];
		Entity& newEntity = entities.emplace_back(myNextEntityID);
		myNextEntityID++;

		return newEntity;
	}
}
