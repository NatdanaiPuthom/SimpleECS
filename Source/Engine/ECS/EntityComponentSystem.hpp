#pragma once
#include "ECS/MemoryPool/MemoryPool.hpp"
#include "ECS/MaxComponents.hpp"
#include "ECS/Entity.hpp"
#include <vector>
#include <bitset>
#include <unordered_map>

namespace Simple
{
	class EntityComponentSystem final
	{
	public:
		using ComponentSignature = std::bitset<GLOBAL_MAX_COMPONENTS>;
	public:
		EntityComponentSystem();
		~EntityComponentSystem();

		void Initialize();

		Entity& CreateEntity();

		template<typename T>
		T* AddComponent(Entity& aEntity);
	private:
		std::unordered_map<ComponentSignature, std::vector<Entity>> mySignatureToEntities;
		std::vector<ComponentSignature> myEntitySignatures;
		std::vector<MemoryPool> myComponents;
		size_t myNextEntityID;
	};

	template<typename T>
	inline T* EntityComponentSystem::AddComponent(Entity& aEntity)
	{
		const size_t componentIdentityID = ComponentIdentityID<T>().GetID();
		const size_t success = myComponents[componentIdentityID].CreateObject();

		T* newComponent = nullptr;

		if (success < GLOBAL_MAX_COMPONENTS)
		{
			newComponent = myComponents[componentIdentityID].GetObjectAtIndex<T>(success);
			myEntitySignatures[aEntity.id].set(componentIdentityID, true);
		}

		return newComponent;
	}
}
