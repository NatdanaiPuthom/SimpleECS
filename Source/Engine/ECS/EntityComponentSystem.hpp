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
		bool AddComponentByID(const size_t aComponentIdentityID, EntityData& aEntityData);
		bool RemoveComponentByID(const size_t aComponentIdentityID, EntityData& aEntityData);
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

		const bool result = AddComponentByID(ComponentIdentityID<T>().GetID(), it->second);

		return result;
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
