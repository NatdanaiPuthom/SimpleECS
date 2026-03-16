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

	const std::unordered_map<size_t, const ComponentTypeIdentity>& ECSRegistry::GetRegisteredComponents() const
	{
		return myRegisteredComponentIdentities;
	}

	const std::unordered_map<size_t, const SystemTypeIdentity>& ECSRegistry::GetRegisteredSystems() const
	{
		return myRegisteredSystemIdentities;
	}

	const ComponentTypeIdentity& ECSRegistry::GetComponentTypeIdentity(const size_t aID) const
	{
		const auto it = myRegisteredComponentIdentities.find(aID);

		if (it != myRegisteredComponentIdentities.end())
		{
			return it->second;
		}

		DebugAssert(false, "Component is not registered.");
		static ComponentTypeIdentity dummy;
		return dummy;
	}

	const SystemTypeIdentity& ECSRegistry::GetSystemTypeIdentity(const size_t aID) const
	{
		const auto it = myRegisteredSystemIdentities.find(aID);

		if (it != myRegisteredSystemIdentities.end())
		{
			return it->second;
		}

		DebugAssert(false, "System is not registered.");
		static SystemTypeIdentity dummy;
		return dummy;
	}
}