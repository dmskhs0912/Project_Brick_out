#pragma once
#ifndef rank_h
#define rank_h

#include "ofMain.h"

class Rank {
public:
	Rank(string name, int score);
	~Rank();

	string name; // 등록된 이름
	int score; // 도달한 점수

	
};

#endif /* ball_h */