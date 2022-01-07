#include "CSysRender.h"
#include "CCompPosition.h"
#include "CCompActor.h"
#include "CCompContainer.h"
#include "CCompCameraControlled.h"

#include <iostream>

CSysRender::CSysRender(CStorage* store, CEntityManager* manager) : CSystem(store, manager) {
	this->renderer = store->getRenderer();
}

CSysRender::~CSysRender() {

}

SDLRenderer* CSysRender::getRenderer() {
	return this->renderer;
}

void CSysRender::updateAll() {
	for (EntityID ent : this->manager->getList()) {
		update(ent);

		
	}
}

void CSysRender::update(EntityID entity) {
		update(entity, 0, 0);		
}

void CSysRender::update(EntityID entity, long offset_x, long offset_y) {
	CCompActor* compAct = manager->get(entity).getComponent<CCompActor>();
	CCompPosition* pos = manager->get(entity).getComponent<CCompPosition>();


	if (compAct == nullptr || pos == nullptr) return;

	
	CActor* act = &manager->get(entity).getComponent<CCompActor>()->actor;
	int width, height;
	Point_d absPos = pos->getAbsolutePos();
	std::vector<SDL_Texture*>* textures = act->getSelected();

	for(int i = 0; i < textures->size(); ++i) {
		SDL_QueryTexture((*textures)[i], nullptr, nullptr, &width, &height);
		width = (int)(width * act->getScaleX());
		height = (int)(height * act->getScaleY());

		SDL_Rect rect = {absPos.x - width/2 - offset_x, absPos.y - height/2 - offset_y, 
			width, height};
		SDL_Rect viewport = this->renderer->getViewport();

		if(!(rect.x + width <= viewport.x || rect.y + height <= viewport.y
			|| rect.x >= viewport.w || rect.y>= viewport.h))
		{
			if(SDL_RectEmpty(&act->getTilePos())) {
				this->renderer->drawTexture((*textures)[i], nullptr, &rect);
			} else {
				this->renderer->drawTexture((*textures)[i], &act->getTilePos(), &rect);
			}
		}
	}

	if (manager->get(entity).hasComponent<CCompContainer>()) {
		std::vector<EntityID> &list = manager->get(entity).getComponent<CCompContainer>()->entityList;
		CCompPosition* pos = manager->get(entity).getComponent<CCompPosition>();

		for (EntityID ent2 : list) {
			update(ent2);
		}
	}
}