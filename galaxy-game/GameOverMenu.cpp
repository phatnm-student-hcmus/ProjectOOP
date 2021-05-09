#include "GameOverMenu.h"
GameOverMenu::GameOverMenu() {
	font_ = TTF_OpenFont(s_alberto_font_file_path, s_font_size_start_menu - 16);

	loadImage(s_game_over_menu_img_file_path);

	restart_game_ = new TextObject("RESTART", TextObject::WHITE_TEXT, font_);
	restart_game_->setRect(536, 200);

	main_menu_ = new TextObject("MAIN MENU", TextObject::TEXTCOLOR::WHITE_TEXT, font_);
	main_menu_->setRect(517, 260);

	setting_ = new TextObject("SETTING", TextObject::WHITE_TEXT, font_);
	setting_->setRect(538, 320);

	leader_board_ = new TextObject("LEADER BOARD", TextObject::WHITE_TEXT, font_);
	leader_board_->setRect(492, 380);

	help_ = new TextObject("HELP", TextObject::WHITE_TEXT, font_);
	help_->setRect(563, 440);

	exit_game_ = new TextObject("EXIT GAME", TextObject::WHITE_TEXT, font_);
	exit_game_->setRect(518, 500);
}

GameOverMenu::~GameOverMenu() {
	if (ptr_object_ != NULL) {
		SDL_FreeSurface(ptr_object_);
		ptr_object_ = NULL;
	}

	TTF_CloseFont(font_);
	font_ = NULL;

	if (restart_game_ != NULL) {
		delete restart_game_;
		restart_game_ = NULL;
	}

	if (main_menu_ != NULL) {
		delete main_menu_;
		main_menu_ = NULL;
	}

	if (setting_ != NULL) {
		delete setting_;
		setting_ = NULL;
	}

	if (leader_board_ != NULL) {
		delete leader_board_;
		leader_board_ = NULL;
	}

	if (help_ != NULL) {
		delete help_;
		help_ = NULL;
	}

	if (exit_game_ != NULL) {
		delete exit_game_;
		exit_game_ = NULL;
	}
}
int GameOverMenu::showMenu(SDL_Surface* screen) {
	int x_mouse, y_mouse;

	bool selected[5] = { 0 };

	SDL_Event m_event;
	int ret_val = -1;
	while (true) {
		show(screen);
		showAllText(screen);

		while (SDL_PollEvent(&m_event)) {
			switch (m_event.type) {
			case SDL_QUIT:
				return RETURN_VALUE::EXIT_GAME;
			case SDL_MOUSEMOTION:
				x_mouse = m_event.motion.x;
				y_mouse = m_event.motion.y;

				mouseMotionCheck(x_mouse, y_mouse);

				break;
			case SDL_MOUSEBUTTONDOWN:
				ret_val = mouseButtonDown(x_mouse, y_mouse);
				if (ret_val != -1) {
					return ret_val;
				}
				break;
			default:
				break;
			}
		}
		SDL_Flip(screen);
	}
}

void GameOverMenu::showAllText(SDL_Surface* screen) {
	restart_game_->show(screen);
	main_menu_->show(screen);
	setting_->show(screen);
	leader_board_->show(screen);
	help_->show(screen);
	exit_game_->show(screen);
}

void GameOverMenu::mouseMotionCheck(const int& x_mouse, const int& y_mouse) {
	if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, restart_game_->getRect())) {
		restart_game_->setTextColor(TextObject::YELLOW_TEXT);
	}
	else {
		restart_game_->setTextColor(TextObject::WHITE_TEXT);
	}

	if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, main_menu_->getRect())) {
		main_menu_->setTextColor(TextObject::YELLOW_TEXT);
	}
	else {
		main_menu_->setTextColor(TextObject::WHITE_TEXT);
	}

	if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, setting_->getRect())) {
		setting_->setTextColor(TextObject::YELLOW_TEXT);
	}
	else {
		setting_->setTextColor(TextObject::WHITE_TEXT);
	}

	if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, leader_board_->getRect())) {
		leader_board_->setTextColor(TextObject::YELLOW_TEXT);
	}
	else {
		leader_board_->setTextColor(TextObject::WHITE_TEXT);
	}

	if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, help_->getRect())) {
		help_->setTextColor(TextObject::YELLOW_TEXT);
	}
	else {
		help_->setTextColor(TextObject::WHITE_TEXT);
	}

	if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, exit_game_->getRect())) {
		exit_game_->setTextColor(TextObject::YELLOW_TEXT);
	}
	else {
		exit_game_->setTextColor(TextObject::WHITE_TEXT);
	}
}
int GameOverMenu::mouseButtonDown(const int& x_mouse, const int& y_mouse) {
	if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, restart_game_->getRect())) {
		return RETURN_VALUE::RESTART;
	}

	if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, main_menu_->getRect())) {
		return RETURN_VALUE::MAIN_MENU;
	}

	if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, setting_->getRect())) {
		return RETURN_VALUE::SETTING;
	}

	if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, leader_board_->getRect())) {
		return RETURN_VALUE::LEADER_BOARD;
	}

	if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, help_->getRect())) {
		return RETURN_VALUE::HELP;
	}

	if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, exit_game_->getRect())) {
		return RETURN_VALUE::EXIT_GAME;
	}
	return -1;
}