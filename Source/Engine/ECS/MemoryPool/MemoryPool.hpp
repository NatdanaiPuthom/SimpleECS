#pragma once
#include "ECS/Concepts/Component.hpp"
#include "ECS/Component/ComponentTypeIdentity.hpp"
#include "Debugger/Assert.hpp"
#include <string_view>
#include <span>

namespace Simple
{
	class MemoryPool final
	{
	public:
		struct OperationStatus
		{
			size_t createdObjectIndex = 0;
			bool success = false;
		};
	private:
		enum class Byte : unsigned char {};
	public:
		MemoryPool(const ComponentTypeIdentity& aComponentTypeIdentity, const size_t aReservedCount = 1);
		~MemoryPool();

		MemoryPool(const MemoryPool& aOther);
		MemoryPool& operator=(const MemoryPool& aOther);

		MemoryPool(MemoryPool&& aOther) noexcept;
		MemoryPool& operator=(MemoryPool&& aOther) noexcept;

		const MemoryPool::OperationStatus CreateObject(const void* aDefaultValue = nullptr);

		size_t DestroyObject(const size_t aIndex);

		template<IsComponent T>
		T* GetObjectAtIndex(const size_t aIndex) const;

		template<IsComponent T>
		std::span<T> GetObjects() const;

		size_t GetAvailableMemorySpace() const;
		size_t GetOccupiedMemorySpace() const;
		size_t GetCapacity() const;

		void PrintMemoryStatus() const;
	private:
		bool Allocate(const size_t aSize);
		bool Reallocate(const size_t aRequiredAdditionalBytes);
		void CopyObjeccts(const MemoryPool& aSource);
	private:
		ComponentTypeIdentity myComponentTypeIdentity;

		Byte* myCurrentMemoryAddress;
		Byte* myStartMemoryAddress;
		Byte* myEndMemoryAddress;

		size_t myCount;
	};

	template<IsComponent T>
	inline T* MemoryPool::GetObjectAtIndex(const size_t aIndex) const
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

	template<IsComponent T>
	inline std::span<T> MemoryPool::GetObjects() const
	{
		return std::span<T>(reinterpret_cast<T*>(myStartMemoryAddress), myCount);
	}
}
