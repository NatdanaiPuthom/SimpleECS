#pragma once
#include "ECS/Concepts/Component.hpp"
#include "ECS/Concepts/System.hpp"
#include "ECS/Component/ComponentTypeIdentity.hpp"
#include "ECS/Systems/SystemTypeIdentity.hpp"
#include "ECS/MemoryPool/MemoryPool.hpp"
#include <unordered_map>

namespace Simple
{
	class ECSRegistry final
	{
	public:
		static ECSRegistry* GetInstance();
	public:
		void Destroy();

		const std::unordered_map<size_t, const ComponentTypeIdentity>& GetRegisteredComponents() const;
		const std::unordered_map<size_t, const SystemTypeIdentity>& GetRegisteredSystems() const;
		const ComponentTypeIdentity& GetComponentTypeIdentity(const size_t aID) const;
		const SystemTypeIdentity& GetSystemTypeIdentity(const size_t aID) const;

		template<IsComponent T>
		bool RegisterComponent();

		template<IsSystem T>
		bool RegisterSystem();

	private:
		ECSRegistry();
	private:
		inline static ECSRegistry* myPtr = nullptr;
		std::unordered_map<size_t, const ComponentTypeIdentity> myRegisteredComponentIdentities;
		std::unordered_map<size_t, const SystemTypeIdentity> myRegisteredSystemIdentities;
	};

	template<IsComponent T>
	inline bool ECSRegistry::RegisterComponent()
	{
		ComponentTypeIdentity componentIdentity = ComponentTypeIdentity::GetComponentTypeIdentity<T>();

		if (myRegisteredComponentIdentities.contains(componentIdentity.GetID()))
		{
			DebugAssert(false, "Component already registered.");
			return false;
		}

		const auto [it, result] = myRegisteredComponentIdentities.insert({ componentIdentity.GetID(), componentIdentity });
		return result;
	}

	template<IsSystem T>
	inline bool ECSRegistry::RegisterSystem()
	{
		SystemTypeIdentity systemTypeIdentity = SystemTypeIdentity::GetSystemTypeIdentity<T>();

		if (myRegisteredSystemIdentities.contains(systemTypeIdentity.GetID()))
		{
			DebugAssert(false, "System already registered.");
			return false;
		}

		const auto [it, result] = myRegisteredSystemIdentities.insert({ systemTypeIdentity.GetID() , systemTypeIdentity});
		return result;
	}

	template<IsComponent T>
	class ECSRegisterComponent final
	{
	public:
		ECSRegisterComponent()
		{
			[[maybe_unused]] static bool registered = []()
				{
					ECSRegistry::GetInstance()->RegisterComponent<T>();
					return true;
				}();
		}
	};

	template<IsSystem T>
	class ECSRegisterSystem final
	{
	public:
		ECSRegisterSystem()
		{
			[[maybe_unused]] static bool registered = []()
				{
					ECSRegistry::GetInstance()->RegisterSystem<T>();
					return true;
				}();
		}
	};

#define REGISTER_COMPONENT(aComponent) inline ECSRegisterComponent<aComponent> Global_Reflection_ECS_Registered_Component_##aComponent
#define REGISTER_SYSTEM(aSystem) inline ECSRegisterSystem<aSystem> Global_Reflection_ECS_Registered_System_##aSystem

}
