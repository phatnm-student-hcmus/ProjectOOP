#ifndef _GAME_H_
#define _GAME_H_

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm> //swap

#include "Spaceship.h"
#include "ExplosionObject.h"

#include "ThreatObject.h"
#include "UFO.h"
#include "Boss.h"

#include "Laser.h"

#include "TextObject.h"

#include "StartMenu.h"
#include "GameOverMenu.h"
#include "SettingMenu.h"
#include "HelpMenu.h"
#include "LeaderBoard.h"
#include "LeaderBoardData.h"
#include "VictotyMenu.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

using namespace std;

class ExplosionObject;
class Game {
private:
	SDL_Surface* screen_;
	SDL_Surface* background_;

	Mix_Chunk* background_sound_;
	Mix_Chunk* start_game_background_sound_;

	SDL_Event event_;

	TTF_Font* font_;

	//game object

	Spaceship* spaceship_;

	vector<ThreatObject*> list_threat_object_;
	Boss* boss;
	
	ExplosionObject* spaceship_explosion_;
	ExplosionObject* ufo_explosion_;
	
	TextObject* score_text_;
	TextObject* live_text_;

	//game attributes
	string player_name_;
	int score_;
	int win_stage_1_;
	bool is_new_game_;
	Uint32 survival_time_;
	int section_;
	int boss_health_;
	int current_level_;
	int time_to_win_stage_1_;

	enum LEVEL 	{
		EASY = 0,
		NORMAL = 1,
		NIGHTMARE = 2
	};
	static const int spaceship_health_lvl_[3];
	static const int boss_health_lvl_[3];
	static const int time_survival_lvl_[3];


	//Menu
	static const int list_menu_size = 6;
	enum MENU_INDEX 	{
		START = 0,
		GAME_OVER = 1,
		SETTING = 2,
		LEADER_BOARD = 3,
		HELP = 4,
		VICTORY = 5
	};
	/*	START = 0,
		GAME_OVER = 1,
		SETTING = 2,
		LEADER_BOARD = 3,
		HELP = 4,
		VICTORY = 5*/
	Menu* list_menu_[list_menu_size];


	vector<LeaderBoardData> leader_board_data_;

	enum FILE_PATH_INDEX 	{
		LEADER_BOARD_DATA = 0,
		SAVE_GAME = 1
	};
	static const string file_path_[2];
public:
	Game();
	~Game();

/*---------------get-set methods---------------*/
public:
	//screen
	SDL_Surface* getScreen();
	void setScreen(SDL_Surface* screen);

	//background
	SDL_Surface* getBackground();
	void setBackground(const string& background_file_path);
	void setBackground(SDL_Surface* background);

	//spaceship
	Spaceship* getSpaceship();
	void setSpaceship(Spaceship* spaceship);

	//threatOBject
	vector<ThreatObject*> getListThreatObject();
	void setListThrearObject(const vector<ThreatObject*> list);
	void addThreatObject(ThreatObject* threat_object);
	bool removeThreatObject(ThreatObject* threat_object);

/*--------------------- GAME METHOD ---------------*/
public:
	bool init();

	void restart(const int& level = LEVEL::EASY);

	void run();

	bool inputPlayerName();
	int levelChoosing();

	//return 1: continue
	bool loadData();

	void sortData();
	bool writeData();

	bool writeSaveGame(const int& stage, const int& boss_health = -1, const bool& clear_data = false);

/*------------------ PROCESS MENU METHOD -------------*/

	void processStartGameMenuReturnVal(const int& ret_val, bool& run);
	void processGameOverMenuReturnVal(const int& ret_val_, bool& run);
	void processSettingMenuReturnVal(const int& ret_val, bool& run);
	void processLeaderBoardMenuReturnVal(const int& ret_val, bool& run);
	void processHelpMenuReturnVal(const int& ret_val, bool& run);


};
#endif // !_GAME_H_
