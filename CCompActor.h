#ifndef _CCOMPACTOR_H_
#define _CCOMPACTOR_H_

#include "CComponent.h"
#include "CActor.h"

class CCompActor : public CComponent<CCompActor> {
public:
	CActor actor;
};

#endif //_CCOMPACTOR_H_