#include "AudioIcon.h"

AudioIcon::AudioIcon() {
	loadImage(s_clip_audio_icon_img_file_path, 255, 255, 255);
	rect_.w = AUDIO_ICON_WIDTH;
	rect_.h = AUDIO_ICON_HEIGHT;

	for (int i = 0; i < 2; i++) {
		clip_[i].x = i * AUDIO_ICON_WIDTH;
		clip_[i].y = 0;
		clip_[i].w = AUDIO_ICON_WIDTH;
		clip_[i].h = AUDIO_ICON_HEIGHT;
	}
	check_sound_ = NULL;
	is_mute_ = false;
}
AudioIcon::~AudioIcon() {
	if (ptr_object_ != NULL) {
		SDL_FreeSurface(ptr_object_);
		ptr_object_ = NULL;
	}
}

void AudioIcon::show(SDL_Surface* screen) {
	if (!is_mute_) {
		rect_ = CommonFunction::applySurface(ptr_object_, screen, &clip_[1], rect_.x, rect_.y);
	}
	else 	{
		rect_ = CommonFunction::applySurface(ptr_object_, screen, &clip_[0], rect_.x, rect_.y);
	}
}

bool AudioIcon::get_is_mute() {
	return is_mute_;
}
void AudioIcon::set_is_mute(const bool& state) {
	is_mute_ = state;
}

void AudioIcon::playSound(const int& channel, const int& loops) {
	CommonFunction::playSound(check_sound_, channel, loops);
}