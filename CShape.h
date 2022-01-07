#ifndef _CSHAPE_H_
#define _CSHAPE_H_

#include "CPoint.h"
#include "SDLRenderer.h"

class CRectangle;

class CShape {
public:
	CShape();
	CShape(Point_d origin);
	CShape(double x, double y);
	virtual ~CShape();

	virtual CShape* clone() = 0;
	virtual CShape* create() = 0;

	void setOrigin(Point_d origin);
	Point_d getOrigin();

	virtual CRectangle getAABB() = 0;

	virtual bool inside(Point_d point) = 0;
	//virtual bool collides(CShape* shape) = 0;

	virtual void draw(SDLRenderer*, bool ignore_origin = false) = 0;
	virtual void drawFilled(SDLRenderer*, bool ignore_origin = false) = 0;
	SDL_Texture* toTexture(SDLRenderer*);
	SDL_Texture* toTextureFilled(SDLRenderer*);

protected:
	Point_d origin;
};

#endif