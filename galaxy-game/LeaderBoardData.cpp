#include "LeaderBoardData.h"

LeaderBoardData::LeaderBoardData() : name_{ 0 } {
	score_ = 0;
	time_ = 0;
}
LeaderBoardData::LeaderBoardData(char name[32], int score, int time) {
	strcpy_s(name_, 32, name);
	score_ = score;
	time_ = time;
}
LeaderBoardData::LeaderBoardData(string name, int score, int time) : name_{ 0 } {
	int i = 0;
	for (i; i < name.size() && i < 32; i++) {
		name_[i] = name[i];
	}
	for (i; i < 32; i++) {
		name_[i] = 0;
	}
	score_ = score;
	time_ = time;
}


int LeaderBoardData::getScore() {
	return score_;
}
int LeaderBoardData::getTime() {
	return time_;
}
string LeaderBoardData::getData() {
	string str_name(name_);
	while (str_name.size() < 20) 	{
		str_name.push_back(' ');
	}

	string str_score = to_string(score_);
	while (str_score.size() < 9) 	{
		str_score = " " + str_score;
	}

	string str_time = to_string(time_);
	while (str_time.size() < 9) {
		str_time = " " + str_time;
	}

	string ret = str_name + str_score + str_time;
	cout << ret.size() << endl;
	return ret;
}

void LeaderBoardData::Swap(LeaderBoardData& l2) {
	for (int i = 0; i < 32; i++) {
		swap(name_[i], l2.name_[i]);
	}
	swap(score_, l2.score_);
	swap(time_, l2.time_);
}