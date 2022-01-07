#include "CActor.h"

#include "extra_std.h"

//////////////////////////////////////////////
//											//
//			CTaggedTexture					//
//											//
//////////////////////////////////////////////

CTaggedTexture::CTaggedTexture(SDL_Texture* tex) {
	this->tags.resize((unsigned int)TagMode::TOTAL);
	this->texture = tex;
}

CTaggedTexture::CTaggedTexture(std::initializer_list<std::string> tags, TagMode mode, SDL_Texture* tex) {
	this->tags.resize((unsigned int)TagMode::TOTAL);
	this->texture = tex;
	this->tags[(unsigned int)mode].insert(tags.begin(), tags.end());
}


CTaggedTexture::CTaggedTexture(std::initializer_list<std::string> requiredtags, std::initializer_list<std::string> excludetags, SDL_Texture* tex) {
	this->tags.resize((unsigned int)TagMode::TOTAL);
	this->texture = tex;
	this->tags[(unsigned int)TagMode::REQUIRED].insert(requiredtags.begin(), requiredtags.end());
	this->tags[(unsigned int)TagMode::EXCLUDE].insert(excludetags.begin(), excludetags.end());
}

CTaggedTexture::~CTaggedTexture() {

}

SDL_Texture* CTaggedTexture::getTexture() {
	return this->texture;
}

void CTaggedTexture::addTag(const char* tag, TagMode mode) {
	this->tags[(unsigned int)mode].insert(tag);
}

void CTaggedTexture::addTags(std::initializer_list<std::string> tags, TagMode mode) {
	this->tags[(unsigned int)mode].insert(tags.begin(), tags.end());
}

bool CTaggedTexture::hasTag(std::string& tag, TagMode mode) {
	return std::find(this->tags[(unsigned int)mode].begin(), this->tags[(unsigned int)mode].end(), tag) != this->tags[(unsigned int)mode].end();
}

bool CTaggedTexture::hasRequiredTag(std::string &tag) {
	return hasTag(tag, TagMode::REQUIRED);
}

bool CTaggedTexture::hasExcludeTag(std::string &tag) {
	return hasTag(tag, TagMode::EXCLUDE);
}

bool CTaggedTexture::compatible(std::set<std::string> &tags) {
	if (hasExcludeTag(std::string(""))) {
		return this->tags[(unsigned int)TagMode::REQUIRED] == tags;
	}

	for (std::string tag : tags) {
		if (hasExcludeTag(tag)) return false;
	}

	bool result = true;
	for (std::string tag : this->tags[(unsigned int)TagMode::REQUIRED]) {
		result = result && (std::find(tags.begin(), tags.end(), tag) != tags.end());
	}

	return result;	
}

//////////////////////////////////////////////
//											//
//				CActor						//
//											//
//////////////////////////////////////////////

CActor::CActor() {
	this->scalex = 1.0f;
	this->scaley = 1.0f;

	this->tilePos.x = 0;
	this->tilePos.y = 0;
	this->tilePos.w = 0;
	this->tilePos.h = 0;
}

CActor::~CActor() {

}

void CActor::addTexture(CTaggedTexture& tex) {
	this->textures.push_back(tex);
	if(tex.compatible(this->tags)) {
		this->selected.push_back(tex.getTexture());
	}
}

void CActor::addDefaultTexture(SDL_Texture* tex) {
	this->addTexture(CTaggedTexture({ "" }, TagMode::EXCLUDE, tex));
}

void CActor::addTag(const char* tag) {
	this->tags.insert(tag);
	this->update();
}

void CActor::addTags(std::initializer_list<std::string> pTags) {
	this->tags.insert(pTags.begin(), pTags.end());
	this->update();
}

void CActor::removeTag(const char* tag) {
	this->tags.erase(tag);
	this->update();	
}

void CActor::selectPrevious() {
	std::vector<SDL_Texture*> tmp = this->selected;
	this->selected = this->previous;
	this->previous = tmp;
}

std::vector<SDL_Texture*>* CActor::getSelected() {
	return &this->selected;
}

void CActor::setScale(float x, float y) {
	this->scalex = x;
	this->scaley = y;
}

void CActor::setTilePos(SDL_Rect &tilepos) {
	this->tilePos = tilepos;
}

float CActor::getScaleX() {
	return this->scalex;
}

float CActor::getScaleY() {
	return this->scaley; 
}

SDL_Rect CActor::getTilePos() {
	return this->tilePos;
}

void CActor::update() {
	std::vector<SDL_Texture*> tex = findTextures();
	if (tex.empty()) tex = defaultTextures;

	this->previous = this->selected;
	this->selected = tex;
}

std::vector<SDL_Texture*> CActor::findTextures() {	
	std::vector<SDL_Texture*> result;
	for(unsigned int index = 0; index < this->textures.size(); ++index) {
		if(this->textures[index].compatible(tags)) {
			result.push_back(this->textures[index].getTexture());
		}
	}

	return result;
}