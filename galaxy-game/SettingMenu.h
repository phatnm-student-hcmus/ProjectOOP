#pragma once
#include "Menu.h"
#include "TextObject.h"
#include "AudioIcon.h"
#include "SoundMark.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define LINE_WIDTH_1 425
#define LINE_WIDTH_2 235
#define LINE_HEIGHT 4
#define SOUND_MARK_WIDTH 10
#define SOUND_MARK_HEIGHT 30
class SettingMenu : public Menu {
private:
	//for init Map RGB
	SDL_Surface* ptr_screen_;
	/*0: startgame music
	* 1: ingame music
	* 2: shooting sound
	* 3: spaceship_explosion_sound
	* 4: ufo_explosion_sound
	* 5: got hit sound
	*/
	Mix_Chunk* sound_[6];
	AudioIcon* icons[4];
	
	Uint32 line_color_[4];
	SDL_Rect line_rect_[4];
	Uint32 sound_mark_color_[4];
	SDL_Rect sound_mark_rect_[4];
	TextObject* percent_volume_[4];
	int dragging_;

	static const int s_max_volume[6];
	static int s_prev_volume[6];
	//0: blue
	//1: green
	
	TextObject resume_, exit_game_;
	/*START_MENU = 0,
		IN_GAME = 1*/
	bool open_from_;
	SDL_Event m_event_;
public:
	SettingMenu();
	SettingMenu(SDL_Surface* screen);	
	~SettingMenu();

	enum SOUND_INDEX 	{
		MUSIC = 0,
		SHOOTING = 1,
		EXPLOSION = 2,
		GOT_HIT = 3
	};

	enum MAX_VOLUME {
		MAX_VOL_0 = 128,
		MAX_VOL_1 = 64,
		MAX_VOL_2 = 15,
		MAX_VOL_3 = 128,
		MAX_VOL_4 = 128,
		MAX_VOL_5 = 128
	};

	enum RETURN_VALUE 	{
		RESUME_GAME = 0,
		EXIT_GAME = 1
	};
	enum MOUSE_BTN_DOWN_RET {
		NOTHING = -1,
		RESUME = 0,
		CHANGE_VOLUME = 1,
		EXIT = 2,
		DRAG = 3
	};

	enum OPEN_FROM {
		START_MENU = 0,
		IN_GAME = 1
	};
	/*	RESUME = 0,
		EXIT = 1*/
	int showMenu(SDL_Surface* screen);
	void showAllText(SDL_Surface* screen);

	void mouseMotionCheck(const int& x_mouse, const int& y_mouse);
	/*	NOTHING = -1,
		RESUME = 0,
		CHANGE_VOLUME = 1,
		EXIT = 2,
		DRAG = 3*/
	virtual int mouseButtonDown(const int& x_mouse, const int& y_mouse);

	void setSound(Mix_Chunk* s1, Mix_Chunk* s2, Mix_Chunk* s3, Mix_Chunk* s4, Mix_Chunk* s5, Mix_Chunk* s6);

	void changeSoundVolume();
	
	bool getOpenFrom();
	void setOpenFrom(const bool& state);
};

