#ifndef _CCOMPPLAYER_H_
#define _CCOMPPLAYER_H_

#include "CComponent.h"
;

class CCompPlayer : public CComponent<CCompPlayer>{
public:
	CCompPlayer() {
		turn = 0;
		id = 0;
		name = "";
		selectedEntity = emptyid;
	}

	unsigned int turn;
	unsigned int id;
	std::string name;
	std::vector<unsigned int> friendly;
	EntityID selectedEntity;
	std::vector<EntityID> controlledEntities;
};

#endif //_CCOMPPLAYER_H_