#include "CSysMessaging.h"
#include "CCompMessage.h"
#include "CCompPosition.h"
#include "CCompMovement.h"
#include "CCompMapped.h"
#include "CCompPlayerControlled.h"
#include "CCompPlayer.h"

#include "CCompCamera.h"
#include "CCompPointer.h"
#include "CCompCameraControlled.h"
#include "CCompInput.h"
#include "CCompContainer.h"

#include <iostream>

EInputResult CSysMessaging::update(CComponentManager* compManager, SDL_Event* eventHandler) {
	std::vector<EntityID> managerList = this->manager->getList();
	//std::cout << "managerList Size: " << managerList.size() << std::endl;

	for (int index = 0; index < managerList.size(); ++index) {
		EntityID managerEntity = managerList[index];
		EInputResult result = update(managerEntity, eventHandler);

		if (!(result & DESTROYED) && (*manager)[managerEntity].hasComponent<CCompContainer>()) {
			std::vector<unsigned int> &list = (*manager)[managerEntity].getComponent<CCompContainer>()->entityList;
			CCompPosition* pos = (*manager)[managerEntity].getComponent<CCompPosition>();

			EInputResult result = NONE;
			for (auto containerIt = list.rbegin(); containerIt != list.rend() && result != CONSUMED; containerIt++) {
				result = update((*containerIt), eventHandler);
			}
			/*for (EntityID tempEntity : list) {
			update(ent2, eventHandler);
			}*/
		}
	}

	return EInputResult::NONE;
}


/*
void CSysMessaging::update(CComponentManager* compManager, SDL_Event* eventHandler) {
	for(unsigned int index = 0; index < compManager->getPool<CCompMessage>()->size(); index++) {
		update(manager->getID(compManager->getPool<CCompMessage>())[index].entity), eventHandler);
	}
}
*/

EInputResult CSysMessaging::update(SDL_Event* eventHandler) {
	EntityID entity = (EntityID)(eventHandler->user.data2);

	if (entity == emptyid) return EInputResult::NONE;

	if ((int)eventHandler->user.data1 == CCompPosition::componentID) {
		CCompMapped* mapped = manager->get(entity).getComponent<CCompMapped>();

		if (mapped) {
			mapped->updateTilePos();
		}
	}

	else if ((int)eventHandler->user.data1 == CCompPlayerControlled::componentID) {
		CCompPlayerControlled*	playercontrolled	= manager->get(entity).getComponent<CCompPlayerControlled>();
		CCompPlayer*			player				= manager->get(playercontrolled->getPlayer()).getComponent<CCompPlayer>();

		if (player) {
			player->controlledEntities.push_back(entity);
		}
	}

	return EInputResult::CONSUMED;
}

EInputResult CSysMessaging::update(EntityID entity, SDL_Event* eventHandler) {
	if((int)eventHandler->user.data1 == CCompPosition::componentID) {
		CCompMapped* mapped = manager->get(entity).getComponent<CCompMapped>();

		if(mapped) {
			mapped->updateTilePos();
		}
	}
	else if ((int)eventHandler->user.data1 == CCompPlayerControlled::componentID) {
		CCompPlayerControlled*	playercontrolled	= manager->get(entity).getComponent<CCompPlayerControlled>();
		CCompPlayer*			player				= manager->get(playercontrolled->getPlayer()).getComponent<CCompPlayer>();

		if (player) {
			player->controlledEntities.push_back(manager->getID(playercontrolled->entity));
		}
	}
	else if ((int)eventHandler->user.data1 == CCompCamera::componentID) {
		CCompPointer* pointer = manager->get(entity).getComponent<CCompPointer>();
		CCompCameraControlled* camcon = manager->get(entity).getComponent<CCompCameraControlled>();

		if (pointer && camcon) {
			
		}
	}

	return EInputResult::NONE;
}