#pragma once
#include "ECS/Constants/ComponentsSignature.hpp"
#include "ECS/Concepts/Component.hpp"
#include <string>

namespace Simple
{
	class Entity final
	{
	public:
		Entity(const size_t aID);
		Entity(const size_t aID, const char* aName);

		~Entity();

		Entity(const Entity& aOther) noexcept;
		Entity& operator=(const Entity& aOther) noexcept;

		Entity(Entity&& aOther) noexcept;
		Entity& operator=(Entity&& aOther) noexcept;

		void SetName(const char* aName);

		size_t GetID() const;
		const std::string& GetName() const;
		const ComponentsSignature GetComponentsSignature() const;

	private:
		std::string myName;
		ComponentsSignature myComponentsSignature;
		size_t myID;
	};
}
