#pragma once
#include <iostream>
#include <string>
#include <vector>


using namespace std;


class LeaderBoardData {
public:
	char name_[32];
	int score_, time_;

public:
	LeaderBoardData();
	LeaderBoardData(char name[32], int score, int time);
	LeaderBoardData(string name, int score, int time);
	
	int getTime();
	int getScore();
	string getData();
	void Swap(LeaderBoardData& l2);
};

