#include "ComponentManager.hpp"
#include "ECS/ECSRegistry.hpp"

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

		for (const auto& [hash, comp] : allRegisteredComponents)
		{
			myComponents.emplace_back(comp.identity);
		}
	}
}
