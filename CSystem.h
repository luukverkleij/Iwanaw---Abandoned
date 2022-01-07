#ifndef _CSYSTEM_H_
#define _CSYSTEM_H_

#include "CEntity.h"
#include "CStorage.h"
#include "CEntityManager.h"

#include <list>

class CSystem {
public:
	CSystem(CStorage* store, CEntityManager* manager) {
		this->store = store;
		this->manager = manager;
	}

	virtual ~CSystem() {};

	CStorage* getStorage() {return this->store;}
	CEntityManager* getManager() { return this->manager; }

protected:
	CStorage* store;
	CEntityManager* manager;
};

#endif //CSYSTEM_H_