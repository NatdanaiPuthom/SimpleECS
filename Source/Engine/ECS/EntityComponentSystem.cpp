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
		const ComponentsSignature signature;

		std::vector<Entity>& entities = mySignatureToEntities[signature];
		Entity& newEntity = entities.emplace_back(myNextEntityID);
		myNextEntityID++;

		/*
		newEntity.SetEntityDataIndex(myEntityDatas.size() + 1);

		EntityData entityData;
		entityData.signature = signature;
		entityData.indexInEntities = entities.size() - 1;

		myEntityDatas.push_back(entityData);
	

		*/

		return newEntity;
	}

	void EntityComponentSystem::DestroyEntity(size_t aEntityIndex)
	{
		aEntityIndex;
		//const EntityData& entityData = myEntityDatas[aEntityIndex];
		//const ComponentSignature signature = entityData.signature;

		//std::vector<Entity>& entities = mySignatureToEntities[signature];

		//entities[entityData.indexInEntities] = std::move(entities.back());
		//entities[entityData.indexInEntities].SetEntityDataIndex(aEntityIndex);
		//entities.pop_back();

		////myEntityDatas[aEntityIndex].signature = myEntitySignatures.back();
		////myEntitySignatures.pop_back();

		//for (size_t i = 0; i < signature.size(); i++)
		//{
		//	if (signature.test(i) == true)
		//	{
		//		const size_t entityToComponentIndex = entities[aEntityIndex].GetEntityToComponentIndex();
		//		const size_t componentIndex = myEntityToComponentDatas[entityToComponentIndex].componentIndex;
		//		myComponents[i].DestroyObject(componentIndex);
		//	}
		//}
	}
}
