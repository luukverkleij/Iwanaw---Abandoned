#include "CSysInput.h"

#include "CCompMap.h"
#include "CCompPointer.h"
#include "CCompPosition.h"
#include "CCompActor.h"
#include "CCompCameraControlled.h"
#include "CCompTile.h"

bool CSysInput::updateMap(EntityID entity, SDL_Event* eventHandler) {
	return false;
}

EInputResult CSysInput::inputMap(EntityID entity, SDL_Event* eventHandler) {
	CCompMap* map = manager->get(entity).getComponent<CCompMap>();
	if (map == nullptr)
		return NONE;

	CCompPointer* point = manager->get((EntityID)(eventHandler->user.data1)).getComponent<CCompPointer>();
	CCompPosition* pointer_pos = manager->get((EntityID)(eventHandler->user.data1)).getComponent<CCompPosition>();

	CCompActor* act = (*manager)[entity].getComponent<CCompActor>();
	EntityID cameraEntity = manager->get(entity).getComponent<CCompCameraControlled>()->camera;
	CCompPosition* camPos = manager->get(cameraEntity).getComponent<CCompPosition>();

	int x = pointer_pos->getX() + (int)camPos->getX();
	int y = pointer_pos->getY() + (int)camPos->getY();
	int tile = map->getTile(x, y);

	if(tile >= 0 && tile < map->tiles.size()) {
		if (map->previousTile != tile) {
			inputMapPreviousTile(map, point, eventHandler);
			map->previousTile = tile;

			point->hovers.push_back(map->tiles[tile]);
			inputHoverOn(map->tiles[tile]);
		}

		EntityID tileID = map->tiles[tile];
		for (EntityID ent : manager->get(tileID).getComponent<CCompTile>()->entities) {
			update(ent, eventHandler);
		}
	}
	else {
		inputMapPreviousTile(map, point, eventHandler);
		map->previousTile = -1;
	}

	return CONSUMED;
}

EInputResult CSysInput::inputMapPreviousTile(CCompMap* map, CCompPointer* point, SDL_Event* eventHandler) {
	if (map->previousTile >= 0) {
		auto it = std::find(point->hovers.begin(), point->hovers.end(), map->tiles[map->previousTile]);

		if (it != point->hovers.end()) {
			point->hovers.erase(it);
			inputHoverOff(map->tiles[map->previousTile]);
		}

		EntityID tileID = map->tiles[map->previousTile];
		for (EntityID ent : manager->get(tileID).getComponent<CCompTile>()->entities) {
			update(ent, eventHandler);
		}
	}

	return CONSUMED;
}