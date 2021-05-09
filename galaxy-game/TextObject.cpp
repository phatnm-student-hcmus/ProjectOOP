#include "TextObject.h"

TextObject::TextObject() {
	text_content_ = "";
	text_color_ = { 0,0,0 };
	text_font_ = NULL;
}

TextObject::TextObject(const string& content, const int& color, TTF_Font* font) {
	text_content_ = content;

	setTextColor(color);

	text_font_ = font;
}

TextObject::~TextObject(){
	text_font_ = NULL;
}

void TextObject::show(SDL_Surface* screen) {
	ptr_object_ = TTF_RenderText_Blended(text_font_, text_content_.c_str(), text_color_);

	rect_ = CommonFunction::applySurface(ptr_object_, screen, rect_.x, rect_.y);

	SDL_FreeSurface(ptr_object_);
	ptr_object_ = NULL;
}

void TextObject::setTextContent(string text) {

	text_content_ = text;

}

string TextObject::getTextContent(){
	return text_content_;
}
void TextObject::setTextColor(const int& type) {
	switch (type) 	{
	case RED_TEXT:
		text_color_ = { 0xff, 0, 0 };
		break;
	case GREEN_TEXT:
		text_color_ = { 0, 0xff, 0 };
		break;
	case BLUE_TEXT:
		text_color_ = { 0,0,0xff };
		break;
	case YELLOW_TEXT:
		text_color_ = { 246, 239, 122 };
		break;
	case WHITE_TEXT:
		text_color_ = { 0xff, 0xff, 0xff };
		break;
	case BLACK_TEXT:
		text_color_ = { 0,0,0 };
		break;
	case LIGHT_BLACK_TEXT:
		text_color_ = { 96,96,96 };
		break;
	default:
		break;
	}
}
void TextObject::setFont(TTF_Font* font) {
	text_font_ = font;
}