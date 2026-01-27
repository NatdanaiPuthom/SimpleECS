#pragma once
#include <vector>
#include <string>
#include <bitset>
#include <unordered_map>
#include "ECS/ECSRegistry.hpp"
#include "ECS/Component/TestComponents.hpp"

namespace Simple
{
	inline constexpr size_t Global_MAX_COMPONENTS = 64;

	class Entity final
	{
		std::string name;
		size_t id;
	};


	class EntityManager final
	{
	public:
		using ComponentSignature = std::bitset<Global_MAX_COMPONENTS>;


		EntityManager()
		{
		}

		~EntityManager()
		{
		}

		void Initialize()
		{
			ComponentSignature signature;
			signature.set(ComponentIdentityID<EmilTest>::GetID(), true);
			signature;

			auto it = mySignatureToEntities.find(signature);

			if (it != mySignatureToEntities.end())
			{
				int i = 5;
				i;
			}
			else
			{
				int i = 5;
				i;
			}


			Entity entity;
			mySignatureToEntities[signature].push_back(entity);


			it = mySignatureToEntities.find(signature);

			if (it != mySignatureToEntities.end())
			{
				int i = 5;
				i;
			}
			else
			{
				int i = 5;
				i;
			}
		}

		template<typename T>
		void AddComponent()
		{
		}
	private:
		std::unordered_map< std::bitset<64>, std::vector<Entity>> mySignatureToEntities;
	};
}
