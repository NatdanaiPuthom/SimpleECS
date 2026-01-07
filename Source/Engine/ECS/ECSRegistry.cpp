#include "ECSRegistry.hpp"

namespace Simple
{
	ECSRegistry::ECSRegistry()
	{
	}

	ECSRegistry* ECSRegistry::GetInstance()
	{
		if (myPtr == nullptr)
		{
			myPtr = new ECSRegistry();
		}

		return myPtr;
	}

	void ECSRegistry::Destroy()
	{
		delete myPtr;
		myPtr = nullptr;
	}

	const std::unordered_map<ECSComponentHashCode, TypeErasureComponent, RegisteredComponentsIdentityHash>& ECSRegistry::GetRegisteredComponents() const
	{
		return myRegisteredComponents;
	}
}