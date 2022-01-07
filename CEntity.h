#ifndef _CENTITY_H_
#define _CENTITY_H_

#include <vector>
#include <typeindex>

typedef unsigned int EntityID;
const EntityID emptyid = 0;

class CComponentBase;
class CComponentManager;
class CEntityManager;

class CEntity {
	friend class CEntityManager;
public:
	CEntity();
	CEntity(std::string);
	CEntity(CEntityManager*);
	~CEntity();

	void setName(std::string);
	std::string getName();
	EntityID getID();

	void setManager(CEntityManager* manager);
	void changeManager(CEntityManager* manager);
	CEntityManager* getManager();

	template<typename T>
	void createComponent(T& comp);

	template<typename T>
	void updateComponent(T*);

	template<typename T>
	void attachComponent(T*);

	void destroyComponent(size_t);
	void detachComponent(size_t);
	void detachAll();

	template<typename T>
	void destroyComponent();

	template<typename T>
	void detachComponent();

	template<typename>bool	hasComponent();
	bool					hasComponent(size_t);

	template<typename T> T* getComponent();
	CComponentBase*			getComponent(size_t);

	void updatePointer();

public:
	//Copy
	CEntity(const CEntity& entity);
	CEntity(const CEntity& entity, CComponentManager* manager);
	CEntity(const CEntity& entity, CEntityManager* manager);

	CEntity& operator=(const CEntity&);

private:
	CEntityManager* manager;
	std::string name;
	std::vector<CComponentBase*> components;
};

template<typename T>
bool CEntity::hasComponent() {
	return (this->hasComponent(CComponent<T>::componentID));
}

template<typename T>
T* CEntity::getComponent() {
	return static_cast<T*>(this->getComponent(CComponent<T>::componentID));
}

template<typename T>
void CEntity::createComponent(T& comp) {
	if (this->manager == nullptr) return;

	//if (components.size() <= comp->componentID) components.resize(CComponentBase::total_components);

	if (hasComponent(comp->componentID)) {
		this->components[comp->componentID]->entity = nullptr;
	}

	T::create(comp, manager->getComponentManager());
}

template<typename T>
void CEntity::attachComponent(T* comp) {
	if(components.size() <= comp->componentID) components.resize(CComponentBase::total_components);

	if(hasComponent(comp->componentID)) this->components[comp->componentID]->entity = nullptr;
	this->components[comp->componentID] = comp;

	if (this->manager != nullptr) {
		this->manager->updateEntity(this);
	}
}

template<typename T>
void CEntity::destroyComponent() {
	this->destroyComponent(T::componentID);
}

template<typename T>
void CEntity::detachComponent() {
	this->dettachComponent(T::componentID);
}


// FOR INTERN USE ONLY.
template<typename T>
void CEntity::updateComponent(T* comp) {
	this->components[comp->componentID] = comp;
}

#endif // _CENTITY_H_