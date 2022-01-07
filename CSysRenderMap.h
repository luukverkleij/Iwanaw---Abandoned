#ifndef _CSYSRENDERMAP_H_
#define _CSYSRENDERMAP_H_

#include "CSysRender.h"
#include "CCompMap.h"

class CSysRenderMap : public CSystem {
private:
	CSysRender* render;

public:
	CSysRenderMap(CSysRender* render);
	~CSysRenderMap();

	void update(EntityID map);
	void renderTile(EntityID tile, long x, long y);
	void renderTileEntities(EntityID tile, long x, long y);
	void renderGrid(EntityID map, long x, long y);
	void renderTileContainer(EntityID map, long x, long y);
};

#endif