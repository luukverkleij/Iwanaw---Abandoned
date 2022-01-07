#ifndef _CMAPCREATOR_H_
#define _CMAPCREATOR_H_

#include "CEntity.h"
#include "CCompMap.h"
#include "CCompTile.h"

class CMapCreator {
public:
	CMapCreator(CComponentManager* manager);
	~CMapCreator();

	void addTile(CEntity& entity);
	void addTile(std::vector<CEntity>& list);

	void createMap(CEntity* mapentity, std::vector<std::vector<unsigned int>> &tiles, unsigned int tileradius);

private:
	CComponentManager* manager;
	std::vector<CEntity> tileset;
};

#endif //_CMAPCREATOR_H_