#include "External/SimpleUtilities/MemoryTracking/MemoryTracker.hpp"
//#include "External/ECS/ECSRegistry.hpp"
//#include "External/ECS/EntityManager.hpp"

#include "Engine/ECS/MemoryPool/MemoryPool.hpp"
#include "Engine/ECS/Component/TestComponents.hpp"
#include "Engine/Debugger/Console/Console.hpp"

using namespace Simple;

struct HelloWorld : Component
{
	int value = 5;
};

int main()
{
	Console console;
	console.Init();

	[[maybe_unused]] MemoryPool emilPool = MemoryPool::CreatePool<EmilTest>();
	emilPool.PrintMemoryStatus();


	return 0;
}
