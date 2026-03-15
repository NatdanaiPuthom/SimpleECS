#include "Entity.hpp"
#include "ECS/Constants/MaxEntities.hpp"
#include <format>
#include <limits>

namespace Simple
{
	Entity::Entity()
	{
		myID = std::numeric_limits<size_t>().max();
		myName = "Dummy Entity";
	}

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
		, myComponentsSignature(aOther.myComponentsSignature)
	{
	}

	Entity& Entity::operator=(const Entity& aOther) noexcept
	{
		if (this != &aOther)
		{
			this->myID = aOther.myID;
			this->myName = aOther.myName;
			this->myComponentsSignature = aOther.myComponentsSignature;
		}

		return *this;
	}

	Entity::Entity(Entity&& aOther) noexcept
		: myID(aOther.myID)
		, myName(std::move(aOther.myName))
		, myComponentsSignature(std::move(aOther.myComponentsSignature))
	{
	}

	Entity& Entity::operator=(Entity&& aOther) noexcept
	{
		if (this != &aOther)
		{
			this->myID = aOther.myID;
			this->myName = std::move(aOther.myName);
			this->myComponentsSignature = std::move(aOther.myComponentsSignature);

			aOther.myID = GLOBAL_MAX_ENTITIES;
			aOther.myName = "Invalid Entity";
			aOther.myComponentsSignature.reset();
		}

		return *this;
	}

	bool Entity::HasComponentByID(const size_t aComponentIdentityID)
	{
		return myComponentsSignature.test(aComponentIdentityID);
	}

	void Entity::AddComponent(const size_t aComponentIdentityID)
	{
		myComponentsSignature.set(aComponentIdentityID, true);
	}

	void Entity::RemoveComponent(const size_t aComponentIdentityID)
	{
		myComponentsSignature.set(aComponentIdentityID, false);
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
