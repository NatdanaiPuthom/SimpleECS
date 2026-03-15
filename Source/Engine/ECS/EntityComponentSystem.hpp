#pragma once
#include "ECS/MemoryPool/MemoryPool.hpp"
#include "ECS/Constants/MaxComponents.hpp"
#include "ECS/Constants/ComponentsSignature.hpp"
#include "ECS/Entity.hpp"
#include <vector>
#include <unordered_map>
#include <array>

namespace Simple
{
	using EntityID = size_t;

	class EntityComponentSystem final
	{
	private:
		using ComponentIndex = size_t;

		struct EntityData
		{
			size_t index;
			ComponentsSignature componentSignature;
		};
	public:
		EntityComponentSystem();
		~EntityComponentSystem();

		void Initialize();

		EntityID CreateEntity();
		Entity& GetEntity(const EntityID aEntityID);

		void DestroyEntity(size_t aEntityIndex);

		template<IsComponent T>
		bool AddComponent(const EntityID aEntityID);

		template<IsComponent T>
		bool RemoveComponent(const EntityID aEntityID);
		//private:
		std::unordered_map<ComponentsSignature, std::vector<Entity>> mySignatureToEntities;
		std::unordered_map<EntityID, EntityData> myEntityIDToEntityData;

		std::vector<MemoryPool> myComponents; //NOTE(27/02/2026) ComponentIdentityID<T>().GetID() is the Index
		std::array<std::unordered_map<EntityID, ComponentIndex>, GLOBAL_MAX_COMPONENTS> myEntityIDToComponentIndex; //NOTE(27/02/2026) ComponentIdentityID<T>().GetID() is the Array Index
		std::array<std::unordered_map<ComponentIndex, EntityID>, GLOBAL_MAX_COMPONENTS> myComponentIndexToEntityID; //NOTE(27/02/2026) ComponentIdentityID<T>().GetID() is the Array Index

		size_t myNextEntityID;
	};

	template<IsComponent T>
	inline bool EntityComponentSystem::AddComponent(const EntityID aEntityID)
	{
		auto it = myEntityIDToEntityData.find(aEntityID);

		if (it == myEntityIDToEntityData.end())
		{
			DebugAssert(false, "Entity with this ID doesn't exist.");
			return false;
		}

		EntityData& entityData = it->second;

		const size_t entityIndex = entityData.index;
		std::vector<Entity>& entities = mySignatureToEntities[entityData.componentSignature];

		const bool componentAlreadyExist = entities[entityIndex].HasComponent<T>();

		if (componentAlreadyExist == true)
		{
			DebugAssert(false, "Duplicate component type on entity is not allowed");
			return false;
		}

		const size_t componentIdentityID = ComponentIdentityID<T>().GetID();
		const MemoryPool::OperationStatus status = myComponents[componentIdentityID].CreateObject();

		if (status.success == true)
		{
			const size_t entityID = entities[entityIndex].GetID();
			const size_t componentIndex = status.createdObjectIndex;

			entities[entityIndex].AddComponent(componentIdentityID);

			const ComponentsSignature newSignature = entities[entityIndex].GetComponentsSignature();
			myEntityIDToComponentIndex[componentIdentityID][entityID] = componentIndex;
			myComponentIndexToEntityID[componentIdentityID][componentIndex] = entityID;

			std::vector<Entity>& newEntities = mySignatureToEntities[newSignature];
			newEntities.push_back(std::move(entities[entityIndex]));

			entityData.componentSignature = newSignature;
			entityData.index = newEntities.size() - 1;

			if (entityIndex != entities.size() - 1)
			{
				entities[entityIndex] = std::move(entities.back());

				const size_t movedEntityID = entities[entityIndex].GetID();
				myEntityIDToEntityData.at(movedEntityID).index = entityIndex;
			}

			entities.pop_back();
		}

		return status.success;
	}

	template<IsComponent T>
	inline bool EntityComponentSystem::RemoveComponent(const EntityID aEntityID)
	{
		auto it = myEntityIDToEntityData.find(aEntityID);

		if (it == myEntityIDToEntityData.end())
		{
			DebugAssert(false, "Entity with this ID doesn't exist.");
			return false;
		}

		EntityData& entityData = it->second;

		const size_t entityIndex = entityData.index;
		std::vector<Entity>& entities = mySignatureToEntities[entityData.componentSignature];

		const bool componentAlreadyExist = entities[entityIndex].HasComponent<T>();

		if (componentAlreadyExist == false)
		{
			DebugAssert(false, "No component to remove.");
			return false;
		}

		const size_t componentIdentityID = ComponentIdentityID<T>().GetID();
		const ComponentIndex componentIndex = myEntityIDToComponentIndex[componentIdentityID].at(aEntityID);
		const ComponentIndex lastComponentIndex  = myComponents[componentIdentityID].GetCount() - 1;
		const EntityID lastComponentEntityID = myComponentIndexToEntityID[componentIdentityID].at(lastComponentIndex);

		const bool success = myComponents[componentIdentityID].DestroyObject(componentIndex);

		if (success == false)
		{
			DebugAssert(false, "Failed to remove component.");
			return false;
		}

		entities[entityIndex].RemoveComponent(componentIdentityID);

		if (componentIndex != lastComponentIndex)
		{
			myEntityIDToComponentIndex[componentIdentityID].at(lastComponentIndex) = componentIndex;
			myComponentIndexToEntityID[componentIdentityID].at(componentIndex) = lastComponentEntityID;
			myComponentIndexToEntityID[componentIdentityID].erase(lastComponentIndex);
		}
		else
		{
			myComponentIndexToEntityID[componentIdentityID].erase(componentIndex);
		}

		myEntityIDToComponentIndex[componentIdentityID].erase(aEntityID);

		const ComponentsSignature newSignature = entities[entityIndex].GetComponentsSignature();
		std::vector<Entity>& newEntities = mySignatureToEntities[newSignature];

		newEntities.push_back(std::move(entities[entityIndex]));

		entityData.componentSignature = newSignature;
		entityData.index = newEntities.size() - 1;

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
