#ifndef _EXPLOSION_OBJECT_H_
#define _EXPLOSION_OBJECT_H_
#include "BaseObject.h"
#include "CommonFunction.h"

#define EXPLOSION_WIDTH 165
#define EXPLOSION_HEIGHT 165

class ExplosionObject : public BaseObject {
private:
	SDL_Rect clip_[4];
	Mix_Chunk* explosion_sound_;	
public:
	ExplosionObject();
	~ExplosionObject(){}
	ExplosionObject(const string& clip_file_path, const string& sound_file_path);

	static void* boom(void* args);
	void show(SDL_Surface* des);

	void setExplosionRect(BaseObject* obj);

	void setExplosionSound(Mix_Chunk* sound);
	void playSound(const int& channel = -1, const int& loops = 0);

	void explode(SDL_Surface* screen);
	Mix_Chunk* getExplosionSound() {
		return explosion_sound_;
	}
};

//this class contains arguments for static boom method in class ExplosionObject
class ExplosionSupporter {
private:
	ExplosionObject* explosionObject_;
	SDL_Surface* screen_;
	SDL_Rect clip_[4];
public:
	ExplosionSupporter() {
		explosionObject_ = NULL;
		screen_ = NULL;
		for (int i = 0; i < 4; i++) {
			clip_[i] = SDL_Rect();
		}
	}
	ExplosionSupporter(ExplosionObject* expObj, SDL_Surface* screen, const SDL_Rect clip[4]) {
		explosionObject_ = expObj;
		screen_ = screen;
		for (int i = 0; i < 4; i++) {
			clip_[i] = clip[i];
		}
	}

	ExplosionObject* getExplosionObject() {
		return explosionObject_;
	}
	SDL_Surface* getScreen() {
		return screen_;
	}
	SDL_Rect* getClip() {
		return clip_;
	}
};
#endif // !_EXPLOSION_OBJECT_H_
