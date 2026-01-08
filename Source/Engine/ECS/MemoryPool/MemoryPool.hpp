#pragma once
#include "ECS/Component/Component.hpp"
#include "ECS/Component/ComponentTypeIdentity.hpp"
#include "Debugger/Assert.hpp"
#include <string_view>

namespace Simple
{
	class MemoryPool final
	{
	private:
		enum class Byte : unsigned char {};
	public:

		template<IsComponent T>
		static MemoryPool CreatePool();

		~MemoryPool();

		MemoryPool(const MemoryPool& aOther) = delete;
		MemoryPool& operator=(const MemoryPool& aOther) = delete;

		MemoryPool(MemoryPool&& aOther) noexcept;
		MemoryPool& operator=(MemoryPool&& aOther) noexcept;

		size_t CreateObject();

		template<IsComponent T>
		T* GetObjectAtIndex(const size_t aIndex);

		size_t GetAvailableMemorySpace() const;
		size_t GetOccupiedMemorySpace() const;
		size_t GetCapacity() const;

		void PrintMemoryStatus() const;
	private:
		MemoryPool(const size_t aTypeSize, const size_t aTypeAlignment, const size_t aReserveAmount = 1);

		void Allocate(const size_t aSize);
		void Reallocate(const size_t aRequiredAdditionalBytes);
	private:
		ComponentTypeIdentity myComponentTypeIdentity;

		Byte* myCurrentMemoryAddress;
		Byte* myStartMemoryAddress;
		Byte* myEndMemoryAddress;

		size_t myAlignment;
		size_t mySize;
		size_t myCount;
	};

	template<IsComponent T>
	inline MemoryPool MemoryPool::CreatePool()
	{
		MemoryPool pool(sizeof(T), alignof(T));
		pool.myComponentTypeIdentity = ComponentTypeIdentity::GetComponentTypeIdentity<T>();

		return pool;
	}

	template<IsComponent T>
	inline T* MemoryPool::GetObjectAtIndex(const size_t aIndex)
	{
		if (ComponentTypeIdentity::GetComponentTypeIdentity<T>() != myComponentTypeIdentity)
		{
			const std::string_view expected = myComponentTypeIdentity.GetName();
			const std::string_view actual = typeid(T).name();

			DebugAssert(false, "Component Type Mismatch", expected.data(), actual.data());
			return nullptr;
		}

		return reinterpret_cast<T*>(myStartMemoryAddress + mySize * aIndex);
	}
}
