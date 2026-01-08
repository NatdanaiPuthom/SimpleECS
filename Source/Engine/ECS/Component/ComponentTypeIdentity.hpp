#pragma once
#include "Component.hpp"
#include <typeindex>

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
		using CreateComponentFunctionPtr = size_t(*)(void* aAddress);
		using MoveComponentFunctionPtr = void(*)(void* aDestination, void* aSource);
		using DestroyComponentFunctionPtr = void(*)(void* aComponentAddress);

		ComponentTypeIdentity();
		ComponentTypeIdentity(const size_t aID, const char* aName, CreateComponentFunctionPtr aCreateFunctionPtr, MoveComponentFunctionPtr aMoveFunctionPtr, DestroyComponentFunctionPtr aDestroyFunctionPtr);

		~ComponentTypeIdentity();

		template<IsComponent T>
		static ComponentTypeIdentity GetComponentTypeIdentity();

		size_t InvokeCreate(void* aAddress) const;
		void InvokeMove(void* aDestinationAddress, void* aSourceAddress) const;
		void InvokeDestroy(void* aComponentAddress) const;

		bool IsValid() const;

		size_t GetID() const;
		const char* GetName() const;

		bool operator==(const ComponentTypeIdentity& aOther) const;
		bool operator!=(const ComponentTypeIdentity& aOther) const;

		ComponentTypeIdentity(const ComponentTypeIdentity& aOther);
		ComponentTypeIdentity& operator=(const ComponentTypeIdentity& aOther);

		ComponentTypeIdentity(ComponentTypeIdentity&& aOther) noexcept;
		ComponentTypeIdentity& operator=(ComponentTypeIdentity&& aOther) noexcept;

	private:
		size_t myID;
		const char* myName;
		CreateComponentFunctionPtr myCreateComponentFunctionPointer;
		MoveComponentFunctionPtr myMoveComponentFunctionPointer;
		DestroyComponentFunctionPtr myDestroyComponentFunctionPointer;
	};

	template<IsComponent T>
	inline ComponentTypeIdentity ComponentTypeIdentity::GetComponentTypeIdentity()
	{
		static const char* name = typeid(T).name();
		static const size_t id = ComponentIdentityID<T>::GetID();

		static CreateComponentFunctionPtr createFunctionPointer = [](void* aAddress) -> size_t
			{
				[[maybe_unused]] T* component = new(aAddress)T();
				return sizeof(T);
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

		static ComponentTypeIdentity componentIdentity(id, name, createFunctionPointer, moveFunctionPointer, destroyFunctionPointer);
		return componentIdentity;
	}
}

//TO-DO 08/01/2026 Insider [11312.210]: Intellisense -> E3244: template constraint not satisfied type constraint failed for "int" atomic constraint evaluates to false
//Comment: I tried, I have no clue, GPT no clue also Claude
