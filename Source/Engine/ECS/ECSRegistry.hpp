#pragma once
#include "ECS/Component/Component.hpp"
#include "ECS/Component/ComponentTypeIdentity.hpp"
#include "ECS/MemoryPool/MemoryPool.hpp"
#include <unordered_map>

namespace Simple
{
	struct TypeErasureComponent final
	{
		ComponentTypeIdentity identity;
		size_t sizeOf = 0;
		size_t alignOf = 0;
	};

	struct ECSComponentHashCode final
	{
		const size_t value;

		template<IsComponent T>
		static ECSComponentHashCode CreateUniqueHashCode()
		{
			return ECSComponentHashCode(ComponentIdentityID<T>::GetID());
		}

		bool operator==(const ECSComponentHashCode& aOther) const noexcept
		{
			return this->value == aOther.value;
		}

	private:
		explicit ECSComponentHashCode(const size_t aHashCode) : value(aHashCode) {}
	};

	struct RegisteredComponentsIdentityHash final
	{
		std::size_t operator()(const ECSComponentHashCode& aKey) const noexcept
		{
			return aKey.value;
		}
	};

	class ECSRegistry final
	{
	public:
		static ECSRegistry* GetInstance();
	public:
		void Destroy();

		const std::unordered_map<ECSComponentHashCode, TypeErasureComponent, RegisteredComponentsIdentityHash>& GetRegisteredComponents() const;

		template<IsComponent T>
		bool Register();

		template<IsComponent T>
		const TypeErasureComponent& GetTypeErasureComponent() const;
	private:
		ECSRegistry();
	private:
		inline static ECSRegistry* myPtr = nullptr;
		std::unordered_map<ECSComponentHashCode, TypeErasureComponent, RegisteredComponentsIdentityHash> myRegisteredComponents;
	};

	template<IsComponent T>
	inline bool ECSRegistry::Register()
	{
		TypeErasureComponent component;
		component.identity = ComponentTypeIdentity::GetComponentTypeIdentity<T>();
		component.sizeOf = sizeof(T);
		component.alignOf = alignof(T);

		const ECSComponentHashCode hashCode = ECSComponentHashCode::CreateUniqueHashCode<T>();

		const auto [it, success] = myRegisteredComponents.insert({ hashCode, component });
		return success;
	}

	template<IsComponent T>
	inline const TypeErasureComponent& ECSRegistry::GetTypeErasureComponent() const
	{
		const ECSComponentHashCode hashCode = ECSComponentHashCode::CreateUniqueHashCode<T>();

		const auto& it = myRegisteredComponents.find(hashCode);

		if (it != myRegisteredComponents.end())
		{
			return it->second;
		}

		struct Dummy : Component
		{
			bool valid = false;
		};

		static const TypeErasureComponent invalid = {};
		return invalid;
	}

	template<IsComponent T>
	class ECSRegisterComponent final
	{
	public:
		ECSRegisterComponent()
		{
			[[maybe_unused]] static bool registered = []()
				{
					ECSRegistry::GetInstance()->Register<T>();
					return true;
				}();
		}
	};

#define REGISTER_COMPONENT(aComponent) inline ECSRegisterComponent<aComponent> Global_Reflection_ECS_Registered_Component_##aComponent

}

