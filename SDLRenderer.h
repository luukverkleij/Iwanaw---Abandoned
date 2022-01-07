#ifndef _SDLRENDERER_H_
#define _SDLRENDERER_H_

#include "SDLWindow.h"

class SDLRenderer { 
private:
	SDL_Renderer *renderer;
public:
	SDLRenderer(SDLWindow* window, int index, Uint32 flags);
	SDLRenderer(SDL_Window* window, int index, Uint32 flags);
	SDLRenderer(SDL_Surface* surface);
	~SDLRenderer();

	bool isCreated();
	bool isTargetSupported();

	void clear();
	void present();

	void setTarget(SDL_Texture* texture);
	void setClipRect(SDL_Rect* rect);
	void setLogicalSize(int w, int h);
	void setScale(float scaleX, float scaleY);
	void setViewport(const SDL_Rect* rect);

	int			getPixels(SDL_Rect* rect, Uint32 format, void* pixels, int pitch);
	SDL_Rect	getClipRect();
	void		getLogicalSize(int* w, int* h);
	void		getScale(float* scaleX, float* scaleY);
	SDL_Rect	getViewport();

	void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	void setDrawBlendMode(SDL_BlendMode mode);

	void drawTexture(SDL_Texture* texture, SDL_Rect* srcrect, SDL_Rect* dstrect);
	void drawTextureEx(SDL_Texture* texture, SDL_Rect* srcrect, SDL_Rect* dstrect, 
		const double angle, SDL_Point* center, SDL_RendererFlip flip);

	void drawPoint(int x, int y);
	void drawPoint(SDL_Point point);
	void drawPoints(const SDL_Point* points, int count);

	void drawLine(int x1, int y1, int x2, int y2, bool aa);
	void drawLine(SDL_Point points[2], bool aa);
	void drawLines(const SDL_Point* points, int count);

	void drawRect(const SDL_Rect* rect);
	void drawRects(const SDL_Rect* rect, int count);
	void drawFillRect(const SDL_Rect* rect);
	void drawFillRects(const SDL_Rect* rect, int count);

	void drawTriangle(SDL_Point points[3], bool aa);
	void drawFillTriangle(SDL_Point points[3]);
	
	void drawHexagon(SDL_Point origin, Uint32 radius, bool aa);
	void drawFillHexagon(SDL_Point origin, Uint32 radius);

	void drawPolygon(SDL_Point* points, int count, bool aa);
	void drawFillPolygon(SDL_Point* points, int count);

	void drawEllipse(SDL_Point origin, Uint16 radiusX, Uint16 radiusY, bool aa);
	void drawFillEllipse(SDL_Point origin, Uint16 radiusX, Uint16 radiusY);

	SDL_Renderer* get();
};

#endif //_SDLRENDERER_H_