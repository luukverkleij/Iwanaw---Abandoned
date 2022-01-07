#include "CSysRender.h"
#include "CSysMovement.h"
#include "CSysRenderMap.h"
#include "CSysInput.h"
#include "CSysMessaging.h"

#include "CCompActor.h"
#include "CCompPosition.h"
#include "CCompMovement.h"
#include "CCompMap.h"
#include "CCompCamera.h"
#include "CCompCameraControlled.h"
#include "CCompInput.h"
#include "CCompMapped.h"
#include "CCompTile.h"
#include "CCompCollision.h"
#include "CCompPlayer.h"
#include "CCompPlayerControlled.h"
#include "CCompPointer.h"
#include "CCompMessage.h"
#include "CCompContainer.h"
#include "CCompContained.h"
#include "CCompTurnBased.h"
#include "CCompUnit.h"
#include "CCompBuilding.h"

#include "CComponentManager.h"
#include "CEntityManager.h"

#include "CMapCreator.h"

#include "CRectangle.h"
#include "CHexagon.h"

#include "SDLRenderer.h"
#include "SDLEvent.h"
#include "SDLTTF.h"

#include "Cidf.h"

#include <iostream>
#include <sstream>
#include <random>

#include <SDL_image.h>
#include <SDL_ttf.h>

#include "extra_std.h"

void createtestmap(EntityID mapentity, CEntityManager* manager, CStorage* storage) {
	CEntity tile_grass("tile_grass");

	CCompPosition::create(&tile_grass, manager->getComponentManager());
	CCompActor::create(&tile_grass, manager->getComponentManager());
	CCompTile::create(&tile_grass, manager->getComponentManager());
	CCompMessage::create(&tile_grass, manager->getComponentManager());
	CCompInput::create(&tile_grass, manager->getComponentManager());
	
	tile_grass.getComponent<CCompActor>()->actor.addDefaultTexture(storage->getTexture("hex"));
	tile_grass.getComponent<CCompActor>()->actor.addTexture(CTaggedTexture({ "hover" }, { "" }, storage->getTexture("hex_hover")));
	tile_grass.getComponent<CCompActor>()->actor.addTexture(CTaggedTexture({ "reach" }, { "" }, storage->getTexture("hex_reach")));
	tile_grass.getComponent<CCompActor>()->actor.addTexture(CTaggedTexture({ "reach", "hover" }, { "" }, storage->getTexture("hex_reach_hover")));
	tile_grass.getComponent<CCompActor>()->actor.addTexture(CTaggedTexture({ "attack" }, { "" }, storage->getTexture("hex_attack")));
	tile_grass.getComponent<CCompActor>()->actor.addTexture(CTaggedTexture({ "attack", "hover" }, { "" }, storage->getTexture("hex_attack_hover")));

	tile_grass.getComponent<CCompTile>()->cost = 1;

	CEntity tile_mountain(tile_grass);
	CEntity tile_forest(tile_grass);

	tile_mountain.setName("tile_mountain");
	tile_mountain.getComponent<CCompActor>()->actor.addTexture(CTaggedTexture(storage->getTexture("terrain_mountain")));
	tile_mountain.getComponent<CCompTile>()->cost = 3;

	tile_forest.setName("tile_forest");
	tile_forest.getComponent<CCompActor>()->actor.addTexture(CTaggedTexture(storage->getTexture("terrain_forest")));
	tile_forest.getComponent<CCompTile>()->cost = 2;

	std::vector<CEntity*> tileset;
	tileset.push_back(&tile_grass);
	tileset.push_back(&tile_mountain);
	tileset.push_back(&tile_forest);

	/*
	CMapCreator creator(manager);
	creator.addTile(tile_grass);
	creator.addTile(tile_mountain);
	creator.addTile(tile_forest);
	*/

	std::vector<std::vector<unsigned int>> map(20);
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<unsigned int> dist(1, 10);

	for (unsigned int x = 0; x < map.size(); ++x) {
		map[x].resize(20);
		for (unsigned int y = 0; y < map.size(); ++y) {
			unsigned int dice = dist(mt);

			if (dice == 9) map[x][y] = 1;
			else if (dice == 10) map[x][y] = 2;
		}		
	}

	//creator.createMap(mapentity, map, 31);
	CCompMap::create(manager, mapentity, tileset, map, 31);
}

bool _looping = true;

EInputResult closeButton(CEntityManager* m, EntityID ent, EntityID point)  {
	_looping = false;

	return CONSUMED;
}

EInputResult moveMenu(CEntityManager* m, EntityID ent, EntityID point) {
	CCompMovement* mov = m->get(ent).getComponent<CCompMovement>();
	CCompPosition* pos = m->get(ent).getComponent<CCompPosition>();

	mov->path.clear();
	mov->path.push_back(Point_f(750, 100));

	return CONSUMED;
}

EInputResult moveMenuBack(CEntityManager* m, EntityID ent, EntityID point)  {
	CCompMovement* mov = m->get(ent).getComponent<CCompMovement>();
	CCompPosition* pos = m->get(ent).getComponent<CCompPosition>();

	mov->path.clear();
	mov->path.push_back(Point_f(750, -90));

	return CONSUMED;
}

EInputResult close() {
	_looping = false;

	return CONSUMED;
}

void eventing(SDL_Event* handle) {
	if(handle->type == SDL_WINDOWEVENT) {
		switch(handle->window.event) {
		case SDL_WINDOWEVENT_CLOSE: close(); break;
		}
	}
}

int main(int argc, char* args[])
{

	Cidf idf("resources\\test.txt");
	idf.node.print();

	std::cout << "IDF IS THE BEST yEAHH!! " << std::endl << std::endl << std::endl;

	CEntityManager		entityManager;
	CComponentManager	compManager;

	// = new CEntity("Entity player 1");
	//CEntity map;// = new CEntity;
	//CEntity* camera = new CEntity;
	CEntity tile;// = new CEntity;
	//CEntity* player = new CEntity("Player 1");
	//CEntity* player2 = new CEntity("Player 2");
	//CEntity* pointer = new CEntity;
	CEntity shapetest;// = new CEntity;
	CEntity container;// = new CEntity;
	//CEntity* menu = new CEntity("Test Menu");
	// = new CEntity("Quit Button");

	EntityID map = entityManager.create();
	EntityID camera = entityManager.create();
	EntityID player = entityManager.create();
	EntityID player2 = entityManager.create();
	EntityID pointer = entityManager.create();
	EntityID menu = entityManager.create();
	EntityID entity = entityManager.create();
	EntityID button = entityManager.create();

	std::cout << "Creating components..." << std::endl;

	CCompActor::create(entity, &entityManager);
	CCompMapped::create(entity, &entityManager);
	CCompPosition::create(entity, &entityManager);
	CCompMovement::create(entity, &entityManager);
	CCompCollision::create(entity, &entityManager);
	CCompPlayerControlled::create(entity, &entityManager);
	CCompMessage::create(entity, &entityManager);
	CCompInput::create(entity, &entityManager);
	CCompTurnBased::create(entity, &entityManager);
	CCompUnit::create(entity, &entityManager);

	CCompPosition::create(camera, &entityManager);
	CCompMovement::create(camera, &entityManager);
	CCompCamera::create(camera, &entityManager);
	CCompInput::create(camera, &entityManager);

	CCompPosition::create(&tile, &compManager);
	CCompActor::create(&tile, &compManager);
	CCompTile::create(&tile, &compManager);
	CCompMessage::create(&tile, &compManager);

	CCompPointer::create(pointer, &entityManager);
	CCompPosition::create(pointer, &entityManager);
	CCompMovement::create(pointer, &entityManager);
	CCompPlayerControlled::create(pointer, &entityManager);

	CCompPosition::create(&shapetest, &compManager);
	CCompActor::create(&shapetest, &compManager);

	CCompPlayer::create(player, &entityManager);
	CCompPlayer::create(player2, &entityManager);

	entityManager[player].getComponent<CCompPlayer>()->id = 1;
	entityManager[player2].getComponent<CCompPlayer>()->id = 2;

	entityManager[entity].getComponent<CCompPosition>()->setPos(30, 32);

	entityManager[entity].getComponent<CCompMovement>()->speedX = 100;
	entityManager[entity].getComponent<CCompMovement>()->speedY = 100;

	entityManager[entity].getComponent<CCompCollision>()->setShape( new CRectangle(32, 32) );
	entityManager[entity].getComponent<CCompPlayerControlled>()->setPlayer(player);

	entityManager[entity].getComponent<CCompMapped>()->setTileSpeed(3);

	entityManager[entity].getComponent<CCompUnit>()->health = 10;
	entityManager[entity].getComponent<CCompUnit>()->fuel = 100;

	entityManager[camera].getComponent<CCompMovement>()->speedX = 500.0f;
	entityManager[camera].getComponent<CCompMovement>()->speedY = 500.0f;

	entityManager[camera].getComponent<CCompMovement>()->accelerationX = 5.00f;
	entityManager[camera].getComponent<CCompMovement>()->accelerationY = 5.00f;

	entityManager[camera].getComponent<CCompMovement>()->deaccelerationX = 5.00f;
	entityManager[camera].getComponent<CCompMovement>()->deaccelerationY = 5.00f;

	entityManager[pointer].getComponent<CCompPlayerControlled>()->setPlayer(player);

	std::cout << "SDL_Init..." << std::endl;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	std::cout << "Create SDLWindow..." << std::endl;

	SDLWindow window("Testing etc.", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

	if(!window.isCreated()) {
		std::cout << "SDL Window could not be created! SDL Error:"
			<< SDL_GetError() << std::endl;
	}

	std::cout << "Create SDLRnederer..." << std::endl;

	SDLRenderer renderer(&window, -1, SDL_RENDERER_ACCELERATED );

	if(!renderer.isCreated()) {
		std::cout << "SDL Renderer could not be created! SDL Error:"
			<< SDL_GetError() << std::endl;
	}

	if (TTF_Init() < 0) {
		std::cout << "SDL TTF could not be initialized! SDL Error:"
			<< SDL_GetError() << std::endl;
	}

	CStorage storage(&renderer);

	CSysRender render(&storage, &entityManager);
	CSysRenderMap maprender(&render);


	CSysMovement move(&storage, &entityManager);
	CSysInput input(&storage, &entityManager);
	CSysMessaging message(&storage, &entityManager);

	std::cout << "Loading textures..." << std::endl;

	storage.loadTexture("test", "resources//test.png");
	storage.loadTexture("test_hover", "resources//test_hover.png");
	storage.loadTexture("test_select", "resources//test_select.png");
	storage.loadTexture("test_select_hover", "resources//test_select_hover.png");

	storage.loadTexture("hex", "resources//hexagon.png");
	storage.loadTexture("hex_hover", "resources//hexagon_hover.png");
	storage.loadTexture("hex_reach", "resources//hexagon_reach.png");
	storage.loadTexture("hex_reach_hover", "resources//hexagon_reach_hover.png");
	storage.loadTexture("hex_attack", "resources//hexagon_attack.png");
	storage.loadTexture("hex_attack_hover", "resources//hexagon_attack_hover.png");

	storage.loadTexture("terrain_mountain", "resources//terrain_mountain_4.png");
	storage.loadTexture("terrain_forest", "resources//terrain_forest.png");

	SDL_SetRenderDrawColor(renderer.get(), 0, 0, 128, 128);
	storage.saveTexture("menu", CRectangle(100, 200).toTextureFilled(&renderer));
	storage.saveTexture("button", CRectangle(96, 30).toTextureFilled(&renderer));
	storage.saveTexture("buttontext", SDLTTF("resources//default.ttf", 12).createBlended(renderer, "Quit"));

	SDL_Surface* test_surf = IMG_Load("resources//test.png");
	window.setIcon(test_surf);
	SDL_FreeSurface(test_surf);

	entityManager[entity].getComponent<CCompActor>()->actor.addDefaultTexture(storage.getTexture("test"));
	entityManager[entity].getComponent<CCompActor>()->actor.addTexture(CTaggedTexture({ "hover" }, { "" }, storage.getTexture("test_hover")));
	entityManager[entity].getComponent<CCompActor>()->actor.addTexture(CTaggedTexture({ "select" }, { "" }, storage.getTexture("test_select")));
	entityManager[entity].getComponent<CCompActor>()->actor.addTexture(CTaggedTexture({ "select", "hover" }, { "" }, storage.getTexture("test_select_hover")));

	tile.getComponent<CCompActor>()->actor.addDefaultTexture(storage.getTexture("hex"));
	tile.getComponent<CCompActor>()->actor.addTexture(CTaggedTexture({ "hover" }, { "" }, storage.getTexture("hex_hover")));
	tile.getComponent<CCompActor>()->actor.addTexture(CTaggedTexture({ "reach" }, { "" }, storage.getTexture("hex_reach")));
	tile.getComponent<CCompActor>()->actor.addTexture(CTaggedTexture({ "reach", "hover" }, { "" }, storage.getTexture("hex_reach_hover")));
	tile.getComponent<CCompActor>()->actor.addTexture(CTaggedTexture(storage.getTexture("terrain_mountain")));

	tile.getComponent<CCompTile>()->cost = 1;

	//
	//	From 11 seconds to 2, pretty good!
	// ~But still slow, check the CComponent::Create
	//

	/*std::cout << "Create test map" << std::endl;
	CEntity* testmap = createtestmap(&compManager, &storage);
	std::cout << "test map created" << std::endl;*/

	//CEntity* testmap = 
		
	std::cout << "Create map..." << std::endl;
	unsigned int createMapTime = SDL_GetTicks();
	createtestmap(map, &entityManager, &storage);
	createMapTime = SDL_GetTicks() - createMapTime;
	std::cout << "Map created in " << estd::toString(createMapTime) << " miliseconds" << std::endl;

	CCompInput::create(map, &entityManager);
	CCompCameraControlled::create(map, &entityManager);
	CCompContainer::create(map, &entityManager);

	entityManager[map].getComponent<CCompCameraControlled>()->camera = camera;
	entityManager[entity].getComponent<CCompMapped>()->setMap(map);

	EntityID entity2 = entityManager.create(entity);

	unsigned int pointtile = entityManager[map].getComponent<CCompMap>()->getTile(100, 100);
	Point_d entity2pos = entityManager[map].getComponent<CCompMap>()->getTilePos(pointtile);

	entityManager[entity2].setName("Entity player 2");
	entityManager[entity2].getComponent<CCompPosition>()->setPos(entity2pos.x, entity2pos.y);
	entityManager[entity2].getComponent<CCompPlayerControlled>()->setPlayer(player2);


	/*
	 *		Create MENU!
	 */
	std::cout << "Maak die menu jahh!" << std::endl;

	CCompPosition::create(menu, &entityManager);
	CCompActor::create(menu, &entityManager);
	CCompCollision::create(menu, &entityManager);
	CCompInput::create(menu, &entityManager);
	CCompMovement::create(menu, &entityManager);
	CCompContainer::create(menu, &entityManager);

	entityManager[menu].getComponent<CCompPosition>()->setPos(750, -90);
	entityManager[menu].getComponent<CCompActor>()->actor.addDefaultTexture(storage.getTexture("menu"));
	entityManager[menu].getComponent<CCompActor>()->actor.addTexture(CTaggedTexture({ "hover" }, {}, storage.getTexture("menu")));
	entityManager[menu].getComponent<CCompCollision>()->setShape(new CRectangle(100, 200));
	entityManager[menu].getComponent<CCompInput>()->listenerHoverOn = moveMenu;
	entityManager[menu].getComponent<CCompInput>()->listenerHoverOff = moveMenuBack;

	entityManager[menu].getComponent<CCompMovement>()->speedX = 1000;
	entityManager[menu].getComponent<CCompMovement>()->speedY = 1000;

	CCompPosition::create(button, &entityManager);
	CCompActor::create(button, &entityManager);
	CCompCollision::create(button, &entityManager);
	CCompInput::create(button, &entityManager);
	CCompMovement::create(button, &entityManager);
	CCompContained::create(button, &entityManager);

	entityManager[button].getComponent<CCompActor>()->actor.addDefaultTexture(storage.getTexture("button"));
	entityManager[button].getComponent<CCompActor>()->actor.addTexture(CTaggedTexture({ "hover" }, {}, storage.getTexture("button")));
	entityManager[button].getComponent<CCompActor>()->actor.addTexture(CTaggedTexture({}, {}, storage.getTexture("buttontext")));
	entityManager[button].getComponent<CCompCollision>()->setShape(new CRectangle(96, 30));
	entityManager[button].getComponent<CCompInput>()->listenerAction1 = closeButton;
	entityManager[button].getComponent<CCompPosition>()->setPos(0, 80);

	//button.getComponent<CCompContained>()->parent = &menu;
	//menu.getComponent<CCompContainer>()->entityList.push_back(&button);

	//EntityID pointer2 = entityManager.create(pointer);
	EntityID camera2 = entityManager.create(camera);
	//EntityID menu2 = entityManager.create(menu);

	entityManager[menu].getComponent<CCompContainer>()->entityList.push_back(button);
	entityManager[button].getComponent<CCompContained>()->parent = menu;

	entityManager[map].getComponent<CCompCameraControlled>()->camera = camera2;

	/*
	menu2->getComponent<CCompContainer>()->entityList.clear();
	menu2->getComponent<CCompContainer>()->entityList.push_back(button2);
	button2->getComponent<CCompContained>()->parent = menu2;*/

	/*
	*		I NEED THIS
	*/

	renderer.setDrawBlendMode(SDL_BLENDMODE_BLEND);

	/*
	 *		FPS Counter!
	 */
	
	//std::cin.get();
	std::cout << "Starting the game loop..." << std::endl;
	int x = 0;
	int begin = SDL_GetTicks();
	int begin_update = SDL_GetTicks();
	unsigned int interval_update = 50;
	double interval = 1000.0f/interval_update;

	/*
	*		Game Loop!
	*/

	while(_looping) {
		x++;
		if((SDL_GetTicks() - begin) >= 1000) { 
			std::stringstream stream;
			stream << "FPS: " << x;
			window.setTitle(stream.str());
			x = 0;
			begin = SDL_GetTicks();

			std::cout << "Entity: " << entityManager[entity].getComponent<CCompPosition>()->getAbsolutePos().toString() << std::endl;
			std::cout << "Entity2: " << entityManager[entity2].getComponent<CCompPosition>()->getAbsolutePos().toString() << std::endl;

	
		}
		// event handling part of the loop.
		SDLEvent eventHandler;
		while(eventHandler.poll()) {
			if(eventHandler.get()->type >= SDL_USEREVENT) {
				if(eventHandler.get()->type == (Uint32)INTENT::MESSAGE) {
					message.update(eventHandler.get());
				}
				else {					
					input.updateAll(eventHandler.get());
				}
			}
			else {			
				eventing(eventHandler.get());
				eventHandler.toIntent();
			}			
		}


		if(SDL_GetTicks() - begin_update >= interval) {
			begin_update = SDL_GetTicks();
			move.updateAll(interval / 1000.f);

			move.update(entity, interval / 1000.f);
			move.update(entity2, interval / 1000.f);
		}

		// Render part of the loop.
		renderer.setDrawColor(255, 255, 255, 255);
		renderer.clear();
		
		maprender.update(map);
		render.updateAll();

		renderer.present();
		
	}
	
	//Quit SDL
	TTF_Quit();
	SDL_Quit();
	
	return 0;    
}