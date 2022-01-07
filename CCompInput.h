#ifndef _CCOMPINPUT_H_
#define _CCOMPINPUT_H_

#include "CComponent.h"

enum EInputResult {
	NONE = 0x00,
	CONSUMED = 0x01,
	DESTROYED = 0x02,
};

class CCompInput : public CComponent<CCompInput> {
public:
	std::function <EInputResult(CEntityManager*, EntityID, EntityID)> listenerAction1;
	std::function <EInputResult(CEntityManager*, EntityID, EntityID)> listenerAction2;
	std::function <EInputResult(CEntityManager*, EntityID, EntityID)> listenerHoverOn;
	std::function <EInputResult(CEntityManager*, EntityID, EntityID)> listenerHoverOff;
};

#endif //_CCOMPINPUT_H_