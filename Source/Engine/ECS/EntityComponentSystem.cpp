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
		const Entity& newEntity = entities.emplace_back(id);
		const EntityID newEntityID = newEntity.GetID();

		myNextEntityID++;

		EntityData entityData;
		entityData.id = id;
		entityData.index = entities.size() - 1;
		entityData.componentSignature = signature;

		myEntityIDToEntityData[id] = entityData;

		return newEntityID;
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

	bool EntityComponentSystem::DestroyEntity(const EntityID aEntityID)
	{
		auto it = myEntityIDToEntityData.find(aEntityID);

		if (it == myEntityIDToEntityData.end())
		{
			DebugAssert(false, "Entity with this ID doesn't exist.");
			return false;
		}

		EntityData& entityData = it->second;

		for (size_t i = 0; i < entityData.componentSignature.size(); i++)
		{
			if (entityData.componentSignature.test(i) == true)
			{
				const bool result = RemoveComponentByID(i, entityData);

				if (result == false)
				{
					DebugAssert(false, "Failed to remove component during entity destruction.");
					return false;
				}
			}
		}

		const size_t entityIndex = entityData.index;
		std::vector<Entity>& entities = mySignatureToEntities[entityData.componentSignature];

		if (entityIndex != entities.size() - 1)
		{
			entities[entityIndex] = std::move(entities.back());

			const EntityID movedEntityID = entities[entityIndex].GetID();
			myEntityIDToEntityData[movedEntityID].index = entityIndex;
		}

		entities.pop_back();
		myEntityIDToEntityData.erase(aEntityID);

		return true;
	}

	bool EntityComponentSystem::RemoveComponentByID(const size_t aComponentTypeUniqueID, EntityData& aEntityData)
	{	
		const size_t entityIndex = aEntityData.index;
		std::vector<Entity>& entities = mySignatureToEntities[aEntityData.componentSignature];

		const bool componentAlreadyExist = entities[entityIndex].HasComponent(aComponentTypeUniqueID);

		if (componentAlreadyExist == false)
		{
			DebugAssert(false, "No component to remove.");
			return false;
		}

		const ComponentIndex componentIndex = myEntityIDToComponentIndex[aComponentTypeUniqueID].at(aEntityData.id);
		const ComponentIndex lastComponentIndex = myComponents[aComponentTypeUniqueID].GetCount() - 1;
		const EntityID lastComponentEntityID = myComponentIndexToEntityID[aComponentTypeUniqueID].at(lastComponentIndex);

		const bool result = myComponents[aComponentTypeUniqueID].DestroyObject(componentIndex);

		if (result == false)
		{
			DebugAssert(false, "Failed to remove component.");
			return false;
		}

		entities[entityIndex].RemoveComponent(aComponentTypeUniqueID);

		if (componentIndex != lastComponentIndex)
		{
			myEntityIDToComponentIndex[aComponentTypeUniqueID].at(lastComponentIndex) = componentIndex;
			myComponentIndexToEntityID[aComponentTypeUniqueID].at(componentIndex) = lastComponentEntityID;
			myComponentIndexToEntityID[aComponentTypeUniqueID].erase(lastComponentIndex);
		}
		else
		{
			myComponentIndexToEntityID[aComponentTypeUniqueID].erase(componentIndex);
		}

		myEntityIDToComponentIndex[aComponentTypeUniqueID].erase(aEntityData.id);

		const ComponentsSignature newSignature = entities[entityIndex].GetComponentsSignature();
		std::vector<Entity>& newEntities = mySignatureToEntities[newSignature];

		newEntities.push_back(std::move(entities[entityIndex]));

		aEntityData.componentSignature = newSignature;
		aEntityData.index = newEntities.size() - 1;

		if (entityIndex != entities.size() - 1)
		{
			entities[entityIndex] = std::move(entities.back());

			const EntityID movedEntityID = entities[entityIndex].GetID();
			myEntityIDToEntityData[movedEntityID].index = entityIndex;
		}

		entities.pop_back();

		return true;
	}
}
