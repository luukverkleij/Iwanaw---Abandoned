#include "CSysInput.h"

#include "CCompInput.h"
#include "CCompCamera.h"
#include "CCompMovement.h"
#include "CCompActor.h"
#include "CCompHover.h"
#include "CCompMap.h"
#include "CCompPosition.h"
#include "CCompCollision.h"
#include "CCompCameraControlled.h"
#include "CCompMapped.h"
#include "CCompPlayerControlled.h"
#include "CCompPlayer.h"
#include "CCompPointer.h"
#include "CCompContainer.h"

#include <iostream>

void CSysInput::updateAll(SDL_Event* eventHandler) {

	std::vector<EntityID> managerList = this->manager->getList();

	EInputResult result = NONE;
	for (int index = managerList.size() - 1; result == EInputResult::NONE && index >= 0; --index) {
		EntityID managerEntity = managerList[index];

		if ((*manager)[managerEntity].hasComponent<CCompContainer>()) {
			std::vector<unsigned int> &list = (*manager)[managerEntity].getComponent<CCompContainer>()->entityList;
			CCompPosition* pos = (*manager)[managerEntity].getComponent<CCompPosition>();

			for (auto containerIt = list.rbegin(); result == EInputResult::NONE && containerIt != list.rend(); containerIt++) {
				result = update((*containerIt), eventHandler);
			}
		}

		result = update(managerEntity, eventHandler);
	}

	if (result != NONE) {
		std::cout << "EVENT CONSUMED" << std::endl;
	}
}

EInputResult CSysInput::update(EntityID entity, SDL_Event* eventHandler) {

	if (eventHandler->type >= SDL_USEREVENT) {
		if (eventHandler->type == (Uint32)INTENT::MOVECAMERA){
			return inputMoveCamera(entity, eventHandler);
		}
		else if (eventHandler->type == (Uint32)INTENT::STOPCAMERA) {
			return inputStopCamera(entity, eventHandler);
		}
		else if (eventHandler->type == (Uint32)INTENT::HOVER) {

			if (manager->get(entity).hasComponent<CCompMap>()) {
				return inputMap(entity, eventHandler);
			}
			else {
				return inputHover(entity, eventHandler);
			}
		}
		else if (eventHandler->type == (Uint32)INTENT::POINTER) {
			return inputPointer(entity, eventHandler);
		}
		else if (eventHandler->type == (Uint32)INTENT::TURN) {
			return inputStartTurn(entity);
		}
		else if (eventHandler->type == (Uint32)INTENT::EMESSAGE) {
			if (manager->get(entity).hasComponent<CCompPointer>()) {
				CCompPointer* pointer = manager->get(entity).getComponent<CCompPointer>();
				EntityID messageEntity = EntityID(eventHandler->user.data1);

				auto it = std::find(pointer->hovers.begin(), pointer->hovers.end(), messageEntity);
				if (it != pointer->hovers.end()) pointer->hovers.erase(it);

				return CONSUMED;
			}
		}
	}

	return NONE;
}