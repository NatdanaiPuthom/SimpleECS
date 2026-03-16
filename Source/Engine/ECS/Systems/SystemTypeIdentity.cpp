#include "SystemTypeIdentity.hpp"
#include "ECS/Constants/Invalidindex.hpp"

namespace Simple
{
	SystemTypeIdentity::SystemTypeIdentity()
		: myName("Invalid")
		, myID(GLOBAL_INVALID_INDEX_SIZE_T)
		, myCloneSystemFunctionPointer(nullptr)
	{
	}

	SystemTypeIdentity::SystemTypeIdentity(const char* aName, const size_t aID, CloneSystemFunctionPtr aCloneFunctionPtr)
		: myName(aName)
		, myID(aID)
		, myCloneSystemFunctionPointer(aCloneFunctionPtr)
	{
	}

	SystemTypeIdentity::~SystemTypeIdentity()
	{
		myName = "Invalid";
		myID = GLOBAL_INVALID_INDEX_SIZE_T;
		myCloneSystemFunctionPointer = nullptr;
	}

	SystemTypeIdentity::SystemTypeIdentity(const SystemTypeIdentity& aOther) noexcept
		: myName(aOther.myName)
		, myID(aOther.myID)
		, myCloneSystemFunctionPointer(aOther.myCloneSystemFunctionPointer)
	{
	}

	SystemTypeIdentity& SystemTypeIdentity::operator=(const SystemTypeIdentity& aOther) noexcept
	{
		if (this != &aOther)
		{
			this->myName = aOther.myName;
			this->myID = aOther.myID;
			this->myCloneSystemFunctionPointer = aOther.myCloneSystemFunctionPointer;
		}

		return *this;
	}

	SystemTypeIdentity::SystemTypeIdentity(SystemTypeIdentity&& aOther) noexcept
		: myName(std::move(aOther.myName))
		, myID(aOther.myID)
		, myCloneSystemFunctionPointer(aOther.myCloneSystemFunctionPointer)
	{
		aOther.myName = "Invalid";
		aOther.myID = GLOBAL_INVALID_INDEX_SIZE_T;
		aOther.myCloneSystemFunctionPointer = nullptr;
	}

	SystemTypeIdentity& SystemTypeIdentity::operator=(SystemTypeIdentity&& aOther) noexcept
	{
		if (this != &aOther)
		{
			this->myName = std::move(aOther.myName);
			this->myID = aOther.myID;
			this->myCloneSystemFunctionPointer = aOther.myCloneSystemFunctionPointer;

			aOther.myName = "Invalid";
			aOther.myID = GLOBAL_INVALID_INDEX_SIZE_T;
			aOther.myCloneSystemFunctionPointer = nullptr;
		}

		return *this;
	}

	size_t SystemTypeIdentity::GetID() const
	{
		return myID;
	}

	const std::string& SystemTypeIdentity::GetName() const
	{
		return myName;
	}

	std::unique_ptr<System> SystemTypeIdentity::Clone() const
	{
		return myCloneSystemFunctionPointer();
	}
}
