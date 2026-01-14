#pragma once
#include "Component.hpp"
#include "ECS/ECSRegistry.hpp"

namespace Simple
{
	struct EmilTest : Component
	{
		int value1 = 0;
		int value2 = 1;
	};

	REGISTER_COMPONENT(EmilTest);

	struct EricoTest : Component
	{
		int value1 = 2;
		int value2 = 3;
	};

	REGISTER_COMPONENT(EricoTest);
}
