#include "SettingMenu.h"

const int SettingMenu::s_max_volume[6] = { 128,64,15,128,128,128 };
int SettingMenu::s_prev_volume[6] = { 128,64,15,128,128,128 };

SettingMenu::SettingMenu() {
	m_event_ = SDL_Event();
	ptr_screen_ = NULL;
	for (int i = 0; i < 4; i++) {
		sound_[i] = NULL;
		icons[i] = NULL;
		percent_volume_[i] = NULL;
		line_rect_[i] = SDL_Rect();
		sound_mark_rect_[i] = SDL_Rect();
		line_color_[i] = 0;
		sound_mark_color_[i] = 0;
	}
	sound_[4] = NULL;
	sound_[5] = NULL;
	dragging_ = -1;

	open_from_ = OPEN_FROM::START_MENU;
}

SettingMenu::SettingMenu(SDL_Surface* screen) {
	ptr_screen_ = screen;
	loadImage(s_setting_menu_in_game_img_file_path, 1);

	font_ = TTF_OpenFont(s_alberto_font_file_path, s_font_size_start_menu);
	for (int i = 0; i < 4; i++) {
		sound_[i] = NULL;
		icons[i] = new AudioIcon;
		percent_volume_[i] = new TextObject("100% ", TextObject::TEXTCOLOR::WHITE_TEXT, font_);

		sound_mark_rect_[i].x = 490 + LINE_WIDTH_1 - SOUND_MARK_WIDTH/2; 
		//cout << "rect 0: " << sound_mark_rect_[i].x << endl;
		sound_mark_rect_[i].w = SOUND_MARK_WIDTH;
		sound_mark_rect_[i].h = SOUND_MARK_HEIGHT;
		//blue 
		
		//green
		
	}

	sound_[4] = NULL;
	sound_[5] = NULL;
	line_rect_[0].x = 490;
	line_rect_[0].y = 265 - LINE_HEIGHT/2;
	line_rect_[0].w = LINE_WIDTH_1;
	line_rect_[0].h = LINE_HEIGHT;
	line_color_[0] = SDL_MapRGB(screen->format, 109, 207, 246);

	line_rect_[1].x = 680;
	line_rect_[1].y = 340 - LINE_HEIGHT / 2;
	line_rect_[1].w = LINE_WIDTH_2;
	line_rect_[1].h = LINE_HEIGHT;
	line_color_[1] = SDL_MapRGB(screen->format, 109, 207, 246);

	line_rect_[2].x = 680;
	line_rect_[2].y = 400 - LINE_HEIGHT / 2;
	line_rect_[2].w = LINE_WIDTH_2;
	line_rect_[2].h = LINE_HEIGHT;
	line_color_[2] = SDL_MapRGB(screen->format, 109, 207, 246);

	line_rect_[3].x = 680;
	line_rect_[3].y = 460 - LINE_HEIGHT / 2;
	line_rect_[3].w = LINE_WIDTH_2;
	line_rect_[3].h = LINE_HEIGHT;
	line_color_[3] = SDL_MapRGB(screen->format, 109, 207, 246);

	sound_mark_rect_[0].y = 265 - LINE_HEIGHT / 2 - SOUND_MARK_HEIGHT / 2;
	sound_mark_rect_[1].y = 340 - LINE_HEIGHT / 2 - SOUND_MARK_HEIGHT / 2;
	sound_mark_rect_[2].y = 400 - LINE_HEIGHT / 2 - SOUND_MARK_HEIGHT / 2;
	sound_mark_rect_[3].y = 460 - LINE_HEIGHT / 2 - SOUND_MARK_HEIGHT / 2;

	sound_mark_color_[0] = SDL_MapRGB(screen->format, 109, 207, 246);
	sound_mark_color_[1] = SDL_MapRGB(screen->format, 140, 200, 96);
	sound_mark_color_[2] = SDL_MapRGB(screen->format, 140, 200, 96);
	sound_mark_color_[3] = SDL_MapRGB(screen->format, 140, 200, 96);

	icons[0]->setRect(930, 265 - LINE_HEIGHT / 2 - icons[0]->getRect().h / 2);
	icons[1]->setRect(930, 340 - LINE_HEIGHT / 2 - icons[1]->getRect().h / 2);
	icons[2]->setRect(930, 400 - LINE_HEIGHT / 2 - icons[2]->getRect().h / 2);
	icons[3]->setRect(930, 460 - LINE_HEIGHT / 2 - icons[3]->getRect().h / 2);

	percent_volume_[0]->setRect(1000, 265 - LINE_HEIGHT / 2 - 72 / 2);
	percent_volume_[1]->setRect(1000, 340 - LINE_HEIGHT / 2 - 72 / 2);
	percent_volume_[2]->setRect(1000, 400 - LINE_HEIGHT / 2 - 72 / 2);
	percent_volume_[3]->setRect(1000, 460 - LINE_HEIGHT / 2 - 72 / 2);
	dragging_ = -1;


	resume_ = TextObject("RESUME", TextObject::TEXTCOLOR::WHITE_TEXT, font_);
	resume_.setRect(SCREEN_WIDTH/2 - 112/2, 150);

	exit_game_ = TextObject("EXIT GAME", TextObject::TEXTCOLOR::WHITE_TEXT, font_);
	exit_game_.setRect(100, 500);

	open_from_ = OPEN_FROM::START_MENU;
}
SettingMenu::~SettingMenu() {
	ptr_screen_ = NULL;
	for (int i = 0; i < 6; i++) {
		sound_[i] = NULL;
	}
	for (int i = 0; i < 4; i++) {
		delete percent_volume_[i];
	}
}


int SettingMenu::showMenu(SDL_Surface* screen) {
	int ret_val = -1;
	int x_mouse, y_mouse;
	while (true) 	{
		changeSoundVolume();
		show(screen);
		showAllText(screen);
		//show rectangle
		for (int i = 0; i < 4; i++) {
			icons[i]->show(screen);
			SDL_FillRect(screen, &line_rect_[i], line_color_[i]);
			SDL_FillRect(screen, &sound_mark_rect_[i], sound_mark_color_[i]);
		}

		while (SDL_PollEvent(&m_event_)) 	{
			switch (m_event_.type) {
			case SDL_QUIT:
				ret_val = RETURN_VALUE::EXIT_GAME;
				return ret_val;
			case SDL_MOUSEMOTION:
				x_mouse = m_event_.motion.x;
				y_mouse = m_event_.motion.y;

				mouseMotionCheck(x_mouse, y_mouse);

				break;
			case SDL_MOUSEBUTTONDOWN:
				x_mouse = m_event_.motion.x;
				y_mouse = m_event_.motion.y;

				ret_val = mouseButtonDown(x_mouse, y_mouse);
				
				switch (ret_val) 	{
				case MOUSE_BTN_DOWN_RET::NOTHING:
					break;
				case MOUSE_BTN_DOWN_RET::RESUME:
					return RETURN_VALUE::RESUME_GAME;
				case MOUSE_BTN_DOWN_RET::CHANGE_VOLUME:
					break;
				case MOUSE_BTN_DOWN_RET::EXIT:
					return RETURN_VALUE::EXIT_GAME;
				case MOUSE_BTN_DOWN_RET::DRAG:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				x_mouse = m_event_.motion.x;
				y_mouse = m_event_.motion.y;
				ret_val = mouseButtonDown(x_mouse, y_mouse);

				dragging_ = -1;
				changeSoundVolume();
				break;
			case SDL_KEYDOWN:
				if (m_event_.key.keysym.sym == SDLK_ESCAPE) {
					return RETURN_VALUE::RESUME_GAME;
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
void SettingMenu::showAllText(SDL_Surface* screen) {
	int percent_volume[4] = { 0 };

	//music
	if (sound_[0]) {
		if (open_from_ == OPEN_FROM::START_MENU) {
			percent_volume[0] = sound_[0]->volume * 100 / s_max_volume[0];
		}
	}
	if (sound_[1]) {
		if (open_from_ == OPEN_FROM::IN_GAME) {
			percent_volume[0] = sound_[1]->volume * 100 / s_max_volume[1];
		}
	}
	if (sound_[2]) {
		percent_volume[1] = sound_[2]->volume * 100 / s_max_volume[2];
	}
	if (sound_[3]) {
		percent_volume[2] = sound_[3]->volume * 100 / s_max_volume[3];
	}
	if (sound_[4]) {
		percent_volume[2] = sound_[4]->volume * 100 / s_max_volume[4];
	}
	if (sound_[5]) {
		percent_volume[3] = sound_[5]->volume * 100 / s_max_volume[5];
	}

	for (int i = 0; i < 4; i++) {
		percent_volume_[i]->setTextContent(to_string(percent_volume[i]) + "%");
		percent_volume_[i]->show(screen);
	}
	resume_.show(screen);
	exit_game_.show(screen);
}

void SettingMenu::mouseMotionCheck(const int& x_mouse, const int& y_mouse){
	if (dragging_ == -1) {
		for (int i = 0; i < 4; i++) {
			if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, sound_mark_rect_[i])) 		
			{
				//red
				sound_mark_color_[i] = SDL_MapRGB(ptr_screen_->format, 0xff, 0, 0);
			}
			else {
				//blue
				sound_mark_color_[i] = SDL_MapRGB(ptr_screen_->format, 0, 0xff, 0);
			}
		}
		if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, resume_.getRect())) 				
		{
			resume_.setTextColor(TextObject::TEXTCOLOR::YELLOW_TEXT);
		}
		else 	{
			resume_.setTextColor(TextObject::TEXTCOLOR::WHITE_TEXT);
		}
		if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, exit_game_.getRect())) 		{
			exit_game_.setTextColor(TextObject::TEXTCOLOR::YELLOW_TEXT);
		}
		else {
			exit_game_.setTextColor(TextObject::TEXTCOLOR::WHITE_TEXT);
		}
	}
	else 	{
		sound_mark_color_[dragging_] = SDL_MapRGB(ptr_screen_->format, 0xff, 0, 0);
	}
}

int SettingMenu::mouseButtonDown(const int& x_mouse, const int& y_mouse) {
	int ret = -1;
	if (dragging_ == -1) {
		for (int i = 0; i < 4; i++) {
			if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, sound_mark_rect_[i])) 			
			{
				dragging_ = i;
				cout << dragging_ << endl;
				return DRAG;
			}
			else if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, icons[i]->getRect()) && m_event_.type != SDL_MOUSEBUTTONDOWN) {
				if (icons[i]->get_is_mute() == false) {
					icons[i]->set_is_mute(true);
					sound_mark_rect_[i].x = line_rect_[i].x;
				}
				else 	{
					icons[i]->set_is_mute(false);
					sound_mark_rect_[i].x = line_rect_[i].x + line_rect_[i].w - sound_mark_rect_[i].w / 2;
				}
			}
		}
		if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, resume_.getRect())) 		{
			ret = MOUSE_BTN_DOWN_RET::RESUME;
		}

		if (CommonFunction::checkMouseFocusWithRect(x_mouse, y_mouse, exit_game_.getRect())) {
			ret = MOUSE_BTN_DOWN_RET::EXIT;
		}
	}
	else 	{
		//p: index of sound mark
		int p = dragging_;

		sound_mark_rect_[p].x = MAX(line_rect_[p].x, MIN(x_mouse, line_rect_[p].x + line_rect_[p].w - sound_mark_rect_[p].w / 2));
		if (sound_mark_rect_[p].x == line_rect_[p].x) {
			icons[p]->set_is_mute(true);
			sound_mark_rect_[p].x = line_rect_[p].x;
		}
		else 	{
			icons[p]->set_is_mute(false);
		}
		ret = 1;
	}
	return ret;
}

void SettingMenu::setSound(Mix_Chunk* s1, Mix_Chunk* s2, Mix_Chunk* s3, Mix_Chunk* s4, Mix_Chunk* s5, Mix_Chunk* s6) {
	sound_[0] = s1;
	sound_[1] = s2;
	sound_[2] = s3;
	sound_[3] = s4;
	sound_[4] = s5;
	sound_[5] = s6;
}

void  SettingMenu::changeSoundVolume() {
	int percent_volume[4] = { 0 };
	for (int i = 0; i < 4; i++) {
		percent_volume[i] = 100 * (sound_mark_rect_[i].x - line_rect_[i].x) / (line_rect_[i].w - sound_mark_rect_[i].w/2);
	}

	//music
	if (sound_[0] && sound_[1]) {
		if (open_from_ == OPEN_FROM::START_MENU) {
			sound_[0]->volume = percent_volume[0] * MAX_VOLUME::MAX_VOL_0 / 100;
			sound_[1]->volume = 0;
		}
		else if (open_from_ == OPEN_FROM::IN_GAME) {
			sound_[0]->volume = 0;
			sound_[1]->volume = percent_volume[0] * MAX_VOLUME::MAX_VOL_1 / 100;
		}
	}
	if (sound_[2]) {
		sound_[2]->volume = percent_volume[1] * 15 / 100;
	}
	if (sound_[3]) {
		sound_[3]->volume = percent_volume[2] * 128 / 100;
	}
	if (sound_[4]) {
		sound_[4]->volume = percent_volume[2] * 128 / 100;
	}
	if (sound_[5]) {
		sound_[5]->volume = percent_volume[3] * 128 / 100;
	}

}


bool SettingMenu::getOpenFrom() {
	return open_from_;
}
void SettingMenu::setOpenFrom(const bool& state) {
	open_from_ = state;
	if (state == OPEN_FROM::IN_GAME) {
		if (sound_[0])
			sound_[0]->volume = 0;
		if (sound_[1])
		Mix_VolumeChunk(sound_[1], MAX_VOLUME::MAX_VOL_1);
	}
	else 	{
		if (sound_[0])
			Mix_VolumeChunk(sound_[0], MAX_VOLUME::MAX_VOL_0);
		if(sound_[1])
			sound_[1]->volume = 0;
	}
}