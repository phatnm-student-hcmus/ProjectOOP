#include "Menu.h"

Menu::~Menu() {
	if (ptr_object_ != NULL) {
		SDL_FreeSurface(ptr_object_);
		ptr_object_ = NULL;
	}
	TTF_CloseFont(font_);
	font_ = NULL;
}