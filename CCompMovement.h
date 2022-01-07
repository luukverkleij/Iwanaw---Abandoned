#ifndef _CCOMPMOVEMENT_H_
#define _CCOMPMOVEMENT_H_

#include "CComponent.h"

#include <limits>
#include <cstdint>

#include "CPoint.h"

class CCompMovement : public CComponent<CCompMovement> {
public:
	float speedX, speedY;
	float velocityX, velocityY;
	//float nextX, nextY;
	std::vector<Point_f> path;
	float accelerationX, accelerationY;
	float deaccelerationX, deaccelerationY;
	float moveX, moveY;

	CCompMovement::CCompMovement() {
		this->speedX = 0;
		this->speedY = 0;

		this->velocityX = 0;
		this->velocityY = 0;

		//this->nextX = std::numeric_limits<float>::infinity();
		//this->nextY = std::numeric_limits<float>::infinity();

		this->accelerationX = 0;
		this->accelerationY = 0;

		this->deaccelerationX = 0;
		this->deaccelerationY = 0;

		this->moveX = 0;
		this->moveY = 0;
	}
};

#endif //_CCOMPMOVEMENT_H_