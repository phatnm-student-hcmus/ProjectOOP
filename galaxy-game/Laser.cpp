#include "Laser.h"

Laser::Laser(){
	dame_per_hit_ = 1;
	is_move_ = false;
	shooting_sound_ = NULL;
}

Laser::Laser(string file_path) {

	dame_per_hit_ = 1;
	is_move_ = false;

	loadImage(file_path, 0, 0XFF, 0XFF);
	shooting_sound_ = NULL;

}
Laser::Laser(string file_path, string sound_file_path) {

	dame_per_hit_ = 1;
	is_move_ = false;

	loadImage(file_path, 0, 0XFF, 0XFF);
	shooting_sound_ = CommonFunction::loadSound(sound_file_path);

}

Laser::~Laser() {
	if (ptr_object_ != NULL) {
		SDL_FreeSurface(ptr_object_);
		ptr_object_ = NULL;
	}
}
bool Laser::isMove() {
	return is_move_;
}

void Laser::setIsMove(const bool& state) {
	is_move_ = state;
}

void Laser::setDame(const int& dame) {
	dame_per_hit_ = dame;
}
int Laser::getDame() {
	return dame_per_hit_;
}

void Laser::setLaserRect(BaseObject* obj, const int& type) {
	rect_.x = obj->getRect().x + obj->getRect().w / 2 - LASER_WIDTH / 2;
	rect_.y = obj->getRect().y + type*obj->getRect().h;
}

void Laser::move() {
	is_move_ = true;
	rect_.y -= y_val_;
	if (rect_.y < 0 || rect_.y > SCREEN_HEIGHT) {
		is_move_ = false;
	}
}

void Laser::setShootingSound(Mix_Chunk* sound) {
	shooting_sound_ = sound;
}
Mix_Chunk* Laser::getShootingSound() {
	return shooting_sound_;
}
void Laser::playSound(const int& loops) {
	CommonFunction::playSound(shooting_sound_, loops);
}