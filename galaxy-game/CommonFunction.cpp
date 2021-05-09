#include "CommonFunction.h"
#include "TextObject.h"

SDL_Surface* CommonFunction::loadImage(string file_path, const int& red, const int& green, const int& blue) {
	SDL_Surface* load_image = NULL,
		* optimize_image = NULL;

	load_image = IMG_Load(file_path.c_str());
	if (load_image != NULL) {
		optimize_image = SDL_DisplayFormat(load_image);
		SDL_FreeSurface(load_image);
	}
	else {
		cout << "cannot open image";
	}


	if (optimize_image != NULL) {
		Uint32 color_key = SDL_MapRGB(optimize_image->format, red, green, blue);
		SDL_SetColorKey(optimize_image, SDL_SRCCOLORKEY, color_key);
	}
	return optimize_image;
}

SDL_Rect CommonFunction::applySurface(SDL_Surface* src, SDL_Surface* des, const int& x, const int& y) {
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	SDL_BlitSurface(src, NULL, des, &offset);

	return offset;
}

SDL_Rect CommonFunction::applySurface(SDL_Surface* src, SDL_Surface* des, SDL_Rect* clip, const int& x, const int& y) {
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	try {
		SDL_BlitSurface(src, clip, des, &offset);
	}
	catch (...) {
		;
	}

	return offset;
}

/*return true if*/
bool CommonFunction::checkCollision(const SDL_Rect& object1, const SDL_Rect& object2) {
	int left_a = object1.x;
	int right_a = object1.x + object1.w;
	int top_a = object1.y;
	int bottom_a = object1.y + object1.h;

	int left_b = object2.x;
	int right_b = object2.x + object2.w;
	int top_b = object2.y;
	int bottom_b = object2.y + object2.h;

	// Case 1: size object 1 < size object 2
	if (left_a > left_b && left_a < right_b)   {
		if (top_a > top_b && top_a < bottom_b)     {
			return true;
		}
	}

	if (left_a > left_b && left_a < right_b)   {
		if (bottom_a > top_b && bottom_a < bottom_b)     {
			return true;
		}
	}

	if (right_a > left_b && right_a < right_b)   {
		if (top_a > top_b && top_a < bottom_b)     {
			return true;
		}
	}

	if (right_a > left_b && right_a < right_b)   {
		if (bottom_a > top_b && bottom_a < bottom_b)     {
			return true;
		}
	}

	// Case 2: size object 1 < size object 2
	if (left_b > left_a && left_b < right_a)   {
		if (top_b > top_a && top_b < bottom_a)     {
			return true;
		}
	}

	if (left_b > left_a && left_b < right_a)   {
		if (bottom_b > top_a && bottom_b < bottom_a)     {
			return true;
		}
	}

	if (right_b > left_a && right_b < right_a)   {
		if (top_b > top_a && top_b < bottom_a)     {
			return true;
		}
	}

	if (right_b > left_a && right_b < right_a)   {
		if (bottom_b > top_a && bottom_b < bottom_a)     {
			return true;
		}
	}

	// Case 3: size object 1 = size object 2
	if (top_a == top_b && right_a == right_b && bottom_a == bottom_b)   {
		return true;
	}

	return false;
}

bool CommonFunction::checkMouseFocusWithRect(const int& x, const int& y, const SDL_Rect& rect) {
	if (x >= rect.x && x <= rect.x + rect.w &&
		y >= rect.y && y <= rect.y + rect.h) 
	{
		return true;
	}
	return false;
}

void CommonFunction::cleanUp(SDL_Surface*& s_screen, SDL_Surface*& s_bkground) {
	if (s_screen != NULL) {
		SDL_FreeSurface(s_screen);
	}
	if (s_bkground != NULL) {
		SDL_FreeSurface(s_bkground);
	}
}

Mix_Chunk* CommonFunction::loadSound(const string& sound_file_path) {
	return Mix_LoadWAV(sound_file_path.c_str());
}

void CommonFunction::playSound(Mix_Chunk* sound, const int& channel, const int loops) {
	Mix_PlayChannel(channel, sound, loops);
}