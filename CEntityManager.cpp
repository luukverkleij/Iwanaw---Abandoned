#include "CEntityManager.h"
#include "Intent.h"

#include "CCompContainer.h"
#include "CCompContained.h"
#include "CCompMapped.h"
#include "CCompTile.h"

#include <exception>

CEntityManager::CEntityManager() {
	this->componentsWeak = false;
	this->components = new CComponentManager();
}

CEntityManager::CEntityManager(CComponentManager* compManager) {
	this->componentsWeak = true;
	this->components = compManager;
}

CEntityManager::~CEntityManager() {
	for(unsigned int index = 0; index < entities.size(); ++index) {
		this->entities[index].detachAll();
	}

	if (!this->componentsWeak) delete this->components;
}

EntityID CEntityManager::create() {
	return create(CEntity(this));
}

EntityID CEntityManager::create(const CEntity &ent) {
	if (this->entities.capacity() == this->entities.size()) {
		this->entities.reserve(this->entities.size() * 2);
		updatePointers();
	}

	this->entities.push_back(ent);
	this->entities.back().setManager(this);

	if (isControlled(&this->entities.back())) {
		this->controlledEntities.insert(this->entities.size());
	}
	else this->livingEntities.insert(this->entities.size());

	return this->entities.size();
}

EntityID CEntityManager::create(EntityID id) {
	if (this->entities.capacity() == this->entities.size()) {
		this->entities.reserve(this->entities.size() * 2);
		updatePointers();
	}

	return create(this->get(id));
}

EntityID CEntityManager::replace(EntityID id, const CEntity& entity) {
	entities[id - 1] = entity;
	entities[id - 1].setManager(this);

	this->livingEntities.erase(id);
	this->controlledEntities.erase(id);

	if (isControlled(id)) {
		this->controlledEntities.insert(id);
	}
	else {
		this->livingEntities.insert(id);
	}

	return id;
}

std::vector<EntityID> CEntityManager::createMass(unsigned int num) {
	return createMass(num, &CEntity());
}

std::vector<EntityID> CEntityManager::createMassControlled(unsigned int num) {
	return createMassControlled(num, &CEntity());
}

std::vector<EntityID> CEntityManager::createMass(unsigned int num, CEntity* entity) {
	std::vector<EntityID> result;
	result.reserve(num);

	if (this->entities.capacity() - this->entities.size() < num) {
		this->entities.resize(this->entities.size() + num, (*entity));
		updatePointers();
	}

	increaseComponentsPool(num, entity);

	if (isControlled(entity)) {
		// Looping through *IDS*, so position+1
		for (unsigned int index = num; index > 0; --index) {
			result.push_back(this->entities.size() - index + 1);
			this->entities[this->entities.size() - index].setManager(this);
			this->controlledEntities.insert(this->entities.size() - index);
		}
	}
	else {
		// Looping through *IDS*, so position+1
		for (unsigned int index = num; index > 0; --index) {
			result.push_back(this->entities.size() - index + 1);
			this->entities[this->entities.size() - index].setManager(this);
			this->livingEntities.insert(this->entities.size() - index);
		}
	}

	return result;
}

std::vector<EntityID> CEntityManager::createMassControlled(unsigned int num, CEntity* entity) {
	std::vector<EntityID> result;
	result.reserve(num);

	if (this->entities.capacity() - this->entities.size() < num) {
		this->entities.resize(this->entities.size() + num, (*entity));
		updatePointers();
	}

	increaseComponentsPool(num, entity);

	// Looping through *IDS*, so position+1
	for (unsigned int index = num; index > 0; --index) {
		result.push_back(this->entities.size() - index + 1);
		this->entities[this->entities.size() - index].setManager(this);
		this->controlledEntities.insert(this->entities.size() - index);
	}

	return result;
}

void CEntityManager::increaseComponentsPool(unsigned int num, CEntity* entity) {
	for (unsigned int index = 0; index < CComponentBase::total_components; ++index) {
		if (entity->hasComponent(index)) {			
			entity->getComponent(index)->poolIncrease(this->components, num);
		}
	}
}

void CEntityManager::destroy(EntityID id) {
	if (this->get(id).hasComponent<CCompContainer>()) {
		destroyContainer(id);
	}

	this->livingEntities.erase(id);
	this->controlledEntities.erase(id);

	this->destroyedEntities.insert(id);

	destroyThrow(id);
}

void CEntityManager::destroyContainer(EntityID id) {
	CCompContainer* container = this->get(id).getComponent<CCompContainer>();
	for (unsigned int i = 0; i < container->entityList.size(); ++i) {
		destroy(container->entityList[i]);
		std::cout << "Destroyed Contained Entity" << std::endl;
	}
}

void CEntityManager::destroyThrow(EntityID id) {
	SDL_Event message;
	SDL_zero(message);
	message.type = (Uint32)INTENT::EMESSAGE;
	message.user.type = (Uint32)INTENT::EMESSAGE;
	message.user.data1 = (void*)id;
	SDL_PushEvent(&message);
}

CEntity& CEntityManager::get(EntityID id) {
	return this->entities[id - 1]; 
}

EntityID CEntityManager::getID(CEntity* entity) {
	for (unsigned int index = 0; index < this->entities.size(); ++index) {
		if (&this->entities[index] == entity) {
			return index + 1;
		}
	}

	return emptyid;
}

CComponentManager* CEntityManager::getComponentManager() {
	return this->components;
}

std::vector<EntityID> CEntityManager::getList() {
	return std::vector<EntityID>(this->livingEntities.begin(), this->livingEntities.end());
}

void CEntityManager::updateEntity(CEntity* entity) {
	EntityID id = this->getID(entity);

	if (id == emptyid) {
		std::cout << "In updateEntity: given entity is not in this manager!" << std::endl;
		return;
	}

	if (this->isControlled(id)) {
		auto livingIt = livingEntities.find(id);
		if (livingIt != livingEntities.end()) {
			livingEntities.erase(livingIt);
			controlledEntities.insert(id);
		}
	}
	else {
		auto controlledIt = controlledEntities.find(id);
		if (controlledIt != controlledEntities.end()) {
			controlledEntities.erase(controlledIt);
			livingEntities.insert(id);
		}
	}
}


//
//Operators
//

CEntity& CEntityManager::operator[](const EntityID& id) {
	return get(id);
}

//
// Private Functions
//

void CEntityManager::updatePointers() {
	for (CEntity& ent : this->entities) {
		ent.updatePointer();
	}
}

bool CEntityManager::isControlled(EntityID id) {
	return isControlled(&this->get(id));
}

bool CEntityManager::isControlled(CEntity* ent) {
	return ent->hasComponent<CCompContained>() || ent->hasComponent<CCompMapped>() || ent->hasComponent<CCompTile>();
}