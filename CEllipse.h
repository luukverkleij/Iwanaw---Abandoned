#ifndef _CELLIPSE_H_
#define _CELLIPSE_H_

#include "CShape.h"

class CEllipse : public CShape {
public:
	CEllipse();
	CEllipse(double radiusX, double radiusY);
	CEllipse(Point_d point, double radiusX, double radiusY);
	~CEllipse(){}

	CShape* create();
	CShape* clone();

	CRectangle getAABB();

	bool inside(Point_d point);
	bool collides(CEllipse* ellipse);

	void draw(SDLRenderer*, bool);
	void drawFilled(SDLRenderer*, bool);

private:
	double radiusX, radiusY;
};

#endif