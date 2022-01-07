#ifndef _CACTOR_H_
#define _CACTOR_H_

#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include <array>

#include <SDL.h>

enum class TagMode {
	REQUIRED,
	EXCLUDE,
	TOTAL
};

class CTaggedTexture {
public:
	CTaggedTexture(SDL_Texture*);
	CTaggedTexture(std::initializer_list<std::string>, TagMode, SDL_Texture*);
	CTaggedTexture(std::initializer_list<std::string>, std::initializer_list<std::string>, SDL_Texture*);
	~CTaggedTexture();

	SDL_Texture* getTexture();

	void addTag(const char*, TagMode);
	void addTags(std::vector<std::string> &, TagMode);
	void addTags(std::initializer_list<std::string>, TagMode);

	bool hasTag(std::string&, TagMode);
	bool hasRequiredTag(std::string&);
	bool hasExcludeTag(std::string&);
	bool compatible(std::set<std::string>&);

private:
	std::vector<std::set<std::string>> tags;
	SDL_Texture* texture;
};

class CActor{
public:
	CActor();
	~CActor();

	void addTag(const char* name);
	void addTags(std::initializer_list<std::string> list);
	void removeTag(const char* name);

	void addDefaultTexture(SDL_Texture*);
	void addTexture(CTaggedTexture &texture);
	void selectPrevious();

	std::vector<SDL_Texture*>* getSelected();

	void setScale(float x, float y);
	void setTilePos(SDL_Rect &tilepos);

	float getScaleX();
	float getScaleY();

	SDL_Rect getTilePos();

private:
	void update();
	std::vector<SDL_Texture*> findTextures();

private:
	std::vector<SDL_Texture*> defaultTextures;
	std::vector<CTaggedTexture> textures;
	std::set<std::string> tags;

	std::vector<SDL_Texture*> selected;
	std::vector<SDL_Texture*> previous;

	float scalex, scaley;
	SDL_Rect tilePos;

};

#endif //_CACTOR_H_