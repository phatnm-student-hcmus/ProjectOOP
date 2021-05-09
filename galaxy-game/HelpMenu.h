#pragma once
#include "Menu.h"
#include "TextObject.h"

#define HELP_MENU_WIDTH 1200
#define HELP_MENU_HEIGHT 600

class HelpMenu : public Menu {
private:
	TextObject* back_button_;

public:
	HelpMenu();
	~HelpMenu();

	int showMenu(SDL_Surface* screen);
	void showAllText(SDL_Surface* screen);

	void mouseMotionCheck(const int& x_mouse, const int& y_mouse);
	int mouseButtonDown(const int& x_mouse, const int& y_mouse);
};

