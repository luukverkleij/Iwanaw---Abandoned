#ifndef _SDLWINDOW_H_
#define _SDLWINDOW_H_

#include <string>

#include <SDL.h>

class SDLWindow {
private:
	SDL_Window* window;

public:
	SDLWindow(std::string titlename, int x, int y, int w, int h, Uint32 flags);
	SDLWindow(SDLWindow* window);
	~SDLWindow();

	bool isCreated();

	void hide();
	void show();
	void maximize();
	void minimize();
	void raise();
	void restore();

	void*		getData(std::string name);
	Uint32		getFlags();
	bool		getGrab();
	void		getPosition(int* x, int* y);
	void		getSize(int* w, int* h);
	std::string getTitle();

	void setData(std::string name, void* userdata);
	void setFullscreen(Uint32 flags);
	void setGrab(bool grabbed);
	void setIcon(SDL_Surface* icon);
	void setPosition(int x, int y);
	void setSize(int w, int h);
	void setBordered(bool bordered);
	void setTitle(std::string title);

	SDL_Window* get();
};

#endif //_SDLWINDOW_H_