#ifndef _CCOMPCONTAINED_H_
#define _CCOMPCONTAINED_H_

#include "CComponent.h"

class CCompContained : public CComponent<CCompContained> {
public:
	unsigned int parent;

private:
};

#endif //_CCOMPCONTAINED_H_