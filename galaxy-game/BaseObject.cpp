#include "BaseObject.h"

BaseObject::BaseObject() {
	rect_.x = 0;
	rect_.y = 0;
	rect_.w = 0;
	rect_.h = 0;

	ptr_object_ = NULL;

	x_val_ = 0;
	y_val_ = 0;
}
BaseObject::~BaseObject() {
	if (ptr_object_ != NULL) {
		SDL_FreeSurface(ptr_object_);
		ptr_object_ = NULL;
	}
}

void BaseObject::show(SDL_Surface* des) {
	if (ptr_object_ != NULL)
		rect_ = CommonFunction::applySurface(ptr_object_, des, rect_.x, rect_.y);
}

bool BaseObject::loadImage(const string& file_path, const int& red, const int& green, const int& blue) {
	ptr_object_ = CommonFunction::loadImage(file_path, red, green, blue);
	return (ptr_object_ != NULL);

}
void BaseObject::setRect(const int& x, const int& y) {
	rect_.x = x;
	rect_.y = y;
}

void BaseObject::setWidthHeight(const int& w, const int& h) {
	rect_.w = w;
	rect_.h = h;
}

void BaseObject::set_x_val_y_val(const int& x_val, const int& y_val) {
	x_val_ = x_val;
	y_val_ = y_val;
}

void BaseObject::set_x_val(const int& x_val) {
	x_val_ = x_val;
}

int BaseObject::get_x_val() {
	return x_val_;
}

void BaseObject::set_y_val(const int& y_val) {
	y_val_ = y_val;
}

int BaseObject::get_y_val() {
	return y_val_;
}

SDL_Rect BaseObject::getRect() {
	return rect_;
}
SDL_Surface* BaseObject::getSurface() {
	return ptr_object_;
}