#pragma once
#include <string>
#include <format>

namespace Simple
{
	struct Entity final
	{
		std::string name;
		const size_t id;

		Entity(const size_t aID)
			: id(aID)
			, name(std::format("New Entity {}", aID))
		{
		}

		Entity(const size_t aID, const char* aName)
			: id(aID)
			, name(aName)
		{
		}

		Entity(const Entity& aOther) noexcept
			: id(aOther.id)
			, name(aOther.name)
		{
		}

		Entity(Entity&& aOther) noexcept
			: id(aOther.id)
			, name(std::move(aOther.name))
		{
		}
	};
}
