#include "External/SimpleUtilities/MemoryTracking/MemoryTracker.hpp"

#include "Engine/Debugger/Console/Console.hpp"

#include "Engine/ECS/ECSRegistry.hpp"
#include "Engine/ECS/Managers/ComponentManager.hpp"
#include "Engine/ECS/EntityManager.hpp"

using namespace Simple;

int main()
{
	Console console;
	console.Initialize();

	ComponentManager componentManager;
	componentManager.Initialize();

	EntityManager entityManager;
	entityManager.Initialize();

	ECSRegistry::GetInstance()->Destroy();

	return 0;
}
