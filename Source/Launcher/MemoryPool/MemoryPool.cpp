#include "MemoryPool.hpp"
#include <cstring>
#include <new>

namespace Simple
{
	MemoryPool::MemoryPool(const size_t aTypeSize, const size_t aTypeAlignment, const size_t aTypeHashCode, const size_t aReserveAmount)
		: myCurrentMemoryAddress(nullptr)
		, myStartMemoryAddress(nullptr)
		, myEndMemoryAddress(nullptr)
		, myTypeHashCode(aTypeHashCode)
		, myTypeAlignment(aTypeAlignment)
	{
		myStartMemoryAddress = static_cast<char*>(::operator new[](aTypeSize* aReserveAmount, std::align_val_t(aTypeAlignment)));
		myEndMemoryAddress = myStartMemoryAddress + (aTypeSize * aReserveAmount);
		myCurrentMemoryAddress = myStartMemoryAddress;

		std::memset(myStartMemoryAddress, 0xFF, aTypeSize * aReserveAmount);
	}

	MemoryPool::~MemoryPool()
	{
		::operator delete[](myStartMemoryAddress, std::align_val_t(myTypeAlignment));

		myStartMemoryAddress = nullptr;
		myEndMemoryAddress = nullptr;
		myCurrentMemoryAddress = nullptr;
	}
}