#pragma once
#include "ECS/MemoryPool/MemoryPool.hpp"
#include "ECS/Constants/MaxComponents.hpp"
#include "ECS/Constants/ComponentsSignature.hpp"
#include "ECS/Entity.hpp"
#include <vector>
#include <unordered_map>
#include <array>
#include <algorithm>

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
		bool RemoveComponent(Entity& aEntity);
	private:
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
		const size_t index = entityData.index;
		std::vector<Entity>& entities = mySignatureToEntities[entityData.componentSignature];

		const bool componentAlreadyExist = entities[index].HasComponent<T>();

		if (componentAlreadyExist == true)
		{
			DebugAssert(false, "Duplicate component type on entity is not allowed");
			return false;
		}

		const size_t componentIdentityID = ComponentIdentityID<T>().GetID();
		const MemoryPool::OperationStatus status = myComponents[componentIdentityID].CreateObject();

		if (status.success == true)
		{
			const size_t entityID = entities[index].GetID();
			const size_t componentIndex = status.createdObjectIndex;

			entities[index].AddComponent(componentIdentityID);
			entityData.componentSignature = entities[index].GetComponentsSignature();

			myEntityIDToComponentIndex[componentIdentityID][entityID] = componentIndex;
			myComponentIndexToEntityID[componentIdentityID][componentIndex] = entityID;

			if (index != entities.size() - 1)
			{
				const EntityID lastEntityID = entities.back().GetID();
				myEntityIDToEntityData[lastEntityID].index = index;

				entities[index] = entities.back();
				mySignatureToEntities[entityData.componentSignature].push_back(std::move(entities.back()));
			}

			entities.pop_back();
		}

		return status.success;
	}

	template<IsComponent T>
	inline bool EntityComponentSystem::RemoveComponent(Entity& aEntity)
	{
		const size_t componentIdentityID = ComponentIdentityID<T>().GetID();
		const size_t entityID = aEntity.GetID();

		std::unordered_map<EntityID, ComponentIndex>& entityIDToComponentIndexMap = myEntityIDToComponentIndex[componentIdentityID];

		auto it = entityIDToComponentIndexMap.find(entityID);

		if (it == entityIDToComponentIndexMap.end())
		{
			DebugAssert(false, "Component not found on entity.");
			return false;
		}

		const size_t componentIndex = it->second;
		const size_t lastIndex = myComponents[componentIdentityID].DestroyObject(componentIndex);

		entityIDToComponentIndexMap.erase(entityID);

		std::unordered_map<ComponentIndex, EntityID>& componentIndexToEntityID = myComponentIndexToEntityID[componentIdentityID];
		const size_t lastComponentEntityIDOwner = componentIndexToEntityID[lastIndex];

		lastComponentEntityIDOwner;

		return true;
	}
}
