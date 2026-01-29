#include "ComponentTypeIdentity.hpp"
#include "ECS/MaxComponents.hpp"

namespace Simple
{
	ComponentTypeIdentity::ComponentTypeIdentity()
		: myID(GLOBAL_MAX_COMPONENTS)
		, myAlignment(0)
		, mySize(0)
		, myName("Invalid")
		, myCreateComponentFunctionPointer(nullptr)
		, myCopyComponentFunctionPointer(nullptr)
		, myMoveComponentFunctionPointer(nullptr)
		, myDestroyComponentFunctionPointer(nullptr)
	{
	}

	ComponentTypeIdentity::ComponentTypeIdentity(const size_t aID, const size_t aAlignment, const size_t aSize, const char* aName, CreateComponentFunctionPtr aCreateFunctionPtr, CopyComponentFunctionPtr aCopyFunctionPtr, MoveComponentFunctionPtr aMoveFunctionPtr, DestroyComponentFunctionPtr aDestroyFunctionPtr)
		: myID(aID)
		, myAlignment(aAlignment)
		, mySize(aSize)
		, myName(aName)
		, myCreateComponentFunctionPointer(aCreateFunctionPtr)
		, myCopyComponentFunctionPointer(aCopyFunctionPtr)
		, myMoveComponentFunctionPointer(aMoveFunctionPtr)
		, myDestroyComponentFunctionPointer(aDestroyFunctionPtr)
	{
	}

	ComponentTypeIdentity::~ComponentTypeIdentity()
	{
		myID = GLOBAL_MAX_COMPONENTS;
		myAlignment = 0;
		mySize = 0;
		myName = "Invalid";

		myCreateComponentFunctionPointer = nullptr;
		myCopyComponentFunctionPointer = nullptr;
		myMoveComponentFunctionPointer = nullptr;
		myDestroyComponentFunctionPointer = nullptr;
	}

	size_t ComponentTypeIdentity::GetID() const
	{
		return myID;
	}

	size_t ComponentTypeIdentity::GetAlignment() const
	{
		return myAlignment;
	}

	size_t Simple::ComponentTypeIdentity::GetSize() const
	{
		return mySize;
	}

	const std::string& ComponentTypeIdentity::GetName() const
	{
		return myName;
	}

	size_t ComponentTypeIdentity::InvokeCreate(void* aDestinationAddress, const void* aDefaultValue) const
	{
		if (myCreateComponentFunctionPointer != nullptr)
		{
			return myCreateComponentFunctionPointer(aDestinationAddress, aDefaultValue);
		}

		return 0;
	}

	void ComponentTypeIdentity::InvokeCopy(void* aDestinationAddress,const void* aSourceAddress) const
	{
		if (myCopyComponentFunctionPointer != nullptr)
		{
			myCopyComponentFunctionPointer(aDestinationAddress, aSourceAddress);
		}
	}

	void ComponentTypeIdentity::InvokeMove(void* aDestinationAddress, void* aSourceAddress) const
	{
		if (myMoveComponentFunctionPointer != nullptr)
		{
			myMoveComponentFunctionPointer(aDestinationAddress, aSourceAddress);
		}
	}

	void ComponentTypeIdentity::InvokeDestroy(void* aComponentAddress) const
	{
		if (myDestroyComponentFunctionPointer != nullptr)
		{
			myDestroyComponentFunctionPointer(aComponentAddress);
		}
	}

	bool ComponentTypeIdentity::IsValid() const
	{
		return (myID < GLOBAL_MAX_COMPONENTS);
	}

	bool ComponentTypeIdentity::operator==(const ComponentTypeIdentity& aOther) const
	{
		return 
			this->myID == aOther.myID &&
			this->mySize == aOther.mySize &&
			this->myAlignment == aOther.myAlignment;
	}

	bool ComponentTypeIdentity::operator!=(const ComponentTypeIdentity& aOther) const
	{
		return
			this->myID != aOther.myID ||
			this->myAlignment != aOther.myAlignment ||
			this->mySize != aOther.mySize;
	}

	ComponentTypeIdentity::ComponentTypeIdentity(const ComponentTypeIdentity& aOther) noexcept
		: myID(aOther.myID)
		, myAlignment(aOther.myAlignment)
		, mySize(aOther.mySize)
		, myName(aOther.myName)
		, myCreateComponentFunctionPointer(aOther.myCreateComponentFunctionPointer)
		, myCopyComponentFunctionPointer(aOther.myCopyComponentFunctionPointer)
		, myMoveComponentFunctionPointer(aOther.myMoveComponentFunctionPointer)
		, myDestroyComponentFunctionPointer(aOther.myDestroyComponentFunctionPointer)
	{
	}

	ComponentTypeIdentity& ComponentTypeIdentity::operator=(const ComponentTypeIdentity& aOther) noexcept
	{
		if (this != &aOther)
		{
			this->myID = aOther.myID;
			this->myAlignment = aOther.myAlignment;
			this->mySize = aOther.mySize;
			this->myName = aOther.myName;
			this->myCreateComponentFunctionPointer = aOther.myCreateComponentFunctionPointer;
			this->myCopyComponentFunctionPointer = aOther.myCopyComponentFunctionPointer;
			this->myMoveComponentFunctionPointer = aOther.myMoveComponentFunctionPointer;
			this->myDestroyComponentFunctionPointer = aOther.myDestroyComponentFunctionPointer;
		}

		return *this;
	}

	ComponentTypeIdentity::ComponentTypeIdentity(ComponentTypeIdentity&& aOther) noexcept
		: myID(aOther.myID)
		, myAlignment(aOther.myAlignment)
		, mySize(aOther.mySize)
		, myName(aOther.myName)
		, myCreateComponentFunctionPointer(aOther.myCreateComponentFunctionPointer)
		, myCopyComponentFunctionPointer(aOther.myCopyComponentFunctionPointer)
		, myMoveComponentFunctionPointer(aOther.myMoveComponentFunctionPointer)
		, myDestroyComponentFunctionPointer(aOther.myDestroyComponentFunctionPointer)
	{
		aOther.myID = GLOBAL_MAX_COMPONENTS;
		aOther.myAlignment = 0;
		aOther.mySize = 0;
		aOther.myName = "Invalid";
		aOther.myCreateComponentFunctionPointer = nullptr;
		aOther.myCopyComponentFunctionPointer = nullptr;
		aOther.myMoveComponentFunctionPointer = nullptr;
		aOther.myDestroyComponentFunctionPointer = nullptr;
	}

	ComponentTypeIdentity& ComponentTypeIdentity::operator=(ComponentTypeIdentity&& aOther) noexcept
	{
		if (this != &aOther)
		{
			this->myID = aOther.myID;
			this->myAlignment = aOther.myAlignment;
			this->mySize = aOther.mySize;
			this->myName = aOther.myName;
			this->myCreateComponentFunctionPointer = aOther.myCreateComponentFunctionPointer;
			this->myCopyComponentFunctionPointer = aOther.myCopyComponentFunctionPointer;
			this->myMoveComponentFunctionPointer = aOther.myMoveComponentFunctionPointer;
			this->myDestroyComponentFunctionPointer = aOther.myDestroyComponentFunctionPointer;

			aOther.myID = GLOBAL_MAX_COMPONENTS;
			aOther.myAlignment = 0;
			aOther.mySize = 0;
			aOther.myName = "Invalid";
			aOther.myCreateComponentFunctionPointer = nullptr;
			aOther.myCopyComponentFunctionPointer = nullptr;
			aOther.myMoveComponentFunctionPointer = nullptr;
			aOther.myDestroyComponentFunctionPointer = nullptr;
		}

		return *this;
	}
}
