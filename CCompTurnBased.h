#ifndef _CCOMPTURNBASED_H_
#define _CCOMPTURNBASED_H_

#include "CComponent.h"

class CCompTurnBased : public CComponent <CCompTurnBased> {
public:
	CCompTurnBased() {
		this->used = false;
	}

	bool used;

private:

};

#endif //_CCOMPTURNBASED_H_