#include "External/SimpleUtilities/MemoryTracking/MemoryTracker.hpp"

#include "Engine/ECS/MemoryPool/MemoryPool.hpp"
#include "Engine/ECS/Component/TestComponents.hpp"
#include "Engine/Debugger/Console/Console.hpp"

using namespace Simple;

int main()
{
	Console console;
	console.Init();

	[[maybe_unused]]TypeErasureComponent comp1 = ECSRegistry::GetInstance()->GetTypeErasureComponent<EmilTest>();
	
	[[maybe_unused]] const auto& it = ECSRegistry::GetInstance()->GetRegisteredComponents();

	MemoryPool pool = MemoryPool::CreatePool<EmilTest>();
	pool.PrintMemoryStatus();

	ECSRegistry::GetInstance()->Destroy();

	return 0;
}
