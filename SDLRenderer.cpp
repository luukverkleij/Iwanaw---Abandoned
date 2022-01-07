#include "SDLRenderer.h"

#include "SDL2_gfxPrimitives.h"

SDLRenderer::SDLRenderer(SDLWindow* window, int index, Uint32 flags) {
	this->renderer = SDL_CreateRenderer(window->get(), index, flags);
}

SDLRenderer::SDLRenderer(SDL_Window* window, int index, Uint32 flags) {
	this->renderer = SDL_CreateRenderer(window, index, flags);
}

SDLRenderer::SDLRenderer(SDL_Surface* surface) {
	this->renderer = SDL_CreateSoftwareRenderer(surface);
}

SDLRenderer::~SDLRenderer() {
	SDL_DestroyRenderer(this->renderer);
}

//
// Zero check.
//

bool SDLRenderer::isCreated() {
	return this->renderer != nullptr;
}

//
// boolean functions (isX)
//
bool SDLRenderer::isTargetSupported() {
	return (bool)SDL_RenderTargetSupported(this->renderer);
}

//
//	Renderer functions.
//

void SDLRenderer::clear() {
	SDL_RenderClear(this->renderer);
}

void SDLRenderer::present() {
	SDL_RenderPresent(this->renderer);
}

//
// Set Functions.
//
void SDLRenderer::setTarget(SDL_Texture* texture) {
	SDL_SetRenderTarget(this->renderer, texture);
}

void SDLRenderer::setClipRect(SDL_Rect* rect) {
	SDL_RenderSetClipRect(this->renderer, rect);
}

void SDLRenderer::setLogicalSize(int w, int h) {
	SDL_RenderSetLogicalSize(this->renderer, w, h);
}

void SDLRenderer::setScale(float scaleX, float scaleY) {
	SDL_RenderSetScale(this->renderer, scaleX, scaleY);
}

void SDLRenderer::setViewport(const SDL_Rect* rect) {
	SDL_RenderSetViewport(this->renderer, rect);
}

//
// Get Functions.
//
int	SDLRenderer::getPixels(SDL_Rect* rect, Uint32 format, void* pixels, int pitch) {
	return SDL_RenderReadPixels(this->renderer, rect, format, pixels, pitch);
}

SDL_Rect SDLRenderer::getClipRect() {
	SDL_Rect rect;
	SDL_RenderGetClipRect(this->renderer, &rect);
	return rect;
}

void SDLRenderer::getLogicalSize(int* w, int* h) {
	SDL_RenderGetLogicalSize(this->renderer, w, h);
}

void SDLRenderer::getScale(float* scaleX, float* scaleY) {
	SDL_RenderGetScale(this->renderer, scaleX, scaleY);
}

SDL_Rect SDLRenderer::getViewport() {
	SDL_Rect rect;
	SDL_RenderGetViewport(this->renderer, &rect);
	return rect;
}

//
// Set functions for draw functions.
//
void SDLRenderer::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	SDL_SetRenderDrawColor(this->renderer, r, g, b, a);
}

void SDLRenderer::setDrawBlendMode(SDL_BlendMode mode) {
	SDL_SetRenderDrawBlendMode(this->renderer, mode);
}

//
// Draw Functions.
//

void SDLRenderer::drawTexture(SDL_Texture* texture, SDL_Rect* srcrect, SDL_Rect* dstrect) {
	SDL_RenderCopy(this->renderer, texture, srcrect, dstrect);
}

void SDLRenderer::drawTextureEx(SDL_Texture* texture, SDL_Rect* srcrect, SDL_Rect* dstrect, 
		const double angle, SDL_Point* center, SDL_RendererFlip flip) {
	SDL_RenderCopyEx(this->renderer, texture, srcrect, dstrect, angle, center, flip);
}

void SDLRenderer::drawLine(SDL_Point points[2], bool aa) {
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(this->renderer, &r, &g, &b, &a);

	if(aa) aalineRGBA(this->renderer, points[0].x, points[0].y, points[1].x, points[1].y, r, g, b, a);
	else lineRGBA(this->renderer, points[0].x, points[0].y, points[1].x, points[1].y, r, g, b, a);
}

void SDLRenderer::drawLine(int x1, int y1, int x2, int y2, bool aa) {
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(this->renderer, &r, &g, &b, &a);

	if(aa) aalineRGBA(this->renderer, x1, y1, x2, y2, r, g, b, a);
	else lineRGBA(this->renderer, x1, y1, x2, y2, r, g, b, a);
}

void SDLRenderer::drawLines(const SDL_Point* points, int count) {
	SDL_RenderDrawLines(this->renderer, points, count);
}

void SDLRenderer::drawPoint(int x, int y) {
	SDL_RenderDrawPoint(this->renderer, x, y);
}

void SDLRenderer::drawPoints(const SDL_Point* points, int count) {
	SDL_RenderDrawPoints(this->renderer, points, count);
}

void SDLRenderer::drawRect(const SDL_Rect* rect) {
	SDL_RenderDrawRect(this->renderer, rect);
}

void SDLRenderer::drawRects(const SDL_Rect* rect, int count) {
	SDL_RenderDrawRects(this->renderer, rect, count);
}

void SDLRenderer::drawFillRect(const SDL_Rect* rect) {		
	SDL_RenderFillRect(this->renderer, rect);
}

void SDLRenderer::drawFillRects(const SDL_Rect* rect, int count) {
	SDL_RenderFillRects(this->renderer, rect, count);
}

void SDLRenderer::drawTriangle(SDL_Point p[3], bool aa) {
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(this->renderer, &r, &g, &b, &a);

	if(aa)	aatrigonRGBA(this->renderer, p[0].x, p[0].y, p[1].x, p[1].y, p[2].x, p[2].y, r, g, b, a);
	else	trigonRGBA(this->renderer, p[0].x, p[0].y, p[1].x, p[1].y, p[2].x, p[2].y, r, g, b, a);
}

void SDLRenderer::drawFillTriangle(SDL_Point p[3]) {
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(this->renderer, &r, &g, &b, &a);

	filledTrigonRGBA(this->renderer, p[0].x, p[0].y, p[1].x, p[1].y, p[2].x, p[2].y, r, g, b, a);
}

void SDLRenderer::drawHexagon(const SDL_Point origin, Uint32 radius, bool aa) {
	Sint16 x[7];
	Sint16 y[7];

	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(this->renderer, &r, &g, &b, &a);

	for(Uint8 i = 0; i < 6; ++i) {
		float angle = 2.f * M_PI / 6.f * ((float)i + .5f);
		x[i] = origin.x + (float)radius * cos(angle);
		y[i] = origin.y + (float)radius * sin(angle);
	}
	x[6] = x[0];
	y[6] = y[0];

	if(aa)
		aapolygonRGBA(this->renderer, x, y, 7, r, g, b, a);
	else
		polygonRGBA(this->renderer, x, y, 7, r, g, b, a);
}

void SDLRenderer::drawFillHexagon(const SDL_Point origin, Uint32 radius) {
	Sint16 x[7];
	Sint16 y[7];

	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(this->renderer, &r, &g, &b, &a);

	for(Uint8 i = 0; i < 6; ++i) {
		float angle = 2.f * M_PI / 6.f * ((float)i + .5f);
		x[i] = origin.x + (float)radius * cos(angle);
		y[i] = origin.y + (float)radius * sin(angle);
	}
	x[6] = x[0];
	y[6] = y[0];

	filledPolygonRGBA(this->renderer, x, y, 7, r, g, b, a);
}

void SDLRenderer::drawPolygon(SDL_Point* points, int count, bool aa) {
	Sint16 *x = new Sint16[count];
	Sint16 *y = new Sint16[count];

	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(this->renderer, &r, &g, &b, &a);

	for(unsigned int i = 0; i < count; ++i) {
		x[i] = points[i].x;
		y[i] = points[i].y;
	}

	if(aa)
		aapolygonRGBA(this->renderer, x, y, count, r, g, b, a);
	else
		aapolygonRGBA(this->renderer, x, y, count, r, g, b, a);
}

void SDLRenderer::drawFillPolygon(SDL_Point* points, int count) {
	Sint16 *x = new Sint16[count];
	Sint16 *y = new Sint16[count];

	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(this->renderer, &r, &g, &b, &a);

	for(unsigned int i = 0; i < count; ++i) {
		x[i] = points[i].x;
		y[i] = points[i].y;
	}

	filledPolygonRGBA(this->renderer, x, y, count, r, g, b, a);
}

void SDLRenderer::drawEllipse(SDL_Point origin, Uint16 radiusX, Uint16 radiusY, bool aa) {
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(this->renderer, &r, &g, &b, &a);

	aaellipseRGBA(this->renderer, origin.x + radiusX, origin.y + radiusY, radiusX, radiusY, r, g, b, a);
}

void SDLRenderer::drawFillEllipse(SDL_Point origin, Uint16 radiusX, Uint16 radiusY) {
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(this->renderer, &r, &g, &b, &a);

	filledEllipseRGBA(this->renderer, origin.x + radiusX, origin.y + radiusY, radiusX, radiusY, r, g, b, a);
}

//
// Return the SDL_Renderer struct.
//



SDL_Renderer* SDLRenderer::get() {
	return this->renderer;
}