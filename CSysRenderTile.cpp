#include "CSysRenderMap.h"
#include "CCompTile.h"
#include "CCompActor.h"

CSysRenderTile::CSysRenderTile(CSysRender* render) {
	this->render = render;
}

CSysRenderTile::~CSysRenderTile() {

}

void CSysRenderTile::update(CEntity* tile, long offsetX, long offsetY) {
	CCompTile* comp = tile->getComponent<CCompTile>();
	render->update(tile, offsetX, offsetY);
	for(CEntity* ent : comp->entities) {
		render->update(ent, offsetX, offsetY);
	}
}

CSysRender* CSysRenderTile::getRenderer() {
	return this->render;
}