#include "VictotyMenu.h"

VictotyMenu::VictotyMenu() {
	font_ = TTF_OpenFont(s_alberto_font_file_path, s_font_size_start_menu);

	loadImage(s_victory_background_file_path);

	play_again_ = new TextObject("PLAY AGAIN", TextObject::TEXTCOLOR::WHITE_TEXT, font_);
	play_again_->setRect(488, 300);

	exit_game_ = new TextObject("EXIT GAME", TextObject::TEXTCOLOR::WHITE_TEXT, font_);
	exit_game_->setRect(494, 400);

}
VictotyMenu::~VictotyMenu() {
	delete play_again_;
	delete exit_game_;
}

int VictotyMenu::showMenu(SDL_Surface* screen) {
	int ret_val = -1;
	int x_mouse, y_mouse;
	while (true) {
		show(screen);
		showAllText(screen);

		SDL_Event m_event;
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

				if (ret_val != RETURN_VALUE::DEFAULT) {
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
	return -1;
}
void VictotyMenu::showAllText(SDL_Surface* screen) {
	if (play_again_) {
		play_again_->show(screen);
	}

	if (exit_game_) {
		exit_game_->show(screen);
	}
}

void VictotyMenu::mouseMotionCheck(const int& x_mouse, const int& y_mouse) {
	if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, play_again_->getRect())) {
		play_again_->setTextColor(TextObject::TEXTCOLOR::RED_TEXT);
	}
	else 	{
		play_again_->setTextColor(TextObject::TEXTCOLOR::WHITE_TEXT);
	}

	if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, exit_game_->getRect())) {
		exit_game_->setTextColor(TextObject::TEXTCOLOR::RED_TEXT);
	}
	else {
		exit_game_->setTextColor(TextObject::TEXTCOLOR::WHITE_TEXT);
	}
}
int VictotyMenu::mouseButtonDown(const int& x_mouse, const int& y_mouse) {
	if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, play_again_->getRect())) {
		return RETURN_VALUE::PLAY_AGAIN;
	}

	if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, exit_game_->getRect())) {
		return RETURN_VALUE::EXIT_GAME;
	}

	return RETURN_VALUE::DEFAULT;
}