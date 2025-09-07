#include "MemoryPool.hpp"

namespace Simple
{
	MemoryPool::MemoryPool(const size_t aTypeSize, const size_t aTypeAlignment, const size_t aTypeHashCode, const std::string& aComponentName, const size_t aDefaultMemoryReserveSize)
		: myCurrentMemoryAddress(nullptr)
		, myStartMemoryAddress(nullptr)
		, myEndMemoryAddress(nullptr)
		, myName(aComponentName)
		, myTypeHashCode(aTypeHashCode)
		, myTypeAlignment(aTypeAlignment)
	{
		myStartMemoryAddress = static_cast<char*>(::operator new[](aTypeSize* aDefaultMemoryReserveSize, std::align_val_t(aTypeAlignment)));
		myEndMemoryAddress = myStartMemoryAddress + (aTypeSize * aDefaultMemoryReserveSize);
		myCurrentMemoryAddress = myStartMemoryAddress;

		std::memset(myStartMemoryAddress, 0xFF, aTypeSize * aDefaultMemoryReserveSize);
	}

	MemoryPool::~MemoryPool()
	{
		::operator delete[](myStartMemoryAddress, std::align_val_t(myTypeAlignment));

		myStartMemoryAddress = nullptr;
		myEndMemoryAddress = nullptr;
		myCurrentMemoryAddress = nullptr;
	}
}