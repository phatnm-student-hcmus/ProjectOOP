#pragma once
#include "BaseObject.h"
#define AUDIO_ICON_WIDTH 54
#define AUDIO_ICON_HEIGHT 50

class AudioIcon : public BaseObject {
private:
	//auto init
	SDL_Rect clip_[2];
	//auto init
	Mix_Chunk* check_sound_;

	bool is_mute_;

public:
	AudioIcon();
	~AudioIcon();

	void show(SDL_Surface* des);

	bool get_is_mute();
	void set_is_mute(const bool& state);

	void playSound(const int& channel, const int& loops);
};

