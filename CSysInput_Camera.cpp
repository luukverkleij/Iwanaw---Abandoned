#include "CSysInput.h"

#include "CCompCamera.h"
#include "CCompMovement.h"

EInputResult CSysInput::inputMoveCamera(EntityID entity, SDL_Event* eventHandler) {
	CCompCamera* cam = manager->get(entity).getComponent<CCompCamera>();
	CCompMovement* move = manager->get(entity).getComponent<CCompMovement>();

	if(cam != nullptr && move != nullptr) {
		if(eventHandler->user.code == (Sint32)CAMERACODE::NORTH) move->moveY--;
		else if(eventHandler->user.code == (Sint32)CAMERACODE::SOUTH) move->moveY++;
		else if(eventHandler->user.code == (Sint32)CAMERACODE::WEST) move->moveX--;
		else if(eventHandler->user.code == (Sint32)CAMERACODE::EAST) move->moveX++;

		return NONE;
	}

	return NONE;
}

EInputResult CSysInput::inputStopCamera(EntityID entity, SDL_Event* eventHandler) {
	CCompCamera* cam = manager->get(entity).getComponent<CCompCamera>();
	CCompMovement* move = manager->get(entity).getComponent<CCompMovement>();

	if(cam != nullptr && move != nullptr) {
		if(eventHandler->user.code == (Sint32)CAMERACODE::NORTH) move->moveY++;
		else if(eventHandler->user.code == (Sint32)CAMERACODE::SOUTH) move->moveY--;
		else if(eventHandler->user.code == (Sint32)CAMERACODE::WEST) move->moveX++;
		else if(eventHandler->user.code == (Sint32)CAMERACODE::EAST) move->moveX--;

		return NONE;
	}

	return NONE;
}