#ifndef _CSYSMESSAGING_H_
#define _CSYSMESSAGING_H_

#include "CSystem.h"
#include "CComponentManager.h"
#include "CCompInput.h"
#include <sdl.h>

class CSysMessaging : public CSystem {
public:
	CSysMessaging(CStorage* s, CEntityManager* m) : CSystem(s, m) {};

	EInputResult update(CComponentManager*, SDL_Event*);
	EInputResult update(EntityID, SDL_Event*);
	EInputResult update(SDL_Event*);
};

#endif