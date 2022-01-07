#ifndef _CHEXAGON_H_
#define _CHEXAGON_H_

#include "CShape.h"

class CHexagon : public CShape {
public:
	CHexagon();
	CHexagon(double radius);
	CHexagon(Point_d point, double radius);
	~CHexagon();

	CShape* clone();
	CShape* create();

	CRectangle getAABB();

	bool inside(Point_d point);
	bool collision(CHexagon* hex);

	void draw(SDLRenderer*, bool ignore_origin);
	void drawFilled(SDLRenderer*, bool ignore_origin);
private:
	double radius;
};

#endif