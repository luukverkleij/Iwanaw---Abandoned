#ifndef _CCOMPHOVER_H_
#define _CCOMPHOVER_H_

#include "CComponent.h"

class CCompHover : public CComponent<CCompHover> {
public:
	CCompHover() {
		this->hover = false;
	}

	~CCompHover() {
	}

	bool hover;
};

#endif