#pragma once
#include "ECS/Constants/MaxComponents.hpp"
#include <bitset>

namespace Simple
{
	using ComponentsSignature = std::bitset<GLOBAL_MAX_COMPONENTS>;
}
