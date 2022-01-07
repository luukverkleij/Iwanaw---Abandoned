#ifndef _CPATHFINDING_H_
#define _CPATHFINDING_H_

#include "CSystem.h"

#include "CCompMapped.h"
#include "CCompMap.h"

#include <queue>

class Node {
public:
	Node(int position, double weight) {
		this->position = position;
		this->weight = weight;
	}

	std::vector<unsigned int> path;
	int position;
	double weight;
	bool stackable;

};
inline bool operator==(const Node& l, const Node& r) {return l.position == r.position;}
inline bool operator<(const Node& l, const Node& r) {return l.weight == r.weight? r.path.size() < l.path.size() : l.weight < r.weight;}

class CSysPathFinding : CSystem {
public:
	CSysPathFinding(CStorage* s, CEntityManager* m) : CSystem(s, m) {};

	void update(EntityID entity);
	void getNeightbours(CCompMapped* mapped, EntityID);
	void insertNode(CCompMap* map, EntityID, Point_i32& position);

	bool inVisited(Node& node);
	void updateUnvisited(Node& node);

	bool canAttackNeighbour(CCompMapped*, EntityID, int);
	bool hasEnemy(CCompMapped*, EntityID, Point_i32&);
private:

	std::priority_queue<Node> unvisited;
	std::vector<Node> visited;
	std::vector<Node> visitedAttackable;
};

#endif //_CPATHFINDING_H_