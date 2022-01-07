#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include <vector>
#include <string>
#include <memory>
#include <iostream>

#include "CEntity.h"
#include "CEntityManager.h"
#include "Intent.h"
#include "CComponentManager.h"

#include "extra_std.h"


// Because we need base classes for base classes in c++
class CComponentBase {
	friend class CComponentManager;
	friend class CEntityManager;
public:
	static unsigned int total_components;
	CEntity* entity;

	virtual ~CComponentBase(){};

	virtual void destroy() = 0;
	virtual void copy(CEntity* entity) = 0;
	virtual void copy(CEntity* entity, CComponentManager* manager) = 0;

protected:
	CComponentBase(){entity = nullptr;};

private:
	virtual void poolIncrease(CComponentManager*, unsigned int) = 0;
	virtual void* getPoolPtr() = 0;
	virtual unsigned int getPoolIndex() = 0;
};

/**
 * \brief Component class that implements 
 * \todo example pageuhhhh
 * \todo make the class pretty like a princess! That wasn't gay right? Kinda insecure about my masculine 
 */
template<typename T>
class CComponent : public CComponentBase {
	friend class CComponentManager;

public:
	virtual ~CComponent(){
		if (entity) {
			//std::cout << "Destructing " << entity->getName() << " component nummer " << componentID << std::endl;
		}
	};

	// The vector holding the conpoments. Very cache friendly! :P
	//static std::vector<T> container;
	static unsigned int componentID;
	static std::vector<std::vector<T>> containers;

	bool hasManager;
	std::vector<T>* ptrPool;
	unsigned int poolID;

	static unsigned int createPool() {
		containers.push_back(std::vector<T>());
		return containers.size() - 1;
	}

	static void increasePool(unsigned int index, unsigned int size) {
		if (containers[index].capacity() - containers[index].size() < size) {
			containers[index].reserve(containers[index].size() + size);
		}

		for(unsigned int i = 0; i < containers[index].size(); i++) {
			T* comp = &containers[index][i];
			if (comp->entity) {
				comp->entity->updateComponent(&containers[index][i]);
			}
		}
	}

	static void destroyPool(unsigned int index) {
		containers[index].clear();
	}

	static T& create(EntityID entity, CEntityManager* manager) {
		return create(&manager->get(entity), manager->getComponentManager());
	}

	static T& create(CEntity* entity, CComponentManager* manager) {
		T& result = create(entity, manager->getPool<T>());

		result.hasManager = true;
		result.poolID = manager->getIndex<T>();

		return result;
	}

	static T& create(const T& comp, CEntity* entity, CComponentManager* manager) {
		T& result = create(comp, entity, manager->getPool<T>());

		result.hasManager = true;
		result.poolID = manager->getIndex<T>();

		return result;
	}

	static T& create(CEntity* entity, std::vector<T> *pPool) {
		// Rember the capacity before the push_back.
		std::vector<T> &pool = (*pPool);
		int cap = pool.capacity();

		// Create a new component.
		pool.push_back(T());
		T* base = &pool[pool.size() - 1];

		//ENTITIES :D:d
		base->entity = entity;
		entity->attachComponent<T>(base);

		// POOL POINTER! (get it? like pool party? HA!)
		base->ptrPool = pPool;

		// Capacity has changed. update all the entities.
		if (cap != (int)pool.capacity()) {
			std::cout << "capacity changed of " << estd::toString(componentID) << std::endl;

			for (unsigned int i = 0; i < pool.size() - 1; i++) {
				if (pool[i].entity) pool[i].entity->updateComponent(&pool[i]);
			}
		}

		return pool.back();
	}

	static T& create(const T &comp, CEntity* entity, std::vector<T> *pPool) {
		// Rember the capacity before the push_back.
		std::vector<T> &pool = (*pPool);
		int cap = pool.capacity();

		// Create a new component.
		pool.push_back(comp);
		T* base = &pool[pool.size() - 1];

		//ENTITIES :D:d
		base->entity = entity;
		entity->attachComponent<T>(base);

		// POOL POINTER! (get it? like pool party? HA!)
		base->ptrPool = pPool;

		// Capacity has changed. update all the entities.
		if(cap != (int)pool.capacity()) {
			std::cout << "capacity changed of " << estd::toString(componentID) << std::endl;

			for(unsigned int i = 0; i < pool.size(); ++i) {
				if(pool[i].entity) pool[i].entity->updateComponent(&pool[i]);
			}
		}

		return pool.back();
	}

	static void destroy(T* comp) {
		std::vector<T> &pool = (*comp->getPool());
		
		// Initialize a index and assign it to the size of the container.
		unsigned int index = 0;

		// Loop through the vector while the comp is not yet found.
		for (; index < pool.size(); ++index) {
			// If the address of the comp is the same as the one in the container, break
			if ((&pool[index]) == comp) break;
		}

		// If index is not the initialized value, the comp has been found. Remove it from the vector.
		if (index < pool.size()) {
			if (index < pool.size() - 1) {
				pool[index] = pool[pool.size() - 1];
				pool[index].entity->updateComponent(&pool[index]);
			}

			pool.pop_back();
		}
	}

	// This function is silly. It works though.
	void destroy() {
		destroy(static_cast<T*>(this));
	}

	// Silly function number 2 and 3.
	void copy(CEntity* entity) {
		create((*static_cast<T*>(this)), entity, &containers[this->poolID]);
	}

	void copy(CEntity* entity, CComponentManager* manager) {
		create((*static_cast<T*>(this)), entity, manager);
	}

	void throwMessage() {
		if (entity == nullptr) return;

		SDL_Event message;
		SDL_zero(message);
		message.type = (Uint32)INTENT::MESSAGE;
		message.user.data1 = (void*)componentID;
		message.user.data2 = (void*)entity->getID();
		SDL_PushEvent(&message);
	}

	template<typename H>
	bool isComponent() {
		return this->componentID == H::componentID;
	}


protected:
	CComponent(){};

private:
	void* getPoolPtr() { return (void*)this->ptrPool; };
	unsigned int getPoolIndex() { return this->poolID; };

	std::vector<T>* getPool() {
		if (hasManager) return (&containers[poolID]);
		else return this->ptrPool;
	}

	void poolIncrease(CComponentManager* mgr, unsigned int cap) {
		mgr->reserve<T>(400);
		//mgr->reserve<this>(400);
	}
};

template <typename T>
std::vector<std::vector<T>> CComponent<T>::containers;

template <typename T>
unsigned int  CComponent<T>::componentID = CComponentBase::total_components++;

#endif //_COMPONENT_H_