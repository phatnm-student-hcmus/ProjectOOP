#pragma once
#include "Menu.h"
#include "TextObject.h"

class VictotyMenu :  public Menu {
private:
	TextObject* play_again_,* exit_game_;

public:
	VictotyMenu();
	~VictotyMenu();

	int showMenu(SDL_Surface* screen);
	void showAllText(SDL_Surface* screen);

	void mouseMotionCheck(const int& x_mouse, const int& y_mouse);

	enum RETURN_VALUE 	{
		DEFAULT = -1,
		EXIT_GAME = 0,
		PLAY_AGAIN = 1
	};

	/*	DEFAULT = -1,
		EXIT_GAME = 0,
		PLAY_AGAIN = 1*/
	int mouseButtonDown(const int& x_mouse, const int& y_mouse);
};

