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

	EntityID EntityComponentSystem::CreateEntity()
	{
		const ComponentsSignature signature;
		const size_t id = myNextEntityID;

		std::vector<Entity>& entities = mySignatureToEntities[signature];
		Entity& newEntity = entities.emplace_back(id);
		myNextEntityID++;

		EntityData entityData;
		entityData.index = entities.size() - 1;
		entityData.componentSignature = signature;

		myEntityIDToEntityData[id] = entityData;

		return newEntity.GetID();
	}

	Entity& EntityComponentSystem::GetEntity(const EntityID aEntityID)
	{
		auto it = myEntityIDToEntityData.find(aEntityID);

		if (it == myEntityIDToEntityData.end())
		{
			DebugAssert(false, "Entity with this ID doesn't exist.");
			static Entity dummy;
			return dummy;
		}

		const EntityData& entityData = it->second;
		std::span<Entity> entities = mySignatureToEntities[entityData.componentSignature];
		Entity& entity = entities[entityData.index];

		return entity;
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
