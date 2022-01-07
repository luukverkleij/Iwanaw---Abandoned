#include "SDLTTF.h"

SDLTTF::SDLTTF(const char* filename, int size) {
	if (!TTF_WasInit()) {
		TTF_Init();
	}

	this->font = TTF_OpenFont(filename, size);
}

SDLTTF::~SDLTTF() {
	TTF_CloseFont(font);
}

SDL_Texture* SDLTTF::createBlended(SDLRenderer& renderer, const char* text) {
	int width, height;
	SDL_Surface* surface;
	SDL_Texture* texture;

	surface = TTF_RenderText_Blended(font, text, SDL_Color{ 0, 0, 0 });
	texture = SDL_CreateTextureFromSurface(renderer.get(), surface);

	SDL_FreeSurface(surface);

	return texture;
}

