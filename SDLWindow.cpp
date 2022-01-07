#include "SDLWindow.h"

//
// CONSTRUCTORS AND DESTRUCTORS
//

SDLWindow::SDLWindow(std::string titlename, int x, int y, int w, int h, Uint32 flags) {
	this->window = SDL_CreateWindow(titlename.c_str(), x, y, w, h, flags);
}

SDLWindow::SDLWindow(SDLWindow* window) {
	this->window = SDL_CreateWindowFrom(window->window);
}

SDLWindow::~SDLWindow() {
	SDL_DestroyWindow(this->window);
}

//
// DEFAULT WINDOW FUNCTIONS
//

void SDLWindow::hide() {
	SDL_HideWindow(this->window);
}

void SDLWindow::show() {
	SDL_ShowWindow(this->window);
}

void SDLWindow::maximize() {
	SDL_MaximizeWindow(this->window);
}

void SDLWindow::minimize() {
	SDL_MinimizeWindow(this->window);
}

void SDLWindow::raise() {
	SDL_RaiseWindow(this->window);
}

void SDLWindow::restore() {
	SDL_RestoreWindow(this->window);
}

//
// GETS
//

void* SDLWindow::getData(std::string name) {
	return SDL_GetWindowData(this->window, name.c_str());
}

Uint32 SDLWindow::getFlags() {
	return SDL_GetWindowFlags(this->window);
}

bool SDLWindow::getGrab() {
	return (bool)SDL_GetWindowGrab(this->window);
}

void SDLWindow::getPosition(int* x, int* y) {
	SDL_GetWindowPosition(this->window, x ,y);
}

void SDLWindow::getSize(int* w, int* h) {
	SDL_GetWindowSize(this->window, w, h);
}

std::string SDLWindow::getTitle() {
	return SDL_GetWindowTitle(this->window);
}

//
// SETS
//

void SDLWindow::setData(std::string name, void* userdata) {
	SDL_SetWindowData(this->window, name.c_str(), userdata);
}

void SDLWindow::setFullscreen(Uint32 flags) {
	SDL_SetWindowFullscreen(this->window, flags);
}

void SDLWindow::setGrab(bool grabbed) {
	SDL_SetWindowGrab(this->window, (SDL_bool)grabbed);
}

void SDLWindow::setIcon(SDL_Surface* icon) {
	SDL_SetWindowIcon(this->window, icon);
}

void SDLWindow::setPosition(int x, int y) {
	SDL_SetWindowPosition(this->window, x, y);
}

void SDLWindow::setSize(int w, int h) {
	SDL_SetWindowSize(this->window, w, h);
}

void SDLWindow::setBordered(bool bordered) {
	SDL_SetWindowBordered(this->window, (SDL_bool)bordered);
}

void SDLWindow::setTitle(std::string title) {
	SDL_SetWindowTitle(this->window, title.c_str());
}

bool SDLWindow::isCreated() {
	return !(this->window == nullptr);
}

//
// MISC.
//
SDL_Window* SDLWindow::get() {
	return this->window;
}
