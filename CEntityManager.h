#ifndef _CENTITYMANAGER_H_
#define _CENTITYMANAGER_H_

#include <unordered_set>

#include "CEntity.h"
#include "CComponentManager.h"

/**
  * \brief This class is a attempted to make a manager for a collection of entities that interact with each other.
  * \todo not to fuck it up...
  */
class CEntityManager {
public:
	CEntityManager();
	CEntityManager(CComponentManager* compManager);
	~CEntityManager();

	EntityID create();
	EntityID create(const CEntity&);
	EntityID create(EntityID);

	EntityID replace(EntityID, const CEntity&);

	std::vector<EntityID> createMass(unsigned int);
	std::vector<EntityID> createMassControlled(unsigned int);
	std::vector<EntityID> createMass(unsigned int, CEntity*);
	std::vector<EntityID> createMassControlled(unsigned int num, CEntity* entity);
	
		// Used for createMass to increase pools of the entities components
		void increaseComponentsPool(unsigned int, CEntity*);

	void destroy(EntityID);
	void destroyContainer(EntityID);
	void destroyThrow(EntityID);

	CEntity& get(EntityID);
	EntityID getID(CEntity*);

	CComponentManager*					getComponentManager();
	std::vector<EntityID>				getList();

	//operators
	CEntity& operator[](const EntityID&);

	bool isControlComponent(CComponentBase* comp);
	void updateEntity(CEntity* entity);

private:
	bool isControlled(EntityID);
	bool isControlled(CEntity*);
	void updatePointers();

private:
	std::vector<CEntity> entities;

	std::unordered_set<EntityID> livingEntities;
	std::unordered_set<EntityID> controlledEntities;
	std::unordered_set<EntityID> destroyedEntities;

	bool componentsWeak;
	CComponentManager* components;
};


#endif //_CENTITYMANAGER_H_