#include "ExplosionObject.h"

ExplosionObject::ExplosionObject() : BaseObject() {
	rect_.w = EXPLOSION_WIDTH;
	rect_.h = EXPLOSION_HEIGHT;
	for (int i = 0; i < 4; i++) {
		clip_[i].x = i * EXPLOSION_WIDTH;
		clip_[i].y = 0;
		clip_[i].w = EXPLOSION_WIDTH;
		clip_[i].h = EXPLOSION_HEIGHT;
	}
	explosion_sound_ = NULL;
}

ExplosionObject::ExplosionObject(const string& clip_file_path, const string& sound_file_path) {
	rect_.w = EXPLOSION_WIDTH;
	rect_.h = EXPLOSION_HEIGHT;
	for (int i = 0; i < 4; i++) {
		clip_[i].x = i * rect_.w;
		clip_[i].y = 0;
		clip_[i].w = rect_.w;
		clip_[i].h = rect_.h;
	}
	ptr_object_ = CommonFunction::loadImage(clip_file_path, 0, 0xff, 0xff);
	explosion_sound_ = CommonFunction::loadSound(sound_file_path);
}

void* ExplosionObject::boom(void* args) {
	ExplosionSupporter* temp = (ExplosionSupporter*)args;
	SDL_Surface* ptr_object = temp->getExplosionObject()->getSurface();
	SDL_Rect rect = temp->getExplosionObject()->getRect();
	SDL_Surface* des = temp->getScreen();
	SDL_Rect* clip = temp->getClip();

	for (int i = 0; i < 4; i++) {
		CommonFunction::applySurface(ptr_object, des, &clip[i], rect.x, rect.y);
		SDL_Flip(des);
		SDL_Delay(10);
	}
	return NULL;
}

void ExplosionObject::show(SDL_Surface* des) {
	pthread_t thread_exp;
	ExplosionSupporter agrs(this, des, clip_);
	pthread_create(&thread_exp, NULL, &ExplosionObject::boom, (void*)&agrs);
	pthread_join(thread_exp, NULL);

}

void ExplosionObject::setExplosionRect(BaseObject* obj) {
	int x = obj->getRect().x + obj->getRect().w / 2 - rect_.w / 2;
	int y = obj->getRect().y + obj->getRect().h / 2 - rect_.h / 2;
	setRect(x, y);
}


void ExplosionObject::setExplosionSound(Mix_Chunk* sound) {
	explosion_sound_ = sound;
}
void ExplosionObject::playSound(const int& channel, const int& loops) {
	CommonFunction::playSound(explosion_sound_, channel, loops);
}

void ExplosionObject::explode(SDL_Surface* screen) {
	CommonFunction::playSound(explosion_sound_, 3, 0);
	show(screen);
}