#include "CRectangle.h"

#include "extra_std.h"

CRectangle::CRectangle() {
	this->width = this->height = 0;
}

CRectangle::CRectangle(double width, double height) {
	this->width = width;
	this->height = height;
}

CRectangle::CRectangle(Point_d origin, double width, double height)
	: CShape(origin)
{
	this->width = width;
	this->height = height;
}

CShape* CRectangle::create() {
	return new CRectangle;
}

CShape* CRectangle::clone(){
	return new CRectangle((*this));
}

CRectangle CRectangle::getAABB() {
	return *this;
}

void CRectangle::setTopLeft(Point_d topleft) {
	this->origin = Point_d(topleft.x + width/2, topleft.y + height/2);
}

double CRectangle::getWidth() {
	return width;
}

double CRectangle::getHeight() {
	return this->height;
}

Point_d CRectangle::getTopLeft() {
	return Point_d(this->origin.x - this->width/2, this->origin.y - this->height/2);
}

Point_d CRectangle::getTopRight(){
	return Point_d(this->origin.x + this->width/2, this->origin.y - this->height/2);
}

Point_d CRectangle::getBottomLeft(){
	return Point_d(this->origin.x - this->width/2, this->origin.y + this->height/2);
}

Point_d CRectangle::getBottomRight(){
	return Point_d(this->origin.x + this->width/2, this->origin.y + this->height/2);
}

bool CRectangle::inside(Point_d point) {
	return point.x >= (this->origin.x - this->width/2) &&
		point.y >= (this->origin.y - this->height/2) &&
		point.x <= (this->origin.x + this->width/2) &&
		point.y <= (this->origin.y + this->height/2);
}

void CRectangle::draw(SDLRenderer* r, bool ignore) {
	SDL_Rect rect = {ignore ? 0 : this->origin.x, ignore ? 0 : this->origin.y, width, height};
	r->drawRect(&rect);
	//SDL_RenderDrawRect(r->get(), &rect);
}

void CRectangle::drawFilled(SDLRenderer* r, bool ignore) {
	SDL_Rect rect = {ignore ? 0 : this->origin.x, ignore ? 0 : this->origin.y, width, height};
	r->drawFillRect(&rect);
	//SDL_RenderFillRect(r->get(), &rect);
}
