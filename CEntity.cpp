#include "CEntity.h"

#include "CComponent.h"
#include "CComponentManager.h"
#include "CEntityManager.h"

#include <iostream>

CEntity::CEntity() {
	this->components.resize(CComponentBase::total_components, nullptr);
	this->name		= "NamelessEntity";
	this->manager	= nullptr;
}

CEntity::CEntity(std::string name) {
	this->components.resize(CComponentBase::total_components, nullptr);
	this->name		= name;
	this->manager	= nullptr;
}

CEntity::CEntity(CEntityManager* manager) {
	this->components.resize(CComponentBase::total_components, nullptr);
	this->name = "NamelessEntity";
	this->manager = manager;
}

CEntity::~CEntity() {
	//std::cout << "Destructing entity " << name << std::endl;

	for (unsigned int index = 0; index < this->components.size(); ++index) {
		if (this->components[index] != nullptr) {
			this->components[index]->destroy();
			this->components[index] = nullptr;
		}
	}
	//this->components.clear();
}

void CEntity::destroyComponent(size_t type) {
	if(this->components[type]) {
		this->components[type]->destroy();
		this->components[type] = nullptr;
	}
}

void CEntity::detachComponent(size_t type) {
	if (this->components[type]) {
		if (this->components[type]) {
			this->components[type]->entity = nullptr;
			this->components[type] = nullptr;
		}
	}
}

void CEntity::detachAll() {
	for (size_t index = 0; index < this->components.size(); ++index) {
		if (this->components[index]) {
			this->components[index]->entity = nullptr;
			this->components[index] = nullptr;
		}
	}
}

bool CEntity::hasComponent(size_t type) {
	return this->components.size() > type && this->components[type] != nullptr;
}

CComponentBase* CEntity::getComponent(size_t type) {
	return this->components[type];
}

void CEntity::setName(std::string name) {
	this->name = name;
}

std::string CEntity::getName() {
	return this->name;
}

EntityID CEntity::getID() {
	if (this->manager == nullptr) return emptyid;
	return this->manager->getID(this);
}

void CEntity::changeManager(CEntityManager* manager) {
	if (this->manager != nullptr && this->manager != manager) {
		this->manager = manager;

		manager->create(*this);
		this->manager->destroy(this->manager->getID(this));
	}
	else {
		this->manager = manager;
	}
}

void CEntity::setManager(CEntityManager* manager) {
	this->manager = manager;
}

CEntityManager* CEntity::getManager() {
	return this->manager;
}

/**
* Copy
*/

CEntity::CEntity(const CEntity& entity) {
	this->components.resize(CComponentBase::total_components, nullptr);
	this->name = entity.name + " constr-copy";
	this->manager = entity.manager;

	for (CComponentBase* comp : entity.components) {
		if (comp != nullptr) {
			comp->copy(this);
		}
	}
}

CEntity::CEntity(const CEntity& entity, CComponentManager* manager) {
	this->components.resize(CComponentBase::total_components, nullptr);
	this->name = entity.name + " copy";
	this->manager = nullptr;

	for (CComponentBase* comp : entity.components) {
		if (comp != nullptr) {
			comp->copy(this, manager);
		}
	}
}

CEntity::CEntity(const CEntity& entity, CEntityManager* manager) {
	this->components.resize(CComponentBase::total_components, nullptr);
	this->name = entity.name + " copy";
	this->manager = manager;

	for (CComponentBase* comp : entity.components) {
		if (comp != nullptr) {
			comp->copy(this, manager->getComponentManager());
		}
	}
}

CEntity& CEntity::operator=(const CEntity& entity) {
	this->components.resize(CComponentBase::total_components, nullptr);
	this->name = entity.name + " =copy";

	for (CComponentBase* comp : entity.components) {
		if (comp != nullptr) {
			comp->copy(this);
		}
	}

	return (*this);
}

void CEntity::updatePointer() {
	for (CComponentBase* comp : this->components) {
		if(comp != nullptr) comp->entity = this;
	}
}