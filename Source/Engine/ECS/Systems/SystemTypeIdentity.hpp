#pragma once
#include "ECS/Concepts/System.hpp"
#include <string>

namespace Simple
{
	inline size_t SystemIdentityIDGenerateNextID()
	{
		static size_t counter = 0;
		return counter++;
	}

	template<IsSystem T>
	struct SystemIdentityID final
	{
	public:
		static size_t GetID()
		{
			static const size_t id = SystemIdentityIDGenerateNextID();
			return id;
		}

		static const char* GetName()
		{
			static const char* name = typeid(T).name();
			return name;
		}
	};

	class SystemTypeIdentity final
	{
	private:
		using CloneSystemFunctionPtr = std::unique_ptr<System>(*)();
	public:
		SystemTypeIdentity();
		SystemTypeIdentity(const char* aName, const size_t aID, CloneSystemFunctionPtr aCloneFunctionPtr);

		~SystemTypeIdentity();

		SystemTypeIdentity(const SystemTypeIdentity& aOther) noexcept;
		SystemTypeIdentity& operator=(const SystemTypeIdentity& aOther) noexcept;

		SystemTypeIdentity(SystemTypeIdentity&& aOther) noexcept;
		SystemTypeIdentity& operator=(SystemTypeIdentity&& aOther) noexcept;

		size_t GetID() const;
		const std::string& GetName() const;

		std::unique_ptr<System> Clone() const;

		template<IsSystem T>
		static SystemTypeIdentity GetSystemTypeIdentity();

	private:
		std::string myName;
		size_t myID;
		CloneSystemFunctionPtr myCloneSystemFunctionPointer;
	};

	template<IsSystem T>
	inline SystemTypeIdentity SystemTypeIdentity::GetSystemTypeIdentity()
	{
		static const char* name = SystemIdentityID<T>::GetName();
		static const size_t id = SystemIdentityID<T>::GetID();

		static CloneSystemFunctionPtr cloneSystemFunctionPointer = []() -> std::unique_ptr<System>
			{
				return std::make_unique<T>();
			};

		static SystemTypeIdentity systemIdentity(name, id, cloneSystemFunctionPointer);
		return systemIdentity;
	}
}
