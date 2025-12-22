#include "ECSRegistry.hpp"

namespace Simple
{
	ECSRegistry* ECSRegistry::GetInstance()
	{
		if (!myPtr)
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
}