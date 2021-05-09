#ifndef _LASER_H_
#define _LASER_H_

#include "BaseObject.h"

#define LASER_WIDTH 5
#define LASER_HEIGHT 35
#define SPHERE_WIDTH 10
#define SPHERE_HEIGHT 10

class Laser : public BaseObject {
private:
	int dame_per_hit_;
	int is_move_;
	Mix_Chunk* shooting_sound_;
public:
	Laser();
	Laser(string img_file_path);
	Laser(string img_file_path, string sound_file_path);
	~Laser();
	
	bool isMove();
	void setIsMove(const bool& state);

	void setDame(const int& dame);
	int getDame();
	
	//type = 0: spaceship or type = 1: threat object
	void setLaserRect(BaseObject* obj, const int& type);

	void move();

	void setShootingSound(Mix_Chunk* sound);
	Mix_Chunk* getShootingSound();
	void playSound(const int& loops = 0);
};
#endif // !_LASER_H_



