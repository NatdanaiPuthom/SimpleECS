#pragma once
#include <vector>
#include "MemoryPool/MemoryPool.hpp"
#include "ECSRegistry.hpp"
#include <iostream>
#include <format>

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
		void AddComponent()
		{
			const auto& registeredComponents = ECSRegistry::GetInstance()->GetRegisteredComponents();

			for (const auto& [hash, comp] : registeredComponents)
			{
				myComponents.emplace_back(MemoryPool(comp.identity));
		
				std::cout << std::format("Component: {} HashCode: {}", comp.identity.GetName(), hash.value) << std::endl;
			}
		}

	private:
		std::vector<MemoryPool> myComponents;
	};
}
