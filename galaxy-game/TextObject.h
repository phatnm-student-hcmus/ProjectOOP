#pragma once
#include "BaseObject.h"
#include <SDL_ttf.h>

class TextObject : public BaseObject {
private:

	string text_content_;
	SDL_Color text_color_;
	TTF_Font* text_font_;

public:
	enum TEXTCOLOR {
		RED_TEXT = 1,
		GREEN_TEXT = 2,
		BLUE_TEXT = 3,
		YELLOW_TEXT = 4,
		WHITE_TEXT = 5,
		BLACK_TEXT = 6,
		LIGHT_BLACK_TEXT = 7
	};

	TextObject();
	TextObject(const string& content, const int& color, TTF_Font* font);
	~TextObject();

	void show(SDL_Surface* screen);

	void setTextContent(string text);
	string getTextContent();

	void setTextColor(const int& type);
	void setFont(TTF_Font*);

};