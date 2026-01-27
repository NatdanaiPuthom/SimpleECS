#pragma once
#include "ECS/MemoryPool/MemoryPool.hpp"
#include <vector>

namespace Simple
{
	class ComponentManager final
	{
	public:
		ComponentManager();
		~ComponentManager();

		void Initialize();

	private:
		std::vector<MemoryPool> myComponents;
	};
}
