#ifndef _CCOMPCAMERACONTROLLED_H_
#define _CCOMPCAMERACONTROLLED_H_

#include "CComponent.h"

class CCompCameraControlled : public CComponent<CCompCameraControlled> {
public:
	EntityID camera;
};

#endif //_CCOMPCAMERACONTROLLED_H_