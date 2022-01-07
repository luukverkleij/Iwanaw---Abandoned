#ifndef _CSYSRENDER_H_
#define _CSYSRENDER_H_

#include "CSystem.h"

#include "SDLRenderer.h"

class CSysRender : public CSystem {
public:
	CSysRender(CStorage* store, CEntityManager* manager);
	~CSysRender();

	void updateAll();

	void update(EntityID entity);
	void update(EntityID entity, long offset_x, long offset_y);

	SDLRenderer* getRenderer();

private:
	SDLRenderer* renderer;
};

#endif //_CSYSRENDER_H_