#include "CMapCreator.h"
#include "CCompPosition.h"

CMapCreator::CMapCreator(CComponentManager* manager) {
	this->manager = manager;
}

CMapCreator::~CMapCreator() {

}

void CMapCreator::addTile(CEntity& entity) {
	std::cout << "addTile: " << entity.getName() << std::endl;
	this->tileset.push_back(entity);
	//this->tileset.push_back(CEntity((*entity), this->manager));
}

void CMapCreator::createMap(CEntity* mapentity, std::vector<std::vector<unsigned int>> &tiles, unsigned int tileradius) {
	/*CComponent<CCompMap>::create(mapentity, manager);
	CCompMap* map = mapentity->getComponent<CCompMap>();

	map->width = tiles[0].size();
	map->height = tiles.size();
	map->radius = tileradius;
	int tilewidth = tileradius*sqrt(3);

	map->tiles.reserve(map->width*map->height);	

	for (unsigned int y = 0; y < map->height; y++) {
		for (unsigned int x = 0; x < map->width; x++) {
			//map->tiles[y * map->height + x] = this->tileset[tiles[x][y]];
			map->tiles.push_back(this->tileset[tiles[x][y]]);

			CEntity* tile = &map->tiles.back();
			CCompPosition* pos = tile->getComponent<CCompPosition>();

			pos->setY(y*tileradius*1.5f + tileradius);
			pos->setX(x*tilewidth + tilewidth/2);

			if (y % 2 != 0) pos->setX(pos->getX() + tilewidth / 2.f);
		}
	}*/
}