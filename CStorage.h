#ifndef _CSTORAGE_H_
#define _CSTORAGE_H_

#include <string>
#include <map>
#include <vector>
#include <typeindex>
#include <typeinfo>

#include <SDL_image.h>

#include "SDLRenderer.h"
#include "CValue.h"

class CStorage {
public:
	CStorage(SDLRenderer* r);
	~CStorage();

	SDLRenderer* getRenderer();
	void destroyAll();

	void loadTexture(std::string key, std::string filename);
	void saveTexture(std::string key, SDL_Texture* tex);
	void destroyTexture(std::string key);
	SDL_Texture* getTexture(std::string key);

	template <typename T>
	void save(std::string key, T value) {
		if(value[typeinfo(T)].count(key)) destroy(key);

		values[typeinfo(T)][key] = new CValue<T>(value);
	}

	template <typename T>
	void saveList(std::string key, std::vector<T> list) {
		if(value[typeinfo(T)].count(key)) destroyList(key);

		values[typeinfo(T)][key] = new CValueList<T>(value);
	}

	template <typename T>
	void destroy(std::string key) {
		delete value[typeinfo(T)][key]
	}

	template <typename T>
	void destroyList(std::string key) {
		delete value[typeinfo(T)][key]
	}



private:
	SDLRenderer* renderer;
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::type_index, std::map<std::string, CValueBase*>> values;
	std::map<std::type_index, std::map<std::string, CValueBase*>> lists;

};

#endif //_CSTORAGE_H_