#include "CCompMapped.h"
#include "CCompMap.h"
#include "CCompTile.h"
#include "CCompActor.h"

#include <limits>
#include <iostream>

Paths::Paths() {

}

Paths::Paths(std::vector<std::vector<unsigned int>> c) {
	this->container = c;
}

std::vector<unsigned int> Paths::getPositions() {
	std::vector<unsigned int> result;

	for(unsigned int index = 0; index < container.size(); ++index) {
		result.push_back(container[index].back());
	}	

	return result;
}

std::vector<unsigned int> Paths::getPath(unsigned int tile) {
	std::vector<unsigned int> result;

	for(unsigned int index = 0; index < container.size(); ++index) {
		if(container[index].back() == tile) result = container[index];
	}	

	return result;
}

std::vector<unsigned int> Paths::getAttackPositions() {
	return this->attackPositions;
}

void Paths::setAttacks(std::vector<unsigned int>& attacks) {
	this->attackPositions = attacks;
}

bool Paths::isAttack(unsigned int tile) {
	return std::find(this->attackPositions.begin(), this->attackPositions.end(), tile) != this->attackPositions.end();
}



CCompMapped::CCompMapped() {
	this->map = emptyid;
	this->tilePos = std::numeric_limits<unsigned int>::max();
}

void CCompMapped::setMap(EntityID map) {
	this->map = map;
	updateTilePos();
	throwMessage();
}

void CCompMapped::setTilePos(unsigned int tilePos) {
	if(this->tilePos != tilePos) throwMessage();
	this->tilePos = tilePos;
}

void CCompMapped::setTileSpeed(double tileSpeed) {
	this->tileSpeed = tileSpeed;
	throwMessage();
}

void CCompMapped::setPaths(Paths paths) {
	this->paths = paths;
}

void CCompMapped::removePaths() {
	std::cout << "Paths removed!" << std::endl;
	this->paths = Paths();
}

EntityID CCompMapped::getMap() {
	return this->map;
}

unsigned int CCompMapped::getTilePos() {
	return this->tilePos;
}

double CCompMapped::getTileSpeed() {
	return this->tileSpeed;
}

Paths CCompMapped::getPaths() {
	return this->paths;
}

void CCompMapped::updateTilePos() {
	if(this->map == emptyid)
		return;

	CEntityManager* manager = this->entity->getManager();
	EntityID thisID = manager->getID(this->entity);

	CCompPosition* comppos = this->entity->getComponent<CCompPosition>();
	CCompMap* compmap = manager->get(this->map).getComponent<CCompMap>();

	unsigned int oldpos = this->tilePos;
	this->tilePos = compmap->getTile(comppos->getX(), comppos->getY());

	if(oldpos != tilePos) {
		CCompTile* newTile = manager->get(compmap->tiles[this->tilePos]).getComponent<CCompTile>();

		if(oldpos != std::numeric_limits<unsigned int>::max()) {
			CCompTile* oldTile = manager->get(compmap->tiles[oldpos]).getComponent<CCompTile>();

			std::vector<EntityID>::iterator iterator;
			iterator = std::find(oldTile->entities.begin(), oldTile->entities.end(), thisID);

			if(iterator != oldTile->entities.end())
				oldTile->entities.erase(iterator);
		}
		newTile->entities.push_back(thisID);		
	}
}