#pragma once
#include "ECS/ECSRegistry.hpp"

namespace Simple
{
	class TestSystem final : public System
	{
	public:
		TestSystem();
		~TestSystem() override;

		void Initialize(EntityComponentSystem* aEntityComponentSystem) override;
		void Update(EntityComponentSystem* aEntityComponentSystem) override;

		void EarlyUpdate(EntityComponentSystem* aEntityComponentSystem) override;
		void FixedUpdate(EntityComponentSystem* aEntityComponentSystem) override;
		void LateUpdate(EntityComponentSystem* aEntityComponentSystem) override;
	};

	REGISTER_SYSTEM(TestSystem);
}
