#pragma once
#include "ECS/MemoryPool/MemoryPool.hpp"
#include "ECS/Constants/MaxComponents.hpp"
#include "ECS/Constants/ComponentsSignature.hpp"
#include "ECS/Constants/InvalidIndex.hpp"
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
			EntityID id = GLOBAL_INVALID_INDEX_SIZE_T;
			size_t index = GLOBAL_INVALID_INDEX_SIZE_T;
			ComponentsSignature componentSignature;
		};
	public:
		EntityComponentSystem();
		~EntityComponentSystem();

		void Initialize();

		EntityID CreateEntity();
		Entity& GetEntity(const EntityID aEntityID);

		bool DestroyEntity(const EntityID aEntityID);

		template<IsComponent T>
		bool AddComponent(const EntityID aEntityID);

		template<IsComponent T>
		bool RemoveComponent(const EntityID aEntityID);

	private:
		bool RemoveComponentByID(const size_t aComponentTypeUniqueID, EntityData& aEntityData);
		//private:
	public: //test debug
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

		const bool result = RemoveComponentByID(ComponentIdentityID<T>().GetID(), it->second);

		return result;
	}
}
