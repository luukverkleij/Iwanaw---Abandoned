#include "CEllipse.h"

#include "CRectangle.h"

#include <cmath>

CEllipse::CEllipse() {
	this->radiusX = this->radiusY = 0;
}

CEllipse::CEllipse(double radiusX, double radiusY) {
	this->radiusX = radiusX;
	this->radiusY = radiusY;
}

CEllipse::CEllipse(Point_d origin, double radiusX, double radiusY)
	: CShape(origin) {
		this->radiusX = radiusX;
		this->radiusY = radiusY;
}

CShape* CEllipse::create() {
	return new CEllipse;
}

CShape* CEllipse::clone() {
	return new CEllipse((*this));
}

CRectangle CEllipse::getAABB() {
	return CRectangle(origin, radiusX*2.f, radiusY*2.f);
}

bool CEllipse::inside(Point_d point) {
	return pow(point.x - origin.x, 2) / pow(this->radiusX, 2)
		+ pow(point.y - origin.y, 2) / pow(this->radiusY, 2) <= 1;
}

void CEllipse::draw(SDLRenderer* r, bool ignore) {
	SDL_Point pos = ignore ? SDL_Point{ 0, 0 } : SDL_Point{ this->origin.x, this->origin.y };
	r->drawEllipse(pos, this->radiusX, this->radiusY, true);
}

void CEllipse::drawFilled(SDLRenderer* r, bool ignore) {
	SDL_Point pos = ignore ? SDL_Point{ 0, 0 } : SDL_Point{ this->origin.x, this->origin.y };
	r->drawFillEllipse(pos, this->radiusX, this->radiusY);
}