#ifndef _THREAT_OBJECT_H_
#define _THREAT_OBJECT_H_

#include "BaseObject.h"
#include "Laser.h"
#include "Spaceship.h"
#include "ExplosionObject.h"

#include <vector> 
#include <iomanip>
#include <ctime>

class Spaceship;

class ThreatObject : public BaseObject {
protected:
	Uint32 time_;
	vector<Laser*> list_laser_;
	int health_;
	ExplosionObject* self_explosion_;

	int score_;
	
public:
	ThreatObject();
	virtual ~ThreatObject() = 0;

	virtual void move() = 0;

	//return true if health == 0
	virtual bool decreaseHealth(const int& amount);
	
	virtual bool isOuterScreen() = 0;

	virtual void attack(SDL_Surface* screen, Spaceship* spaceship) = 0;

	virtual bool isCollistion(ThreatObject* thhreat_obj);

	virtual void setExplosionRect(BaseObject* object);
	virtual void setExplosion(ExplosionObject* explosion);
	virtual void setExplosionSound(Mix_Chunk* sound);
	virtual Mix_Chunk* getExplosionSound();
	virtual void explode(SDL_Surface* screen);

	int getScore();

	virtual vector<Laser*>& getListLaser();

	virtual int getHealth();
};
#endif // !_THREAT_OBJECT_H_