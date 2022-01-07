#ifndef _CCOMPMAP_H_
#define _CCOMPMAP_H_

#include "CComponent.h"
#include "CShape.h"

#include <sdl.h>

/**
 * /brief Maps, am i right?
 * /todo Change create(manager, entity, centity* ...) into create(manager, entity, const centity& ...)
 *	@todo make it fast! creating a map is extreemly slow!
 */

class CCompMap : public CComponent<CCompMap> {
public:
	//static void create(CComponentManager* manager, CEntity* entity, SDL_Texture* tex, unsigned int width, unsigned int height, unsigned int radius);
	//static void create(CComponentManager* manager, CEntity* entity, CEntity* tile, unsigned int width, unsigned int height, unsigned int radius);

	static void create(CEntityManager* manager, EntityID entity, SDL_Texture* tex, unsigned int width, unsigned int height, unsigned int radius);
	static void create(CEntityManager* manager, EntityID entity, CEntity* tile, unsigned int width, unsigned int height, unsigned int radius);
	static void create(CEntityManager* manager, EntityID entity, std::vector<CEntity*> tileset, std::vector<std::vector<unsigned int>> &tiles, unsigned int tileradius);

	CCompMap() {
		radius = 0;
		width = 0;
		height = 0;
		previousTile = -1;
	}

	//std::vector<CEntity> tiles;
	std::vector<EntityID> tiles;
	unsigned int radius;
	unsigned int width, height;
	int previousTile;

public:
	unsigned int getTile(double x, double y);
	Point_i32 getTileCoords(double x, double y);
	Point_i32 getTileCoordsMin(double x, double y);
	Point_i32 getTileCoordsMax(double x, double y);
	Point_d getTilePos(int tile);

	std::vector<Point_i32> getNeighbours(unsigned int tile);

	bool tileExists(Point_i32 tile);
	bool tileExists(unsigned int tile);

	bool canStack(CEntityManager*, EntityID, unsigned int tile);
	bool canWalk(CEntityManager*, EntityID, unsigned int tile);
	bool canAttack(CEntityManager*, EntityID, unsigned int tile);
};

#endif