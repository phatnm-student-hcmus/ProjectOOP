#include "ThreatObject.h"
ThreatObject::ThreatObject() {
	health_ = 1;
	rect_.x = 0;
	rect_.y = 0;
	rect_.w = 0;
	rect_.h = 0;
	
	time_ = SDL_GetTicks();
	x_val_ = 0;
	y_val_ = 0;

	ptr_object_ = NULL;
	self_explosion_ = NULL;

	score_ = 0;
}

ThreatObject::~ThreatObject() {
	if (ptr_object_ != NULL) {
		SDL_FreeSurface(ptr_object_);
		ptr_object_ = NULL;
	}
	for (const auto& ptr_laser : list_laser_) {
		delete ptr_laser;
	}
	list_laser_.clear();
}

bool ThreatObject::decreaseHealth(const int& amount) {
	health_ = health_ - amount;
	return health_ <= 0;
}

bool ThreatObject::isCollistion(ThreatObject* threat_obj) {
	return CommonFunction::checkCollision(rect_, threat_obj->getRect());
}

void ThreatObject::setExplosionRect(BaseObject* object) {
	self_explosion_->setExplosionRect(object);
}
void ThreatObject::setExplosion(ExplosionObject* explosion) {
	self_explosion_ = explosion;
	self_explosion_->setExplosionRect(this);
}
void ThreatObject::setExplosionSound(Mix_Chunk* sound) {
	self_explosion_->setExplosionSound(sound);
}
Mix_Chunk* ThreatObject::getExplosionSound() {
	return self_explosion_->getExplosionSound();
}
void ThreatObject::explode(SDL_Surface* screen) {
	self_explosion_->explode(screen);
}

int ThreatObject::getScore() {
	return score_;
}

vector<Laser*>& ThreatObject::getListLaser() {
	return list_laser_;
}

int ThreatObject::getHealth() {
	return health_;
}
