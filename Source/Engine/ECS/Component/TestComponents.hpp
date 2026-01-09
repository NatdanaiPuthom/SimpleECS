#pragma once
#include "Component.hpp"
#include "ECS/ECSRegistry.hpp"
#include <iostream>

namespace Simple
{
	struct EmilTest : Component
	{
		int value1 = 0;
		int value2 = 1;

		~EmilTest()
		{
			std::cout << "Emil Called Destructor!" << std::endl;
		}
	};

	REGISTER_COMPONENT(EmilTest);

	struct EricoTest : Component
	{
		int value1 = 2;
		int value2 = 3;

		~EricoTest()
		{
			std::cout << "Erico Called Destructor!" << std::endl;
		}
	};

	REGISTER_COMPONENT(EricoTest);
}

