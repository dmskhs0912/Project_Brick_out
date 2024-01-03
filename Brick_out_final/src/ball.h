#pragma once
#ifndef ball_h
#define ball_h

#include "ofMain.h"
#define BALL_RADIUS 5
#define MAX_BOUNCE_ANGLE 0.785

// ���Ӽ� ���� �ذ��� ���� ���� ����
class Stage;
class Paddle;
class Brick;

class Ball {
public :
	Ball() : x(0), y(0), speedX(0), speedY(0) {}
	Ball(float x, float y);
	~Ball();

	void readyUpdate(const Paddle paddle);
	void update(Paddle& paddle, Stage& stage, int& score);
	void shoot();
	void draw();

	float x, y; // ���� ��ġ
	float speedX, speedY; // ���� �ӵ� ����

};

#endif /* ball_h */