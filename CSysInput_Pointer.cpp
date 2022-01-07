#include "CSysInput.h"

#include "CCompPointer.h"
#include "CCompPosition.h"
#include "CCompPlayerControlled.h"
#include "CCompContained.h"

#include "CCompTile.h"
#include "CCompCollision.h"
#include "CCompMapped.h"
#include "CCompCameraControlled.h"
#include "CCompActor.h"

bool CSysInput::updatePointer(EntityID entity, SDL_Event* eventHandler) {
	return false;
}

EInputResult CSysInput::inputPointer(EntityID entity, SDL_Event* eventHandler) {
	CCompPointer* point = manager->get(entity).getComponent<CCompPointer>();
	CCompPosition* pos = manager->get(entity).getComponent<CCompPosition>();
	CCompPlayerControlled* con = manager->get(entity).getComponent<CCompPlayerControlled>();
	CCompCameraControlled* cam = manager->get(entity).getComponent<CCompCameraControlled>();
	double offset_x = 0;
	double offset_y = 0;

	EInputResult result = EInputResult::NONE;

	if (cam) {
		CCompPosition* camPos = manager->get(cam->camera).getComponent<CCompPosition>();
		offset_x = camPos->getX();
		offset_y = camPos->getY();
	}

	if (point == nullptr || pos == nullptr) return NONE;

	Point_d oldPos = pos->getAbsolutePos();
	//double oldX = pos->getX() - offset_x;
	//double oldY = pos->getY() - offset_y;

	if(eventHandler->user.type == (Uint32)INTENT::POINTER) {
		if(eventHandler->user.code == (Uint32)POINTER::SET) {
			pos->setPos((int)eventHandler->user.data1, (int)eventHandler->user.data2);
		}
	}

	if(eventHandler->user.type == (Uint32)INTENT::POINTER) {
		if(eventHandler->user.code == (Uint32)POINTER::ACTION1) {
			for (int index = point->hovers.size() - 1;result == NONE && index >= 0; --index) {
				if (manager->get(point->hovers[index]).getComponent<CCompInput>()->listenerAction1) {
					result = manager->get(point->hovers[index]).getComponent<CCompInput>()->listenerAction1(manager, point->hovers[index], entity);
				}
				if (result == NONE) {
					if (manager->get(point->hovers[index]).hasComponent<CCompTile>()
						|| manager->get(point->hovers[index]).hasComponent<CCompPlayerControlled>()) {
						inputDeselect(con->getPlayer());
					}
					result = inputSelect(con->getPlayer(), point->hovers[index]);
				}
			}
			
		}
		else if(eventHandler->user.code == (Uint32)POINTER::ACTION2) {
			result = inputMove(entity);
		}
	}

	pos = manager->get(entity).getComponent<CCompPosition>();
	if (eventHandler->user.code == (Uint32)POINTER::UPDATE || oldPos != pos->getAbsolutePos()) {
		//std::cout << "pointer position changed" << std::endl;

		SDL_Event intent;
		SDL_zero(intent);
		intent.type = (Uint32)INTENT::HOVER;
		intent.user.data1 = (void*)entity;
		SDL_PushEvent(&intent);
	}

	return result;
}

EInputResult CSysInput::inputHover(EntityID entity, SDL_Event* eventHandler) {
	CCompCollision* col = manager->get(entity).getComponent<CCompCollision>();
	CCompPosition* compPos = manager->get(entity).getComponent<CCompPosition>();
	CCompContained* contained = manager->get(entity).getComponent<CCompContained>();
	if(col == nullptr || compPos == nullptr) return NONE;

	Point_d pos = compPos->getAbsolutePos();
	/*if (contained != nullptr) {
		Point_d containerPos = contained->parent->getComponent<CCompPosition>()->getPos();
		pos.x += containerPos.x;
		pos.y += containerPos.y;
	}*/

	CCompMapped* map = manager->get(entity).getComponent<CCompMapped>();
	CCompCameraControlled* cam = manager->get(entity).getComponent<CCompCameraControlled>();
	if(map != nullptr) {
		cam = manager->get(map->getMap()).getComponent<CCompCameraControlled>();
	}

	/*if(cam != nullptr) {
		pos.x -= cam->camera->getComponent<CCompPosition>()->getX();
		pos.y -= cam->camera->getComponent<CCompPosition>()->getY();
	}*/

	col->getShape()->setOrigin(Point_d(pos.x, pos.y));
	
	EntityID ent = (unsigned int)eventHandler->user.data1;
	CCompPointer* point = manager->get(ent).getComponent<CCompPointer>();
	CCompPosition* pointer_pos = manager->get(ent).getComponent<CCompPosition>();
	
	std::vector<EntityID>::iterator it = std::find(point->hovers.begin(), point->hovers.end(), entity);
	if(it == point->hovers.end()) {
		if(col->getShape()->inside(pointer_pos->getPos())) {
			point->hovers.push_back(entity);
			return inputHoverOn(entity);
		}
	}
	else {
		if (!col->getShape()->inside(pointer_pos->getPos())) {
			point->hovers.erase(it);
			EInputResult result = inputHoverOff(entity);
			return result;
		}
		else
			return CONSUMED;
	}

	return NONE;
}

EInputResult CSysInput::inputHoverOn(EntityID entity) {
	CCompActor* act = manager->get(entity).getComponent<CCompActor>();
	CCompInput* inp = manager->get(entity).getComponent<CCompInput>();

	if(act != nullptr)
		act->actor.addTag("hover");

	std::cout << "Hover on - " << manager->get(entity).getName() << std::endl;

	if (manager->get(entity).hasComponent<CCompPosition>()) {
		std::cout << "Absolute Position: " << manager->get(entity).getComponent<CCompPosition>()->getAbsolutePos().toString();
	}

	if (inp->listenerHoverOn) return inp->listenerHoverOn(manager, entity, 0);

	return CONSUMED;
}

EInputResult CSysInput::inputHoverOff(EntityID entity) {
	CCompActor* act = manager->get(entity).getComponent<CCompActor>();
	CCompInput* inp = manager->get(entity).getComponent<CCompInput>();
	EInputResult result = CONSUMED;

	if(act != nullptr)
		act->actor.removeTag("hover");

	std::cout << "Hover off - " << manager->get(entity).getName() << std::endl;

	if (inp->listenerHoverOff) result = inp->listenerHoverOff(manager, entity, 0);

	return result;
}