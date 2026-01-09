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
		MemoryPool(const ComponentTypeIdentity& aComponentTypeIdentity, const size_t aReservedCount = 1);
		~MemoryPool();

		MemoryPool(const MemoryPool& aOther);
		MemoryPool& operator=(const MemoryPool& aOther);

		MemoryPool(MemoryPool&& aOther) noexcept;
		MemoryPool& operator=(MemoryPool&& aOther) noexcept;

		size_t CreateObject(const void* aDefaultValue = nullptr);

		template<IsComponent T>
		T* GetObjectAtIndex(const size_t aIndex);

		size_t GetAvailableMemorySpace() const;
		size_t GetOccupiedMemorySpace() const;
		size_t GetCapacity() const;

		void PrintMemoryStatus() const;
	private:
		void Allocate(const size_t aSize);
		void Reallocate(const size_t aRequiredAdditionalBytes);
		void CopyComponents(const MemoryPool& aSource);
	private:
		ComponentTypeIdentity myComponentTypeIdentity;

		Byte* myCurrentMemoryAddress;
		Byte* myStartMemoryAddress;
		Byte* myEndMemoryAddress;

		size_t myCount;
	};

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

		return reinterpret_cast<T*>(myStartMemoryAddress + myComponentTypeIdentity.GetSize() * aIndex);
	}
}
