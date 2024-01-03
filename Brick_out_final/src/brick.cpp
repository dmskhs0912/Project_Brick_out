#include "brick.h"

void Brick::draw()
{
	if (isAlive) { // ������ ���� ������ �׷�����
		// ���� �׸���
		ofFill();
		ofSetColor(color);
		ofDrawRectangle(x, y, BRICK_WIDTH, BRICK_HEIGHT);
		
		// ���� ������ �׸���
		ofNoFill();
		ofSetColor(0);
		ofSetLineWidth(1.5);
		ofDrawRectangle(x, y, BRICK_WIDTH, BRICK_HEIGHT);
	}
}

// ���� ������ �浹�� �Ǻ��ϴ� �Լ�. �е��� �Ͱ� ����.
bool Brick::checkCollision(const Ball& ball)
{
	if (!isAlive) return false; // �̹� ���� �����̶�� �浹�� �����ؾ���.

	// ������ �����ڸ��� ���� �߽� ���� ���� ����� �� ã��
	float nearestX = max(x, min(ball.x, x + BRICK_WIDTH));
	float nearestY = max(y, min(ball.y, y + BRICK_HEIGHT));

	// ���� �߽ɰ� �����ڸ��� ����� �� ������ �Ÿ�
	float deltaX = ball.x - nearestX;
	float deltaY = ball.y - nearestY;

	// �� �Ÿ��� ������ �������� �������� �۰ų� ���ٸ� �浹 �߻��� ��.
	return (deltaX * deltaX + deltaY * deltaY) <= (BALL_RADIUS * BALL_RADIUS);
}

// ���� �浹 �� life, isAlive�� ����
void Brick::collision()
{
	life--; // ���� ����

	// ���ҵ� ���� ���� ���� ��ȯ / isAlive ����
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
	
	// ������ ���� ���� ������ �ٸ�
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