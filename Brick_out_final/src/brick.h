#pragma once
#ifndef brick_h
#define brick_h

#include "ofMain.h"
#include "ball.h"
#define BRICK_WIDTH 50
#define BRICK_HEIGHT 20
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))


class Brick {
public :
	Brick(float x, float y, ofColor color);
	~Brick(); 

	void draw();
	bool checkCollision(const Ball& ball);
	void collision();


	float x, y; // ��ġ
	int life; // ����
	ofColor color; // ����
	bool isAlive; // ���� ����
};

#endif /* brick_h */