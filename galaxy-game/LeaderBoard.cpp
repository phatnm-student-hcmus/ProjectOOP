#include "LeaderBoard.h"

LeaderBoard::LeaderBoard() {
	font_ = TTF_OpenFont(s_alberto_font_file_path, s_font_size_in_game);

	loadImage(s_leader_board_background_file_path);

	for (int i = 0; i < SIZE_LIST; i++) {
		name_[i] = NULL;
		score_[i] = NULL;
		time_[i] = NULL;
	}
	TTF_Font* font_menu = TTF_OpenFont(s_alberto_font_file_path, s_font_size_start_menu);
	back_ = new TextObject("BACK", TextObject::TEXTCOLOR::WHITE_TEXT, font_menu);
	back_->setRect(550, 510);
}

LeaderBoard::~LeaderBoard() {
	for (int i = 0; i < SIZE_LIST; i++) {
		name_[i] = score_[i] = time_[i] = NULL;
	}
	delete back_;
	back_ = NULL;
}
int LeaderBoard::showMenu(SDL_Surface* screen) {
	int ret_val = -1;
	int x_mouse, y_mouse;

	bool selected[5] = { 0 };

	SDL_Event m_event;

	while (true) {
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
void LeaderBoard::showAllText(SDL_Surface* screen) {
	for (int i = 0; i < SIZE_LIST; i++) {
		if (name_[i] != NULL) {
			name_[i]->show(screen);
		}
		if (score_[i] != NULL) {
			score_[i]->show(screen);
		}
		if (time_[i] != NULL) {
			time_[i]->show(screen);
		}
	}
	back_->show(screen);
}

void LeaderBoard::mouseMotionCheck(const int& x_mouse, const int& y_mouse) {
	if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, back_->getRect())) {
		back_->setTextColor(TextObject::TEXTCOLOR::YELLOW_TEXT);
	}
	else {
		back_->setTextColor(TextObject::TEXTCOLOR::WHITE_TEXT);
	}
}
int LeaderBoard::mouseButtonDown(const int& x_mouse, const int& y_mouse) {
	if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, back_->getRect())) {
		return 1;
	}
	return -1;
}

void LeaderBoard::setData(vector<LeaderBoardData> data) {
	for (int i = 0; i < (int)data.size() && i < 5; i++) {
		if (score_[i] != NULL) {
			name_[i]->setTextContent(data[i].name_);
			name_[i]->setRect(340, 230 + 50 * i);

			score_[i]->setTextContent(to_string(data[i].score_));
			score_[i]->setRect(700, 230 + 50 * i);

			time_[i]->setTextContent(to_string(data[i].time_));
			time_[i]->setRect(800, 230 + 50 * i);

		}
		else 	{
			name_[i] = new TextObject(data[i].name_, TextObject::TEXTCOLOR::LIGHT_BLACK_TEXT, font_);
			name_[i]->setRect(340, 230 + 50 * i);

			score_[i] = new TextObject(to_string(data[i].score_), TextObject::TEXTCOLOR::WHITE_TEXT, font_);
			score_[i]->setRect(700, 230 + 50 * i);

			time_[i] = new TextObject(to_string(data[i].time_), TextObject::TEXTCOLOR::WHITE_TEXT, font_);
			time_[i]->setRect(800, 230 + 50 * i);
		}
	}
}