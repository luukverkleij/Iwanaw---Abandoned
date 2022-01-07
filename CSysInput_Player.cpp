#include "CSysInput.h"

#include "CSysPathFinding.h"

#include "CCompPlayer.h"
#include "CCompActor.h"
#include "CCompPlayerControlled.h"
#include "CCompMovement.h"
#include "CCompPosition.h"
#include "CCompMapped.h"
#include "CCompMap.h"
#include "CCompTurnBased.h"
#include "CCompUnit.h"
#include "CCompTile.h"

#include "CCompCamera.h"
#include "CCompCameraControlled.h"

//MENU
#include "CCompCollision.h"
#include "CCompContainer.h"
#include "CCompContained.h"
#include "SDLTTF.h"
#include "CEllipse.h"

#include "CHexagon.h"

EInputResult CSysInput::inputStartTurn(EntityID entity) {
	CCompPointer* pointer = manager->get(entity).getComponent<CCompPointer>();

	if (pointer == nullptr) return NONE;

	CCompPlayerControlled* pointerplayer	= manager->get(entity).getComponent<CCompPlayerControlled>();
	CCompPlayer* playercomp					= manager->get(pointerplayer->getPlayer()).getComponent<CCompPlayer>();	

	playercomp->turn++;
	for (EntityID ent : playercomp->controlledEntities) {
		CCompTurnBased* turnbased = manager->get(ent).getComponent<CCompTurnBased>();
		if (turnbased) turnbased->used = false;
	}

	return CONSUMED;
}

EInputResult CSysInput::inputDeselect(EntityID entity) {
	CCompPlayer* player = manager->get(entity).getComponent<CCompPlayer>();
	EntityID entitySelected = player->selectedEntity;

	if (entitySelected == emptyid) return NONE;

	CCompActor* act = manager->get(entitySelected).getComponent<CCompActor>();
	if(act != nullptr) {
		act->actor.removeTag("select");
	}

	CCompMapped* mapped = manager->get(entitySelected).getComponent<CCompMapped>();
	if(mapped != nullptr) {
		CCompMap* map = manager->get(mapped->getMap()).getComponent<CCompMap>();
		for(unsigned int index : mapped->getPaths().getPositions()) {		
			CCompActor* act = manager->get(map->tiles[index]).getComponent<CCompActor>();
			act->actor.removeTag("reach");
			act->actor.removeTag("attack");
		}
	}

	CCompUnit* unit = manager->get(entitySelected).getComponent<CCompUnit>();
	if (unit != nullptr) {
		CCompMap* map = manager->get(mapped->getMap()).getComponent<CCompMap>();
		for (unsigned int tile : unit->attackable) {
			CCompActor* act = manager->get(map->tiles[tile]).getComponent<CCompActor>();
			act->actor.removeTag("attack");
		}

		unit->selectedTile = std::numeric_limits<unsigned int>::max();
		unit->attackable.clear();
	}

	player->selectedEntity = emptyid;

	return CONSUMED;
}

EInputResult CSysInput::inputSelect(EntityID playerID, EntityID entityID) {
	if (!manager->get(entityID).hasComponent<CCompPlayerControlled>()) return NONE;

	CCompPlayer* player = manager->get(playerID).getComponent<CCompPlayer>();

	player->selectedEntity = entityID;
	CCompActor* act = manager->get(entityID).getComponent<CCompActor>();
	CCompMapped* mapped = manager->get(entityID).getComponent<CCompMapped>();
	CCompMovement* mov = manager->get(entityID).getComponent<CCompMovement>();
	CCompTurnBased* turn = manager->get(entityID).getComponent<CCompTurnBased>();
	CCompUnit* unit = manager->get(entityID).getComponent<CCompUnit>();

	if (unit) {
		std::cout << "Unit HP, Fuel: " << unit->health << ", " << unit->fuel << std::endl;
	}

	if (turn && turn->used) return NONE;

	if(mov != nullptr) {
		if (mov->velocityX || mov->velocityY) return NONE;
	}

	if(act != nullptr) {
		act->actor.addTag("select");
	}

	if(mapped != nullptr) {
		CSysPathFinding wub(this->store, this->manager);
		wub.update(entityID);
	}

	return CONSUMED;
}

EInputResult CSysInput::inputMove(EntityID entity) {
	EntityID playerID = manager->get(entity).getComponent<CCompPlayerControlled>()->getPlayer();
	CCompPosition* pos = manager->get(entity).getComponent<CCompPosition>();
	CCompPlayer* player = manager->get(playerID).getComponent<CCompPlayer>();

	if(player != nullptr && player->selectedEntity != emptyid) {
		CCompPlayerControlled* selectPlayer = manager->get(player->selectedEntity).getComponent<CCompPlayerControlled>();

		if (manager->get(selectPlayer->getPlayer()).getComponent<CCompPlayer>()->id != player->id)
			return NONE;

		/*
			Check if the entity has a movement component.player->selectedEntity
			If it has, MAKE THE UI FOR PATH YES!
		*/

		CCompMovement* mov = manager->get(player->selectedEntity).getComponent<CCompMovement>();
		if(mov != nullptr) {
			CCompMapped* mapped = manager->get(player->selectedEntity).getComponent<CCompMapped>();

			if(mapped == nullptr) {
				mov->path.clear();
				mov->path.push_back(Point_f(pos->getX(), pos->getY()));
			}
			else {
				EntityID selectedEntity = player->selectedEntity;
				CCompUnit* unit = manager->get(selectedEntity).getComponent<CCompUnit>();
				CCompMapped* mapped = manager->get(selectedEntity).getComponent<CCompMapped>();
				CCompMap* map = manager->get(mapped->getMap()).getComponent<CCompMap>();
				EntityID camEntity = map->entity->getComponent<CCompCameraControlled>()->camera;
				CCompPosition* cam = manager->get(camEntity).getComponent<CCompPosition>();

				Point_d tilePos = pos->getAbsolutePos();
				int tile = map->getTile(pos->getX() + cam->getX(), pos->getY() + cam->getY());

				if (unit->attackable.size()) {
					if (std::find(unit->attackable.begin(), unit->attackable.end(), tile) != unit->attackable.end()) {
						inputMove(selectedEntity, unit->selectedTile);
						EntityID enemyID = manager->get(map->tiles[tile]).getComponent<CCompTile>()->entities.back();
						manager->get(enemyID).getComponent<CCompUnit>()->health -= 5;
					}
				}
				else {

					std::vector<unsigned int> paths = mapped->getPaths().getPositions();
					if (std::find(paths.begin(), paths.end(), tile) != paths.end()) {
						std::cout << "Click menu created at (" << pos->getX() + cam->getX() << "," << pos->getY() + cam->getY() << ")" << std::endl;
						return inputOptions(playerID, selectedEntity, tile);
					}
				}

			}
		}
	}

	return EInputResult::NONE;
}

EInputResult CSysInput::inputOptions(EntityID playerID, EntityID selectedID, unsigned int tile) {
	CCompMapped* mapped = manager->get(selectedID).getComponent<CCompMapped>();
	CCompMap* map = manager->get(mapped->getMap()).getComponent<CCompMap>();
	EntityID cameraID = manager->get(mapped->getMap()).getComponent<CCompCameraControlled>()->camera;
	Point_d tilePos = map->getTilePos(tile);
	bool hasAttack = mapped->getPaths().isAttack(tile);


	auto moveButton_action1 = [this, selectedID, tile](CEntityManager* manager, EntityID button, EntityID point) -> EInputResult {
		std::cout << "Lambda function called! " << std::endl;

		inputMove(selectedID, tile);

		EntityID menu = manager->get(button).getComponent<CCompContained>()->parent;
		manager->destroy(menu);

		return DESTROYED;
	};

	auto attackButton_action1 = [this, selectedID, tile](CEntityManager* manager, EntityID button, EntityID point) -> EInputResult {
		this->inputAttack(selectedID, tile);
		
		EntityID menu = manager->get(button).getComponent<CCompContained>()->parent;
		manager->destroy(menu);
		return DESTROYED;
	};

	auto hoverOff = [](CEntityManager* manager, EntityID menu, EntityID point) -> EInputResult {
		manager->destroy(menu);
		return DESTROYED;
	};

	std::cout << "Creating click menu..." << std::endl;
	EntityID clickmenu = manager->create();
	manager->get(clickmenu).setName("Click Menu");

	store->getRenderer()->setDrawColor(255, 255, 255, 200);
	this->store->saveTexture("buttonMove", CEllipse(26, 26).toTextureFilled(store->getRenderer()));
	store->getRenderer()->setDrawColor(0, 0, 0, 200);
	this->store->saveTexture("buttonMove outline", CEllipse(27, 27).toTexture(store->getRenderer()));
	this->store->saveTexture("buttonMove text", SDLTTF("resources//default.ttf", 12).createBlended((*this->store->getRenderer()), "Move"));

	store->getRenderer()->setDrawColor(255, 255, 255, 200);
	this->store->saveTexture("buttonAttack", CEllipse(26, 26).toTextureFilled(store->getRenderer()));
	store->getRenderer()->setDrawColor(0, 0, 0, 200);
	this->store->saveTexture("buttonAttack outline", CEllipse(27, 27).toTexture(store->getRenderer()));
	this->store->saveTexture("buttonAttack text", SDLTTF("resources//default.ttf", 12).createBlended((*this->store->getRenderer()), "Attack"));

	EntityID buttonMove = manager->create();

	CCompPosition::create(clickmenu, manager);
	CCompActor::create(clickmenu, manager);
	CCompCollision::create(clickmenu, manager);
	CCompInput::create(clickmenu, manager);
	CCompMovement::create(clickmenu, manager);
	CCompContainer::create(clickmenu, manager);
	CCompCameraControlled::create(clickmenu, manager);

	CCompPosition::create(buttonMove, manager);
	CCompActor::create(buttonMove, manager);
	CCompCollision::create(buttonMove, manager);
	CCompInput::create(buttonMove, manager);
	CCompMovement::create(buttonMove, manager);
	CCompContained::create(buttonMove, manager);

	store->getRenderer()->setDrawColor(0, 0, 128, 128);
	store->saveTexture("clickmenu", CRectangle(150, 150).toTextureFilled(store->getRenderer()));

	manager->get(clickmenu).getComponent<CCompPosition>()->setPos(tilePos.x, tilePos.y);
	manager->get(clickmenu).getComponent<CCompActor>()->actor.addDefaultTexture(store->getTexture("clickmenu"));
	manager->get(clickmenu).getComponent<CCompActor>()->actor.addTexture(CTaggedTexture({ "hover" }, {}, store->getTexture("clickmenu")));
	manager->get(clickmenu).getComponent<CCompCollision>()->setShape(new CRectangle(150, 150));
	manager->get(clickmenu).getComponent<CCompInput>()->listenerHoverOff = hoverOff;
	manager->get(clickmenu).getComponent<CCompCameraControlled>()->camera = cameraID;

	manager->get(buttonMove).getComponent<CCompActor>()->actor.addDefaultTexture(store->getTexture("buttonMove"));
	manager->get(buttonMove).getComponent<CCompActor>()->actor.addDefaultTexture(store->getTexture("buttonMove outline"));
	manager->get(buttonMove).getComponent<CCompActor>()->actor.addTexture(CTaggedTexture({ "hover" }, {}, store->getTexture("buttonMove")));
	manager->get(buttonMove).getComponent<CCompActor>()->actor.addTexture(CTaggedTexture({}, {}, store->getTexture("buttonMove text")));
	manager->get(buttonMove).getComponent<CCompCollision>()->setShape(new CEllipse(26, 26));
	manager->get(buttonMove).getComponent<CCompInput>()->listenerAction1 = moveButton_action1;
	manager->get(buttonMove).getComponent<CCompPosition>()->setPos(0, -70);

	manager->get(clickmenu).getComponent<CCompContainer>()->entityList.push_back(buttonMove);
	manager->get(buttonMove).getComponent<CCompContained>()->parent = clickmenu;

	if (hasAttack) {
		EntityID buttonAttack = manager->create();

		CCompPosition::create(buttonAttack, manager);
		CCompActor::create(buttonAttack, manager);
		CCompCollision::create(buttonAttack, manager);
		CCompInput::create(buttonAttack, manager);
		CCompMovement::create(buttonAttack, manager);
		CCompContained::create(buttonAttack, manager);

		manager->get(buttonAttack).getComponent<CCompActor>()->actor.addDefaultTexture(store->getTexture("buttonAttack"));
		manager->get(buttonAttack).getComponent<CCompActor>()->actor.addDefaultTexture(store->getTexture("buttonAttack outline"));
		manager->get(buttonAttack).getComponent<CCompActor>()->actor.addTexture(CTaggedTexture({ "hover" }, {}, store->getTexture("buttonAttack")));
		manager->get(buttonAttack).getComponent<CCompActor>()->actor.addTexture(CTaggedTexture({}, {}, store->getTexture("buttonAttack text")));
		manager->get(buttonAttack).getComponent<CCompCollision>()->setShape(new CEllipse(26, 26));
		manager->get(buttonAttack).getComponent<CCompInput>()->listenerAction1 = attackButton_action1;
		manager->get(buttonAttack).getComponent<CCompPosition>()->setPos(0, 0);

		manager->get(clickmenu).getComponent<CCompContainer>()->entityList.push_back(buttonAttack);
		manager->get(buttonAttack).getComponent<CCompContained>()->parent = clickmenu;
	}

	return CONSUMED;
}

EInputResult CSysInput::inputMove(EntityID entityID, unsigned int tile) {
	EntityID playerID = manager->get(entityID).getComponent<CCompPlayerControlled>()->getPlayer();
	CCompTurnBased* turn = manager->get(entityID).getComponent<CCompTurnBased>();
	CCompMovement* mov = manager->get(entityID).getComponent<CCompMovement>();
	CCompMapped* mapped = manager->get(entityID).getComponent<CCompMapped>();
	CCompMap* map = manager->get(mapped->getMap()).getComponent<CCompMap>();

	mov->path.clear();
	for (unsigned int pathtile : mapped->getPaths().getPath(tile)) {
		Point_d pathPos = map->getTilePos(pathtile);
		mov->path.push_back(Point_f(pathPos.x, pathPos.y));
		std::cout << "path pos " << pathPos.x << ", " << pathPos.y << std::endl;
	}

	if (turn) {
		turn->used = true;
	}

	inputDeselect(playerID);

	return CONSUMED;
}

EInputResult CSysInput::inputAttack(EntityID entityID, unsigned int tile) {
	CCompMapped* mapped = manager->get(entityID).getComponent<CCompMapped>();

	EntityID mapID = manager->get(entityID).getComponent<CCompMapped>()->getMap();
	CCompMap* map = manager->get(mapID).getComponent <CCompMap>();

	std::vector<Point_i32> neighbours = map->getNeighbours(tile);

	for (Point_i32 neighbourTile : neighbours) {
		int neighbourTileNum = neighbourTile.x + neighbourTile.y * map->width;
		if (map->canAttack(manager, entityID, neighbourTileNum)) {
			EntityID tileID = map->tiles[neighbourTileNum];
			manager->get(tileID).getComponent<CCompActor>()->actor.addTag("attack");
			manager->get(entityID).getComponent<CCompUnit>()->attackable.push_back(neighbourTileNum);
		}
	}

	CCompUnit* unit = manager->get(entityID).getComponent<CCompUnit>();
	if (unit->attackable.size() > 0) {
		unit->selectedTile = tile;
		CCompMap* map = manager->get(mapped->getMap()).getComponent<CCompMap>();
		for (unsigned int index : mapped->getPaths().getPositions()) {
			CCompActor* act = manager->get(map->tiles[index]).getComponent<CCompActor>();
			act->actor.removeTag("reach");
			act->actor.removeTag("attack");
		}
	}	

	return CONSUMED;
}

