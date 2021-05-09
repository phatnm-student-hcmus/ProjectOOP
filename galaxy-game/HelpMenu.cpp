#include "HelpMenu.h"

HelpMenu::HelpMenu() {
	font_ = TTF_OpenFont(s_showcard_gothic_font_file_path, s_font_size_start_menu);

	loadImage(s_help_menu_img_file_path, 123, 112,123);
	setRect(0, 0);
	setWidthHeight(HELP_MENU_WIDTH, HELP_MENU_HEIGHT);

	back_button_ = new TextObject("Back", TextObject::TEXTCOLOR::WHITE_TEXT, font_);
	back_button_->setRect(150, SCREEN_HEIGHT - 100);
}
HelpMenu::~HelpMenu() {
	if (ptr_object_ != NULL) {
		SDL_FreeSurface(ptr_object_);
		ptr_object_ = NULL;
	}
	if (back_button_ != NULL) {
		delete back_button_;
		back_button_ = NULL;
	}
}

int HelpMenu::showMenu(SDL_Surface* screen) {
	int ret_val = -1;
	int x_mouse, y_mouse;

	SDL_Event m_event;

	while (true) {
		show(screen);
		showAllText(screen);

		while (SDL_PollEvent(&m_event)) {
			switch (m_event.type) {
			case SDL_QUIT:
				ret_val = -1;
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

				if (ret_val != -1) {
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
void HelpMenu::showAllText(SDL_Surface* screen) {
	back_button_->show(screen);
}

void HelpMenu::mouseMotionCheck(const int& x_mouse, const int& y_mouse) {
	if (x_mouse >= back_button_->getRect().x && x_mouse <= back_button_->getRect().x + back_button_->getRect().w &&
		y_mouse >= back_button_->getRect().y && y_mouse <= back_button_->getRect().y + back_button_->getRect().h) 	{
		back_button_->setTextColor(TextObject::TEXTCOLOR::YELLOW_TEXT);
	}
	else 	{
		back_button_->setTextColor(TextObject::TEXTCOLOR::WHITE_TEXT);
	}
}
int HelpMenu::mouseButtonDown(const int& x_mouse, const int& y_mouse) {
	if (x_mouse >= back_button_->getRect().x && x_mouse <= back_button_->getRect().x + back_button_->getRect().w &&
		y_mouse >= back_button_->getRect().y && y_mouse <= back_button_->getRect().y + back_button_->getRect().h) {
		return 1;
	}
	return -1;
}
