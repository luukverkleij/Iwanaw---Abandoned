#include "CSysPathFinding.h"

#include "CCompMapped.h"
#include "CCompMap.h"
#include "CCompTile.h"
#include "CCompActor.h"
#include "CCompUnit.h"

#include <iostream>

/**
 * @todo Remake this shiet
 */


/**
 * \brief Dijkstra algoritm for possible paths.
 * \todo To not make it ugly
 */
void CSysPathFinding::update(EntityID entity) {
	CCompMapped*	mapped		= manager->get(entity).getComponent<CCompMapped>();
	CCompActor*		act			= manager->get(entity).getComponent<CCompActor>();
	CCompUnit*		unit		= manager->get(entity).getComponent<CCompUnit>();

	CCompMap*		map			= manager->get(mapped->getMap()).getComponent<CCompMap>();
	
	
	unvisited.push(Node(mapped->getTilePos(), mapped->getTileSpeed()));

	if(mapped->getTileSpeed() > 0) {
		while(!unvisited.empty()) {
			if(!inVisited(unvisited.top())) {
				getNeightbours(mapped, entity);
				visited.push_back(unvisited.top());
			}
			unvisited.pop();
		}
	}

	for (unsigned int index = 0; index < visitedAttackable.size(); ++index) {
		EntityID tileID = map->tiles[visitedAttackable[index].position];
		CCompActor* mapActor = manager->get(tileID).getComponent<CCompActor>();
		mapActor->actor.addTag("attack");
	}

	std::vector<std::vector<unsigned int>> paths;
	std::vector<unsigned int> attacks;
	paths.reserve(visited.size());

	for(unsigned int index = 0; !visited.empty(); ++index) {
		if (visited.back().stackable) {
			paths.push_back(visited.back().path);
			paths.back().push_back(visited.back().position);

			EntityID tileID = map->tiles[visited.back().position];
			CCompActor* mapActor = manager->get(tileID).getComponent<CCompActor>();

			if (canAttackNeighbour(mapped, entity, visited.back().position)) {
				attacks.push_back(visited.back().position);
				mapActor->actor.addTag("attack");
			}
			else {
				mapActor->actor.addTag("reach");
			}

		}

		visited.pop_back();
	}

	Paths pathsObject(paths);
	pathsObject.setAttacks(attacks);
	mapped->setPaths(pathsObject);
}

/**
 * \brief These things are way easier in Haskell
 */
void CSysPathFinding::getNeightbours(CCompMapped* mapped, EntityID entity) {
	CCompMap* map = manager->get(mapped->getMap()).getComponent<CCompMap>();
	Node node = unvisited.top();

	Point_i32 point(node.position%map->width, node.position/map->width);

	int align = point.y%2;

	Point_i32 point1(point.x + align - 1,	point.y - 1);
	Point_i32 point2(point.x + align,		point.y - 1);

	Point_i32 point3(point.x - 1,			point.y);
	Point_i32 point4(point.x + 1,			point.y);

	Point_i32 point5(point.x + align - 1,	point.y + 1);
	Point_i32 point6(point.x + align,		point.y + 1);

	insertNode(map, entity, point1);
	insertNode(map, entity, point2);
	insertNode(map, entity, point3);
	insertNode(map, entity, point4);
	insertNode(map, entity, point5);
	insertNode(map, entity, point6);
}

bool CSysPathFinding::canAttackNeighbour(CCompMapped* mapped, EntityID entity, int pos) {
	CCompMap* map = manager->get(mapped->getMap()).getComponent<CCompMap>();

	Point_i32 point(pos%map->width, pos / map->width);

	int align = point.y % 2;

	Point_i32 point1(point.x + align - 1, point.y - 1);
	Point_i32 point2(point.x + align, point.y - 1);

	Point_i32 point3(point.x - 1, point.y);
	Point_i32 point4(point.x + 1, point.y);

	Point_i32 point5(point.x + align - 1, point.y + 1);
	Point_i32 point6(point.x + align, point.y + 1);

	return hasEnemy(mapped, entity, point1) || hasEnemy(mapped, entity, point2)
		|| hasEnemy(mapped, entity, point3) || hasEnemy(mapped, entity, point4)
		|| hasEnemy(mapped, entity, point5) || hasEnemy(mapped, entity, point6);
}

bool CSysPathFinding::hasEnemy(CCompMapped* mapped, EntityID entity, Point_i32& position) {
	CCompMap* map = manager->get(mapped->getMap()).getComponent<CCompMap>();
	int tile = position.x + position.y * map->width;
	return position.x >= 0 && position.x < map->width &&
		position.y >= 0 && position.y < map->height && map->canAttack(manager, entity, tile);
}

/**
 * \brief If the node is in the map and it inserts the node into unvisited.
 */
void CSysPathFinding::insertNode(CCompMap* map, EntityID entity, Point_i32& position) {

	if(position.x >= 0 && position.x < map->width &&
		position.y >= 0 && position.y < map->height) {
			int tile = position.x + position.y * map->width;
			EntityID tileID = map->tiles[tile];
			double weight = unvisited.top().weight - manager->get(tileID).getComponent<CCompTile>()->cost;

			Node node(tile, weight);
			node.path.insert(node.path.begin(), unvisited.top().path.begin(), unvisited.top().path.end());
			node.path.push_back(unvisited.top().position);
			
			node.stackable = map->canStack(manager, entity, tile);

			if (map->canWalk(manager, entity, tile) && weight >= 0) unvisited.push(node);
	}
}

/**
 * \brief Returns true if the node is in visiteds, else returns false.
 */
bool CSysPathFinding::inVisited(Node& node) {
	return std::find(visited.begin(), visited.end(), node) != visited.end();
}
