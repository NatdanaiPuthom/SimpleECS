#include "MemoryPool.hpp"
#include <new>
#include <cstring>
#include <iostream>

namespace Simple
{
	MemoryPool::MemoryPool(const ComponentTypeIdentity& aComponentTypeIdentity, const size_t aReservedCount)
		: myComponentTypeIdentity(aComponentTypeIdentity)
		, myCurrentMemoryAddress(nullptr)
		, myStartMemoryAddress(nullptr)
		, myEndMemoryAddress(nullptr)
		, myCount(0)
	{
		Allocate(myComponentTypeIdentity.GetSize() * aReservedCount);
	}

	MemoryPool::~MemoryPool()
	{
		for (size_t i = 0; i < myCount; i++)
		{
			myComponentTypeIdentity.InvokeDestroy(myStartMemoryAddress + myComponentTypeIdentity.GetSize() * i);
		}

		::operator delete(myStartMemoryAddress, std::align_val_t(myComponentTypeIdentity.GetAlignment()));

		myStartMemoryAddress = nullptr;
		myEndMemoryAddress = nullptr;
		myCurrentMemoryAddress = nullptr;
		myCount = 0;
	}

	MemoryPool::MemoryPool(const MemoryPool& aOther)
		: myComponentTypeIdentity(aOther.myComponentTypeIdentity)
		, myCount(aOther.myCount)
		, myCurrentMemoryAddress(nullptr)
		, myStartMemoryAddress(nullptr)
		, myEndMemoryAddress(nullptr)
	{
		Allocate(myComponentTypeIdentity.GetSize() * myCount);

		const size_t size = this->myComponentTypeIdentity.GetSize();

		for (size_t i = 0; i < this->myCount; i++)
		{
			Byte* destination = this->myStartMemoryAddress + size * i;
			Byte* source = aOther.myStartMemoryAddress + size * i;
			this->myComponentTypeIdentity.InvokeCopy(destination, source);
		}
	}

	MemoryPool& Simple::MemoryPool::operator=(const MemoryPool& aOther)
	{
		if (this != &aOther)
		{
			this->myComponentTypeIdentity = aOther.myComponentTypeIdentity;
			this->myCount = aOther.myCount;
			this->myCurrentMemoryAddress = nullptr;
			this->myStartMemoryAddress = nullptr;
			this->myEndMemoryAddress = nullptr;

			Allocate(myComponentTypeIdentity.GetSize() * myCount);

			const size_t size = this->myComponentTypeIdentity.GetSize();

			for (size_t i = 0; i < this->myCount; i++)
			{
				Byte* destination = this->myStartMemoryAddress + size * i;
				const Byte* source = aOther.myStartMemoryAddress + size * i;
				this->myComponentTypeIdentity.InvokeCopy(destination, source);
			}
		}

		return *this;
	}

	MemoryPool::MemoryPool(MemoryPool&& aOther) noexcept
		: myCurrentMemoryAddress(aOther.myCurrentMemoryAddress)
		, myStartMemoryAddress(aOther.myStartMemoryAddress)
		, myEndMemoryAddress(aOther.myEndMemoryAddress)
		, myCount(aOther.myCount)
	{
		aOther.myCurrentMemoryAddress = nullptr;
		aOther.myStartMemoryAddress = nullptr;
		aOther.myEndMemoryAddress = nullptr;
		aOther.myCount = 0;
	}

	MemoryPool& MemoryPool::operator=(MemoryPool&& aOther) noexcept
	{
		if (this != &aOther)
		{
			this->myCurrentMemoryAddress = aOther.myCurrentMemoryAddress;
			this->myStartMemoryAddress = aOther.myStartMemoryAddress;
			this->myEndMemoryAddress = aOther.myEndMemoryAddress;
			this->myCount = aOther.myCount;

			aOther.myCurrentMemoryAddress = nullptr;
			aOther.myStartMemoryAddress = nullptr;
			aOther.myEndMemoryAddress = nullptr;
			aOther.myCount = 0;
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
		myStartMemoryAddress = static_cast<Byte*>(::operator new(aSize, std::align_val_t(myComponentTypeIdentity.GetAlignment())));
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
			Byte* oldComponentAddress = oldStartMemoryAddress + myComponentTypeIdentity.GetSize() * i;
			Byte* newComponentAddress = myStartMemoryAddress + myComponentTypeIdentity.GetSize() * i;

			myComponentTypeIdentity.InvokeMove(newComponentAddress, oldComponentAddress);
		}

		::operator delete(oldStartMemoryAddress, std::align_val_t(myComponentTypeIdentity.GetAlignment()));

		myCurrentMemoryAddress = myStartMemoryAddress + occupiedMemorySpace;
		myEndMemoryAddress = myStartMemoryAddress + newMemoryCapacity;
	}

	size_t MemoryPool::CreateObject(const void* aDefaultValue)
	{
		if (myComponentTypeIdentity.GetSize() > GetAvailableMemorySpace())
		{
			Reallocate(myComponentTypeIdentity.GetSize());
		}

		myCurrentMemoryAddress += myComponentTypeIdentity.InvokeCreate(myCurrentMemoryAddress, aDefaultValue);
		return myCount++;
	}
}
