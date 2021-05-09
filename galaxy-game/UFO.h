#ifndef _UFO_H_
#define _UFO_H_
#include "ThreatObject.h"

#define UFO_BAD_WIDTH 60
#define UFO_BAD_HEIGHT 56
#define UFO_GOOD_WIDTH 130
#define UFO_GOOD_HEIGHT 120

#define RATE_SPOND_GOOD_UFO 10

class UFO : public ThreatObject {
private:
	int shoting_time_;
	int type_;
public:
	UFO();
	~UFO();
	enum UFO_TYPE {
		GOOD = 1,	//GOOD UFO no attack, but has more HP,
		BAD = 0		//BAD is oposite
	};

	void move();

	bool isOuterScreen();

	void attack(SDL_Surface* screen, Spaceship* spaceship);

};
#endif // !_UFO_H_