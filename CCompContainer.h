#ifndef _CCOMPCONTAINER_H_
#define _CCOMPCONTAINER_H_

#include "CComponent.h"

class CCompContainer : public CComponent<CCompContainer> {
public:
	std::vector<unsigned int> entityList;

private:

};

#endif