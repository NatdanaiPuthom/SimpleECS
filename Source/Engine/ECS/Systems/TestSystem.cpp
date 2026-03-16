#include "TestSystem.hpp"
#include "Debugger/Console/Console.hpp"

namespace Simple
{
	TestSystem::TestSystem()
	{
	}

	TestSystem::~TestSystem()
	{
	}

	void TestSystem::Initialize(EntityComponentSystem* /*aEntityComponentSystem*/)
	{
		Console::Print("TestSystem Initalized!", ConsoleTextColor::Yellow);
	}

	void TestSystem::Update(EntityComponentSystem* /*aEntityComponentSystem*/)
	{
		Console::Print("TestSystem Updated!", ConsoleTextColor::Green);
	}

	void TestSystem::EarlyUpdate(EntityComponentSystem* /*aEntityComponentSystem*/)
	{
		Console::Print("TestSystem Early Updated!", ConsoleTextColor::Blue);
	}

	void TestSystem::FixedUpdate(EntityComponentSystem* /*aEntityComponentSystem*/)
	{
		Console::Print("TestSystem Fixed Updated!", ConsoleTextColor::Green);
	}

	void TestSystem::LateUpdate(EntityComponentSystem* /*aEntityComponentSystem*/)
	{
		Console::Print("TestSystem Late Updated!", ConsoleTextColor::Green);
	}
}
