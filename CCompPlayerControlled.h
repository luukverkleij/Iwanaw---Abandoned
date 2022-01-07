#ifndef _CCOMPPLAYERCONTROLLED_H_
#define _CCOMPPLAYERCONTROLLED_H_

#include "CComponent.h"

class CCompPlayerControlled : public CComponent<CCompPlayerControlled> {
public:

	EntityID getPlayer() {
		return this->player;
	}

	void setPlayer(EntityID player) {
		this->player = player;
		this->throwMessage();
	}

protected:
	EntityID player;
};

#endif //_CCOMPPLAYERCONTROLLED_H_