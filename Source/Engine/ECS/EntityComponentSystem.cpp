#include "EntityComponentSystem.hpp"
#include "ECS/ECSRegistry.hpp"

namespace Simple
{
	EntityComponentSystem::EntityComponentSystem()
		: myNextEntityID(0)
	{
	}

	EntityComponentSystem::~EntityComponentSystem()
	{
		myComponents.clear();
		mySignatureToEntities.clear();
		myNextEntityID = 0;
	}

	EntityComponentSystem::EntityComponentSystem(const EntityComponentSystem& aOther)
		: mySystemManager(aOther.mySystemManager)
		, mySignatureToEntities(aOther.mySignatureToEntities)
		, myEntityIDToEntityData(aOther.myEntityIDToEntityData)
		, myComponents(aOther.myComponents)
		, myEntityIDToComponentIndex(aOther.myEntityIDToComponentIndex)
		, myComponentIndexToEntityID(aOther.myComponentIndexToEntityID)
		, myNextEntityID(aOther.myNextEntityID)
	{
	}

	EntityComponentSystem& EntityComponentSystem::operator=(const EntityComponentSystem& aOther)
	{
		if (this != &aOther)
		{
			mySystemManager = aOther.mySystemManager;
			mySignatureToEntities = aOther.mySignatureToEntities;
			myEntityIDToEntityData = aOther.myEntityIDToEntityData;
			myComponents = aOther.myComponents;
			myEntityIDToComponentIndex = aOther.myEntityIDToComponentIndex;
			myComponentIndexToEntityID = aOther.myComponentIndexToEntityID;
			myNextEntityID = aOther.myNextEntityID;
		}

		return *this;
	}

	EntityComponentSystem::EntityComponentSystem(EntityComponentSystem&& aOther) noexcept
		: mySystemManager(std::move(aOther.mySystemManager))
		, mySignatureToEntities(std::move(aOther.mySignatureToEntities))
		, myEntityIDToEntityData(std::move(aOther.myEntityIDToEntityData))
		, myComponents(std::move(aOther.myComponents))
		, myEntityIDToComponentIndex(std::move(aOther.myEntityIDToComponentIndex))
		, myComponentIndexToEntityID(std::move(aOther.myComponentIndexToEntityID))
		, myNextEntityID(aOther.myNextEntityID)
	{
	}

	EntityComponentSystem& EntityComponentSystem::operator=(EntityComponentSystem&& aOther) noexcept
	{
		if (this != &aOther)
		{
			mySystemManager = std::move(aOther.mySystemManager);
			mySignatureToEntities = std::move(aOther.mySignatureToEntities);
			myEntityIDToEntityData = std::move(aOther.myEntityIDToEntityData);
			myComponents = std::move(aOther.myComponents);
			myEntityIDToComponentIndex = std::move(aOther.myEntityIDToComponentIndex);
			myComponentIndexToEntityID = std::move(aOther.myComponentIndexToEntityID);
			myNextEntityID = aOther.myNextEntityID;
		}

		return *this;
	}

	void EntityComponentSystem::Initialize()
	{
		const auto& allRegisteredComponentIdentities = ECSRegistry::GetInstance()->GetRegisteredComponents();

		for (const auto& [hash, componentIdentity] : allRegisteredComponentIdentities)
		{
			myComponents.emplace_back(componentIdentity);
		}

		mySystemManager.Initialize(this);
	}

	void EntityComponentSystem::Update()
	{
		mySystemManager.Update(this);
	}

	void EntityComponentSystem::EarlyUpdate()
	{
		mySystemManager.EarlyUpdate(this);
	}

	void EntityComponentSystem::FixedUpdate()
	{
		mySystemManager.FixedUpdate(this);
	}

	void EntityComponentSystem::LateUpdate()
	{
		mySystemManager.LateUpdate(this);
	}

	void EntityComponentSystem::AddClonedSystem(const size_t aSystemHashCode, std::unique_ptr<System> aSystem)
	{
		mySystemManager.AddSystem(aSystemHashCode, std::move(aSystem));
	}

	EntityID EntityComponentSystem::CreateEntity()
	{
		const ComponentsSignature signature;
		const size_t id = myNextEntityID;

		std::vector<Entity>& entities = mySignatureToEntities[signature];
		const Entity& newEntity = entities.emplace_back(id);
		const EntityID newEntityID = newEntity.GetID();

		myNextEntityID++;

		EntityData entityData;
		entityData.id = id;
		entityData.index = entities.size() - 1;
		entityData.componentSignature = signature;

		myEntityIDToEntityData[id] = entityData;

		return newEntityID;
	}

	Entity& EntityComponentSystem::GetEntity(const EntityID aEntityID)
	{
		auto it = myEntityIDToEntityData.find(aEntityID);

		if (it == myEntityIDToEntityData.end())
		{
			DebugAssert(false, "Entity with this ID doesn't exist.");
			static Entity dummy;
			return dummy;
		}

		const EntityData& entityData = it->second;
		std::span<Entity> entities = mySignatureToEntities[entityData.componentSignature];
		Entity& entity = entities[entityData.index];

		return entity;
	}

	bool EntityComponentSystem::DestroyEntity(const EntityID aEntityID)
	{
		auto it = myEntityIDToEntityData.find(aEntityID);

		if (it == myEntityIDToEntityData.end())
		{
			DebugAssert(false, "Entity with this ID doesn't exist.");
			return false;
		}

		EntityData& entityData = it->second;

		for (size_t i = 0; i < entityData.componentSignature.size(); i++)
		{
			if (entityData.componentSignature.test(i) == true)
			{
				const bool result = RemoveComponentByID(i, entityData);

				if (result == false)
				{
					DebugAssert(false, "Failed to remove component during entity destruction.");
					return false;
				}
			}
		}

		const size_t entityIndex = entityData.index;
		std::vector<Entity>& entities = mySignatureToEntities[entityData.componentSignature];

		if (entityIndex != entities.size() - 1)
		{
			entities[entityIndex] = std::move(entities.back());

			const EntityID movedEntityID = entities[entityIndex].GetID();
			myEntityIDToEntityData[movedEntityID].index = entityIndex;
		}

		entities.pop_back();
		myEntityIDToEntityData.erase(aEntityID);

		return true;
	}

	bool EntityComponentSystem::AddComponentByID(const size_t aComponentIdentityID, EntityData& aEntityData)
	{
		const size_t entityIndex = aEntityData.index;
		std::vector<Entity>& entities = mySignatureToEntities[aEntityData.componentSignature];

		const bool componentAlreadyExist = entities[entityIndex].HasComponentByID(aComponentIdentityID);

		if (componentAlreadyExist == true)
		{
			DebugAssert(false, "Duplicate component type on entity is not allowed");
			return false;
		}

		const MemoryPool::OperationStatus status = myComponents[aComponentIdentityID].CreateObject();

		if (status.success == true)
		{
			const size_t entityID = entities[entityIndex].GetID();
			const size_t componentIndex = status.createdObjectIndex;

			entities[entityIndex].AddComponent(aComponentIdentityID);

			const ComponentsSignature newSignature = entities[entityIndex].GetComponentsSignature();
			myEntityIDToComponentIndex[aComponentIdentityID][entityID] = componentIndex;
			myComponentIndexToEntityID[aComponentIdentityID][componentIndex] = entityID;

			std::vector<Entity>& newEntities = mySignatureToEntities[newSignature];
			newEntities.push_back(std::move(entities[entityIndex]));

			aEntityData.componentSignature = newSignature;
			aEntityData.index = newEntities.size() - 1;

			if (entityIndex != entities.size() - 1)
			{
				entities[entityIndex] = std::move(entities.back());

				const size_t movedEntityID = entities[entityIndex].GetID();
				myEntityIDToEntityData.at(movedEntityID).index = entityIndex;
			}

			entities.pop_back();
		}

		return status.success;
	}

	bool EntityComponentSystem::RemoveComponentByID(const size_t aComponentIdentityID, EntityData& aEntityData)
	{	
		const size_t entityIndex = aEntityData.index;
		std::vector<Entity>& entities = mySignatureToEntities[aEntityData.componentSignature];

		const bool componentAlreadyExist = entities[entityIndex].HasComponentByID(aComponentIdentityID);

		if (componentAlreadyExist == false)
		{
			DebugAssert(false, "No component to remove.");
			return false;
		}

		const ComponentIndex componentIndex = myEntityIDToComponentIndex[aComponentIdentityID].at(aEntityData.id);
		const ComponentIndex lastComponentIndex = myComponents[aComponentIdentityID].GetCount() - 1;
		const EntityID lastComponentEntityID = myComponentIndexToEntityID[aComponentIdentityID].at(lastComponentIndex);

		const bool result = myComponents[aComponentIdentityID].DestroyObject(componentIndex);

		if (result == false)
		{
			DebugAssert(false, "Failed to remove component.");
			return false;
		}

		entities[entityIndex].RemoveComponent(aComponentIdentityID);

		if (componentIndex != lastComponentIndex)
		{
			myEntityIDToComponentIndex[aComponentIdentityID].at(lastComponentIndex) = componentIndex;
			myComponentIndexToEntityID[aComponentIdentityID].at(componentIndex) = lastComponentEntityID;
			myComponentIndexToEntityID[aComponentIdentityID].erase(lastComponentIndex);
		}
		else
		{
			myComponentIndexToEntityID[aComponentIdentityID].erase(componentIndex);
		}

		myEntityIDToComponentIndex[aComponentIdentityID].erase(aEntityData.id);

		const ComponentsSignature newSignature = entities[entityIndex].GetComponentsSignature();
		std::vector<Entity>& newEntities = mySignatureToEntities[newSignature];

		newEntities.push_back(std::move(entities[entityIndex]));

		aEntityData.componentSignature = newSignature;
		aEntityData.index = newEntities.size() - 1;

		if (entityIndex != entities.size() - 1)
		{
			entities[entityIndex] = std::move(entities.back());

			const EntityID movedEntityID = entities[entityIndex].GetID();
			myEntityIDToEntityData[movedEntityID].index = entityIndex;
		}

		entities.pop_back();

		return true;
	}
}
