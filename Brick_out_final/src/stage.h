#pragma once
#ifndef stage_h
#define stage_h

#include "ofMain.h"
#include "brick.h"
#include "rank.h"

class Stage {
public:
	Stage(int number);
	~Stage();
	int stageNumber; // 스테이지 번호
	int aliveBrick; // 현재 스테이지에 살아있는 벽돌 수
	vector<Brick> bricks; // 현재 스테이지 벽돌 정보
	vector<Rank> ranks; // 현재 스테이지 랭크 정보 

	void addBrick(const Brick& brick);
	void addRank(const Rank& rank);
	void sortRanks();
	void readRanks();
	void writeRanks();
};

#endif /* stage_h */