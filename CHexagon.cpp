#include "CHexagon.h"
#include "CRectangle.h"

#include "SDL2_gfxPrimitives.h"

#include <cmath>

CHexagon::CHexagon() {
	this->radius = 0;
}

CHexagon::CHexagon(double radius) {
	this->radius = radius;
}

CHexagon::CHexagon(Point_d origin, double radius) 
	: CShape(origin) {
	this->radius = radius;
}

CHexagon::~CHexagon() {

}

CShape* CHexagon::create() {
	return new CHexagon;
}

CShape* CHexagon::clone() {
	return new CHexagon((*this));
}

CRectangle CHexagon::getAABB() {
	return CRectangle(origin, this->radius*sqrt(3), this->radius*2);  
}

bool CHexagon::inside(Point_d point) {
	int32_t diffX = abs(point.x - origin.x);         
    int32_t diffY = abs(point.y - origin.y);

	int32_t height = (float)this->radius/2 * sqrt(3.f);

    
	if (diffX > height || diffY > this->radius) return false;

    return this->radius*height - this->radius/2*diffX - height*diffY >= 0;   
}

bool CHexagon::collision(CHexagon* hex) {
	return false;
}

void CHexagon::draw(SDLRenderer* renderer, bool ignore) {
	SDL_Point points[7];

	for(Uint8 i = 0; i < 6; ++i) {
		float angle = 2.f * M_PI / 6.f * ((float)i + .5f);
		points[i].x = (float)radius * cos(angle) + (ignore ? this->radius*sqrt(3)/2 : origin.x);
		points[i].y = (float)radius * sin(angle) + (ignore ? this->radius : origin.y);
	}
	points[6] = points[0];

	renderer->drawPolygon(points, 7, true);
}

void CHexagon::drawFilled(SDLRenderer* renderer, bool ignore) {
	SDL_Point points[7];

	for(Uint8 i = 0; i < 6; ++i) {
		float angle = 2.f * M_PI / 6.f * ((float)i + .5f);
		points[i].x = (float)radius * cos(angle) + (ignore ? this->radius*sqrt(3)/2 : origin.x);
		points[i].y = (float)radius * sin(angle) + (ignore ? this->radius : origin.y);
	}
	points[6] = points[0];

	renderer->drawFillPolygon(points, 7);
}