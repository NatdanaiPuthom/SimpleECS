#pragma once
#include <memory>
#include <type_traits>

namespace Simple
{
	class EntityComponentSystem;

	class System
	{
	public:
		System() {};

		virtual ~System() {};

		virtual void Initialize(EntityComponentSystem* /*aEntityComponentSystem*/) {};
		virtual void Update(EntityComponentSystem* /*aEntityComponentSystem*/) {};

		virtual void EarlyUpdate(EntityComponentSystem* /*aEntityComponentSystem*/) {};
		virtual void FixedUpdate(EntityComponentSystem* /*aEntityComponentSystem*/) {};
		virtual void LateUpdate(EntityComponentSystem* /*aEntityComponentSystem*/) {};
	};

	template<typename T>
	concept IsSystem = std::is_base_of_v<Simple::System, T>;
}
