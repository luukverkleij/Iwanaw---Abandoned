#include "CSysRenderMap.h"
#include "CCompPosition.h"
#include "CCompCameraControlled.h"
#include "CCompTile.h"
#include "CCompContainer.h"

#include <iostream>
#include <cmath>

CSysRenderMap::CSysRenderMap(CSysRender* render) : CSystem(render->getStorage(), render->getManager()){
	this->render = render;
}

CSysRenderMap::~CSysRenderMap() {

}

void CSysRenderMap::update(EntityID map) {
	CCompMap* compmap = manager->get(map).getComponent<CCompMap>();
	EntityID cameraEntity = manager->get(map).getComponent<CCompCameraControlled>()->camera;
	CCompPosition* compcam = manager->get(cameraEntity).getComponent<CCompPosition>();

	SDL_Rect viewport = render->getRenderer()->getViewport();

	Point_i32 startTile = compmap->getTileCoordsMin(compcam->getX(), compcam->getY());
	Point_i32 endTile = compmap->getTileCoordsMax(compcam->getX() + viewport.w, compcam->getY() + viewport.h);

	int startX = startTile.x;
	int startY = startTile.y;

	int endX = endTile.x;
	int endY = endTile.y;

	//std::cout << "pixel start: \t(" << compcam->getX() << ", " << compcam->getY() << ") \t\t pixel end: \t(" << compcam->getX() + viewport.w << ", " << compcam->getY() + viewport.h << ")" << std::endl;
	//std::cout << "start: \t\t(" << startX << ", " << startY << ")\t end: \t\t(" << endX << ", " << endY << ")" << std::endl << std::endl;

	if(startX < 0) startX = 0;
	if(startY < 0) startY = 0;
	if(endX > compmap->width-1) endX = compmap->width-1;
	if(endY > compmap->height-1) endY = compmap->height-1;

	for(unsigned int y = startY; y <= endY; y++) {
		for(unsigned int x = startX; x <= endX; x++) {
			unsigned int tile = x + y*compmap->width;
			renderTile(compmap->tiles[tile], compcam->getX(), compcam->getY());
		}
	}

	for(unsigned int y = startY; y <= endY; y++) {
		for(unsigned int x = startX; x <= endX; x++) {
			unsigned int tile = x + y*compmap->width;
			renderTileEntities(compmap->tiles[tile], compcam->getX(), compcam->getY());
		}
	}

}

void CSysRenderMap::renderTile(EntityID tile, long x, long y) {
	render->update(tile, x, y);
}

void CSysRenderMap::renderTileEntities(EntityID tile, long x, long y) {
	CCompTile* comp = (*manager)[tile].getComponent<CCompTile>();
	
	for (EntityID ent : comp->entities) {
		render->update(ent);// , x, y);
		//renderTileContainer(ent, x, y);
	}
}

void CSysRenderMap::renderTileContainer(EntityID entity, long x, long y) {
	CCompContainer* container = (*manager)[entity].getComponent<CCompContainer>();
	if (container) {
		for (EntityID ent : container->entityList) {
			render->update(ent, x, y);
			renderTileContainer(ent, x, y);
		}
	}
}

void CSysRenderMap::renderGrid(EntityID map, long x, long y) {
	CCompMap* compmap = (*manager)[map].getComponent<CCompMap>();
	EntityID camera = (*manager)[map].getComponent<CCompCameraControlled>()->camera;
	CCompPosition* compcam = (*manager)[camera].getComponent<CCompPosition>();

	render->getRenderer()->setDrawColor(255, 255, 0, 255);

	int width = compmap->radius*sqrt(3);
	int ymove = compmap->radius*1.5;
	int ymodulo = compmap->radius*3;
	for(unsigned int i = 0; i < 18; i++) {
		for(unsigned int j = 0; j < 16; j++) {
			SDL_Point point;
			point.x = width/2 + i*width - (int)compcam->getX()%(width*2);
			point.y = compmap->radius + j*ymove - (int)compcam->getY()%ymodulo;
			if(j%2 == 1) point.x += width/2;

			render->getRenderer()->drawHexagon(point, compmap->radius, false);
		}
	}
}