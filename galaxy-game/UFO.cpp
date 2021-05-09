#include "UFO.h"

UFO::UFO() {
	int random_result = rand() % 100;

	//in range(0, RATE_SPOND_GOOD_UFO)
	if (random_result < RATE_SPOND_GOOD_UFO) {
		type_ = GOOD;
		shoting_time_ = 0;
		health_ = 2;
		rect_.x = rand() % (SCREEN_WIDTH - UFO_GOOD_WIDTH);
		rect_.y = 0;
		rect_.w = UFO_GOOD_WIDTH;
		rect_.h = UFO_GOOD_HEIGHT;
		time_ = SDL_GetTicks();
		this->loadImage(s_ufo_img_file_path[type_], 0, 0, 0);

		x_val_ = 0;
		y_val_ = -1;
		score_ = 3;
	}
	else {
		type_ = BAD;
		rect_.x = rand() % (SCREEN_WIDTH - UFO_BAD_WIDTH);
		rect_.y = 0;
		rect_.w = UFO_BAD_WIDTH;
		rect_.h = UFO_BAD_HEIGHT;
		time_ = SDL_GetTicks();
		this->loadImage(s_ufo_img_file_path[type_], 0, 0, 0);

		x_val_ = 0;
		y_val_ = -1;

		shoting_time_ = 1;
		score_ = 1;
	}
}

UFO::~UFO() {
	if (ptr_object_ != NULL) {
		SDL_FreeSurface(ptr_object_);
		ptr_object_ = NULL;
	}
	for (const auto& ptr_laser : list_laser_) {
		delete ptr_laser;
	}
	list_laser_.clear();
}

bool UFO::isOuterScreen() {
	if (rect_.y > SCREEN_HEIGHT) {
		return true;
	}
	return false;
}
void UFO::move() {
	rect_.y -= y_val_;
}

void UFO::attack(SDL_Surface* screen, Spaceship* spaceship) {
	Uint32 time_check = SDL_GetTicks();
	if ((time_check - time_) >= 250 && shoting_time_ > 0) {
		shoting_time_--;
		time_ = time_check;
		Laser* laser = new Laser(s_sphere_img_file_path, "");

		laser->setWidthHeight(SPHERE_WIDTH, SPHERE_HEIGHT);
		laser->set_y_val(-3);
		laser->setLaserRect(this, 1);
		laser->setIsMove(true);
		list_laser_.push_back(laser);
	}

	for (int i = 0; i < (int)list_laser_.size(); i++) {
		Laser* ptr_laser = list_laser_[i];
		if (ptr_laser != NULL) {
			if (ptr_laser->isMove()) {
				if (CommonFunction::checkCollision(spaceship->getRect(), ptr_laser->getRect())) {
					spaceship->gotHit();
					
					delete ptr_laser;
					list_laser_.erase(list_laser_.begin() + i);
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