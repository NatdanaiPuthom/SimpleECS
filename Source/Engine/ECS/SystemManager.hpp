#pragma once
#include "ECS/Concepts/System.hpp"
#include "Debugger/Assert.hpp"
#include <unordered_map>

namespace Simple
{
	class EntityComponentSystem;

	class SystemManager final
	{
		using SystemHashCode = size_t;

		friend class Simple::EntityComponentSystem;
	public:
		SystemManager(const SystemManager& aOther);
		SystemManager& operator=(const SystemManager& aOther);
		SystemManager(SystemManager&& aOther) noexcept;
		SystemManager& operator=(SystemManager&& aOther) noexcept;


		void Initialize(EntityComponentSystem* aEntityComponentSystem);
		void Update(EntityComponentSystem* aEntityComponentSystem);

		void EarlyUpdate(EntityComponentSystem* aEntityComponentSystem);
		void FixedUpdate(EntityComponentSystem* aEntityComponentSystem);
		void LateUpdate(EntityComponentSystem* aEntityComponentSystem);

		template<IsSystem T>
		void AddSystem();

		void AddSystem(const size_t aHashCode, std::unique_ptr<System> aSystem);
	private:
		SystemManager();
		~SystemManager();
	private:
		std::unordered_map<size_t, std::unique_ptr<System>> mySystems;
	};

	template<IsSystem T>
	inline void SystemManager::AddSystem()
	{
		const SystemHashCode hashCode = typeid(T).hash_code();

		if (mySystems.contains(hashCode))
		{
			DebugAssert(false, "System already exist.");
			return;
		}

		mySystems.emplace(hashCode, std::make_unique<T>());
	}
}
