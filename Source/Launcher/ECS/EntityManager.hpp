#pragma once
#include <vector>
#include "MemoryPool/MemoryPool.hpp"
#include "ECSRegistry.hpp"

namespace Simple
{
	struct ComponentID final
	{
		const size_t componentPoolIndex;
		const size_t componentIndexInPool;
	};

	class Entity final
	{
	public:
		Entity(const size_t aEntityID)
			: myID(aEntityID)
		{
		}

		const size_t myID;
		std::vector<ComponentID> myComponentIDs;
	};

	class EntityManager final
	{
	public:
		EntityManager()
		{
			const auto& registeredComponents = ECSRegistry::GetInstance()->GetRegisteredComponents();

			myComponents.resize(registeredComponents.size());

			for (const auto& [key, component] : registeredComponents)
			{
				MemoryPool memoryPool(component.sizeOf, component.alignOf, key.value);
				myComponents[component.poolIndex] = std::move(memoryPool);
			}
		}

		~EntityManager()
		{
			myComponents.clear();
		}

		Entity& CreateEntity()
		{
			myEntities.emplace_back(myNextEntityID++);
			return myEntities.back();
		}

		template<typename T> requires std::is_base_of_v<Component, T>
		ComponentID AddComponent()
		{
			size_t index = ECSRegistry::GetInstance()->GetComponentPoolIndex<T>();

			//myComponents[index].CreateComponent<T>();

			return ComponentID();
		}

		Entity& GetEntity(const size_t aEntityID)
		{
			if (myEntities.empty())
			{
				Entity* dummyEntity = new Entity(0);
				return *dummyEntity;
			}

			if (aEntityID < 0)
			{
				Entity* dummyEntity = new Entity(0);
				return *dummyEntity;
			}

			if (aEntityID > myEntities.size())
			{
				Entity* dummyEntity = new Entity(0);
				return *dummyEntity;
			}

			return myEntities[aEntityID];
		}

	private:
		static inline size_t myNextEntityID = 0;
		std::vector<Entity> myEntities;
		std::vector<MemoryPool> myComponents;
	};
}