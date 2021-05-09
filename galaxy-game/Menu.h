#ifndef _MENU_H_
#define _MENU_H_

#include "BaseObject.h"
class Menu : public BaseObject {
protected:
	TTF_Font* font_ = NULL;
public:
	virtual ~Menu() = 0;
	virtual int showMenu(SDL_Surface* screen) = 0;
	virtual void showAllText(SDL_Surface* screen) = 0;

	virtual void mouseMotionCheck(const int& x_mouse, const int& y_mouse) = 0;
	virtual int mouseButtonDown(const int& x_mouse, const int& y_mouse) = 0;

	//dummy methode
	virtual void set_is_new_game(const int& flag_val) {};
	virtual bool get_is_new_game() { return true; };
	virtual void setSound(Mix_Chunk* s1, Mix_Chunk* s2, Mix_Chunk* s3, Mix_Chunk* s4, Mix_Chunk* s5, Mix_Chunk* s6){}
	virtual void changeSoundVolume(){}
	virtual bool getOpenFrom() { return 0; }
	virtual void setOpenFrom(const bool& state) {};

};

#endif // !_MENU_H_
