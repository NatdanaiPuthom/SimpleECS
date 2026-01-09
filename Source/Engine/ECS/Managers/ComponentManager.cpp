#include "ComponentManager.hpp"
#include "ECS/ECSRegistry.hpp"
#include <iostream>
#include <format>

namespace Simple
{
	ComponentManager::ComponentManager()
	{
	}

	ComponentManager::~ComponentManager()
	{
		myComponents.clear();
	}

	void ComponentManager::Initialize()
	{
		const auto& allRegisteredComponents = ECSRegistry::GetInstance()->GetRegisteredComponents();

		size_t index = 0;
		for (const auto& [hash, comp] : allRegisteredComponents)
		{
			std::cout << std::format("Component: {} HashCode: {}", comp.identity.GetName(), hash.value) << std::endl;
			myComponents.emplace_back(comp.identity);

			for (size_t i = 0; i < 10; i++)
			{
				myComponents[index].CreateObject();
			}

			//TO-DO(09/01/2025): Fix destructor, memory or allocating issues. Need more investigation.

			index++;
		}

		std::cout << "0: " << myComponents[0].GetCapacity() << std::endl;
		std::cout << "1: " << myComponents[1].GetCapacity() << std::endl;
	}
}
