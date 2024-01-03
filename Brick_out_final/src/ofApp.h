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

		// ������ �޼������ draw()���� �� ���� ó���ص� �����ϳ�, �ڵ��� �������� ���� ���� ������ �Լ����̴�.
		void drawStart();
		void drawRank();
		void drawReady();
		void drawPlaying();
		void drawClear();
		void drawStageClear();
		void drawGameOver();

		bool isRestart; // ����� ���� 
		int currentStage; // ���� �÷��� ���� �������� ��ȣ - 1 
		int score; // ���� ����
		int nameCount; // �̸� �Է� �� �� ���� �ԷµǾ����� Ȯ���� ���� ����
		int rankPage; // ��ũ ������ ���� ������ ����
		string name; // �Էµ� �̸�
		vector<Stage> stages; // �������� ����
		Paddle paddle; // �е�
		Ball ball; // ��
		string gameState; // ������ ���� ���� ����
		float windowWidth, windowHeight; // ȭ�� ũ�� ����
};
