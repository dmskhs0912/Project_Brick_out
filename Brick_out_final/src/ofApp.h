#pragma once

#include "ofMain.h"
#include "brick.h"
#include "stage.h"
#include "paddle.h"
#include "ball.h"
#include "rank.h"
#include <string>

class ofApp : public ofBaseApp{

	public:
		ofApp();
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void loadStages();
		void loadOnlyBricks();
		void drawUI();

		// 이하의 메서드들은 draw()에서 한 번에 처리해도 무방하나, 코드의 가독성을 위해 따로 선언한 함수들이다.
		void drawStart();
		void drawRank();
		void drawReady();
		void drawPlaying();
		void drawClear();
		void drawStageClear();
		void drawGameOver();

		bool isRestart; // 재시작 여부 
		int currentStage; // 현재 플레이 중인 스테이지 번호 - 1 
		int score; // 현재 점수
		int nameCount; // 이름 입력 시 몇 글자 입력되었는지 확인을 위한 변수
		int rankPage; // 랭크 열람시 현재 페이지 정보
		string name; // 입력된 이름
		vector<Stage> stages; // 스테이지 정보
		Paddle paddle; // 패들
		Ball ball; // 공
		string gameState; // 게임의 현재 상태 저장
		float windowWidth, windowHeight; // 화면 크기 저장
};
