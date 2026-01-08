#pragma once
#include "Component.hpp"
#include "ECS/ECSRegistry.hpp"

namespace Simple
{
	struct EmilTest : public Component
	{
		int value1 = 0;
		int value2 = 1;
	};

	REGISTER_COMPONENT(EmilTest);

	struct EricoTest : public Component
	{
		int value1 = 0;
		bool value2 = false;
	};

	REGISTER_COMPONENT(EricoTest);
}

