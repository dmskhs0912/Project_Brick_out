#include "ofApp.h"
#include "ofJson.h"
#include <iostream>

//--------------------------------------------------------------
ofApp::ofApp()  {}


// �� �������� �� ���� ������ ���� �ִ� Bricks.json ���Ͽ��� �������� ���� ���� ������ �о� stages�� �����Ѵ�. ��ũ ���� �ؽ�Ʈ ������ �о� �����ϴ� ��ɵ� ����.
void ofApp::loadStages()
{
    ofJson json = ofLoadJson("Bricks.json");
    for (auto& stageJson : json["stages"]) {
        Stage stage(stageJson["stageNumber"]);
        for (auto& brickJson : stageJson["bricks"]) {
            float x = brickJson["x"];
            float y = brickJson["y"];
            ofColor color = ofColor::fromHex(std::stoul(brickJson["color"].get<string>().substr(1), nullptr, 16));
            stage.addBrick(Brick(x, y, color));
            stage.aliveBrick++;
        }
        stages.push_back(stage);
    }

    // ��ũ ���� ���� �о ����.
    for (auto& stage : stages) {
        stage.readRanks();
        stage.sortRanks(); // �ؽ�Ʈ ������ ���ĵǾ� ���� ���� �� �����Ƿ� sort
    }
}

// ���� ������ �ҷ����� �Լ� ����۽� Ȱ��
void ofApp::loadOnlyBricks()
{
    ofJson json = ofLoadJson("Bricks.json");
    int stageIndex = 0;
    for (auto& stageJson : json["stages"]) {
        for (auto& brickJson : stageJson["bricks"]) {
            float x = brickJson["x"];
            float y = brickJson["y"];
            ofColor color = ofColor::fromHex(std::stoul(brickJson["color"].get<string>().substr(1), nullptr, 16));
            stages[stageIndex].addBrick(Brick(x, y, color));
            stages[stageIndex].aliveBrick++;
        }
        stageIndex++;
    }
}

// �÷��� �� �⺻ UI�� �׸��� �Լ�. ��������, ����, ��ũ ������ ����Ѵ�.
void ofApp::drawUI()
{
    ofSetColor(0);
    ofSetLineWidth(5);
    ofDrawLine(950, 0, 950, windowHeight); // ��輱 �׸���
    ofSetLineWidth(2);
    ofNoFill();
    ofDrawRectangle(1025, 50, 100, 30); // �������� ���� ��� ĭ
    string text = "stage : " + ofToString(currentStage + 1);
    ofDrawBitmapString(text, 1030, 67);
    ofDrawRectangle(1025, 100, 100, 30); // ���� ���� ��� ĭ
    text = "score : " + ofToString(score);
    ofDrawBitmapString(text, 1030, 117);
    ofDrawRectangle(1000, 200, 150, 300); // ��ũ ���� ��� ĭ

    int i = 0;
    // ���� ���������� ���ĵ� ranks�� ��ȸ�ϸ� ���� ���� �� �پ� ���
    for (auto& rank : stages[currentStage].ranks) {
        if (i >= 25) break; // ��ũ �ִ� 25�� ���
        text = ofToString(i+1) + ".  " + rank.name + "  " + ofToString(rank.score); // "����.  �̸�  ����" �������� ��µ�
        ofDrawBitmapString(text, 1005, 217 + 10*i);
        i++;
    }
    

}

void ofApp::setup(){
	ofSetWindowTitle("Brick Out"); // Set the app name on the title bar
	ofSetFrameRate(60); // FPS 60���� ����
	ofBackground(255, 255, 255); // ��� ��� ����
	gameState = "start"; // �ʱ� ���� ���´� start

    // ȭ�� ũ�� ���� ����
    windowWidth = ofGetWidth();
    windowHeight = ofGetHeight();
    
    // ��Ÿ �ʱ�ȭ�ؾ��� ���� �ʱ�ȭ
    isRestart = false;
    currentStage = 0;
    score = 0;
    nameCount = 0;
    rankPage = 1;
    loadStages(); // stage ���� �о����
    paddle = Paddle(0, 500); // �е� ����. y��ǥ�� �� 500���� ������

    

    
}

//--------------------------------------------------------------
void ofApp::update(){
    // ready ������ ��� �е鿡 ���� �پ� �ֵ��� update
    if (gameState == "ready")
        ball.readyUpdate(paddle);
    // playing ������ ��� Ball Ŭ������ update �޼ҵ带 ���� ���� ��ġ, �浹, �ӵ� ��ȯ�� ��� ����
    else if (gameState == "playing") {
        ball.update(paddle, stages[currentStage], score);
        
    }
}

// ���콺�� ��ư ���� �ִ��� Ȯ���ϴ� �Լ�.
bool isMouseOverButton(int mouseX, int mouseY, int buttonX, int buttonY, int buttonWidth, int buttonHeight) {
    return mouseX > buttonX && mouseX < buttonX + buttonWidth &&
        mouseY > buttonY && mouseY < buttonY + buttonHeight;
}

// start : ���� ���� �޴� ȭ�� �׸���
void ofApp::drawStart()
{
    // ��ư ũ��
    int buttonWidth = 100;
    int buttonHeight = 50;
    // start, rank, exit ��ư ��ġ
    int startbuttonX = (windowWidth - buttonWidth) / 2;
    int startbuttonY = (windowHeight - buttonHeight) / 2 - 60;
    int rankbuttonX = startbuttonX;
    int rankbuttonY = (windowHeight - buttonHeight) / 2;
    int exitbuttonX = startbuttonX;
    int exitbuttonY = (windowHeight - buttonHeight) / 2 + 60;


    // ���� ������ Ŭ���� �� ����۽� ���� �ʱ�ȭ
    if (isRestart) {
        ofShowCursor(); // ���콺 ������ �ٽ� ���̰� ����
        // �� �������� ���� ���� ���� �ʱ�ȭ �� �ٽ� �о� ��.
        for (auto& stage : stages) {
            stage.bricks.clear();
            stage.aliveBrick = 0;
        }
        loadOnlyBricks();
        // ��Ÿ �ʱ�ȭ �� ���� �ʱ�ȭ
        currentStage = 0;
        score = 0;
        isRestart = false;
    }

    // start, exit ��ư �׸���
    ofSetColor(0);
    ofDrawRectangle(startbuttonX, startbuttonY, buttonWidth, buttonHeight);
    ofDrawRectangle(rankbuttonX, rankbuttonY, buttonWidth, buttonHeight);
    ofDrawRectangle(exitbuttonX, exitbuttonY, buttonWidth, buttonHeight);

    // ��ư ���� ���ڿ� ���
    ofSetColor(255);
    ofDrawBitmapString("Start Game", startbuttonX + 10, startbuttonY + buttonHeight / 2 + 5);
    ofDrawBitmapString("View Ranks", rankbuttonX + 10, rankbuttonY + buttonHeight / 2 + 5);
    ofDrawBitmapString("Exit", exitbuttonX + 35, exitbuttonY + buttonHeight / 2 + 5);

    // ��ư ���� ���콺 �÷��� �� ȿ�� �߰�
    if (isMouseOverButton(ofGetMouseX(), ofGetMouseY(), startbuttonX, startbuttonY, buttonWidth, buttonHeight)) {
        ofNoFill();
        ofSetLineWidth(1.5);
        ofSetColor(255);
        ofDrawRectangle(startbuttonX, startbuttonY, buttonWidth, buttonHeight);
        ofFill();
    }
    if (isMouseOverButton(ofGetMouseX(), ofGetMouseY(), rankbuttonX, rankbuttonY, buttonWidth, buttonHeight)) {
        ofNoFill();
        ofSetLineWidth(1.5);
        ofSetColor(255);
        ofDrawRectangle(rankbuttonX, rankbuttonY, buttonWidth, buttonHeight);
        ofFill();
    }
    if (isMouseOverButton(ofGetMouseX(), ofGetMouseY(), exitbuttonX, exitbuttonY, buttonWidth, buttonHeight)) {
        ofNoFill();
        ofSetLineWidth(1.5);
        ofSetColor(255);
        ofDrawRectangle(exitbuttonX, exitbuttonY, buttonWidth, buttonHeight);
        ofFill();
    }

    // ��ư�� ������ �� gameState ����
    if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT) && isMouseOverButton(ofGetMouseX(), ofGetMouseY(), startbuttonX, startbuttonY, buttonWidth, buttonHeight)) {
        ofHideCursor(); // ���콺 ������ �����
        ball = Ball((paddle.x + PADDLE_WIDTH) / 2, paddle.y - 5); // �� ����
        gameState = "ready"; // ready ���·� ����
    }
    if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT) && isMouseOverButton(ofGetMouseX(), ofGetMouseY(), rankbuttonX, rankbuttonY, buttonWidth, buttonHeight)) {
        rankPage = 1;
        gameState = "rank";
    }
    if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT) && isMouseOverButton(ofGetMouseX(), ofGetMouseY(), exitbuttonX, exitbuttonY, buttonWidth, buttonHeight))
        gameState = "exit";
}

// rank : �������� �� ��ũ ���� ���� ȭ�� �׸���
void ofApp::drawRank()
{
    int i = 0;
    string text = "Stage " + ofToString(rankPage); // ��ܿ� �������� ��ȣ ���
    ofNoFill();
    ofSetColor(0);
    ofSetLineWidth(1.5);
    ofDrawRectangle(windowWidth / 2 - 100, 50, 200, 400);
    ofFill();
    ofDrawBitmapString(text, 570, 40);
    ofDrawBitmapString("A : Prev stage   D : Next stage   Q : Quit to menu", 400, 550); // �ȳ� ���� ���

    // rankPage stage�� ��ũ ������ �ִ� 35������ ����Ѵ�.
    for (auto& rank : stages[rankPage - 1].ranks) {
        if (i >= 35) break; // ��ũ ���� �ִ� 35������ ���
        text = ofToString(i + 1) + ".  " + rank.name + "  " + ofToString(rank.score);
        ofDrawBitmapString(text, 505, 67 + 10 * i);
        i++;
    }
}

// ready : ���� �߻�Ǳ� �� ���� ȭ�� �׸���
void ofApp::drawReady()
{
    // ���� UI �׸���
    drawUI();
    ofSetColor(0);
    ofDrawBitmapString("Click RMB to start game", (windowWidth / 2) - 100, windowHeight / 2); // �ȳ� ���� ���
    // ���� �׸���
    for (auto& brick : stages[currentStage].bricks) {
        brick.draw();
    }

    // �е�, �� �׸���
    paddle.draw();
    ball.draw();

    // ��Ŭ�� �� �� �߻� �� playing ���·� ��ȯ 
    if (ofGetMousePressed(OF_MOUSE_BUTTON_RIGHT)) {
        gameState = "playing";
        ball.shoot();
    }
}

// playing : ���� �߻�Ǿ� ������ ���� ���� ȭ�� �׸���
void ofApp::drawPlaying()
{
    // UI, ���� �׸���
    drawUI();
    for (auto& brick : stages[currentStage].bricks) {
        brick.draw();
    }

    // �е�, �� �׸���
    paddle.draw();
    ball.draw();

    // ���� ȭ�� �ϴ� ��迡 ������ ���� ����
    if (ball.y + BALL_RADIUS > windowHeight) {
        gameState = "gameover";
        score -= 300; // 300�� ����
    }

    // ���� ���������� ���� ������ ������ clear �Ǵ� stageclear
    if (stages[currentStage].aliveBrick == 0) {
        if (currentStage == stages.size() - 1) {
            gameState = "clear";
            score += 300;
        }
        else {
            gameState = "stageclear";
            score += 300;
        }
    }
}

// clear : ��� ���������� Ŭ���� ���� �� ȭ�� �׸���
void ofApp::drawClear()
{
    ofDrawBitmapString("Clear! \nInput your name.", (windowWidth / 2) - 100, windowHeight / 2);
    ofDrawBitmapString("___", (windowWidth / 2) - 100, windowHeight / 2 + 50);
    if (nameCount >= 1) ofDrawBitmapString(name[0], (windowWidth / 2) - 100, windowHeight / 2 + 50);
    if (nameCount >= 2) ofDrawBitmapString(name[1], (windowWidth / 2) - 100 + 8, windowHeight / 2 + 50);
    if (nameCount >= 3) ofDrawBitmapString(name[2], (windowWidth / 2) - 100 + 16, windowHeight / 2 + 50);
}

// gameover : ���� ���� �� ȭ�� �׸���
void ofApp::drawGameOver()
{
    ofDrawBitmapString("Game Over! \nInput your name.", (windowWidth / 2) - 100, windowHeight / 2);
    ofDrawBitmapString("___", (windowWidth / 2) - 100, windowHeight / 2 + 50);
    if (nameCount >= 1) ofDrawBitmapString(name[0], (windowWidth / 2) - 100, windowHeight / 2 + 50);
    if (nameCount >= 2) ofDrawBitmapString(name[1], (windowWidth / 2) - 100 + 8, windowHeight / 2 + 50);
    if (nameCount >= 3) ofDrawBitmapString(name[2], (windowWidth / 2) - 100 + 16, windowHeight / 2 + 50);
}

// stageclear : ������ �������� ���� ���������� Ŭ�������� �� ȭ�� �׸���
void ofApp::drawStageClear()
{
    ofDrawBitmapString("Stage Clear! \nInput your name.", (windowWidth / 2) - 100, windowHeight / 2);
    ofDrawBitmapString("___", (windowWidth / 2) - 100, windowHeight / 2 + 50);
    if (nameCount >= 1) ofDrawBitmapString(name[0], (windowWidth / 2) - 100, windowHeight / 2 + 50);
    if (nameCount >= 2) ofDrawBitmapString(name[1], (windowWidth / 2) - 100 + 8, windowHeight / 2 + 50);
    if (nameCount >= 3) ofDrawBitmapString(name[2], (windowWidth / 2) - 100 + 16, windowHeight / 2 + 50);
}


//--------------------------------------------------------------
void ofApp::draw() {
    // start : ���� ���� �޴� ȭ��
    if (gameState == "start") 
        drawStart();

    // rank : �������� �� ��ũ ���� ���� ȭ��
    else if (gameState == "rank") 
        drawRank();
    
    // ready : ���� �߻�Ǳ� �� ���� ȭ��
    else if (gameState == "ready") 
        drawReady();
    
    // playing : ���� �߻�Ǿ� ������ ���� ���� ȭ��
    else if (gameState == "playing") 
        drawPlaying();

    // clear : ��� ���������� Ŭ���� ���� �� ȭ��
    else if (gameState == "clear") 
        drawClear();

    // stageclear : ������ �������� ���� ���������� Ŭ���� ���� �� ȭ��
    else if (gameState == "stageclear") 
        drawStageClear();

    // gameover : ���� ���� �� ȭ��
    else if (gameState == "gameover") 
        drawGameOver();
    
    // exit : ���� ���� 
    else if (gameState == "exit") 
        ofExit();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // ��ũ ���� �� aŰ�� ���� ������, dŰ�� ���� ������, q�� �޴� �̵� ���
    if (gameState == "rank") {
        if ((key == 'a' || key == 'A') && rankPage != 1)
            rankPage--;
        else if ((key == 'd' || key == 'D') && rankPage != stages.size())
            rankPage++;
        else if (key == 'q' || key == 'Q')
            gameState = "start";
    }

    // ���� ����, �������� Ŭ���, ��ü Ŭ���� �� �̸� �Է�
    else if (gameState == "gameover" || gameState == "stageclear" || gameState == "clear") {
        // �̸� 3���� �Է� �� �ƹ� Ű �Է� �� ���� �������� �Ǵ� ���� ȭ��
        if (nameCount == 3) {
            nameCount = 0;
            stages[currentStage].addRank(Rank(name, score));
            stages[currentStage].sortRanks();
            stages[currentStage].writeRanks();
            name.clear();
            score = 0;
            if (gameState == "stageclear") {
                gameState = "ready";
                currentStage++;
            }
            else {
                gameState = "start";
                isRestart = true;
            }
        }
        else if (key >= 32 && key <= 126) {
            name += static_cast<char>(key);
            nameCount++;
        }
        
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    // ���콺�� ������ ������ �е� ��ġ ����
    if(x+PADDLE_WIDTH/2 <= 950)
        paddle.setX(x - PADDLE_WIDTH / 2);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------


void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
