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
		myEntitySignatures.clear();
		mySignatureToEntities.clear();
		myNextEntityID = 0;
	}

	void EntityComponentSystem::Initialize()
	{
		const auto& allRegisteredComponents = ECSRegistry::GetInstance()->GetRegisteredComponents();

		for (const auto& [hash, comp] : allRegisteredComponents)
		{
			myComponents.emplace_back(comp.identity);
		}
	}

	Entity& EntityComponentSystem::CreateEntity()
	{
		const ComponentSignature signature;
		auto& entities = mySignatureToEntities[signature];

		Entity& newEntity = entities.emplace_back(myNextEntityID);
		myEntitySignatures.push_back(signature);

		myNextEntityID++;

		return newEntity;
	}
}
