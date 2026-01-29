#pragma once
#include "ECS/MemoryPool/MemoryPool.hpp"
#include "ECS/Entity.hpp"
#include <vector>
#include <bitset>
#include <unordered_map>

namespace Simple
{
	inline constexpr size_t GLOBAL_MAX_COMPONENTS = 64;

	class EntityComponentSystem final
	{
	public:
		using ComponentSignature = std::bitset<GLOBAL_MAX_COMPONENTS>;
	public:
		EntityComponentSystem();
		~EntityComponentSystem();

		bool Initialize();

		Entity& CreateEntity();

		template<typename T>
		T* AddComponent(Entity& aEntity);
	private:
		std::unordered_map<ComponentSignature, std::vector<Entity>> mySignatureToEntities;
		std::vector<MemoryPool> myComponents;
		size_t myNextEntityID;
	};

	template<typename T>
	inline T* EntityComponentSystem::AddComponent(Entity& aEntity)
	{
		aEntity;

		size_t success = myComponents[ComponentIdentityID<T>().GetID()].CreateObject();

		T* newComponent = nullptr;

		if (success > 0)
		{
			newComponent = myComponents[ComponentIdentityID<T>().GetID()].GetObjectAtIndex<T>(success);
		}

		return newComponent;
	}
}
