#include "ComponentTypeIdentity.hpp"

namespace Simple
{
	ComponentTypeIdentity::ComponentTypeIdentity()
		: myID(0)
		, myName("Invalid")
	{
	}

	ComponentTypeIdentity::ComponentTypeIdentity(const size_t aID, const char* aName)
		: myID(aID)
		, myName(aName)
	{
	}

	ComponentTypeIdentity::~ComponentTypeIdentity()
	{
		myID = 0;
		myName = "Invalid";
	}

	size_t ComponentTypeIdentity::GetID() const
	{
		return myID;
	}

	const char* ComponentTypeIdentity::GetName() const
	{
		return myName;
	}

	bool ComponentTypeIdentity::IsValid() const
	{
		return (myID != 0);
	}

	bool ComponentTypeIdentity::operator==(const ComponentTypeIdentity& aOther) const
	{
		return this->myID == aOther.myID;
	}

	bool ComponentTypeIdentity::operator!=(const ComponentTypeIdentity& aOther) const
	{
		return this->myID != aOther.myID;
	}

	ComponentTypeIdentity::ComponentTypeIdentity(const ComponentTypeIdentity& aOther)
		: myID(aOther.myID)
		, myName(aOther.myName)
	{
	}

	ComponentTypeIdentity& ComponentTypeIdentity::operator=(const ComponentTypeIdentity& aOther)
	{
		if (this != &aOther)
		{
			this->myID = aOther.myID;
			this->myName = aOther.myName;
		}

		return *this;
	}

	ComponentTypeIdentity::ComponentTypeIdentity(ComponentTypeIdentity&& aOther) noexcept
		: myID(aOther.myID)
		, myName(aOther.myName)
	{
		aOther.myID = 0;
		aOther.myName = "Invalid";
	}

	ComponentTypeIdentity& ComponentTypeIdentity::operator=(ComponentTypeIdentity&& aOther) noexcept
	{
		if (this != &aOther)
		{
			this->myID = aOther.myID;
			this->myName = aOther.myName;

			aOther.myID = 0;
			aOther.myName = "Invalid";
		}

		return *this;
	}

	const size_t ComponentTypeIdentity::GetNextComponentID()
	{
		static size_t nextID = 1;
		return nextID++;
	}
}
