#ifndef _CSYSINPUT_H_
#define _CSYSINPUT_H_

#include "CSystem.h"
#include "CCompInput.h"
#include "CCompPointer.h"
#include "CCompMap.h"

#include "SDLEvent.h"

class CSysInput : public CSystem {
public:
	CSysInput(CStorage* s, CEntityManager* m) : CSystem(s, m) {};

	void updateAll(SDL_Event* eventHandler);

	EInputResult update(EntityID entity, SDL_Event* eventHandler);
		bool updatePointer(EntityID entity, SDL_Event* eventHandler);
		bool updateMap(EntityID entity, SDL_Event* eventHandler);
		bool updateCamera(EntityID entity, SDL_Event* eventHandler);

	EInputResult inputAction(EntityID entity, SDL_Event* eventHandler);
		EInputResult inputDeselect(EntityID entity);
		EInputResult inputSelect(EntityID player, EntityID entity);
		EInputResult inputMove(EntityID entity);
		EInputResult inputOptions(EntityID player, EntityID selectedID, unsigned int tile);
		EInputResult inputMove(EntityID entityID, unsigned int tile);
		EInputResult inputAttack(EntityID entity, unsigned int tile);
	EInputResult inputMoveCamera(EntityID entity, SDL_Event* eventHandler);
	EInputResult inputStopCamera(EntityID entity, SDL_Event* eventHandler);
	EInputResult inputMap(EntityID entity, SDL_Event* eventHandler);
		EInputResult inputMapPreviousTile(CCompMap* map, CCompPointer* point, SDL_Event* eventHandler);
	EInputResult inputHover(EntityID entity, SDL_Event* eventHandler);
		EInputResult inputHoverOn(EntityID entity);
		EInputResult inputHoverOff(EntityID entity);
	EInputResult inputPointer(EntityID entity, SDL_Event* eventHandler);
	EInputResult inputStartTurn(EntityID entity);


};

#endif //_CSYSINPUT_H_