#include "External/SimpleUtilities/MemoryTracking/MemoryTracker.hpp"

#include "Engine/ECS/MemoryPool/MemoryPool.hpp"
#include "Engine/ECS/Component/TestComponents.hpp"
#include "Engine/Debugger/Console/Console.hpp"

#include "Engine/ECS/EntityManager.hpp"
#include "Engine/ECS/Managers/ComponentManager.hpp"

using namespace Simple;

int main()
{
	Console console;
	console.Initialize();

	ComponentManager componentManager;
	componentManager.Initialize();

	ECSRegistry::GetInstance()->Destroy();

	return 0;
}
