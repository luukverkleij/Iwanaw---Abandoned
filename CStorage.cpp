#include "CStorage.h"


CStorage::CStorage(SDLRenderer* r) {
	this->renderer = r;
}

CStorage::~CStorage() {

}

SDLRenderer* CStorage::getRenderer() {
	return this->renderer;
}

void CStorage::loadTexture(std::string key, std::string filename) {
	SDL_Surface* tmp = IMG_Load(filename.c_str());
	this->textures[key] = SDL_CreateTextureFromSurface(renderer->get(), tmp);
	SDL_FreeSurface(tmp);
}

void CStorage::saveTexture(std::string key, SDL_Texture* tex) {
	if(this->textures.count(key)) destroyTexture(key);
	
	this->textures[key] = tex;	
}

void CStorage::destroyTexture(std::string key) {
	SDL_DestroyTexture(this->textures[key]);
}

SDL_Texture* CStorage::getTexture(std::string key) {
	return this->textures.count(key) ? this->textures[key] : nullptr;
}