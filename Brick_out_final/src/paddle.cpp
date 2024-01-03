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


// �е��� x��ǥ ����
void Paddle::setX(float x)
{
	this->x = x;
}

// ���� �е��� �浹 ���θ� �Ǵ��ϴ� �Լ�. Brick�� �Ͱ� �����ϴ�.
bool Paddle::checkCollision(const Ball& ball)
{
	// �е��� �����ڸ��� ���� �߽� ���� ���� ����� �� ã��
	float nearestX = max(x, min(ball.x, x + PADDLE_WIDTH));
	float nearestY = max(y, min(ball.y, y + PADDLE_HEIGHT));

	// ���� �߽ɰ� �����ڸ��� ����� �� ������ �Ÿ�
	float deltaX = ball.x - nearestX;
	float deltaY = ball.y - nearestY;

	// �� �Ÿ��� ������ �������� �������� �۰ų� ���ٸ� �浹 �߻��� ��.
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