#include "Spaceship.h"

Spaceship::Spaceship() {
	setRect(SCREEN_WIDTH / 2 - SPACESHIP_WIDTH / 2, SCREEN_HEIGHT - SPACESHIP_HEIGHT);
	rect_.w = SPACESHIP_WIDTH;
	rect_.h = SPACESHIP_HEIGHT;
	ptr_object_ = NULL;
	x_val_ = 0;
	y_val_ = 0;

	health_ = 5;
	
	ptr_object_ = NULL;
	self_explosion_ = NULL;
	laser_shooting_sound_ = NULL;
	got_hit_sound_ = NULL;
}
Spaceship::~Spaceship() {
	if (ptr_object_ != NULL) {
		SDL_FreeSurface(ptr_object_);
		ptr_object_ = NULL;
	}
}

void Spaceship::handleInputAction(const SDL_Event& events) {
	if (events.type == SDL_KEYDOWN) {
		switch (events.key.keysym.sym) {
		case SDLK_UP:
			y_val_ = -SPACESHIP_SPEED;			
			break;
		case SDLK_DOWN:
			y_val_ = SPACESHIP_SPEED;
			break;
		case SDLK_LEFT:
			x_val_ = -SPACESHIP_SPEED;
			break;
		case SDLK_RIGHT:
			x_val_ = SPACESHIP_SPEED;
			break;
		case SDLK_SPACE:
			Laser* ptr_laser = new Laser(s_laser_img_file_path);
			ptr_laser->setShootingSound(laser_shooting_sound_);
			ptr_laser->playSound();
			ptr_laser->setWidthHeight(LASER_WIDTH, LASER_HEIGHT);
			ptr_laser->setLaserRect(this, 0);
			ptr_laser->set_y_val(10);
			ptr_laser->setIsMove(true);
			list_laser_.push_back(ptr_laser);
			
			break;
		}
	}
	else if (events.type == SDL_KEYUP) {
		switch (events.key.keysym.sym) {
		case SDLK_UP:
			y_val_ = 0;
			break;
		case SDLK_DOWN:
			y_val_ = 0;
			break;
		case SDLK_LEFT:
			x_val_ = 0;
			break;
		case SDLK_RIGHT:
			x_val_ = 0;
			break;
		default:
			break;
		}
	}
}
void Spaceship::move() {
	rect_.x += x_val_;
	if (rect_.x < -SPACESHIP_WIDTH/2 || (rect_.x + SPACESHIP_WIDTH / 2) > SCREEN_WIDTH) {
		rect_.x -= x_val_;
	}

	rect_.y += y_val_;
	if (rect_.y < 0 || (rect_.y + SPACESHIP_HEIGHT) > SCREEN_HEIGHT) {
		rect_.y -= y_val_;
	}
}

void Spaceship::shot(SDL_Surface* screen, vector<ThreatObject*>& list_threat_object, int& score_) {
	for (int i = 0; i < (int)list_laser_.size(); i++) {
		Laser* ptr_laser = list_laser_.at(i);
		if (ptr_laser != NULL) {
			if (ptr_laser->isMove()) {
				ptr_laser->move();
				ptr_laser->show(screen);
			}
			else {
				delete ptr_laser;
				list_laser_.erase(list_laser_.begin() + i);
				ptr_laser = NULL;
				continue;
			}
			bool flag = false;
			for (int j = 0; j < (int)list_threat_object.size(); j++) {
				ThreatObject* ptr_threat = list_threat_object.at(j);
				if (CommonFunction::checkCollision(ptr_laser->getRect(), ptr_threat->getRect())) { 
					if (ptr_threat->decreaseHealth(ptr_laser->getDame())) {
						score_ += ptr_threat->getScore();
						ptr_threat->setExplosionRect(ptr_laser);
						ptr_threat->explode(screen);						
						list_threat_object.erase(list_threat_object.begin() + j);
					}
					list_laser_.erase(list_laser_.begin() + i);
					i--;
					flag = true;
					break;
				}
			}	
			if (flag) {
				break;
			}
		}
	}
}

vector<Laser*> Spaceship::getListLaser() {
	return list_laser_;
}

void Spaceship::setListLaser(vector<Laser*> list_laser) {
	this->list_laser_ = list_laser;
}

bool Spaceship::decreaseHealth(const int& amount) {
	if (amount > 0) {
		playSound(GOT_HIT_SOUND);
	}
	health_ = health_ - amount;
	return health_ <= 0;
}

void Spaceship::setHealth(const int& health) {
	health_ = health;
}
int Spaceship::getHealth() {
	return health_;
}

void Spaceship::gotHit() {
	decreaseHealth(1);
	playSound(GOT_HIT_SOUND);
}

void Spaceship::setSound(Mix_Chunk* sound, const int& type) {
	if (type == LASER_SHOOTING_SOUND) {
		laser_shooting_sound_ = sound;
	}
	else if (type == SELF_EXPLOSION_SOUND) {
		self_explosion_->setExplosionSound(sound);
	}
	else if (type == GOT_HIT_SOUND) {
		got_hit_sound_ = sound;
	}
}

void Spaceship::playSound(const int& type) {
	if (type == LASER_SHOOTING_SOUND) {
		CommonFunction::playSound(laser_shooting_sound_, LASER_SHOOTING_CHANNEL);
	}
	else if (type == SELF_EXPLOSION_SOUND) {
		self_explosion_->playSound();
	}
	else if (type == GOT_HIT_SOUND) {
		CommonFunction::playSound(got_hit_sound_, GOT_HIT_CHANNEL);
	}
}

void Spaceship::setExplosion(ExplosionObject* explosion) {
	self_explosion_ = explosion;
}

void Spaceship::explode(SDL_Surface* screen) {
	self_explosion_->setExplosionRect(this);
	self_explosion_->playSound();
	self_explosion_->show(screen);
}


Mix_Chunk* Spaceship::getShootingSound() {
	return laser_shooting_sound_;
}
Mix_Chunk* Spaceship::getGotHitSound() {
	return got_hit_sound_;
}
