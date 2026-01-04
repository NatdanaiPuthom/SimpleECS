#pragma once
#include "Component.hpp"
#include <typeindex>

namespace Simple
{
	class ComponentTypeIdentity final
	{
	public:
		ComponentTypeIdentity();
		ComponentTypeIdentity(const size_t aID, const char* aName);

		~ComponentTypeIdentity();

		template<IsComponent T> 
		static ComponentTypeIdentity GetComponentTypeIdentity();

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
		static const size_t GetNextComponentID();
	private:
		size_t myID;
		const char* myName;
	};

	template<IsComponent T>
	inline ComponentTypeIdentity ComponentTypeIdentity::GetComponentTypeIdentity()
	{
		static const char* name = typeid(T).name();
		static const size_t id = GetNextComponentID();
		static ComponentTypeIdentity componentIdentity(id, name);

		return componentIdentity;
	}
}
