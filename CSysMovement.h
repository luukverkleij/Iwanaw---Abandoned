#ifndef _CSYSMOVEMENT_H_
#define _CSYSMOVEMENT_H_

#include "CSystem.h"
#include "CCompMovement.h"

class CSysMovement : public CSystem {
public:
	CSysMovement(CStorage* s, CEntityManager* m) : CSystem(s, m) {};

	void updateAll(float deltaTime);

	void update(EntityID entity, float deltaTime);
	void updatePath(EntityID entity, float deltaTime);
	//	void setMovement(EntityID entity, double delta_x, double delta_y);
	void updateVelocity(EntityID entity, float deltaTime);
	void updatePosition(EntityID entity, float deltaTime);
};

#endif //_CSYSMOVEMENT_H_