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
	class EntityComponentSystem final
	{
	public:
	public:
		EntityComponentSystem();
		~EntityComponentSystem();

		void Initialize();

		Entity& CreateEntity();

		void DestroyEntity(size_t aEntityIndex);

		template<IsComponent T>
		bool AddComponent(Entity& aEntity);
	private:
		std::vector<MemoryPool> myComponents; //ComponentIdentityID<T>().GetID() is the Index
		std::unordered_map<ComponentsSignature, std::vector<Entity>> mySignatureToEntities;

		size_t myNextEntityID;
	};

	template<IsComponent T>
	inline bool EntityComponentSystem::AddComponent(Entity& aEntity)
	{
		const size_t componentIdentityID = ComponentIdentityID<T>().GetID();
		const bool componentAlreadyExist = aEntity.GetComponentsSignature().test(componentIdentityID);

		if (componentAlreadyExist == true)
		{
			DebugAssert(false, "Duplicate component type on entity is not allowed");
			return false;
		}

		aEntity.AddComponent(componentIdentityID);

		const MemoryPool::OperationStatus status = myComponents[componentIdentityID].CreateObject();

		return status.success;
	}
}
