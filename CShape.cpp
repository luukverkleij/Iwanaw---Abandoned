#include "CShape.h"
#include "CRectangle.h"

#include "SDL2_gfxPrimitives.h"

CShape::CShape() {
}

CShape::CShape(Point_d origin) {
	this->origin = origin;
}

CShape::CShape(double x, double y) {
	this->origin.set(x, y);
}

CShape::~CShape() {
}

void CShape::setOrigin(Point_d origin) {
	this->origin = origin;
}

Point_d CShape::getOrigin() {
	return this->origin;
}

SDL_Texture* CShape::toTexture(SDLRenderer* cr) {
	SDL_Renderer* r = cr->get();
	CRectangle aabb = this->getAABB();

	Uint8 red, green, blue, alpha;
	SDL_GetRenderDrawColor(r, &red, &green, &blue, &alpha);

	SDL_Texture* tex = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, aabb.getWidth() + 1, aabb.getHeight() + 1);
	SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
	
	SDL_SetRenderTarget(r, tex);
	SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);

	SDL_SetRenderDrawColor(r, 255, 255, 255, 0);
	SDL_RenderFillRect(r, nullptr);

	SDL_SetRenderDrawColor(r, red, green, blue, alpha);
	this->draw(cr, true);

	SDL_SetRenderTarget(r, nullptr);
	//SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);

	return tex;
}

SDL_Texture* CShape::toTextureFilled(SDLRenderer* cr) {
	SDL_Renderer* r = cr->get();
	CRectangle aabb = this->getAABB();

	Uint8 red, green, blue, alpha;
	SDL_GetRenderDrawColor(r, &red, &green, &blue, &alpha);

	SDL_Texture* tex = SDL_CreateTexture(r, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, aabb.getWidth() + 1, aabb.getHeight() + 1);
	SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
	
	SDL_SetRenderTarget(r, tex);
	SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_NONE);

	SDL_SetRenderDrawColor(r, 255, 255, 255, 0);
	SDL_RenderFillRect(r, nullptr);

	SDL_SetRenderDrawColor(r, red, green, blue, alpha);
	this->drawFilled(cr, true);

	SDL_SetRenderTarget(r, nullptr);
	//SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);

	return tex;
}