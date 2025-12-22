#pragma once
#include "Component.hpp"

namespace Simple
{
	struct EmilTest : Component
	{
		int value1 = 0;
		int value2 = 1;
	};

	struct Stephy : Component
	{
		const char* value = "Stephy is cute";
	};
}