#pragma once
#include "Menu.h"
#include "TextObject.h"
class StartMenu : public Menu {
protected:
	TextObject* continue_,
		* start_new_game_,
		* setting_,
		* leader_board_,
		* help_,
		* exit_game_;
	bool is_new_game_;
public:
	StartMenu();
	~StartMenu();
	enum RETURN_VALUE {
		DEFAULT = -1,
		CONTINUE = 0,
		START_GAME = 1,
		SETTING = 2,
		LEADER_BOARD = 3,
		HELP = 4,
		EXIT_GAME = 5
	};
	/*Return value:
	0: continue
	1: start new game
	2: setting_
	3: leader board
	4: help_
	*/
	int showMenu(SDL_Surface* screen);
	void showAllText(SDL_Surface* screen);

	void mouseMotionCheck(const int& x_mouse, const int& y_mouse);
	int mouseButtonDown(const int& x_mouse, const int& y_mouse);

	void set_is_new_game(const int& flag_val);
	bool get_is_new_game();
};

