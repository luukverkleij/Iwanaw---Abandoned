#ifndef _CCOMPTILE_H_
#define _CCOMPTILE_H_

#include "CComponent.h"
#include "CEntityManager.h"

class CCompTile : public CComponent<CCompTile>{
public:
	std::vector<EntityID> entities;
	float cost;
};

#endif