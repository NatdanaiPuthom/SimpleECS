#include "Entity.hpp"
#include "ECS/Constants/MaxEntities.hpp"
#include <format>

namespace Simple
{
	Entity::Entity(const size_t aID)
		: myID(aID)
		, myName(std::format("New Entity {}", aID))
	{
	}

	Entity::Entity(const size_t aID, const char* aName)
		: myID(aID)
		, myName(aName)
	{
	}

	Entity::~Entity()
	{
		myID = GLOBAL_MAX_ENTITIES;
	}

	Entity::Entity(const Entity& aOther) noexcept
		: myID(aOther.myID)
		, myName(aOther.myName)
	{
	}

	Entity& Entity::operator=(const Entity& aOther) noexcept
	{
		if (this != &aOther)
		{
			this->myID = aOther.myID;
			this->myName = aOther.myName;
		}

		return *this;
	}

	Entity::Entity(Entity&& aOther) noexcept
		: myID(aOther.myID)
		, myName(std::move(aOther.myName))
	{
	}

	Entity& Entity::operator=(Entity&& aOther) noexcept
	{
		if (this != &aOther)
		{
			this->myID = aOther.myID;
			this->myName = std::move(aOther.myName);

			aOther.myID = GLOBAL_MAX_ENTITIES;
		}

		return *this;
	}

	void Entity::SetName(const char* aName)
	{
		myName = aName;
	}

	size_t Entity::GetID() const
	{
		return myID;
	}

	const std::string& Entity::GetName() const
	{
		return myName;
	}

	const ComponentsSignature Entity::GetComponentsSignature() const
	{
		return myComponentsSignature;
	}
}
