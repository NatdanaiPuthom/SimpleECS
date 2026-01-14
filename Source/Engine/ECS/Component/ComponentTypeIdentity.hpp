#pragma once
#include "Component.hpp"
#include <typeindex>
#include <type_traits>

namespace Simple
{
	inline size_t ComponentIdentityIDGenerateNextID()
	{
		static size_t counter = 1;
		return counter++;
	}

	template<IsComponent T>
	struct ComponentIdentityID final
	{
	public:
		static size_t GetID()
		{
			static const size_t id = ComponentIdentityIDGenerateNextID();
			return id;
		}
	};

	class ComponentTypeIdentity final
	{
	public:
		using CreateComponentFunctionPtr = size_t(*)(void* aDestinationAddress, const void* aDefaultValuePointer);
		using CopyComponentFunctionPtr = void(*)(void* aDestinationAddress,const void* aSourceAddress);
		using MoveComponentFunctionPtr = void(*)(void* aDestinationAddress, void* aSourceAddress);
		using DestroyComponentFunctionPtr = void(*)(void* aComponentAddress);

		ComponentTypeIdentity();
		ComponentTypeIdentity(const size_t aID, const size_t aAlignment, const size_t aSize, const char* aName, CreateComponentFunctionPtr aCreateFunctionPtr, CopyComponentFunctionPtr aCopyFunctionPtr, MoveComponentFunctionPtr aMoveFunctionPtr, DestroyComponentFunctionPtr aDestroyFunctionPtr);

		~ComponentTypeIdentity();

		template<IsComponent T>
		static ComponentTypeIdentity GetComponentTypeIdentity();

		size_t InvokeCreate(void* aDestinationAddress, const void* aDefaultValue) const;
		void InvokeCopy(void* aDestinationAddress, const void* aSourceAddress) const;
		void InvokeMove(void* aDestinationAddress, void* aSourceAddress) const;
		void InvokeDestroy(void* aComponentAddress) const;

		bool IsValid() const;

		size_t GetID() const;
		size_t GetAlignment() const;
		size_t GetSize() const;
		const char* GetName() const;

		bool operator==(const ComponentTypeIdentity& aOther) const;
		bool operator!=(const ComponentTypeIdentity& aOther) const;

		ComponentTypeIdentity(const ComponentTypeIdentity& aOther) noexcept;
		ComponentTypeIdentity& operator=(const ComponentTypeIdentity& aOther) noexcept;

		ComponentTypeIdentity(ComponentTypeIdentity&& aOther) noexcept;
		ComponentTypeIdentity& operator=(ComponentTypeIdentity&& aOther) noexcept;

	private:
		size_t myID;
		size_t myAlignment;
		size_t mySize;
		const char* myName;
		CreateComponentFunctionPtr myCreateComponentFunctionPointer;
		CopyComponentFunctionPtr myCopyComponentFunctionPointer;
		MoveComponentFunctionPtr myMoveComponentFunctionPointer;
		DestroyComponentFunctionPtr myDestroyComponentFunctionPointer;
	};

	template<IsComponent T>
	inline ComponentTypeIdentity ComponentTypeIdentity::GetComponentTypeIdentity()
	{
		static const char* name = typeid(T).name();
		static const size_t id = ComponentIdentityID<T>::GetID();

		static CreateComponentFunctionPtr createFunctionPointer = [](void* aDestinationAddress, const void* aDefaultValuePointer) -> size_t
			{
				if (aDefaultValuePointer != nullptr)
				{
					const T& defaultValue = *static_cast<const T*>(aDefaultValuePointer);
					new(aDestinationAddress)T(defaultValue);
				}
				else
				{
					new(aDestinationAddress)T();
				}

				return sizeof(T);
			};

		static CopyComponentFunctionPtr copyFunctionPointer = [](void* aDestinationAddress, const void* aSourceAddress) -> void
			{
				if constexpr (std::is_trivially_copyable_v<T>)
				{
					memcpy(aDestinationAddress, aSourceAddress, sizeof(T));
				}
				else
				{
					T& destination = *static_cast<T*>(aDestinationAddress);
					const T& source = *static_cast<const T*>(aSourceAddress);
					destination = source;
				}
			};

		static MoveComponentFunctionPtr moveFunctionPointer = [](void* aDestinationAddress, void* aSourceAddress) -> void
			{
				T* destination = static_cast<T*>(aDestinationAddress);
				T* source = static_cast<T*>(aSourceAddress);

				new(destination)T(std::move(*source));

				source->~T();
			};

		static DestroyComponentFunctionPtr destroyFunctionPointer = [](void* aComponentAddress) -> void
			{
				static_cast<T*>(aComponentAddress)->~T();
			};

		static ComponentTypeIdentity componentIdentity(id, alignof(T), sizeof(T), name, createFunctionPointer, copyFunctionPointer, moveFunctionPointer, destroyFunctionPointer);
		return componentIdentity;
	}
}

//TO-DO 08/01/2026 Insider [11312.210]: Intellisense -> E3244: template constraint not satisfied type constraint failed for "int" atomic constraint evaluates to false
//Comment: I tried, I have no clue, GPT no clue also Claude

//TO-DO  14/01/2026: Same issue with  Intellisense -> E3244 on Visual Studio 2026 Stable version
