#ifndef _CCOMPMAPPED_H_
#define _CCOMPMAPPED_H_

#include "CComponent.h"
#include "CCompPosition.h"

class Paths {
private:
	std::vector<std::vector<unsigned int>> container;
	std::vector<unsigned int> attackPositions;
public:
	Paths();
	Paths(std::vector<std::vector<unsigned int>> c);

	void setAttacks(std::vector<unsigned int>& attack);
	std::vector<unsigned int> getPositions();
	std::vector<unsigned int> getPath(unsigned int tile);
	std::vector<unsigned int> getAttackPositions();

	bool isAttack(unsigned int);
	
};

class CCompMapped : public CComponent<CCompMapped> {
public:
	CCompMapped();

	void setMap(EntityID map);
	void setTilePos(unsigned int tilePos);
	void setTileSpeed(double tileSpeed);
	void setPaths(Paths paths);

	void removePaths();

	EntityID getMap();
	unsigned int getTilePos();
	double getTileSpeed();
	Paths getPaths();

private:
	EntityID map;
	unsigned int tilePos;
	double tileSpeed;
	Paths paths;
	
public:
	void updateTilePos();
};

#endif