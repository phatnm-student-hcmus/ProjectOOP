#pragma once
#include "Menu.h"
#include "TextObject.h"
class GameOverMenu : public Menu {
private:
	TextObject* restart_game_,
		* main_menu_,
		* setting_,
		* leader_board_,
		* help_,
		* exit_game_;
public:
	GameOverMenu();
	~GameOverMenu();
	enum RETURN_VALUE {
		RESTART = 0,
		MAIN_MENU = 1,
		SETTING = 2,
		LEADER_BOARD = 3,
		HELP = 4,
		EXIT_GAME = 5
	};
	/*Return value:
	0: restart game
	1: main menu
	2: setting_
	3: leader board
	4: help_
	5: exit game
	*/
	int showMenu(SDL_Surface* screen);
	void showAllText(SDL_Surface* screen);

	void mouseMotionCheck(const int& x_mouse, const int& y_mouse);
	int mouseButtonDown(const int& x_mouse, const int& y_mouse);
};

