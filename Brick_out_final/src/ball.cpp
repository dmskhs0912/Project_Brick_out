#include "ball.h"
#include "paddle.h"
#include "stage.h"

// ready 화면에서 공이 패들에 붙어있도록 업데이트 
void Ball::readyUpdate(const Paddle paddle)
{
	x = paddle.x + (PADDLE_WIDTH / 2);
	y = paddle.y - 5;
}

// playing 상태일 때 공 위치 및 속도 업데이트 
void Ball::update(Paddle& paddle, Stage& stage, int &score)
{
	// 패들과 충돌 판단 후 속도 업데이트
	if (paddle.checkCollision(*this)) {
		// 패들의 중앙에서 공까지의 거리 계산 후 -1 ~ 1 사이로 값으로 정규화
		float hitPoint = (this->x - (paddle.x + PADDLE_WIDTH / 2)) / (PADDLE_WIDTH / 2);

		// 반사 각도 계산 -1.0은 왼쪽 끝, 1.0은 오른쪽 끝에 해당함.
		float bounceAngle = hitPoint * MAX_BOUNCE_ANGLE;

		// 새로운 속도 벡터 설정
		float normSpeed = sqrt(this->speedX * this->speedX + this->speedY * this->speedY);
		this->speedY = -abs(this->speedY); // 위로 반사
		this->speedX =  normSpeed * sin(bounceAngle); // 각도에 따라 X 속도 조절
		score -= 20; // 패들에 튕길 때마다 20점 감점
	}

	// 각 벽돌과 충돌 판단 후 속도 업데이트
	for (auto& brick : stage.bricks) {
		if (brick.checkCollision(*this)) {
			// 수평 반사: 공이 벽돌의 좌측이나 우측에 부딪혔는지 확인
			if (y + BALL_RADIUS > brick.y && y - BALL_RADIUS < brick.y + BRICK_HEIGHT) {
				speedX = -speedX;
			}
			// 수직 반사: 공이 벽돌의 상단이나 하단에 부딪혔는지 확인
			else if (x + BALL_RADIUS > brick.x && x - BALL_RADIUS < brick.x + BRICK_WIDTH) {
				speedY = -speedY;
			}

			brick.collision();
			if (!brick.isAlive) stage.aliveBrick--; // 충돌에 의해 벽돌이 깨진 경우 aliveBrick 수 감소
			score += 50; // 벽돌에 튕기면 +50점
			break; // 추가 충돌을 방지하기 위해 루프 종료
		}
	}

	// 상단 화면 경계 반사
	if (y - BALL_RADIUS <= 0) {
		speedY = -speedY;
	}

	// 좌측 화면 및 우측 경계 반사
	if (x - BALL_RADIUS <= 0 || x + BALL_RADIUS >= 945) { // drawUI() 에서 UI는 X가 950 이상인 부분에 그려짐. 경계선 두께가 5이므로 945가 기준
		speedX = -speedX;
	}

	// 속도에 의한 위치 업데이트
	x += speedX;
	y += speedY;
}

// ready 화면에서 우클릭 시 공 발사. 랜덤한 방향으로 발사 됨.
void Ball::shoot()
{
	y -= 10; // 발사 시 패들과 충돌을 방지하기 위해 조금 위로 올림
	speedX = ofRandom(-1, 1); // 발사 각도 미세하게 랜덤함.
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