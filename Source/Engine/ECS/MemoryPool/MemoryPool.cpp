#include "MemoryPool.hpp"
#include <new>
#include <cstring>
#include <iostream>

namespace Simple
{
	MemoryPool::MemoryPool(const size_t aTypeSize, const size_t aTypeAlignment, const size_t aReserveAmount)
		: myCurrentMemoryAddress(nullptr)
		, myStartMemoryAddress(nullptr)
		, myEndMemoryAddress(nullptr)
		, myCreateObjectFunction(nullptr)
		, myMoveObjectFunction(nullptr)
		, myDestroyObjectFunction(nullptr)
		, myAlignment(aTypeAlignment)
		, myCount(0)
		, mySize(aTypeSize)
	{
		Allocate(aTypeSize * aReserveAmount);
	}

	MemoryPool::~MemoryPool()
	{
		for (size_t i = 0; i < myCount; i++)
		{
			myDestroyObjectFunction(myStartMemoryAddress + mySize * i);
		}

		::operator delete(myStartMemoryAddress, std::align_val_t(myAlignment));

		myStartMemoryAddress = nullptr;
		myEndMemoryAddress = nullptr;
		myCurrentMemoryAddress = nullptr;
		myCreateObjectFunction = nullptr;
		myMoveObjectFunction = nullptr;
		myDestroyObjectFunction = nullptr;
		myAlignment = 0;
		myCount = 0;
		mySize = 0;
	}

	MemoryPool::MemoryPool(MemoryPool&& aOther) noexcept
		: myCurrentMemoryAddress(aOther.myCurrentMemoryAddress)
		, myStartMemoryAddress(aOther.myStartMemoryAddress)
		, myEndMemoryAddress(aOther.myEndMemoryAddress)
		, myCreateObjectFunction(aOther.myCreateObjectFunction)
		, myMoveObjectFunction(aOther.myMoveObjectFunction)
		, myDestroyObjectFunction(aOther.myDestroyObjectFunction)
		, myAlignment(aOther.myAlignment)
		, myCount(aOther.myCount)
		, mySize(aOther.mySize)
	{
		aOther.myCurrentMemoryAddress = nullptr;
		aOther.myStartMemoryAddress = nullptr;
		aOther.myEndMemoryAddress = nullptr;
		aOther.myCreateObjectFunction = nullptr;
		aOther.myMoveObjectFunction = nullptr;
		aOther.myDestroyObjectFunction = nullptr;
		aOther.myAlignment = 0;
		aOther.myCount = 0;
		aOther.mySize = 0;
	}

	MemoryPool& MemoryPool::operator=(MemoryPool&& aOther) noexcept
	{
		if (this != &aOther)
		{
			myCurrentMemoryAddress = aOther.myCurrentMemoryAddress;
			myStartMemoryAddress = aOther.myStartMemoryAddress;
			myEndMemoryAddress = aOther.myEndMemoryAddress;
			myCreateObjectFunction = aOther.myCreateObjectFunction;
			myMoveObjectFunction = aOther.myMoveObjectFunction;
			myDestroyObjectFunction = aOther.myDestroyObjectFunction;
			myAlignment = aOther.myAlignment;
			myCount = aOther.myCount;
			mySize = aOther.mySize;

			aOther.myCurrentMemoryAddress = nullptr;
			aOther.myStartMemoryAddress = nullptr;
			aOther.myEndMemoryAddress = nullptr;
			aOther.myCreateObjectFunction = nullptr;
			aOther.myMoveObjectFunction = nullptr;
			aOther.myDestroyObjectFunction = nullptr;
			aOther.myAlignment = 0;
			aOther.myCount = 0;
			aOther.mySize = 0;
		}

		return *this;
	}

	size_t MemoryPool::GetAvailableMemorySpace() const
	{
		return myEndMemoryAddress - myCurrentMemoryAddress;
	}

	size_t MemoryPool::GetOccupiedMemorySpace() const
	{
		return myCurrentMemoryAddress - myStartMemoryAddress;
	}

	size_t MemoryPool::GetCapacity() const
	{
		return myEndMemoryAddress - myStartMemoryAddress;
	}

	void MemoryPool::PrintMemoryStatus() const
	{
		std::cout << "--------------------" << std::endl;
		std::cout << "Memory Status:" << std::endl;
		std::cout << "Start Address: " << static_cast<void*>(myStartMemoryAddress) << std::endl;
		std::cout << "End Address: " << static_cast<void*>(myEndMemoryAddress) << std::endl;
		std::cout << "Current Address: " << static_cast<void*>(myCurrentMemoryAddress) << std::endl;
		std::cout << "Memory Capacity: " << GetCapacity() << std::endl;
		std::cout << "Occupied Memory Space: " << GetOccupiedMemorySpace() << std::endl;
		std::cout << "Avaliable Memory Space: " << GetAvailableMemorySpace() << std::endl;
		std::cout << "Component Count: " << myCount << std::endl;
		std::cout << "--------------------" << std::endl;
	}

	void MemoryPool::Allocate(const size_t aSize)
	{
		myStartMemoryAddress = static_cast<Byte*>(::operator new(aSize, std::align_val_t(myAlignment)));
		myEndMemoryAddress = myStartMemoryAddress + aSize;
		myCurrentMemoryAddress = myStartMemoryAddress;

		std::memset(myStartMemoryAddress, 0xFF, aSize);
	}

	void MemoryPool::Reallocate(const size_t aRequiredAdditionalBytes)
	{
		Byte* const oldStartMemoryAddress = myStartMemoryAddress;

		const size_t occupiedMemorySpace = GetOccupiedMemorySpace();
		const size_t oldMemoryCapacity = GetCapacity();

		size_t newMemoryCapacity = oldMemoryCapacity > 0 ? oldMemoryCapacity * 2 : aRequiredAdditionalBytes;

		while (newMemoryCapacity < (occupiedMemorySpace + aRequiredAdditionalBytes))
		{
			newMemoryCapacity *= 2;
		}

		Allocate(newMemoryCapacity);

		for (size_t i = 0; i < myCount; i++)
		{
			Byte* oldComponentAddress = oldStartMemoryAddress + mySize * i;
			Byte* newComponentAddress = myStartMemoryAddress + mySize * i;

			myMoveObjectFunction(newComponentAddress, oldComponentAddress);
		}

		::operator delete(oldStartMemoryAddress, std::align_val_t(myAlignment));

		myCurrentMemoryAddress = myStartMemoryAddress + occupiedMemorySpace;
		myEndMemoryAddress = myStartMemoryAddress + newMemoryCapacity;
	}

	size_t MemoryPool::CreateObject()
	{
		if (mySize > GetAvailableMemorySpace())
		{
			Reallocate(mySize);
		}

		myCurrentMemoryAddress += myCreateObjectFunction(myCurrentMemoryAddress);
		return myCount++;
	}
}
