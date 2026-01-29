#pragma once
#include <string>
#include <format>

namespace Simple
{
	struct Entity final
	{
		Entity(const size_t aID) : id(aID), name(std::format("New Entity {}", aID)) {}
		Entity(const size_t aID, const char* aName) : id(aID), name(aName) {}

		std::string name;
		size_t id;
	};
}
