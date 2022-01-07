#include "CSysMovement.h"

#include "CCompPosition.h"
#include "CCompMovement.h"
#include "CCompMapped.h"
#include "CCompMap.h"
#include "CCompContainer.h"
#include "CCompCamera.h"

#include <limits>
#include <iostream>

void CSysMovement::updateAll(float deltaTime) {
	for (EntityID ent : this->manager->getList()) {
		this->update(ent, deltaTime);

		if (manager->get(ent).hasComponent<CCompContainer>()) {
			std::vector<EntityID> &list = manager->get(ent).getComponent<CCompContainer>()->entityList;

			for (EntityID ent2 : list) {
				update(ent2, deltaTime);
			}
		}
	}
}

void CSysMovement::update(EntityID entity, float deltaTime) {
	if (manager->get(entity).hasComponent<CCompPosition>() && manager->get(entity).hasComponent<CCompMovement>()) {
		updatePath(entity, deltaTime);
		updateVelocity(entity, deltaTime);
		updatePosition(entity, deltaTime);
	}
}

void CSysMovement::updatePath(EntityID entity, float deltaTime) {
	CCompPosition* pos = manager->get(entity).getComponent<CCompPosition>();
	CCompMovement* mov = manager->get(entity).getComponent<CCompMovement>();

	if(mov->path.empty()) return;

	float nextX = mov->path[0].x;
	float nextY = mov->path[0].y;

	if (mov->path[0].x == pos->getX() && mov->path[0].y == pos->getY()) {
		mov->path.erase(mov->path.begin());
		mov->moveX = 0;
		mov->moveY = 0;
		return;
	}

	double deltaX = (double)(nextX) - pos->getX();
	double deltaY = (double)(nextY) - pos->getY();

	double angle = atan2(deltaY, deltaX);
	if (deltaX != 0) mov->moveX = cos(angle);
	if (deltaY != 0) mov->moveY = sin(angle);
}

// The excessive amount of if-clause may need rework
// A: Your favorite if-clause is shit.
// B: But i love my favorite if-clause!
// A: It is shiiiiiiiiiiiiit
void CSysMovement::updateVelocity(EntityID entity, float deltaTime) {
	CCompPosition* pos = manager->get(entity).getComponent<CCompPosition>();
	CCompMovement* mov = manager->get(entity).getComponent<CCompMovement>();

	if(mov->moveX != 0 && mov->velocityX != mov->speedX*mov->moveX) {
		if(mov->accelerationX == 0) mov->velocityX = mov->speedX*mov->moveX;
		else {
			int weight = mov->moveX/abs(mov->moveX);
			mov->velocityX += mov->speedX * mov->accelerationX*deltaTime * weight;
			if(mov->velocityX*weight > mov->speedX*mov->moveX*weight) mov->velocityX = mov->speedX*mov->moveX;
		}
	}
	else if(mov->moveX == 0 && mov->velocityX != 0) {
		if(mov->deaccelerationX == 0) {
			mov->velocityX = 0;
		}
		else {
			mov->velocityX -= mov->speedX * mov->deaccelerationX*deltaTime * mov->velocityX/abs(mov->velocityX);
			if(abs(mov->velocityX) < mov->speedX * mov->deaccelerationX*deltaTime) {
				mov->velocityX = 0;
			}
		}
	}

	if(mov->moveY != 0 && mov->velocityY != mov->speedY*mov->moveY) {
		if(mov->accelerationY == 0) mov->velocityY = mov->speedY*mov->moveY;
		else {
			int weight = mov->moveY/abs(mov->moveY);
			mov->velocityY += mov->speedY * mov->accelerationY*deltaTime * weight;
			if(mov->velocityY*weight > mov->speedY*mov->moveY*weight) mov->velocityY = mov->speedY*mov->moveY;
		} 
	}
	else if(mov->moveY == 0 && mov->velocityY != 0) {
		if(mov->deaccelerationY == 0) {
			mov->velocityY = 0;
		}
		else {
			mov->velocityY -= mov->speedY * mov->deaccelerationY*deltaTime * mov->velocityY/abs(mov->velocityY);
			if(abs(mov->velocityY) < mov->speedY * mov->deaccelerationY*deltaTime) {
				mov->velocityY = 0;
			}
		}
	}
	/*
	if (!mov->path.empty()) {
		std::cout << "pos" << pos->getX() << ", " << pos->getY() << std::endl;
		std::cout << "velocity: " << mov->velocityX << ", " << mov->velocityY << std::endl;
		std::cout << "move: " << mov->moveX << ", " << mov->moveY << std::endl;
		if (!mov->path.empty())
			std::cout << "path[0]: " << mov->path[0].x << ", " << mov->path[0].y << std::endl;
		std::cout << std::endl;
	}*/
}

void CSysMovement::updatePosition(EntityID entity, float deltaTime) {
	CCompPosition* pos = manager->get(entity).getComponent<CCompPosition>();
	CCompMovement* mov = manager->get(entity).getComponent<CCompMovement>();
	CCompCamera* cam = manager->get(entity).getComponent<CCompCamera>();

	Point_i32 oldPos = pos->getPos();

	if(mov->velocityX != 0) {
		if(!mov->path.empty() && abs(mov->path[0].x - pos->getX()) < abs(mov->velocityX * deltaTime)) {
			pos->setX(mov->path[0].x);
			mov->moveX = 0;
		} else pos->setX(pos->getX() + mov->velocityX * deltaTime);


		if (cam && oldPos != (Point_i32)pos->getPos()) {
			SDL_Event intent;
			SDL_zero(intent);
			intent.user.type = (Uint32)INTENT::POINTER;
			intent.user.code = (Uint32)POINTER::UPDATE;
			SDL_PushEvent(&intent);
		}
	}
	if(mov->velocityY != 0){
		if (!mov->path.empty() && abs(mov->path[0].y - pos->getY()) < abs(mov->velocityY * deltaTime)) {
			pos->setY(mov->path[0].y);
			mov->moveY = 0;
		} else pos->setY(pos->getY() + mov->velocityY * deltaTime);

		if (cam && oldPos != (Point_i32)pos->getPos()) {
			SDL_Event intent;
			SDL_zero(intent);
			intent.user.type = (Uint32)INTENT::POINTER;
			intent.user.code = (Uint32)POINTER::UPDATE;
			SDL_PushEvent(&intent);
		}
	}
}