#pragma once
#include <cstddef>

namespace Simple
{
	class MemoryPool final
	{
	public:
		MemoryPool(const size_t aTypeSize, const size_t aTypeAlignment, const size_t aTypeHashCode, const size_t aReserveAmount = 8);
		~MemoryPool();
	public:
		template<typename T>
		T* CreateComponent(const T& aValue = T());
	public:
		size_t GetAvailableMemorySpace() const;
		size_t GetOccupiedMemorySpace() const;
		size_t GetCapacity() const;
	public:
		void PrintMemoryStatus() const;
	private:
		void Allocate(const size_t aSize);
		void Reallocate();
	private:
		std::byte* myCurrentMemoryAddress;
		std::byte* myStartMemoryAddress;
		std::byte* myEndMemoryAddress;

		size_t myTypeHashCode;
		size_t myTypeAlignment;
	};

	template<typename T>
	inline T* MemoryPool::CreateComponent(const T& aValue)
	{
		while (sizeof(T) > GetAvailableMemorySpace())
		{
			Reallocate();
		}

		T* component = new(myCurrentMemoryAddress)T(aValue);
		myCurrentMemoryAddress += sizeof(T);

		return component;
	}
}