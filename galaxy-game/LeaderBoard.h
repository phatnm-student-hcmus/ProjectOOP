#pragma once
#include "Menu.h"
#include <vector>
#include "TextObject.h"
#include "LeaderBoardData.h"

#define	SIZE_LIST 5
class LeaderBoard :  public Menu {
private:
	TextObject* name_[SIZE_LIST];
	TextObject* score_[SIZE_LIST];
	TextObject* time_[SIZE_LIST];

	TextObject* back_;
public:
	LeaderBoard();
	~LeaderBoard();
	int showMenu(SDL_Surface* screen);
	void showAllText(SDL_Surface* screen);

	void mouseMotionCheck(const int& x_mouse, const int& y_mouse);

	enum RETURN_VALUE {
		DEFAULT = -1,
		EXIT_GAME = 0,
		BACK = 1
	};
	/*DEFAULT = 0,
	BACK = 1*/
	int mouseButtonDown(const int& x_mouse, const int& y_mouse);
	void setData(vector<LeaderBoardData> data);
};

