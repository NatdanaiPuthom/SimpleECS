#include "External/SimpleUtilities/MemoryTracking/MemoryTracker.hpp"

#include "Engine/ECS/MemoryPool/MemoryPool.hpp"
#include "Engine/ECS/Component/TestComponents.hpp"
#include "Engine/Debugger/Console/Console.hpp"

using namespace Simple;

struct HelloWorld : Component
{
	int value = 0;
};

int main()
{
	Console console;
	console.Init();

	[[maybe_unused]]TypeErasureComponent comp1 = ECSRegistry::GetInstance()->GetTypeErasureComponent<EmilTest>();
	[[maybe_unused]]TypeErasureComponent comp2 = ECSRegistry::GetInstance()->GetTypeErasureComponent<HelloWorld>();
	
	ECSRegistry::GetInstance()->Destroy();

	return 0;
}
