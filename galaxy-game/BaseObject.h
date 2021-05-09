#ifndef _BASE_OBJECT_H_
#define _BASE_OBJECT_H_

#include "CommonFunction.h"

using namespace std;

class BaseObject {
protected:
	SDL_Rect rect_;
	SDL_Surface* ptr_object_;
	int x_val_, y_val_;

public:
	BaseObject();
	virtual ~BaseObject() = 0;

	virtual void show(SDL_Surface* des);
	bool loadImage(const string& file_path, const int& red = 0, const int& green = 0, const int& blue = 0);

	void setRect(const int& x, const int& y);
	void setWidthHeight(const int& w, const int& h);

	void set_x_val_y_val(const int& x_val, const int& y_val);
	void set_x_val(const int& x_val);
	int get_x_val();
	void set_y_val(const int& y_val);
	int get_y_val();

	SDL_Rect getRect();
	SDL_Surface* getSurface();
};
#endif // !_BASE_OBJECT_H



