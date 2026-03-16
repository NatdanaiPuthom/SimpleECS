#include "SystemManager.hpp"
#include "ECSRegistry.hpp"

namespace Simple
{
	SystemManager::SystemManager()
	{
	}

	SystemManager::~SystemManager()
	{
		mySystems.clear();
	}

	SystemManager::SystemManager(const SystemManager& aOther)
	{
		mySystems.clear();

		for (const auto& [key, system] : aOther.mySystems)
		{
			const SystemTypeIdentity& systemIdentity = ECSRegistry::GetInstance()->GetSystemTypeIdentity(key);
			mySystems.emplace(key, systemIdentity.Clone());
		}
	}

	SystemManager& SystemManager::operator=(const SystemManager& aOther)
	{
		if (this != &aOther)
		{
			mySystems.clear();

			for (const auto& [key, system] : aOther.mySystems)
			{
				const SystemTypeIdentity& systemIdentity = ECSRegistry::GetInstance()->GetSystemTypeIdentity(key);
				mySystems.emplace(key, systemIdentity.Clone());
			}
		}

		return *this;
	}

	SystemManager::SystemManager(SystemManager&& aOther) noexcept
		: mySystems(std::move(aOther.mySystems))
	{
	}

	SystemManager& SystemManager::operator=(SystemManager&& aOther) noexcept
	{
		if (this != &aOther)
		{
			mySystems.clear();
			mySystems = std::move(aOther.mySystems);
		}

		return *this;
	}

	void SystemManager::Initialize(EntityComponentSystem* aEntityComponentSystem)
	{
		for (const auto& [hashCode, system] : ECSRegistry::GetInstance()->GetRegisteredSystems())
		{
			AddSystem(hashCode, system.Clone());
		}

		for (const auto& [key, system] : mySystems)
		{
			system->Initialize(aEntityComponentSystem);
		}
	}

	void SystemManager::Update(EntityComponentSystem* aEntityComponentSystem)
	{
		for (const auto& [key, system] : mySystems)
		{
			system->Update(aEntityComponentSystem);
		}
	}

	void SystemManager::EarlyUpdate(EntityComponentSystem* aEntityComponentSystem)
	{
		for (const auto& [key, system] : mySystems)
		{
			system->EarlyUpdate(aEntityComponentSystem);
		}
	}

	void SystemManager::FixedUpdate(EntityComponentSystem* aEntityComponentSystem)
	{
		for (const auto& [key, system] : mySystems)
		{
			system->FixedUpdate(aEntityComponentSystem);
		}
	}

	void SystemManager::LateUpdate(EntityComponentSystem* aEntityComponentSystem)
	{
		for (const auto& [key, system] : mySystems)
		{
			system->LateUpdate(aEntityComponentSystem);
		}
	}

	void SystemManager::AddSystem(const size_t aHashCode, std::unique_ptr<System> aSystem)
	{
		mySystems[aHashCode] = std::move(aSystem);
	}
}
