#pragma once
#include "ECS/Concepts/Component.hpp"
#include "ECS/ECSRegistry.hpp"

namespace Simple
{
	struct EmilTest : Component
	{
		int value1 = 0;
		int value2 = 1;
	};

	struct EricoTest : Component
	{
		int value1 = 2;
		int value2 = 3;
	};

	struct MathiasTest : Component
	{
		int value1 = 4;
		int value2 = 5;
	};

	REGISTER_COMPONENT(EmilTest);
	REGISTER_COMPONENT(EricoTest);
	REGISTER_COMPONENT(MathiasTest);
}
