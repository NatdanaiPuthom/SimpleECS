#pragma once
#include <vector>
#include "MemoryPool/MemoryPool.hpp"
#include "ECSRegistry.hpp"

namespace Simple
{
	class EntityManager final
	{
	public:
		EntityManager()
		{
		}

		~EntityManager()
		{
			myComponents.clear();
		}

		template<IsComponent T>
		size_t AddComponent()
		{
			size_t index = ECSRegistry::GetInstance()->GetComponentPoolIndex<T>();

			//myComponents[index].CreateComponent<T>();

			return sizeof(T);
		}

	private:
		std::vector<MemoryPool> myComponents;
	};
}
