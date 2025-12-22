#include "MemoryPool.hpp"
#include <cstring>
#include <new>
#include <iostream>

namespace Simple
{
	MemoryPool::MemoryPool()
	{
		Init(0, 0);
		static unsigned int count = 0;
		std::cout << "Initialized Empty MemoryPool: " << ++count << std::endl;
	}

	MemoryPool::MemoryPool(const size_t aTypeSize, const size_t aTypeAlignment, const size_t aTypeHashCode, const size_t aReserveAmount)
	{
		Init(aTypeAlignment, aTypeHashCode);
		Allocate(aTypeSize * aReserveAmount);

		static unsigned int count = 0;
		std::cout << "Initialized  MemoryPool: " << ++count << std::endl;
	}

	MemoryPool::~MemoryPool()
	{
		::operator delete[](myStartMemoryAddress, std::align_val_t(myTypeAlignment));

		myStartMemoryAddress = nullptr;
		myEndMemoryAddress = nullptr;
		myCurrentMemoryAddress = nullptr;
	}

	void MemoryPool::Init(const size_t aTypeAlignment, const size_t aTypeHashCode)
	{
		myCurrentMemoryAddress = nullptr;
		myStartMemoryAddress = nullptr;
		myEndMemoryAddress = nullptr;
		myTypeHashCode = aTypeHashCode;
		myTypeAlignment = aTypeAlignment;
	}

	MemoryPool::MemoryPool(MemoryPool&& aOther) noexcept
		: myCurrentMemoryAddress(aOther.myCurrentMemoryAddress)
		, myStartMemoryAddress(aOther.myStartMemoryAddress)
		, myEndMemoryAddress(aOther.myEndMemoryAddress)
		, myTypeHashCode(aOther.myTypeHashCode)
		, myTypeAlignment(aOther.myTypeAlignment)
	{
		aOther.myCurrentMemoryAddress = nullptr;
		aOther.myStartMemoryAddress = nullptr;
		aOther.myEndMemoryAddress = nullptr;
		aOther.myTypeHashCode = 0;
		aOther.myTypeAlignment = 0;
	}

	MemoryPool& MemoryPool::operator=(MemoryPool&& aOther) noexcept
	{
		if (this != &aOther)
		{
			myCurrentMemoryAddress = aOther.myCurrentMemoryAddress;
			myStartMemoryAddress = aOther.myStartMemoryAddress;
			myEndMemoryAddress = aOther.myEndMemoryAddress;
			myTypeHashCode = aOther.myTypeHashCode;
			myTypeAlignment = aOther.myTypeAlignment;

			aOther.myCurrentMemoryAddress = nullptr;
			aOther.myStartMemoryAddress = nullptr;
			aOther.myEndMemoryAddress = nullptr;
			aOther.myTypeHashCode = 0;
			aOther.myTypeAlignment = 0;
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
		std::cout << "----------" << std::endl;
		std::cout << "Memory Status:" << std::endl;
		std::cout << "Start Address: " << static_cast<void*>(myStartMemoryAddress) << std::endl;
		std::cout << "End Address: " << static_cast<void*>(myEndMemoryAddress) << std::endl;
		std::cout << "Current Address: " << static_cast<void*>(myCurrentMemoryAddress) << std::endl;
		std::cout << "Memory Capacity: " << GetCapacity() << std::endl;
		std::cout << "Occupied Memory Space: " << GetOccupiedMemorySpace() << std::endl;
		std::cout << "Avaliable Memory Space: " << GetAvailableMemorySpace() << std::endl;
		std::cout << "----------" << std::endl;
	}

	void MemoryPool::Allocate(const size_t aSize)
	{
		myStartMemoryAddress = static_cast<std::byte*>(::operator new[](aSize, std::align_val_t(myTypeAlignment)));
		myEndMemoryAddress = myStartMemoryAddress + (aSize);
		myCurrentMemoryAddress = myStartMemoryAddress;

		std::memset(myStartMemoryAddress, 0xFF, aSize);
	}

	void MemoryPool::Reallocate()
	{
		std::byte* const oldStartMemoryAddress = myStartMemoryAddress;

		const size_t occupiedMemorySpace = GetOccupiedMemorySpace();
		const size_t oldMemoryCapacity = GetCapacity();
		const size_t newMemoryCapacity = oldMemoryCapacity * 2;

		Allocate(newMemoryCapacity);

		std::memcpy(myStartMemoryAddress, oldStartMemoryAddress, occupiedMemorySpace);
		::operator delete[](oldStartMemoryAddress, std::align_val_t(myTypeAlignment));

		myCurrentMemoryAddress = myStartMemoryAddress + occupiedMemorySpace;
		myEndMemoryAddress = myStartMemoryAddress + newMemoryCapacity;
	}
}
