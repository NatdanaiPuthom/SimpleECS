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

			for (size_t i = 0; i < 5; i++)
			{
				const size_t index = myComponents[index].CreateObject();
			}

			//TO-DO(09/01/2025): Fix destructor, memory or allocating issues. Need more investigation.

			index++;
		}

	}
}
