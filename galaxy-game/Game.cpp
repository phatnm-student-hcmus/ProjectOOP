#include "Game.h"

const string Game::file_path_[2] = { "data\\leader-board.dat", "data\\save.dat" };
const int Game::spaceship_health_lvl_[3] = { 5,3,1 };
const int Game::boss_health_lvl_[3] = { 100,200,300 };
const int Game::time_survival_lvl_[3] = { 60, 90, 120 };
Game::Game() {
	screen_ = NULL;
	background_ = NULL;

	event_ = SDL_Event();

	background_sound_ = NULL;
	start_game_background_sound_ = NULL;
	font_ = NULL;

	spaceship_ = NULL;

	spaceship_explosion_ = NULL;

	ufo_explosion_ = NULL;

	score_text_ = NULL;
	live_text_ = NULL;

	for (int i = 0; i < list_menu_size; i++) {
		list_menu_[i] = NULL;
	}

	boss = NULL;
	score_ = 0;
	is_new_game_ = true;
	win_stage_1_ = false;
	section_ = 1;
	boss_health_ = -1;
	survival_time_ = -1;
	current_level_ = -1;
	time_to_win_stage_1_ = -1;
}
Game::~Game() {
	if (screen_ != NULL) {
		SDL_FreeSurface(screen_);
		screen_ = NULL;
	}

	if (background_ != NULL) {
		SDL_FreeSurface(background_);
		background_ = NULL;
	}

	if (font_ != NULL) {
		TTF_CloseFont(font_);
		font_ = NULL;
	}
	if (spaceship_ == NULL) {
		delete spaceship_;
		spaceship_ = NULL;
	}

	if (spaceship_explosion_ != NULL) {
		delete spaceship_explosion_;
		spaceship_explosion_ = NULL;
	}

	if (ufo_explosion_ != NULL) {
		delete ufo_explosion_;
		ufo_explosion_ = NULL;
	}
	for (auto& threat_object : list_threat_object_) {
		if (threat_object != NULL) {
			delete threat_object;
			threat_object = NULL;
		}
	}

	for (int i = 0; i < list_menu_size; i++) {
		if (list_menu_[i] != NULL) {
			delete list_menu_[i];
		}
	}

	Mix_CloseAudio();
}

/*---------------get-set methods---------------*/

//screen
SDL_Surface* Game::getScreen() {
	return screen_;
}
void Game::setScreen(SDL_Surface* screen) {
	screen_ = screen;
}

//background
SDL_Surface* Game::getBackground() {
	return background_;
}
void Game::setBackground(const string& background_file_path) {
	setBackground(CommonFunction::loadImage(background_file_path));
}
void Game::setBackground(SDL_Surface* background) {
	if (background != NULL) {
		background_ = background;
	}
}

//spaceship
Spaceship* Game::getSpaceship() {
	return spaceship_;
}
void Game::setSpaceship(Spaceship* spaceship) {
	if (spaceship != NULL) {
		spaceship_ = spaceship;
	}
}

//threatOBject
vector<ThreatObject*> Game::getListThreatObject() {
	return list_threat_object_;
}
void Game::setListThrearObject(const vector<ThreatObject*> list) {
	if (!list.empty()) {
		list_threat_object_ = list;
	}
}
void Game::addThreatObject(ThreatObject* threat_object) {
	if (threat_object != NULL) {
		list_threat_object_.push_back(threat_object);
	}
}
bool Game::removeThreatObject(ThreatObject* threat_object) {
	for (auto it = list_threat_object_.begin(); it != list_threat_object_.end(); it++) {
		if (*it = threat_object) {
			list_threat_object_.erase(it, it + 1);
			return true; // remove successful
		}
	}
	return false; //remove failed
}

/*--------------------- GAME METHOD ---------------*/

bool Game::init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		return false;
	}

	//init screen
	screen_ = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
	if (screen_ == NULL) {
		return false;
	}

	//init audio
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		return false;
	}

	//init font_
	if (TTF_Init() != 0) {
		return false;
	}

	//set up menu
	list_menu_[MENU_INDEX::START] = new StartMenu();
	list_menu_[MENU_INDEX::GAME_OVER] = new GameOverMenu();
	list_menu_[MENU_INDEX::SETTING] = new SettingMenu(screen_);
	list_menu_[MENU_INDEX::SETTING]->setOpenFrom(SettingMenu::OPEN_FROM::START_MENU);
	list_menu_[MENU_INDEX::LEADER_BOARD] = new LeaderBoard;
	list_menu_[MENU_INDEX::HELP] = new HelpMenu();
	list_menu_[MENU_INDEX::VICTORY] = new VictotyMenu;


	//set up ingame background
	background_ = CommonFunction::loadImage(s_background_img_file_path);
	if (background_ == NULL) {
		return false;
	}
	background_sound_ = Mix_LoadWAV(s_background_sound_file_path);
	Mix_VolumeChunk(background_sound_, 64);
	
	start_game_background_sound_ = Mix_LoadWAV(s_start_game_background_sound_file_path);


	//set up font
	font_ = TTF_OpenFont(s_alberto_font_file_path, s_font_size_in_game);
	if (font_ == NULL) {
		return false;
	}

	//set up explosion
	spaceship_explosion_ = new ExplosionObject(s_clip_spaceship_exp_img_file_path, s_spaceship_exp_sound_file_path);
	ufo_explosion_ = new ExplosionObject(s_clip_ufo_exp_img_file_path, s_ufo_exp_sound_file_path);

	//set up sound
	Mix_Chunk* shooting_sound = CommonFunction::loadSound(s_laser_shooting_sound_file_path),
		* got_hit_sound = CommonFunction::loadSound(s_spaceship_got_hit_sound);
	Mix_VolumeChunk(shooting_sound, 0xf);

	//set up space ship
	spaceship_ = new Spaceship;
	spaceship_->loadImage(s_spaceshipe_img_file_path);
	spaceship_->setExplosion(spaceship_explosion_);
	spaceship_->setSound(shooting_sound, Spaceship::LASER_SHOOTING_SOUND);
	spaceship_->setSound(got_hit_sound, Spaceship::GOT_HIT_SOUND);
	

	//set up score_ text
	score_text_ = new TextObject;
	score_text_->setTextContent("SCORE: 0");
	score_text_->setTextColor(TextObject::WHITE_TEXT);
	score_text_->setFont(font_);
	score_text_->setRect(50, 50);

	//set up live text
	live_text_ = new TextObject;
	live_text_->setTextContent(to_string(spaceship_->getHealth()));
	live_text_->setTextColor(TextObject::WHITE_TEXT);
	live_text_->setFont(font_);
	live_text_->setRect(1000, 50);

	list_menu_[MENU_INDEX::SETTING]->setSound(start_game_background_sound_, background_sound_,
		spaceship_->getShootingSound(),
		spaceship_explosion_->getExplosionSound(), ufo_explosion_->getExplosionSound(),
		spaceship_->getGotHitSound());

	//set up boss
	boss = new Boss;
	boss->setExplosion(ufo_explosion_);
	return true;
}

void Game::restart(const int& level) {

	spaceship_->setHealth(spaceship_health_lvl_[level]);

	boss->setHealth(boss_health_lvl_[level]);
	boss->setMaxHealth(boss_health_lvl_[level]);
	
	time_to_win_stage_1_ = time_survival_lvl_[level];

	survival_time_ = 0;
	section_ = 1;
	is_new_game_ = true;
	win_stage_1_ = false;

	spaceship_->setRect(SCREEN_WIDTH / 2 - SPACESHIP_WIDTH / 2, SCREEN_HEIGHT - SPACESHIP_HEIGHT);
	spaceship_->set_x_val_y_val(0, 0);

	auto list_laser = spaceship_->getListLaser();
	for (auto& laser : list_laser) {
		delete laser;
	}
	list_laser.clear();
	spaceship_->setListLaser(list_laser);

	for (auto& threat_object : list_threat_object_) {
		if (threat_object != NULL) {
			delete threat_object;
			threat_object = NULL;
		}
	}

	list_threat_object_.clear();

	score_ = 0;
}

void Game::run() {

	int life = spaceship_->getHealth();

	const int max_ufo_on_screen = 10;
	const int time_ufo_spon = 1000; //ms

	CommonFunction::playSound(start_game_background_sound_, START_GAME_MUSIC_CHANNEL, -1);


	bool run = true;
	//load file; => set start newgame;
	is_new_game_ = true;

	loadData();
	if (section_ == 1) {
		win_stage_1_ = false;
		run = true;
	}
	else {
		win_stage_1_ = true;
		//run = false;
		boss->setHealth(boss_health_);
	}
	list_menu_[MENU_INDEX::START]->set_is_new_game(is_new_game_);
	int ret_val = list_menu_[0]->showMenu(screen_);

	processStartGameMenuReturnVal(ret_val, run);


	//ingame background move
	SDL_Rect screen_background_in_game_rect = SDL_Rect();
	screen_background_in_game_rect.x = 0;
	screen_background_in_game_rect.y = 2400;
	screen_background_in_game_rect.w = 1200;
	screen_background_in_game_rect.h = 600;

	//spaceship_->setHealth(1);
	unsigned int twice = 0;
	CommonFunction::playSound(background_sound_, IN_GAME_MUSIC_CHANNEL, -1);

	if (!run) {
		return;
	}

	//normal stage
	Uint32 time_start;
	bool is_play_again = false;
	do {
		list_menu_[MENU_INDEX::SETTING]->setOpenFrom(SettingMenu::OPEN_FROM::IN_GAME);
		list_menu_[MENU_INDEX::SETTING]->changeSoundVolume();
		time_start = SDL_GetTicks() - 1000 * survival_time_;
		Uint32 start = SDL_GetTicks();
		TextObject text_time("SURVIVAL TIME: 0s", TextObject::TEXTCOLOR::WHITE_TEXT, font_);
		text_time.setRect(50, 100);
		bool resume = false;
		if (!win_stage_1_) {
			SDL_Surface* temp = background_;
			background_ = CommonFunction::loadImage(s_background_img_file_path);
			SDL_FreeSurface(temp);
			is_play_again = false;
			while (run) {
				if (resume) {
					resume = false;
					Uint32 countdown = 3000;
					Uint32 start_countdown = SDL_GetTicks();
					while (SDL_GetTicks() - start_countdown < countdown) {
						TTF_Font* font_countdown = TTF_OpenFont(s_showcard_gothic_font_file_path, 2 * s_font_size_start_menu);
						TextObject countdown_text(to_string((3000 - SDL_GetTicks() + start_countdown) / 1000), TextObject::TEXTCOLOR::RED_TEXT, font_countdown);
						countdown_text.setRect(600 - 35, 300 - 72);

						CommonFunction::applySurface(background_, screen_, &screen_background_in_game_rect, 0, 0);
						countdown_text.show(screen_);
						spaceship_->show(screen_);
						for (const auto& s_laser : spaceship_->getListLaser()) {
							s_laser->show(screen_);
						}
						for (const auto& threat_obj : list_threat_object_) {
							threat_obj->show(screen_);
							for (const auto& t_laser : threat_obj->getListLaser()) {
								t_laser->show(screen_);
							}
						}
						SDL_Flip(screen_);

						SDL_Delay(500);
						TTF_CloseFont(font_countdown);
					}
					time_start += SDL_GetTicks() - start_countdown;
				}
				twice++;
				screen_background_in_game_rect.y -= twice % 2;
				if (screen_background_in_game_rect.y == 0) {
					screen_background_in_game_rect.y = 2400;
				}

				if (spaceship_->getHealth() <= 0) {
					writeSaveGame(1, -1, true);
					spaceship_->explode(screen_);
					run = false;
					break;
				}
				while (SDL_PollEvent(&event_)) {
					if (event_.type == SDL_QUIT) {
						run = false;
						writeSaveGame(1);
						SDL_Delay(1000);
						return;
					}
					else if (event_.type == SDL_KEYDOWN) {
						if (event_.key.keysym.sym == SDLK_ESCAPE) {
							Uint32 start_setting = SDL_GetTicks();
							int ret_val = list_menu_[SETTING]->showMenu(screen_);
							processSettingMenuReturnVal(ret_val, run);
							if (!run) {
								SDL_Delay(500);
								writeSaveGame(1);
								return;
							}
							else {
								resume = true;
							}
							time_start += (SDL_GetTicks() - start_setting);
						}
					}
					spaceship_->handleInputAction(event_);
				}
				CommonFunction::applySurface(background_, screen_, &screen_background_in_game_rect, 0, 0);

				Uint32 x = SDL_GetTicks() - start; //(second)
				if (x > time_ufo_spon) { // ufo is created after every 1 seconds
					start = SDL_GetTicks();
					if (list_threat_object_.size() < max_ufo_on_screen) {
						ThreatObject* new_ufo = new UFO;
						new_ufo->setExplosion(ufo_explosion_);
						list_threat_object_.push_back(new_ufo);
					}
				}
				for (int i = 0; i < (int)list_threat_object_.size(); ++i) {
					ThreatObject* ptr_ufo = list_threat_object_.at(i);

					//check collision between spaceship and ufo
					if (CommonFunction::checkCollision(spaceship_->getRect(), ptr_ufo->getRect())) {
						spaceship_->explode(screen_);

						spaceship_->setRect(SCREEN_WIDTH / 2 - SPACESHIP_WIDTH / 2, SCREEN_HEIGHT - SPACESHIP_HEIGHT);
						SDL_Delay(300);
						time_start += 300;
						//spaceship.show(s_screen);

						delete ptr_ufo;
						list_threat_object_.erase(list_threat_object_.begin() + i);
						ptr_ufo = NULL;
						spaceship_->decreaseHealth(1);
						i--;
						continue;
					}

					ptr_ufo->move();
					ptr_ufo->attack(screen_, spaceship_);
					ptr_ufo->show(screen_);
					if (ptr_ufo->isOuterScreen()) {
						spaceship_->decreaseHealth(1);
						delete ptr_ufo;
						list_threat_object_.erase(list_threat_object_.begin() + i);
						ptr_ufo = NULL;
					}
				}

				spaceship_->move();
				spaceship_->shot(screen_, list_threat_object_, score_);
				spaceship_->show(screen_);

				survival_time_ = (SDL_GetTicks() - time_start) / 1000;

				//check time
				if (survival_time_ >= time_to_win_stage_1_) {
					win_stage_1_ = true;
					TextObject stage_1_complete("Stage 1 complete", TextObject::YELLOW_TEXT, font_);
					stage_1_complete.setRect(498, 250);
					CommonFunction::applySurface(background_, screen_, &screen_background_in_game_rect, 0, 0);
					spaceship_->show(screen_);
					stage_1_complete.show(screen_);
					SDL_Flip(screen_);

					SDL_Delay(1000);
					time_start += 1000;
					run = false;
				}
				text_time.setTextContent("SURVIVAL TIME: " + to_string(survival_time_) + "s");
				text_time.show(screen_);

				score_text_->setTextContent("SCORE: " + to_string(score_));
				score_text_->show(screen_);

				live_text_->setTextContent("HEALTH: " + to_string(spaceship_->getHealth()));
				live_text_->show(screen_);



				//update screen
				if (SDL_Flip(screen_) == -1) {
					return;
				}
			}
		}

		//boss stage
		if (win_stage_1_) {
			SDL_Surface* temp = background_;
			background_ = CommonFunction::loadImage(s_boss_background_file_path);
			SDL_FreeSurface(temp);
			screen_background_in_game_rect.x = 0;
			screen_background_in_game_rect.y = 1200;

			Mix_Chunk* danger_sound = CommonFunction::loadSound(s_danger_sound_file_path);
			CommonFunction::playSound(danger_sound, -1);

			bool resume = false;
			bool run_boss_stage = true;
			int score_temp = score_;
			int health_temp = spaceship_->getHealth();
			restart(current_level_);			
			score_ = score_temp;
			spaceship_->setHealth(health_temp);

			TextObject text_time("SURVIVAL TIME: " + to_string(survival_time_) + "s", TextObject::TEXTCOLOR::WHITE_TEXT, font_);
			text_time.setRect(50, 100);

			CommonFunction::applySurface(background_, screen_, &screen_background_in_game_rect, 0, 0);
			boss->show(screen_);
			spaceship_->show(screen_);
			SDL_Flip(screen_);

			vector<ThreatObject*> boss_vect = { boss };
			//delay 5s
			Uint32 countdown = 5000;
			SDL_Surface* warning = CommonFunction::loadImage(s_warning_text_img_file_path, 255, 255, 255);
			Uint32 start_countdown = SDL_GetTicks();
			while (SDL_GetTicks() - start_countdown < countdown) {
				TTF_Font* font_countdown = TTF_OpenFont(s_showcard_gothic_font_file_path, 2 * s_font_size_start_menu);
				TextObject countdown_text(to_string((countdown - SDL_GetTicks() + start_countdown) / 1000), TextObject::TEXTCOLOR::RED_TEXT, font_countdown);
				countdown_text.setRect(600 - 35, 400 - 72);

				CommonFunction::applySurface(background_, screen_, &screen_background_in_game_rect, 0, 0);
				countdown_text.show(screen_);
				boss->show(screen_);
				spaceship_->show(screen_);

				CommonFunction::applySurface(warning, screen_, 200, -70);
				SDL_Flip(screen_);
				SDL_Delay(1000);
				countdown--;
				TTF_CloseFont(font_countdown);
			}

			//easy win
			//boss->setHealth(10);
			while (run_boss_stage) {
				if (resume) {
					resume = false;
					Uint32 countdown = 3000;
					Uint32 start_countdown = SDL_GetTicks();
					while (SDL_GetTicks() - start_countdown < countdown) {
						TTF_Font* font_countdown = TTF_OpenFont(s_showcard_gothic_font_file_path, 2 * s_font_size_start_menu);
						TextObject countdown_text(to_string((3000 - SDL_GetTicks() + start_countdown) / 1000), TextObject::TEXTCOLOR::RED_TEXT, font_countdown);
						countdown_text.setRect(600 - 35, 300 - 72);

						CommonFunction::applySurface(background_, screen_, &screen_background_in_game_rect, 0, 0);
						countdown_text.show(screen_);
						spaceship_->show(screen_);
						for (const auto& ufo : boss->getListUFO()) {
							ufo->show(screen_);
							for (const auto& t_laser : ufo->getListLaser()) {
								t_laser->show(screen_);
							}
						}
						for (const auto& s_laser : spaceship_->getListLaser()) {
							s_laser->show(screen_);
						}
						//boss
						boss->show(screen_);
						for (const auto& t_laser : boss->getListLaser()) {
							t_laser->show(screen_);
						}

						SDL_Flip(screen_);

						SDL_Delay(500);
						TTF_CloseFont(font_countdown);
					}
					boss->setTimeSkill(SDL_GetTicks() - start_countdown);
					time_start += SDL_GetTicks() - start_countdown;
				}
				//win
				if (boss->decreaseHealth(0)) {

					LeaderBoardData new_data(player_name_, score_, survival_time_);
					leader_board_data_.push_back(new_data);

					writeData();

					score_ += boss->getScore();
					boss->explode(screen_);
					SDL_Delay(2000);
					int menu_victory_ret_val = list_menu_[MENU_INDEX::VICTORY]->showMenu(screen_);
					if (menu_victory_ret_val == VictotyMenu::RETURN_VALUE::EXIT_GAME) {
						SDL_Delay(500);
						return;
					}
					else if (menu_victory_ret_val == VictotyMenu::RETURN_VALUE::PLAY_AGAIN) {
						run = true;
						win_stage_1_ = false;
						run_boss_stage = false;
						restart(current_level_);
						is_play_again = true;
						break;
					}
					//writeData();
					return;
				}
				twice++;
				screen_background_in_game_rect.y -= 1;
				if (screen_background_in_game_rect.y == 0) {
					screen_background_in_game_rect.y = 1200;
				}

				if (spaceship_->getHealth() <= 0) {
					writeSaveGame(2, -1, true);
					spaceship_->explode(screen_);
					run_boss_stage = false;
					//SDL_Delay(1000);
					break;
				}
				while (SDL_PollEvent(&event_)) {
					if (event_.type == SDL_QUIT) {
						run_boss_stage = false;
						SDL_Delay(1000);
						writeSaveGame(2, boss->getHealth());
						return;
					}
					else if (event_.type == SDL_KEYDOWN) {
						if (event_.key.keysym.sym == SDLK_ESCAPE) {
							Uint32 start_setting = SDL_GetTicks();
							int ret_val = list_menu_[SETTING]->showMenu(screen_);
							processSettingMenuReturnVal(ret_val, run_boss_stage);
							if (!run_boss_stage) {
								SDL_Delay(500);
								writeSaveGame(2, boss->getHealth());
								return;
							}
							else {
								resume = true;
							}
							time_start += (SDL_GetTicks() - start_setting);
						}
					}
					spaceship_->handleInputAction(event_);
				}
				CommonFunction::applySurface(background_, screen_, &screen_background_in_game_rect, 0, 0);

				if (boss) {
					ThreatObject* ptr_threat = boss;

					//check collision between spaceship and ufo
					if (CommonFunction::checkCollision(spaceship_->getRect(), ptr_threat->getRect())) {
						spaceship_->explode(screen_);

						spaceship_->setRect(SCREEN_WIDTH / 2 - SPACESHIP_WIDTH / 2, SCREEN_HEIGHT - SPACESHIP_HEIGHT);
						SDL_Delay(300);
						//spaceship.show(s_screen);
						ptr_threat->decreaseHealth(5);
						spaceship_->decreaseHealth(1);
					}

					ptr_threat->move();
					ptr_threat->attack(screen_, spaceship_);
					ptr_threat->show(screen_);
				}
				for (int i = 0; i < (int)boss->getListUFO().size(); ++i) {
					ThreatObject* ptr_ufo = boss->getListUFO().at(i);

					//check collision between spaceship and ufo
					if (CommonFunction::checkCollision(spaceship_->getRect(), ptr_ufo->getRect())) {
						spaceship_->explode(screen_);

						spaceship_->setRect(SCREEN_WIDTH / 2 - SPACESHIP_WIDTH / 2, SCREEN_HEIGHT - SPACESHIP_HEIGHT);
						SDL_Delay(300);
						time_start += 300;
						//spaceship.show(s_screen);

						delete ptr_ufo;
						boss->getListUFO().erase(boss->getListUFO().begin() + i);
						ptr_ufo = NULL;
						spaceship_->decreaseHealth(1);
						i--;
						continue;
					}
					if (ptr_ufo->isOuterScreen()) {
						delete ptr_ufo;
						list_threat_object_.erase(list_threat_object_.begin() + i);
						ptr_ufo = NULL;
					}
				}
				spaceship_->move();
				spaceship_->shot(screen_, boss_vect, score_);
				spaceship_->shot(screen_, boss->getListUFO(), score_);
				spaceship_->show(screen_);

				survival_time_ = (SDL_GetTicks() - time_start) / 1000;

				text_time.setTextContent("SURVIVAL TIME: " + to_string(survival_time_) + "s");
				text_time.show(screen_);

				score_text_->setTextContent("SCORE: " + to_string(score_));
				score_text_->show(screen_);

				live_text_->setTextContent("HEALTH: " + to_string(spaceship_->getHealth()));
				live_text_->show(screen_);

				//update screen
				if (SDL_Flip(screen_) == -1) {
					return;
				}
			}
			run_boss_stage = false;
		}

		//gameover menu
		if (!is_play_again) {
			int cur_bkground_music_volume = background_sound_->volume;
			background_sound_->volume = 0;

			Mix_Chunk* game_over_sound = CommonFunction::loadSound(s_game_over_sfx_file_path);
			CommonFunction::playSound(game_over_sound);
			Mix_FreeChunk(game_over_sound);
			background_sound_->volume = cur_bkground_music_volume;

			win_stage_1_ = false;
			int game_over_ret_val = list_menu_[MENU_INDEX::GAME_OVER]->showMenu(screen_);
			processGameOverMenuReturnVal(game_over_ret_val, run);
			if (!run) {
				writeSaveGame(1, -1, true);
				return;
			}
			current_level_ = levelChoosing();
			restart(current_level_);
		}

	} while (run);

}

bool Game::inputPlayerName() {
	player_name_ = "";
	SDL_Surface* temp = CommonFunction::loadImage(main_background_file_path);
	SDL_Rect regtangle = SDL_Rect();
	regtangle.x = 440;
	regtangle.y = 243;
	regtangle.w = 320;
	regtangle.h = 45;
	Uint32 color = SDL_MapRGB(screen_->format, 255, 255, 255);

	TextObject enter_your_name("Enter your name:", TextObject::TEXTCOLOR::GREEN_TEXT, font_);
	enter_your_name.setRect(497, 160);

	TextObject name(player_name_, TextObject::TEXTCOLOR::BLUE_TEXT, font_);
	name.setRect(450, 250);

	//Mix_VolumeChunk(start_game_background_sound_, 0);
	bool done = false;
	bool can_continue = true;
	string cursor = "_";
	while (!done) {
		while (SDL_PollEvent(&event_)) {
			if (event_.type == SDL_QUIT) {
				SDL_FreeSurface(temp);
				return false;
			}
			if (event_.type == SDL_KEYDOWN) {
				if (event_.key.keysym.sym == SDLK_BACKSPACE) {
					if (player_name_.size() > 0) {
						cursor = "_";
						can_continue = true;
						player_name_.pop_back();
					}
				}
				else if (can_continue && event_.key.keysym.sym >= SDLK_a && event_.key.keysym.sym <= SDLK_z) {
					player_name_.push_back(event_.key.keysym.sym);
				}
				else if (can_continue && event_.key.keysym.sym == SDLK_SPACE) {
					player_name_.push_back(event_.key.keysym.sym);
				}
				else if (can_continue && event_.key.keysym.sym >= SDLK_0 && event_.key.keysym.sym <= SDLK_9) {
					player_name_.push_back(event_.key.keysym.sym);
				}
				else if (can_continue && event_.key.keysym.sym == SDLK_RETURN || event_.key.keysym.sym == SDLK_KP_ENTER) {
					if (!player_name_.empty()){
						done = true;
					}
				}
				else 	{
					Mix_Chunk* beep = CommonFunction::loadSound("sounds\\beep.wav");
					beep->volume = 128;
					CommonFunction::playSound(beep);
					Mix_FreeChunk(beep);
				}
			}
			name.setTextContent(player_name_ + cursor);


			CommonFunction::applySurface(temp, screen_, 0, 0);
			enter_your_name.show(screen_);
			SDL_FillRect(screen_, &regtangle, color);
			name.show(screen_);
			SDL_Flip(screen_);
			if (regtangle.w - name.getRect().w < 10) {
				can_continue = false;
				cursor = "";
			}
		}

	}
	while (player_name_.size() > 0 && player_name_.back() == ' ') {
			player_name_.pop_back();
	}
	SDL_FreeSurface(temp);
	return true;
}

int Game::levelChoosing() {
	SDL_Surface* temp = CommonFunction::loadImage(main_background_file_path);

	TextObject please_choose_difficulty("Please choose difficulty:", TextObject::TEXTCOLOR::GREEN_TEXT, font_);
	please_choose_difficulty.setRect(477, 160);
	
	TTF_Font* font = TTF_OpenFont(s_alberto_font_file_path, 2*s_font_size_start_menu);

	TextObject lvl_text[3] = { TextObject("EASY", TextObject::WHITE_TEXT, font),
								TextObject("NORMAL", TextObject::WHITE_TEXT, font),
								TextObject("HARD", TextObject::WHITE_TEXT, font), };
	lvl_text[EASY].setRect(100, 280);
	lvl_text[NORMAL].setRect(491, 280);
	lvl_text[NIGHTMARE].setRect(1200 - 100 - 207, 280);

	//Mix_VolumeChunk(start_game_background_sound_, 0);

	while (true) {
		CommonFunction::applySurface(temp, screen_, 0, 0);
		please_choose_difficulty.show(screen_);
		for (int i = 0; i < 3; i++) {
			lvl_text[i].show(screen_);
		}
		SDL_Flip(screen_);
		while (SDL_PollEvent(&event_)) {
			if (event_.type == SDL_QUIT) {
				SDL_FreeSurface(temp);
				return false;
			}
			if (event_.type == SDL_MOUSEMOTION) {
				int x_mouse = event_.motion.x,
					y_mouse = event_.motion.y;
				for (int i = 0; i < 3; i++) {
					if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, lvl_text[i].getRect())) {
						lvl_text[i].setTextColor(TextObject::TEXTCOLOR::RED_TEXT);
					}
					else 	{
						lvl_text[i].setTextColor(TextObject::TEXTCOLOR::WHITE_TEXT);
					}
				}
			}
			if (event_.type == SDL_MOUSEBUTTONUP){
				int x_mouse = event_.motion.x,
					y_mouse = event_.motion.y;
				for (int i = 0; i < 3; i++) {
					if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, lvl_text[i].getRect())) {
						return i;
					}
				}
			}
		}

	}
	SDL_FreeSurface(temp);
	return true;
}


bool Game::loadData() {

	ifstream leader_board_data(file_path_[FILE_PATH_INDEX::LEADER_BOARD_DATA], ios::binary);

	//name 32yte
	//score 4byte
	//time 4 byte
	while (!leader_board_data.eof()) 	{
		char name[32] = { 0 };
		int score = 0;
		int time = 0;

		leader_board_data.read(name, 32);
		leader_board_data.read((char*)&score, 4);
		leader_board_data.read((char*)&time, 4);
		if (name[0] == 0) {
			break;
		}
		LeaderBoardData data(name, score, time);

		leader_board_data_.push_back(data);
	}
	leader_board_data.close();

	LeaderBoard* ptr = (LeaderBoard*)list_menu_[MENU_INDEX::LEADER_BOARD];
	ptr->setData(leader_board_data_);

	ifstream save_game_file(file_path_[FILE_PATH_INDEX::SAVE_GAME], ios::binary);

	char cur_lvl;
	save_game_file.read(&cur_lvl, 1);
	current_level_ = cur_lvl;
	if (current_level_ == 0) {
		is_new_game_ = true;
		save_game_file.close();
		return false;
	}
	restart(current_level_);
	char name[32] = { 0 };
	save_game_file.read(name, 32);

	is_new_game_ = false;
	player_name_ = string(name);
	save_game_file.read((char*)&score_, 4);
	save_game_file.read((char*)&survival_time_, 4);

	int sp_health;
	save_game_file.read((char*)&sp_health, 4);
	if (sp_health <= 0) {
		is_new_game_ = true;
		save_game_file.close();
		return false;
	}
	spaceship_->setHealth(sp_health);

	save_game_file.read((char*)&section_, 4);

	save_game_file.read((char*)&boss_health_, 4);

	return true;
}

void Game::sortData(){
	for (int i = 0; i < (int)leader_board_data_.size() - 1; i++) {
		for (int j = i + 1; j < (int)leader_board_data_.size(); j++) {
			if (leader_board_data_[i].score_ < leader_board_data_[j].score_) {
				swap(leader_board_data_[i], leader_board_data_[j]);
			}
			else if ((leader_board_data_[i].score_ == leader_board_data_[j].score_) && (leader_board_data_[i].time_ > leader_board_data_[j].time_)) {
				swap(leader_board_data_[i], leader_board_data_[j]);
			}
		}
	}
	while (leader_board_data_.size() > 5) 	{
		leader_board_data_.pop_back();
	}
}

bool Game::writeData() {
	sortData();
	ofstream ld_b(file_path_[FILE_PATH_INDEX::LEADER_BOARD_DATA], ios::binary || ios::trunc);
	for (int i = 0; i < 5; i++) {
		ld_b.write(leader_board_data_[i].name_, 32);
		ld_b.write((char*)&leader_board_data_[i].score_, 4);
		ld_b.write((char*)&leader_board_data_[i].time_, 4);
	}
	ld_b.close();
	return true;
}

bool Game::writeSaveGame(const int& stage, const int& boss_health, const bool& clear_data) {
	if (!clear_data) {
		ofstream save_game(file_path_[FILE_PATH_INDEX::SAVE_GAME], ios::binary || ios::trunc);

		char cur_lvl = current_level_;
		save_game.write(&cur_lvl, 1);
		char name[32] = { 0 };
		for (int i = 0; i < (int)player_name_.size() && i < 32; i++) {
			name[i] = player_name_[i];
		}

		//game
		save_game.write(name, 32);
		save_game.write((char*)&score_, 4);
		int timee = survival_time_;
		save_game.write((char*)&timee, 4);

		//health
		int sp_health = spaceship_->getHealth();
		save_game.write((char*)&sp_health, 4);

		//stage
		save_game.write((char*)&stage, 4);
		save_game.write((char*)&boss_health, 4);

		save_game.close();
		return true;
	}
	else 	{
		char dummy_data[53] = { 0 };
		is_new_game_ = true;
		//cleardata
		ofstream save_game(file_path_[FILE_PATH_INDEX::SAVE_GAME], ios::out && ios::binary && ios::trunc);
		save_game.write(dummy_data, 52);
		save_game.close();
	}
	return true;
}

/*------------------ PROCESS MENU METHOD -------------*/
void Game::processStartGameMenuReturnVal(const int& ret_val, bool& run) {
	int menu_ret_val = -1;
	switch (ret_val) {
	case StartMenu::RETURN_VALUE::CONTINUE:
		run = true;
		//turn off startgame background music
		Mix_VolumeChunk(start_game_background_sound_, 0);
		list_menu_[MENU_INDEX::SETTING]->setOpenFrom(SettingMenu::OPEN_FROM::IN_GAME);
		list_menu_[MENU_INDEX::SETTING]->changeSoundVolume();
		break;
	case StartMenu::RETURN_VALUE::START_GAME:
		run = true;
		if (is_new_game_) {
			if (!inputPlayerName()) {
				exit(1);
			}
		}
		writeSaveGame(1, -1, true);
		current_level_ = levelChoosing();
		restart(current_level_);
		Mix_VolumeChunk(start_game_background_sound_, 0);
		list_menu_[MENU_INDEX::SETTING]->setOpenFrom(SettingMenu::OPEN_FROM::IN_GAME);
		list_menu_[MENU_INDEX::SETTING]->changeSoundVolume();
		break;
	case StartMenu::RETURN_VALUE::SETTING:
		menu_ret_val = list_menu_[MENU_INDEX::SETTING]->showMenu(screen_);
		if (menu_ret_val == SettingMenu::RETURN_VALUE::EXIT_GAME) {
			run = false;
			break;
		}
		processSettingMenuReturnVal(menu_ret_val, run);
		menu_ret_val = list_menu_[MENU_INDEX::START]->showMenu(screen_);
		processStartGameMenuReturnVal(menu_ret_val, run);
		break;
	case StartMenu::RETURN_VALUE::LEADER_BOARD:
		menu_ret_val = list_menu_[MENU_INDEX::LEADER_BOARD]->showMenu(screen_);
		if (menu_ret_val == LeaderBoard::RETURN_VALUE::EXIT_GAME) {
			run = false;
			break;
		}
		processLeaderBoardMenuReturnVal(menu_ret_val, run);
		menu_ret_val = list_menu_[MENU_INDEX::START]->showMenu(screen_);
		processStartGameMenuReturnVal(menu_ret_val, run);
		break;
	case StartMenu::RETURN_VALUE::HELP:
		menu_ret_val = list_menu_[MENU_INDEX::HELP]->showMenu(screen_);
		if (menu_ret_val == -1) {
			run = false;
			return;
		}
		processHelpMenuReturnVal(menu_ret_val, run);
		menu_ret_val = list_menu_[MENU_INDEX::START]->showMenu(screen_);
		processStartGameMenuReturnVal(menu_ret_val, run);
		return;
		break;
	case StartMenu::RETURN_VALUE::EXIT_GAME:
		run = false;
		return;
	default:
		break;
	}
}

void Game::processGameOverMenuReturnVal(const int& ret_val, bool& run) {
	int menu_ret_val = -1;

	switch (ret_val) {
	case GameOverMenu::RETURN_VALUE::RESTART:
		run = true;
		break;
	case GameOverMenu::RETURN_VALUE::MAIN_MENU:
		list_menu_[MENU_INDEX::SETTING]->setOpenFrom(SettingMenu::OPEN_FROM::START_MENU);
		menu_ret_val = list_menu_[MENU_INDEX::START]->showMenu(screen_);
		if (menu_ret_val == StartMenu::RETURN_VALUE::EXIT_GAME) {
			run = false;
			break;
		}
		processStartGameMenuReturnVal(menu_ret_val, run);
		break;
	case GameOverMenu::RETURN_VALUE::SETTING:
		menu_ret_val = list_menu_[MENU_INDEX::SETTING]->showMenu(screen_);
		if (menu_ret_val == SettingMenu::RETURN_VALUE::EXIT_GAME) {
			run = false;
			break;
		}
		processSettingMenuReturnVal(menu_ret_val, run);
		menu_ret_val = list_menu_[MENU_INDEX::GAME_OVER]->showMenu(screen_);
		processGameOverMenuReturnVal(menu_ret_val, run);
		break;
	case GameOverMenu::RETURN_VALUE::LEADER_BOARD:
		menu_ret_val = list_menu_[MENU_INDEX::LEADER_BOARD]->showMenu(screen_);
		if (menu_ret_val == LeaderBoard::RETURN_VALUE::EXIT_GAME) {
			run = false;
			break;
		}
		processLeaderBoardMenuReturnVal(menu_ret_val, run);
		menu_ret_val = list_menu_[MENU_INDEX::GAME_OVER]->showMenu(screen_);
		processGameOverMenuReturnVal(menu_ret_val, run);
		
		break;
	case GameOverMenu::RETURN_VALUE::HELP:
		menu_ret_val = list_menu_[MENU_INDEX::HELP]->showMenu(screen_);
		processHelpMenuReturnVal(menu_ret_val, run);
		menu_ret_val = list_menu_[MENU_INDEX::GAME_OVER]->showMenu(screen_);
		processGameOverMenuReturnVal(menu_ret_val, run);
		return;
		
		break;
	case GameOverMenu::RETURN_VALUE::EXIT_GAME:
		run = false;
	default:
		break;
	}
}


void Game::processSettingMenuReturnVal(const int& ret_val, bool& run) {
	if (ret_val == SettingMenu::RETURN_VALUE::EXIT_GAME) {
		run = false;
		return;
	}
	return;
}
void Game::processLeaderBoardMenuReturnVal(const int& ret_val, bool& run) {
	if (ret_val == LeaderBoard::RETURN_VALUE::BACK) {
		return;
	}
	else if (ret_val == LeaderBoard::RETURN_VALUE::EXIT_GAME) {
		run = false;
		return;
	}
	else if (ret_val == LeaderBoard::RETURN_VALUE::DEFAULT) {
		return;
	}
}
void Game::processHelpMenuReturnVal(const int& ret_val, bool& run) {
	switch (ret_val) 	{
	case 1:
		return;
		break;
	case -1:
		run = false;

	default:
		break;
	}
}