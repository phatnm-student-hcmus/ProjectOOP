#pragma once
#include "BaseObject.h"
class SoundMark : public BaseObject {
private:
public:
	SoundMark();
	SoundMark(const string& file_path, const int& R = 0, const int& G = 0, const int& B = 0);
	~SoundMark();
};

