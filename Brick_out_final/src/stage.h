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
	int stageNumber; // �������� ��ȣ
	int aliveBrick; // ���� ���������� ����ִ� ���� ��
	vector<Brick> bricks; // ���� �������� ���� ����
	vector<Rank> ranks; // ���� �������� ��ũ ���� 

	void addBrick(const Brick& brick);
	void addRank(const Rank& rank);
	void sortRanks();
	void readRanks();
	void writeRanks();
};

#endif /* stage_h */