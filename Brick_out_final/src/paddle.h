#pragma once
#ifndef paddle_h
#define paddle_h

#include "ofMain.h"

#define PADDLE_WIDTH 65
#define PADDLE_HEIGHT 20
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

class Ball; // 헤더파일 종속성 문제 때문에 전방 선언

class Paddle {
public :
	Paddle() : x(0), y(0) {}
	Paddle(float x, float y);
	~Paddle();

	
	void draw();
	void setX(float x);
	bool checkCollision(const Ball& ball);

	float x, y; // 패들 위치

};

#endif /* paddle_h */