#include "CCompPosition.h"

#include "CCompMap.h"
#include "CCompMapped.h"
#include "CCompContained.h"
#include "CCompCameraControlled.h"

CCompPosition::CCompPosition() {
	setPos(0.0f, 0.0f);
}

CCompPosition::CCompPosition(double x, double y) {
	setPos(x, y);
}

void CCompPosition::setX(double x){
	this->setPos(x, this->y);
}
void CCompPosition::setY(double y){
	this->setPos(this->x, y);
}
void CCompPosition::setPos(double x, double y){
	this->x = x; 
	this->y = y;
	throwMessage();
}
void CCompPosition::updateMapped() {
	if(entity != nullptr && this->entity->hasComponent<CCompMapped>()) {
		this->entity->getComponent<CCompMapped>()->updateTilePos();
		
	}
}
double CCompPosition::getX(){
	return this->x;
}
double CCompPosition::getY(){
	return this->y;
}

Point_d CCompPosition::getPos() {
	return Point_d(this->x, this->y);
}

Point_d CCompPosition::getAbsolutePos() {
	Point_d result(this->x, this->y);
	CEntityManager* man = this->entity->getManager();

	CCompMapped* mapped = this->entity->getComponent<CCompMapped>();
	if (mapped) {
		result -= man->get( man->get(mapped->getMap()).getComponent<CCompCameraControlled>()->camera ).getComponent<CCompPosition>()->getAbsolutePos();
	}

	CCompCameraControlled* cameracon = this->entity->getComponent<CCompCameraControlled>();
	if (cameracon) {
		result -= man->get(cameracon->camera).getComponent<CCompPosition>()->getAbsolutePos();
	}

	CCompContained* contained = this->entity->getComponent<CCompContained>();
	if (contained) {
		result += man->get(contained->parent).getComponent<CCompPosition>()->getAbsolutePos();
	}

	return result;
}