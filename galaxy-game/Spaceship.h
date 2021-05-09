#ifndef _SPACESHIP_H_
#define _SPACESHIP_H_
#include "BaseObject.h"
#include "ThreatObject.h"
#include "ExplosionObject.h"
#include "Laser.h"
#include <vector>

#define SPACESHIP_HEIGHT 95
#define SPACESHIP_WIDTH 115
#define SPACESHIP_SPEED 7

class Laser;
class ThreatObject;

class Spaceship : public BaseObject {
private:
	vector<Laser*> list_laser_;
	int health_;

	ExplosionObject* self_explosion_;
	Mix_Chunk* laser_shooting_sound_;
	Mix_Chunk* got_hit_sound_;
public:
	Spaceship();
	~Spaceship();
	enum soundType {
		LASER_SHOOTING_SOUND = 0,
		SELF_EXPLOSION_SOUND = 1,
		GOT_HIT_SOUND = 2
	};
	void handleInputAction(const SDL_Event& event);
	void move();
	void shot(SDL_Surface* screen, vector<ThreatObject*>& list_threat_object, int& score_);

	vector<Laser*> getListLaser();

	void setListLaser(vector<Laser*> list_laser);

	bool decreaseHealth(const int& amount);
	void setHealth(const int& health);
	int getHealth();

	void gotHit();

	//type 0: shooting_sound or type 1: explosion sound
	void setSound(Mix_Chunk* sound, const int& type = LASER_SHOOTING_SOUND);
	//type 0: shooting_sound or type 1: explosion sound
	void playSound(const int& type = LASER_SHOOTING_SOUND);

	void setExplosion(ExplosionObject* explosion);
	void explode(SDL_Surface* screen);

	Mix_Chunk* getShootingSound();
	Mix_Chunk* getExplosionSound() {
		return self_explosion_->getExplosionSound();
	}
	Mix_Chunk* getGotHitSound();
};
#endif // !_SPACESHIP_H_