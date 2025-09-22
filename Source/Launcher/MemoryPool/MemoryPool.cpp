#include "MemoryPool.hpp"
#include <cstring>
#include <new>
#include <iostream>

namespace Simple
{
	MemoryPool::MemoryPool(const size_t aTypeSize, const size_t aTypeAlignment, const size_t aTypeHashCode, const size_t aReserveAmount)
		: myCurrentMemoryAddress(nullptr)
		, myStartMemoryAddress(nullptr)
		, myEndMemoryAddress(nullptr)
		, myTypeHashCode(aTypeHashCode)
		, myTypeAlignment(aTypeAlignment)
	{
		Allocate(aTypeSize * aReserveAmount);
	}

	MemoryPool::~MemoryPool()
	{
		::operator delete[](myStartMemoryAddress, std::align_val_t(myTypeAlignment));

		myStartMemoryAddress = nullptr;
		myEndMemoryAddress = nullptr;
		myCurrentMemoryAddress = nullptr;
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