#include "CCompMap.h"
#include "CCompTile.h"
#include "CCompActor.h"
#include "CCompPosition.h"

#include "CCompPlayerControlled.h"
#include "CCompPlayer.h"
#include "CCompUnit.h"
#include "CCompBuilding.h"

#include <iostream>
#include <cmath>

void CCompMap::create(CEntityManager* manager, EntityID entity, SDL_Texture* tex, unsigned int width, unsigned int height, unsigned int radius) {
	CComponent<CCompMap>::create(entity, manager);
	CCompMap* map = manager->get(entity).getComponent<CCompMap>();
	map->width = width;
	map->height = height;
	map->radius = radius;
	int tilewidth = radius*sqrt(3);

	CEntity defaultTile;
	CCompTile::create(&defaultTile, manager->getComponentManager());
	CCompActor::create(&defaultTile, manager->getComponentManager());
	CCompPosition::create(&defaultTile, manager->getComponentManager());
	defaultTile.getComponent<CCompActor>()->actor.addDefaultTexture(tex);

	map->tiles = manager->createMassControlled(width*height, &defaultTile);

	for (unsigned int y = 0; y < height; y++) {
		for (unsigned int x = 0; x < width; x++) {
			EntityID tileid = map->tiles[x + y*width];

			CCompActor* act = manager->get(tileid).getComponent<CCompActor>();
			CCompPosition* pos = manager->get(tileid).getComponent<CCompPosition>();

			act->actor.addDefaultTexture(tex);

			pos->setY(y*radius*1.5f);
			pos->setX(x*tilewidth);

			if (y % 2 != 0) pos->setX(pos->getX() + tilewidth / 2.f);
		}
	}
}

void CCompMap::create(CEntityManager* manager, EntityID entity, std::vector<CEntity*> tileset, std::vector<std::vector<unsigned int>> &tiles, unsigned int tileradius) {
	CComponent<CCompMap>::create(entity, manager);
	CCompMap* map = (*manager)[entity].getComponent<CCompMap>();

	map->width = tiles[0].size();
	map->height = tiles.size();
	map->radius = tileradius;
	int tilewidth = tileradius*sqrt(3);

	map->tiles.reserve(map->width*map->height);

	unsigned int defaulttile = estd::max2d(tiles);
	(*manager)[entity].getComponent<CCompMap>()->tiles = manager->createMassControlled(map->width*map->height, tileset[defaulttile]);
	map = (*manager)[entity].getComponent<CCompMap>();

	for (unsigned int y = 0; y < map->height; y++) {
		for (unsigned int x = 0; x < map->width; x++) {
			if (tiles[x][y] != defaulttile) {
				manager->replace(map->tiles[y * map->height + x], *tileset[tiles[x][y]]);
			}

			EntityID tileID = map->tiles[y* map->height + x];
			CCompPosition* pos = manager->get(tileID).getComponent<CCompPosition>();

			pos->setY(y*tileradius*1.5f + tileradius);
			pos->setX(x*tilewidth + tilewidth / 2);

			if (y % 2 != 0) pos->setX(pos->getX() + tilewidth / 2.f);
		}
	}
}

void CCompMap::create(CEntityManager* manager, EntityID entity, CEntity* example_tile, unsigned int width, unsigned int height, unsigned int radius) {
	CComponent<CCompMap>::create(entity, manager);
	CCompMap* map = manager->get(entity).getComponent<CCompMap>();
	map->width = width;
	map->height = height;
	map->radius = radius;
	int tilewidth = radius*sqrt(3);

	map->tiles = manager->createMassControlled(width*height, example_tile);

	for (unsigned int y = 0; y < height; y++) {
		for (unsigned int x = 0; x < width; x++) {
			EntityID tileid = map->tiles[x + y*width];

			CCompPosition* pos = manager->get(tileid).getComponent<CCompPosition>();

			pos->setY(radius + y*radius*1.5f);
			pos->setX(tilewidth / 2 + x*tilewidth);

			if (y % 2 != 0) pos->setX(pos->getX() + tilewidth / 2.f);
		}
	}
}

unsigned int CCompMap::getTile(double x, double y) { 
	Point_i32 point = getTileCoords(x,y);
	return point.x + point.y * this->width;
}

Point_i32 CCompMap::getTileCoords(double x, double y) {
	int side = radius * 3.f / 2.f;
	int height = radius * sqrt(3.f);

	int ci = floor(y / side);
	int cx = y - ci*side;

	int ty = x - (ci%2) * height / 2;
	int cj = floor(ty / height);
	int cy = ty - height*cj;

	if(cx < abs((double)this->radius / 2.f - (double)this->radius * cy / height)) {
		--ci;
		if(ci != 0)			cj -= (ci % 2) * (ci/(abs(ci)));
		if(cy >= height/2)	cj += 1;
	}

	return Point_i32(cj, ci);
}

Point_i32 CCompMap::getTileCoordsMin(double x, double y) {
	double width = radius * sqrt(3.f);
	double diameter = radius * 3.f / 2.f;

	x = x - width/2.f;
	y = y - radius/2.f;

	int result_x = x / width;
	int result_y = y / diameter;

	return Point_i32(result_x, result_y);
}

Point_i32 CCompMap::getTileCoordsMax(double x, double y) {
	double width = radius * sqrt(3.f);
	double diameter = radius * 3.f / 2.f;

	x = x + width / 2.f;
	y = y + radius / 2.f;

	int result_x = x / width;
	int result_y = y / diameter;

	return Point_i32(result_x, result_y);
}

Point_d CCompMap::getTilePos(int tile) {
	int tilewidth = radius*sqrt(3);

	int tile_x = tile % width;
	int tile_y = tile / width;

	double point_x = tile_x * tilewidth		+ tilewidth/2;
	double point_y = tile_y * radius * 1.5f + radius;

	if(tile_y % 2) point_x += tilewidth/2.f;

	//std::cout << "x: " << point_x << ", y: " << point_y << std::endl;

	return Point_d(point_x + 1, point_y + 1);
}

bool CCompMap::tileExists(Point_i32 tile) {
	return tile.x >= 0 && tile.x < this->width
		&& tile.y >= 0 && tile.x < this->height;
}

bool CCompMap::tileExists(unsigned int tile) {
	return tile < this->width*this->height;
}

std::vector<Point_i32> CCompMap::getNeighbours(unsigned int tile) {
	Point_i32 tilePos(tile % this->width, tile / this->width);
	int align = tilePos.y % 2;

	Point_i32 resultPoints[6];
	std::vector<Point_i32> result;
	result.reserve(6);

	resultPoints[0] = Point_i32(tilePos.x + align - 1, tilePos.y - 1);
	resultPoints[1] = Point_i32(tilePos.x + align, tilePos.y - 1);

	resultPoints[2] = Point_i32(tilePos.x - 1, tilePos.y);
	resultPoints[3] = Point_i32(tilePos.x + 1, tilePos.y);

	resultPoints[4] = Point_i32(tilePos.x + align - 1, tilePos.y + 1);
	resultPoints[5] = Point_i32(tilePos.x + align, tilePos.y + 1);

	for (unsigned int index = 0; index < 6; ++index) {
		if (this->tileExists(resultPoints[index]))
			result.push_back(resultPoints[index]);
	}

	return result;
}

/*
* Checks if the given unit can be placed on the given tile.
*/
bool CCompMap::canStack(CEntityManager* m, EntityID entity, unsigned int tile) {
	CCompUnit* unitcomp = m->get(entity).getComponent<CCompUnit>();
	CCompBuilding* buildingcomp = m->get(entity).getComponent<CCompBuilding>();
	CCompTile* tilecomp = m->get(this->tiles[tile]).getComponent<CCompTile>();

	for (EntityID mapent : tilecomp->entities) {
		if ((unitcomp && m->get(mapent).hasComponent<CCompUnit>()) || 
			(buildingcomp && m->get(mapent).hasComponent<CCompBuilding>())) {
			return false;
		}
	}

	return true;
}
/**
*	Checks if the given unit can be placed on the given tile.
*/

bool CCompMap::canWalk(CEntityManager* m, EntityID entity, unsigned int tile) {
	EntityID playerID = m->get(entity).getComponent<CCompPlayerControlled>()->getPlayer();
	CCompPlayer* player = m->get(playerID).getComponent<CCompPlayer>();
	CCompTile* tilecomp = m->get(this->tiles[tile]).getComponent<CCompTile>();

	for (EntityID mapent : tilecomp->entities) {
		if (m->get(mapent).hasComponent<CCompPlayerControlled>()) {
			EntityID playerID = m->get(mapent).getComponent < CCompPlayerControlled>()->getPlayer();
			CCompPlayer* other = m->get(playerID).getComponent<CCompPlayer>();
			
			if (player->id != other->id && std::find(player->friendly.begin(), player->friendly.end(), other->id) == player->friendly.end()) {
				if ((m->get(entity).hasComponent<CCompUnit>() && m->get(entity).hasComponent<CCompUnit>()) ||
					(m->get(entity).hasComponent<CCompBuilding>() && m->get(entity).hasComponent<CCompBuilding>())) {
					return false;
				}
			}
		}
	}

	return true;
}

bool CCompMap::canAttack(CEntityManager* m, EntityID entity, unsigned int tile) {
	CCompUnit* unitcomp = m->get(entity).getComponent<CCompUnit>();
	EntityID playerID = m->get(entity).getComponent<CCompPlayerControlled>()->getPlayer();
	CCompPlayer* player = m->get(playerID).getComponent<CCompPlayer>();
	CCompTile* tilecomp = m->get(this->tiles[tile]).getComponent<CCompTile>();

	if (unitcomp == nullptr) return false;

	for (EntityID mapent : tilecomp->entities) {
		if (m->get(mapent).hasComponent<CCompUnit>() && m->get(mapent).hasComponent<CCompPlayerControlled>()) {
			EntityID otherPlayerID = m->get(mapent).getComponent < CCompPlayerControlled>()->getPlayer();
			CCompPlayer* otherPlayer = m->get(otherPlayerID).getComponent<CCompPlayer>();

			if (otherPlayer->id != player->id) {
				return true;
			}
		}
	}

	return false;
}