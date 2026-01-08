#include "ComponentTypeIdentity.hpp"

namespace Simple
{
	ComponentTypeIdentity::ComponentTypeIdentity()
		: myID(0)
		, myName("Invalid")
		, myCreateComponentFunctionPointer(nullptr)
		, myMoveComponentFunctionPointer(nullptr)
		, myDestroyComponentFunctionPointer(nullptr)
	{
	}

	ComponentTypeIdentity::ComponentTypeIdentity(const size_t aID, const char* aName, CreateComponentFunctionPtr aCreateFunctionPtr, MoveComponentFunctionPtr aMoveFunctionPtr, DestroyComponentFunctionPtr aDestroyFunctionPtr)
		: myID(aID)
		, myName(aName)
		, myCreateComponentFunctionPointer(aCreateFunctionPtr)
		, myMoveComponentFunctionPointer(aMoveFunctionPtr)
		, myDestroyComponentFunctionPointer(aDestroyFunctionPtr)
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

	size_t ComponentTypeIdentity::InvokeCreate(void* aAddress) const
	{
		return myCreateComponentFunctionPointer(aAddress);
	}

	void ComponentTypeIdentity::InvokeMove(void* aDestinationAddress, void* aSourceAddress) const
	{
		myMoveComponentFunctionPointer(aDestinationAddress, aSourceAddress);
	}

	void ComponentTypeIdentity::InvokeDestroy(void* aComponentAddress) const
	{
		myDestroyComponentFunctionPointer(aComponentAddress);
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
		, myCreateComponentFunctionPointer(aOther.myCreateComponentFunctionPointer)
		, myMoveComponentFunctionPointer(aOther.myMoveComponentFunctionPointer)
		, myDestroyComponentFunctionPointer(aOther.myDestroyComponentFunctionPointer)
	{
	}

	ComponentTypeIdentity& ComponentTypeIdentity::operator=(const ComponentTypeIdentity& aOther)
	{
		if (this != &aOther)
		{
			this->myID = aOther.myID;
			this->myName = aOther.myName;
			this->myCreateComponentFunctionPointer = aOther.myCreateComponentFunctionPointer;
			this->myMoveComponentFunctionPointer = aOther.myMoveComponentFunctionPointer;
			this->myDestroyComponentFunctionPointer = aOther.myDestroyComponentFunctionPointer;
		}

		return *this;
	}

	ComponentTypeIdentity::ComponentTypeIdentity(ComponentTypeIdentity&& aOther) noexcept
		: myID(aOther.myID)
		, myName(aOther.myName)
		, myCreateComponentFunctionPointer(aOther.myCreateComponentFunctionPointer)
		, myMoveComponentFunctionPointer(aOther.myMoveComponentFunctionPointer)
		, myDestroyComponentFunctionPointer(aOther.myDestroyComponentFunctionPointer)
	{
		aOther.myID = 0;
		aOther.myName = "Invalid";
		aOther.myCreateComponentFunctionPointer = nullptr;
		aOther.myMoveComponentFunctionPointer = nullptr;
		aOther.myDestroyComponentFunctionPointer = nullptr;
	}

	ComponentTypeIdentity& ComponentTypeIdentity::operator=(ComponentTypeIdentity&& aOther) noexcept
	{
		if (this != &aOther)
		{
			this->myID = aOther.myID;
			this->myName = aOther.myName;
			this->myCreateComponentFunctionPointer = aOther.myCreateComponentFunctionPointer;
			this->myMoveComponentFunctionPointer = aOther.myMoveComponentFunctionPointer;
			this->myDestroyComponentFunctionPointer = aOther.myDestroyComponentFunctionPointer;

			aOther.myID = 0;
			aOther.myName = "Invalid";
			aOther.myCreateComponentFunctionPointer = nullptr;
			aOther.myMoveComponentFunctionPointer = nullptr;
			aOther.myDestroyComponentFunctionPointer = nullptr;
		}

		return *this;
	}
}
