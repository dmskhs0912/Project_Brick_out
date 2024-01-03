#include "brick.h"

void Brick::draw()
{
	if (isAlive) { // 깨지지 않은 벽돌만 그려야함
		// 벽돌 그리기
		ofFill();
		ofSetColor(color);
		ofDrawRectangle(x, y, BRICK_WIDTH, BRICK_HEIGHT);
		
		// 벽돌 윤곽선 그리기
		ofNoFill();
		ofSetColor(0);
		ofSetLineWidth(1.5);
		ofDrawRectangle(x, y, BRICK_WIDTH, BRICK_HEIGHT);
	}
}

// 공과 벽돌의 충돌을 판별하는 함수. 패들의 것과 동일.
bool Brick::checkCollision(const Ball& ball)
{
	if (!isAlive) return false; // 이미 깨진 벽돌이라면 충돌을 무시해야함.

	// 벽돌의 가장자리와 공의 중심 사이 가장 가까운 점 찾기
	float nearestX = max(x, min(ball.x, x + BRICK_WIDTH));
	float nearestY = max(y, min(ball.y, y + BRICK_HEIGHT));

	// 공의 중심과 가장자리와 가까운 점 사이의 거리
	float deltaX = ball.x - nearestX;
	float deltaY = ball.y - nearestY;

	// 그 거리의 제곱이 반지름의 제곱보다 작거나 같다면 충돌 발생한 것.
	return (deltaX * deltaX + deltaY * deltaY) <= (BALL_RADIUS * BALL_RADIUS);
}

// 공과 충돌 시 life, isAlive를 조정
void Brick::collision()
{
	life--; // 수명 감소

	// 감소된 수명에 따라 색상 변환 / isAlive 조정
	if (life == 2)
		color = ofColor::green;
	else if (life == 1)
		color = ofColor::blue;
	else 
		isAlive = false;
}

Brick::Brick(float x, float y,  ofColor color)
{
	this->x = x; 
	this->y = y; 
	this->color = color; 
	this->isAlive = true;
	
	// 벽돌의 색상에 따라 수명이 다름
	if (color == ofColor::red)
		this->life = 3;
	else if (color == ofColor::green)
		this->life = 2;
	else if (color == ofColor::blue)
		this->life = 1;
	else
		this->life = 0;
	
}

Brick::~Brick()
{
	
}