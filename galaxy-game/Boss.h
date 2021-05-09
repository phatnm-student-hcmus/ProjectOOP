#pragma once
#include "ThreatObject.h"
#include "UFO.h"
#include "Laser.h"

#define BOSS_WIDTH 350
#define BOSS_HEIGHT 210
#define SKILL_WIDTH 144
#define SKILL_HEIGHT 143
#define SPON_TIME 2000//ms

class Boss :  public ThreatObject {
private:
	static int s_max_health_;
	vector<ThreatObject*> list_ufo_;
	vector<Laser*> list_laser;
	int amount_spon_;
	Uint32 time_skill_;
	Uint32 time_spon_;
	SDL_Rect health_bar, white_bk;
public:
	Boss();
	~Boss();

	void show(SDL_Surface* des);
	void move();

	bool isOuterScreen();

	void attack(SDL_Surface* screen, Spaceship* spaceship);

	void spond_ufo();

	void explode(SDL_Surface* screen);

	vector<ThreatObject*>& getListUFO();

	void setTimeSkill(const Uint32& delta);

	void setHealth(const int& health);
	static void setMaxHealth(const int& health);
};

