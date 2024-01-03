#include "paddle.h"
#include "ball.h"

void Paddle::draw()
{
	ofFill();
	ofSetColor(100);
	ofDrawRectangle(x, y, PADDLE_WIDTH, PADDLE_HEIGHT);

	ofNoFill();
	ofSetColor(0);
	ofSetLineWidth(1.5);
	ofDrawRectangle(x, y, PADDLE_WIDTH, PADDLE_HEIGHT);
}


// 패들의 x좌표 설정
void Paddle::setX(float x)
{
	this->x = x;
}

// 공과 패들의 충돌 여부를 판단하는 함수. Brick의 것과 동일하다.
bool Paddle::checkCollision(const Ball& ball)
{
	// 패들의 가장자리와 공의 중심 사이 가장 가까운 점 찾기
	float nearestX = max(x, min(ball.x, x + PADDLE_WIDTH));
	float nearestY = max(y, min(ball.y, y + PADDLE_HEIGHT));

	// 공의 중심과 가장자리와 가까운 점 사이의 거리
	float deltaX = ball.x - nearestX;
	float deltaY = ball.y - nearestY;

	// 그 거리의 제곱이 반지름의 제곱보다 작거나 같다면 충돌 발생한 것.
	return (deltaX * deltaX + deltaY * deltaY) <= (BALL_RADIUS * BALL_RADIUS);
}

Paddle::Paddle(float x, float y)
{
	this->x = x;
	this->y = y;
}

Paddle::~Paddle()
{

}