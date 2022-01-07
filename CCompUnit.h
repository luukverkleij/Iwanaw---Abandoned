#ifndef _CCOMPUNIT_H_
#define _CCOMPUNIT_H_

#include "CComponent.h"

class CCompUnit : public CComponent<CCompUnit> {
public:
	unsigned int type;
	unsigned int fuel;
	unsigned int health;

	unsigned int selectedTile;
	std::vector<unsigned int> attackable;

private:

};

#endif //_CCOMPUNIT_H_