#include "SoundMark.h"

SoundMark::SoundMark() {
	;
}
SoundMark::SoundMark(const string& file_path, const int& R, const int& G, const int& B) : BaseObject() {
	loadImage(file_path, R, G, B);
}
SoundMark::~SoundMark() {
	if (ptr_object_ != NULL) {
		SDL_FreeSurface(ptr_object_);
		ptr_object_ = NULL;
	}
}