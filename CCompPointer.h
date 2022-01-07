#ifndef _CCOMPPOINTER_H_
#define _CCOMPPOINTER_H_

#include "CComponent.h"
;
class CCompPointer : public CComponent<CCompPointer> {
public:
	std::vector<EntityID> hovers;

private:
};

#endif