#include "StartMenu.h"

StartMenu::StartMenu() {
	font_ = TTF_OpenFont(s_alberto_font_file_path, s_font_size_start_menu);

	loadImage(main_background_file_path);

	continue_ = new TextObject("CONTINUE", TextObject::WHITE_TEXT, font_);
	
	start_new_game_ = new TextObject("START GAME", TextObject::WHITE_TEXT, font_);
	start_new_game_->setRect(850, 160);

	setting_ = new TextObject("SETTING", TextObject::WHITE_TEXT, font_);
	setting_->setRect(850, 240);

	leader_board_ = new TextObject("LEADER BOARD", TextObject::WHITE_TEXT, font_);
	leader_board_->setRect(850, 320);

	help_ = new TextObject("HELP", TextObject::WHITE_TEXT, font_);
	help_->setRect(850, 400);

	exit_game_ = new TextObject("EXIT GAME", TextObject::WHITE_TEXT, font_);
	exit_game_->setRect(850, 480);

	is_new_game_ = true;
}

StartMenu::~StartMenu() {
	if (ptr_object_ != NULL) {
		SDL_FreeSurface(ptr_object_);
		ptr_object_ = NULL;
	}

	TTF_CloseFont(font_);
	font_ = NULL;

	if (continue_ != NULL) {
		delete continue_;
		continue_ = NULL;
	}

	if (start_new_game_ != NULL) {
		delete start_new_game_;
		start_new_game_ = NULL;
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
int StartMenu::showMenu(SDL_Surface* screen) {
	int ret_val = -1;
	int x_mouse, y_mouse;

	bool selected[5] = { 0 };
	
	SDL_Event m_event;

	while (true) 	{
		show(screen);
		showAllText(screen);

		while (SDL_PollEvent(&m_event)) {
			switch (m_event.type) {
			case SDL_QUIT:
				ret_val = RETURN_VALUE::EXIT_GAME;
				return ret_val;
			case SDL_MOUSEMOTION:
				x_mouse = m_event.motion.x;
				y_mouse = m_event.motion.y;

				mouseMotionCheck(x_mouse, y_mouse);

				break;
			case SDL_MOUSEBUTTONDOWN:
				x_mouse = m_event.motion.x;
				y_mouse = m_event.motion.y;

				ret_val = mouseButtonDown(x_mouse, y_mouse);

				if (ret_val != DEFAULT) {
					return ret_val;
				}
				break;
			default:
				ret_val = -1;
				break;
			}
		}

		SDL_Flip(screen);
	}
	return ret_val;
}

void StartMenu::showAllText(SDL_Surface* screen) {
	if (is_new_game_) {
		start_new_game_->setRect(850, 160);
		setting_->setRect(850, 240);
		leader_board_->setRect(850, 320);
		help_->setRect(850, 400);
		exit_game_->setRect(850, 480);
	}
	else {
		continue_->setRect(850, 120);
		start_new_game_->setRect(850, 200);
		setting_->setRect(850, 280);
		leader_board_->setRect(850, 360);
		help_->setRect(850, 440);
		exit_game_->setRect(850, 520);
	}

	if (!is_new_game_) {
		continue_->show(screen);
	}

	start_new_game_->show(screen);
	setting_->show(screen);
	leader_board_->show(screen);
	help_->show(screen);
	exit_game_->show(screen);
}

void StartMenu::mouseMotionCheck(const int& x_mouse, const int& y_mouse) {
	if (!is_new_game_) {
		if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, continue_->getRect())) {
			continue_->setTextColor(TextObject::YELLOW_TEXT);
		}
		else {
			continue_->setTextColor(TextObject::WHITE_TEXT);
		}
	}
	if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, start_new_game_->getRect())) {
		start_new_game_->setTextColor(TextObject::YELLOW_TEXT);
	}
	else {
		start_new_game_->setTextColor(TextObject::WHITE_TEXT);
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


int StartMenu::mouseButtonDown(const int& x_mouse, const int& y_mouse) {
	if (!is_new_game_) {
		if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, continue_->getRect())) {
			return RETURN_VALUE::CONTINUE;
		}
	}
	if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, start_new_game_->getRect())) {
		return RETURN_VALUE::START_GAME;
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

	return RETURN_VALUE::DEFAULT;
}

void StartMenu::set_is_new_game(const int& flag_val) {
	is_new_game_ = flag_val;
	if (is_new_game_) {
		start_new_game_->setRect(850, 160);
		setting_->setRect(850, 240);
		leader_board_->setRect(850, 320);
		help_->setRect(850, 400);
		exit_game_->setRect(850, 480);
	}
	else {
		continue_->setRect(850, 140);
		start_new_game_->setRect(850, 220);
		setting_->setRect(850, 280);
		leader_board_->setRect(850, 360);
		help_->setRect(850, 440);
		exit_game_->setRect(850, 520);
	}
}
bool StartMenu::get_is_new_game() {
	return is_new_game_;
}