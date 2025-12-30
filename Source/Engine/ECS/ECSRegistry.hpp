#pragma once
#include "Component.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <type_traits>
#include <typeinfo>

namespace Simple
{
	struct ComponentHashCode final
	{
		const size_t value;

		template<typename T> requires std::is_base_of_v<Component, T>
		static ComponentHashCode Create()
		{
			return ComponentHashCode(typeid(T).hash_code());
		}

		bool operator==(const ComponentHashCode& aOther) const noexcept
		{
			return this->value == aOther.value;
		}

	private:
		explicit ComponentHashCode(const size_t aHashCode) : value(aHashCode) {}
	};

	struct TypeErasureComponentProperty final
	{
		std::string name;
		size_t id;
		size_t offset;
	};

	struct TypeErasureComponent final
	{
		std::string name;
		std::vector<TypeErasureComponentProperty> properties;
		size_t poolIndex;
		size_t sizeOf;
		size_t alignOf;
	};

	class ECSRegistry final
	{
	public:
		static ECSRegistry* GetInstance();
	public:
		struct CustomHashComponentHashCode final
		{
			std::size_t operator()(const ComponentHashCode& aKey) const noexcept
			{
				return std::hash<size_t>{}(aKey.value);
			}
		};
	public:
		template<typename T> requires std::is_base_of_v<Component, T>
		bool Register();

		template<typename T> requires std::is_base_of_v<Component, T>
		size_t GetComponentPoolIndex()
		{
			const ComponentHashCode hashCode = ComponentHashCode::Create<T>();

			if (myRegisteredComponents.contains(hashCode))
			{
				return myRegisteredComponents.at(hashCode).poolIndex;
			}

			return 0;
		}

		size_t GetRegisteredComponentsCount() const
		{
			return myRegisteredComponents.size();
		}

		const std::unordered_map<ComponentHashCode, TypeErasureComponent, CustomHashComponentHashCode>& GetRegisteredComponents() const
		{
			return myRegisteredComponents;
		}

		void Destroy();
	private:
		inline static ECSRegistry* myPtr = nullptr;
		inline static size_t myNextComponentIndex = 0;
		std::unordered_map<ComponentHashCode, TypeErasureComponent, CustomHashComponentHashCode> myRegisteredComponents;
	};

	template<typename T> requires std::is_base_of_v<Component, T>
	inline bool ECSRegistry::Register()
	{
		const ComponentHashCode hashCode = ComponentHashCode::Create<T>();

		if (myRegisteredComponents.contains(hashCode))
		{
			return false;
		}


		TypeErasureComponent component;
		component.name = typeid(T).name();
		component.poolIndex = myNextComponentIndex;
		component.sizeOf = sizeof(T);
		component.alignOf = alignof(T);
		myNextComponentIndex++;

		const auto [it, success] = myRegisteredComponents.insert({ hashCode, component });
		return success;
	}
}