#ifndef _CRECTANGLE_H_
#define _CRECTANGLE_H_

#include "CShape.h"
;
class CRectangle : public CShape {
public:
	CRectangle();
	CRectangle(Point_d origin, double width, double height);
	CRectangle(double width, double height);
	~CRectangle(){}

	CShape* clone();
	CShape* create();

	void setTopLeft(Point_d topleft);

	CRectangle getAABB();

	double getWidth();
	double getHeight();

	Point_d getTopLeft();
	Point_d getTopRight();
	Point_d getBottomLeft();
	Point_d getBottomRight();

	bool inside(Point_d point);
	//bool collide(CShape* shape);
	bool collide(CRectangle* rect);

	void draw(SDLRenderer*, bool ignore_origin);
	void drawFilled(SDLRenderer*, bool ignore_origin);

	SDL_Rect toSDL();


private:
	double width, height;
};

#endif