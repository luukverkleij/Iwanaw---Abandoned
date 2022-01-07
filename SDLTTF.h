#ifndef _SDLTTF_H_
#define _SDLTTF_H_

#include "SDLRenderer.h"

#include <sdl_ttf.h>

class SDLTTF {
public:
	SDLTTF(const char* filename, int size);
	~SDLTTF();

	SDL_Texture* createSolid(SDLRenderer& renderer, const char* text);
	SDL_Texture* createShaded(SDLRenderer& renderer, const char* text);
	SDL_Texture* createBlended(SDLRenderer& renderer, const char* text);

private:
	TTF_Font* font;

private:
};

#endif