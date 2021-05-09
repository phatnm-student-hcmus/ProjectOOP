#include "Boss.h"

int Boss::s_max_health_ = 0;
Boss::Boss() {
	
	health_ = s_max_health_;
	rect_.x = SCREEN_WIDTH / 2 - BOSS_WIDTH / 2;
	rect_.y = 25;
	rect_.w = BOSS_WIDTH;
	rect_.h = BOSS_HEIGHT;

	time_ = SDL_GetTicks();
	x_val_ = 3;
	y_val_ = 0;

	ptr_object_ = NULL;
	self_explosion_ = NULL;

	score_ = 50;
	time_skill_ = SDL_GetTicks();
	time_spon_ = SDL_GetTicks();
	loadImage(s_boss_img_file_path, 255, 255, 255);
	amount_spon_ = 3;
	score_ = 50;

	health_bar.x = 200;
	health_bar.y = 6;
	health_bar.w = 800;
	health_bar.h = 19;

	white_bk.x = 200;
	white_bk.y = 6;
	white_bk.w = 800;
	white_bk.h = 19;
}
Boss::~Boss() {
	
	for (const auto& ufo : list_ufo_) {
		delete ufo;
	}
	list_ufo_.clear();

	for (const auto& laser : list_laser) {
		delete laser;
	}
	if (ptr_object_ != NULL) {
		SDL_FreeSurface(ptr_object_);
		ptr_object_ = NULL;
	}
	
}

void Boss::show(SDL_Surface* des) {
 	BaseObject::show(des);
	health_bar.w = 800 * 100 * health_ / s_max_health_ / 100;

	Uint32 white, red;

	white = SDL_MapRGB(des->format, 255, 255, 255);
	red = SDL_MapRGB(des->format, 245, 0, 0);

	SDL_FillRect(des, &white_bk, white);
	SDL_FillRect(des, &health_bar, red);
}

void Boss::move() {
	rect_.x += x_val_;
	if (rect_.x <= 0 || (rect_.x + BOSS_WIDTH) >= SCREEN_WIDTH) {
		rect_.x -= x_val_;
		x_val_ *= -1;
	}

}

bool Boss::isOuterScreen() {
	return false;
}

void Boss::attack(SDL_Surface* screen, Spaceship* spaceship) {
	Uint32 time_check = SDL_GetTicks();
	if ((time_check - time_skill_) >= 3500) {
		time_skill_ = time_check;
		Laser* laser = new Laser(s_boss_skill_img_file_path, "");
		Mix_Chunk* skill_sound = CommonFunction::loadSound(s_boss_skill_sound_file_path);
		CommonFunction::playSound(skill_sound, BOSS_SKILL_CHANNEL, 0);
		laser->setWidthHeight(SKILL_WIDTH, SKILL_HEIGHT);
		laser->set_y_val(-5);
		laser->setLaserRect(this, 1);
		laser->setIsMove(true);
		list_laser_.push_back(laser);
	}
	if (time_check - time_ > 500) {
		time_ = time_check;
		Laser* laser = new Laser(s_sphere_img_file_path, "");

		laser->setWidthHeight(SPHERE_WIDTH, SPHERE_HEIGHT);
		laser->set_y_val(-3);
		laser->setLaserRect(this, 1);
		laser->setRect(laser->getRect().x + (rand() % BOSS_WIDTH) - BOSS_WIDTH / 2, laser->getRect().y);
		laser->setIsMove(true);
		list_laser_.push_back(laser);
	}

	spond_ufo();

	for (int i = 0; i < (int)list_ufo_.size(); i++) {
		ThreatObject* ufo = list_ufo_[i];
		ufo->move();
		ufo->attack(screen, spaceship);
		ufo->show(screen);
		if (ufo->isOuterScreen()) {
			delete ufo;
			list_ufo_.erase(list_ufo_.begin() + i);
			i--;
			ufo = NULL;
		}
	}

	CommonFunction::applySurface(screen, screen, 0, 0);
	//spaceship->shot(screen, list_ufo_, score_);
	for (int i = 0; i < (int)list_laser_.size(); i++) {
		Laser* ptr_laser = list_laser_[i];
		if (ptr_laser != NULL) {
			if (ptr_laser->isMove()) {
				if (CommonFunction::checkCollision(spaceship->getRect(), ptr_laser->getRect())) {
					spaceship->gotHit();

					delete ptr_laser;
					list_laser_.erase(list_laser_.begin() + i);
					i--;
					ptr_laser = NULL;
				}
				else {
					ptr_laser->move();
					ptr_laser->show(screen);
				}
			}
			else {
				delete ptr_laser;
				list_laser_.erase(list_laser_.begin() + i);
				ptr_laser = NULL;
			}
		}		
	}
}

void Boss::spond_ufo() {
	if (SDL_GetTicks() - time_spon_ >= 2000) {
		time_spon_ = SDL_GetTicks();
		for (int i = 0; i < amount_spon_; i++) {
			UFO* ufo = new UFO();
			ufo->setExplosion(self_explosion_);
			list_ufo_.push_back(ufo);
		}
	}
}

void Boss::explode(SDL_Surface* screen) {
	show(screen);
	Mix_Chunk* exp = CommonFunction::loadSound(s_boss_exp_sound_file_path);
	CommonFunction::playSound(exp, -1, 2);
	for (int i = 0; i < 10; i++) {
		self_explosion_->setRect(rand() % (rect_.w/2) + rect_.x, rand() % rect_.h/2);
		self_explosion_->explode(screen);
		SDL_Flip(screen);
	}
	Mix_FreeChunk(exp);
}

vector<ThreatObject*>& Boss::getListUFO() {
	return list_ufo_;
}

void Boss::setTimeSkill(const Uint32& delta) {
	time_skill_ += delta;
}

void Boss::setHealth(const int& health) {
	health_ = health;
}


void Boss::setMaxHealth(const int& health) {
	s_max_health_ = health;
}