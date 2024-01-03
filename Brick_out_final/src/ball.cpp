#include "ball.h"
#include "paddle.h"
#include "stage.h"

// ready ȭ�鿡�� ���� �е鿡 �پ��ֵ��� ������Ʈ 
void Ball::readyUpdate(const Paddle paddle)
{
	x = paddle.x + (PADDLE_WIDTH / 2);
	y = paddle.y - 5;
}

// playing ������ �� �� ��ġ �� �ӵ� ������Ʈ 
void Ball::update(Paddle& paddle, Stage& stage, int &score)
{
	// �е�� �浹 �Ǵ� �� �ӵ� ������Ʈ
	if (paddle.checkCollision(*this)) {
		// �е��� �߾ӿ��� �������� �Ÿ� ��� �� -1 ~ 1 ���̷� ������ ����ȭ
		float hitPoint = (this->x - (paddle.x + PADDLE_WIDTH / 2)) / (PADDLE_WIDTH / 2);

		// �ݻ� ���� ��� -1.0�� ���� ��, 1.0�� ������ ���� �ش���.
		float bounceAngle = hitPoint * MAX_BOUNCE_ANGLE;

		// ���ο� �ӵ� ���� ����
		float normSpeed = sqrt(this->speedX * this->speedX + this->speedY * this->speedY);
		this->speedY = -abs(this->speedY); // ���� �ݻ�
		this->speedX =  normSpeed * sin(bounceAngle); // ������ ���� X �ӵ� ����
		score -= 20; // �е鿡 ƨ�� ������ 20�� ����
	}

	// �� ������ �浹 �Ǵ� �� �ӵ� ������Ʈ
	for (auto& brick : stage.bricks) {
		if (brick.checkCollision(*this)) {
			// ���� �ݻ�: ���� ������ �����̳� ������ �ε������� Ȯ��
			if (y + BALL_RADIUS > brick.y && y - BALL_RADIUS < brick.y + BRICK_HEIGHT) {
				speedX = -speedX;
			}
			// ���� �ݻ�: ���� ������ ����̳� �ϴܿ� �ε������� Ȯ��
			else if (x + BALL_RADIUS > brick.x && x - BALL_RADIUS < brick.x + BRICK_WIDTH) {
				speedY = -speedY;
			}

			brick.collision();
			if (!brick.isAlive) stage.aliveBrick--; // �浹�� ���� ������ ���� ��� aliveBrick �� ����
			score += 50; // ������ ƨ��� +50��
			break; // �߰� �浹�� �����ϱ� ���� ���� ����
		}
	}

	// ��� ȭ�� ��� �ݻ�
	if (y - BALL_RADIUS <= 0) {
		speedY = -speedY;
	}

	// ���� ȭ�� �� ���� ��� �ݻ�
	if (x - BALL_RADIUS <= 0 || x + BALL_RADIUS >= 945) { // drawUI() ���� UI�� X�� 950 �̻��� �κп� �׷���. ��輱 �β��� 5�̹Ƿ� 945�� ����
		speedX = -speedX;
	}

	// �ӵ��� ���� ��ġ ������Ʈ
	x += speedX;
	y += speedY;
}

// ready ȭ�鿡�� ��Ŭ�� �� �� �߻�. ������ �������� �߻� ��.
void Ball::shoot()
{
	y -= 10; // �߻� �� �е�� �浹�� �����ϱ� ���� ���� ���� �ø�
	speedX = ofRandom(-1, 1); // �߻� ���� �̼��ϰ� ������.
	speedY = -5;
}

void Ball::draw()
{
	ofFill();
	ofSetColor(0);
	ofDrawCircle(x, y, BALL_RADIUS);
}

Ball::Ball(float x, float y)
{
	this->x = x;
	this->y = y;
	this->speedX = 0;
	this->speedY = 0;
}

Ball::~Ball()
{

}