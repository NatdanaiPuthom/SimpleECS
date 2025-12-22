#pragma once
#include "ECS/Component.hpp"
#include <cstddef>
#include <type_traits>

namespace Simple
{
	class MemoryPool final
	{
	public:
		MemoryPool();
		MemoryPool(const size_t aTypeSize, const size_t aTypeAlignment, const size_t aTypeHashCode, const size_t aReserveAmount = 8);
		~MemoryPool();

		MemoryPool(const MemoryPool& ) = delete;
		MemoryPool& operator=(const MemoryPool& ) = delete;

		MemoryPool(MemoryPool&& aOther) noexcept;
		MemoryPool& operator=(MemoryPool&& aOther) noexcept;
	public:
		template<typename T> requires std::is_base_of_v<Component, T>
		T* CreateComponent(const T& aValue = T());
	public:
		size_t GetAvailableMemorySpace() const;
		size_t GetOccupiedMemorySpace() const;
		size_t GetCapacity() const;
	public:
		void PrintMemoryStatus() const;
	private:
		void Init(const size_t aTypeAlignment, const size_t aTypeHashCode);
		void Allocate(const size_t aSize);
		void Reallocate();
	private:
		std::byte* myCurrentMemoryAddress;
		std::byte* myStartMemoryAddress;
		std::byte* myEndMemoryAddress;

		size_t myTypeHashCode;
		size_t myTypeAlignment;
	};

	template<typename T> requires std::is_base_of_v<Component, T>
	inline T* MemoryPool::CreateComponent(const T& aValue)
	{
		while (sizeof(T) > GetAvailableMemorySpace())
		{
			Reallocate();
		}

		const size_t index = (myCurrentMemoryAddress - myStartMemoryAddress) / sizeof(T);
		const T* component = new(myCurrentMemoryAddress)T(aValue);
		myCurrentMemoryAddress += sizeof(T);

		return component;
	}
}