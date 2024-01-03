#include "ofApp.h"
#include "ofJson.h"
#include <iostream>

//--------------------------------------------------------------
ofApp::ofApp()  {}


// 각 스테이지 별 벽돌 정보를 갖고 있는 Bricks.json 파일에서 스테이지 별로 벽돌 정보를 읽어 stages에 저장한다. 랭크 정보 텍스트 파일을 읽어 저장하는 기능도 있음.
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

    // 랭크 정보 파일 읽어서 저장.
    for (auto& stage : stages) {
        stage.readRanks();
        stage.sortRanks(); // 텍스트 파일이 정렬되어 있지 않을 수 있으므로 sort
    }
}

// 벽돌 정보만 불러오는 함수 재시작시 활용
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

// 플레이 시 기본 UI를 그리는 함수. 스테이지, 점수, 랭크 정보를 출력한다.
void ofApp::drawUI()
{
    ofSetColor(0);
    ofSetLineWidth(5);
    ofDrawLine(950, 0, 950, windowHeight); // 경계선 그리기
    ofSetLineWidth(2);
    ofNoFill();
    ofDrawRectangle(1025, 50, 100, 30); // 스테이지 정보 출력 칸
    string text = "stage : " + ofToString(currentStage + 1);
    ofDrawBitmapString(text, 1030, 67);
    ofDrawRectangle(1025, 100, 100, 30); // 점수 정보 출력 칸
    text = "score : " + ofToString(score);
    ofDrawBitmapString(text, 1030, 117);
    ofDrawRectangle(1000, 200, 150, 300); // 랭크 정보 출력 칸

    int i = 0;
    // 현재 스테이지의 정렬된 ranks를 순회하며 순위 별로 한 줄씩 출력
    for (auto& rank : stages[currentStage].ranks) {
        if (i >= 25) break; // 랭크 최대 25개 출력
        text = ofToString(i+1) + ".  " + rank.name + "  " + ofToString(rank.score); // "순위.  이름  점수" 형식으로 출력됨
        ofDrawBitmapString(text, 1005, 217 + 10*i);
        i++;
    }
    

}

void ofApp::setup(){
	ofSetWindowTitle("Brick Out"); // Set the app name on the title bar
	ofSetFrameRate(60); // FPS 60으로 설정
	ofBackground(255, 255, 255); // 배경 흰색 설정
	gameState = "start"; // 초기 게임 상태는 start

    // 화면 크기 정보 저장
    windowWidth = ofGetWidth();
    windowHeight = ofGetHeight();
    
    // 기타 초기화해야할 변수 초기화
    isRestart = false;
    currentStage = 0;
    score = 0;
    nameCount = 0;
    rankPage = 1;
    loadStages(); // stage 정보 읽어들임
    paddle = Paddle(0, 500); // 패들 생성. y좌표는 쭉 500으로 고정됨

    

    
}

//--------------------------------------------------------------
void ofApp::update(){
    // ready 상태인 경우 패들에 공이 붙어 있도록 update
    if (gameState == "ready")
        ball.readyUpdate(paddle);
    // playing 상태인 경우 Ball 클래스의 update 메소드를 통해 공의 위치, 충돌, 속도 변환을 모두 관리
    else if (gameState == "playing") {
        ball.update(paddle, stages[currentStage], score);
        
    }
}

// 마우스가 버튼 위에 있는지 확인하는 함수.
bool isMouseOverButton(int mouseX, int mouseY, int buttonX, int buttonY, int buttonWidth, int buttonHeight) {
    return mouseX > buttonX && mouseX < buttonX + buttonWidth &&
        mouseY > buttonY && mouseY < buttonY + buttonHeight;
}

// start : 게임 시작 메뉴 화면 그리기
void ofApp::drawStart()
{
    // 버튼 크기
    int buttonWidth = 100;
    int buttonHeight = 50;
    // start, rank, exit 버튼 위치
    int startbuttonX = (windowWidth - buttonWidth) / 2;
    int startbuttonY = (windowHeight - buttonHeight) / 2 - 60;
    int rankbuttonX = startbuttonX;
    int rankbuttonY = (windowHeight - buttonHeight) / 2;
    int exitbuttonX = startbuttonX;
    int exitbuttonY = (windowHeight - buttonHeight) / 2 + 60;


    // 게임 오버나 클리어 후 재시작시 정보 초기화
    if (isRestart) {
        ofShowCursor(); // 마우스 포인터 다시 보이게 설정
        // 각 스테이지 별로 벽돌 정보 초기화 후 다시 읽어 옴.
        for (auto& stage : stages) {
            stage.bricks.clear();
            stage.aliveBrick = 0;
        }
        loadOnlyBricks();
        // 기타 초기화 할 변수 초기화
        currentStage = 0;
        score = 0;
        isRestart = false;
    }

    // start, exit 버튼 그리기
    ofSetColor(0);
    ofDrawRectangle(startbuttonX, startbuttonY, buttonWidth, buttonHeight);
    ofDrawRectangle(rankbuttonX, rankbuttonY, buttonWidth, buttonHeight);
    ofDrawRectangle(exitbuttonX, exitbuttonY, buttonWidth, buttonHeight);

    // 버튼 위에 문자열 출력
    ofSetColor(255);
    ofDrawBitmapString("Start Game", startbuttonX + 10, startbuttonY + buttonHeight / 2 + 5);
    ofDrawBitmapString("View Ranks", rankbuttonX + 10, rankbuttonY + buttonHeight / 2 + 5);
    ofDrawBitmapString("Exit", exitbuttonX + 35, exitbuttonY + buttonHeight / 2 + 5);

    // 버튼 위로 마우스 올렸을 때 효과 추가
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

    // 버튼이 눌렸을 때 gameState 변경
    if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT) && isMouseOverButton(ofGetMouseX(), ofGetMouseY(), startbuttonX, startbuttonY, buttonWidth, buttonHeight)) {
        ofHideCursor(); // 마우스 포인터 숨기기
        ball = Ball((paddle.x + PADDLE_WIDTH) / 2, paddle.y - 5); // 공 생성
        gameState = "ready"; // ready 상태로 변경
    }
    if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT) && isMouseOverButton(ofGetMouseX(), ofGetMouseY(), rankbuttonX, rankbuttonY, buttonWidth, buttonHeight)) {
        rankPage = 1;
        gameState = "rank";
    }
    if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT) && isMouseOverButton(ofGetMouseX(), ofGetMouseY(), exitbuttonX, exitbuttonY, buttonWidth, buttonHeight))
        gameState = "exit";
}

// rank : 스테이지 별 랭크 정보 열람 화면 그리기
void ofApp::drawRank()
{
    int i = 0;
    string text = "Stage " + ofToString(rankPage); // 상단에 스테이지 번호 출력
    ofNoFill();
    ofSetColor(0);
    ofSetLineWidth(1.5);
    ofDrawRectangle(windowWidth / 2 - 100, 50, 200, 400);
    ofFill();
    ofDrawBitmapString(text, 570, 40);
    ofDrawBitmapString("A : Prev stage   D : Next stage   Q : Quit to menu", 400, 550); // 안내 문구 출력

    // rankPage stage의 랭크 정보를 최대 35개까지 출력한다.
    for (auto& rank : stages[rankPage - 1].ranks) {
        if (i >= 35) break; // 랭크 정보 최대 35개까지 출력
        text = ofToString(i + 1) + ".  " + rank.name + "  " + ofToString(rank.score);
        ofDrawBitmapString(text, 505, 67 + 10 * i);
        i++;
    }
}

// ready : 공이 발사되기 전 게임 화면 그리기
void ofApp::drawReady()
{
    // 우측 UI 그리기
    drawUI();
    ofSetColor(0);
    ofDrawBitmapString("Click RMB to start game", (windowWidth / 2) - 100, windowHeight / 2); // 안내 문구 출력
    // 벽돌 그리기
    for (auto& brick : stages[currentStage].bricks) {
        brick.draw();
    }

    // 패들, 공 그리기
    paddle.draw();
    ball.draw();

    // 우클릭 시 공 발사 및 playing 상태로 전환 
    if (ofGetMousePressed(OF_MOUSE_BUTTON_RIGHT)) {
        gameState = "playing";
        ball.shoot();
    }
}

// playing : 공이 발사되어 게임이 진행 중인 화면 그리기
void ofApp::drawPlaying()
{
    // UI, 벽돌 그리기
    drawUI();
    for (auto& brick : stages[currentStage].bricks) {
        brick.draw();
    }

    // 패들, 공 그리기
    paddle.draw();
    ball.draw();

    // 공이 화면 하단 경계에 닿으면 게임 오버
    if (ball.y + BALL_RADIUS > windowHeight) {
        gameState = "gameover";
        score -= 300; // 300점 감점
    }

    // 현재 스테이지에 남은 벽돌이 없으면 clear 또는 stageclear
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

// clear : 모든 스테이지를 클리어 했을 시 화면 그리기
void ofApp::drawClear()
{
    ofDrawBitmapString("Clear! \nInput your name.", (windowWidth / 2) - 100, windowHeight / 2);
    ofDrawBitmapString("___", (windowWidth / 2) - 100, windowHeight / 2 + 50);
    if (nameCount >= 1) ofDrawBitmapString(name[0], (windowWidth / 2) - 100, windowHeight / 2 + 50);
    if (nameCount >= 2) ofDrawBitmapString(name[1], (windowWidth / 2) - 100 + 8, windowHeight / 2 + 50);
    if (nameCount >= 3) ofDrawBitmapString(name[2], (windowWidth / 2) - 100 + 16, windowHeight / 2 + 50);
}

// gameover : 게임 오버 시 화면 그리기
void ofApp::drawGameOver()
{
    ofDrawBitmapString("Game Over! \nInput your name.", (windowWidth / 2) - 100, windowHeight / 2);
    ofDrawBitmapString("___", (windowWidth / 2) - 100, windowHeight / 2 + 50);
    if (nameCount >= 1) ofDrawBitmapString(name[0], (windowWidth / 2) - 100, windowHeight / 2 + 50);
    if (nameCount >= 2) ofDrawBitmapString(name[1], (windowWidth / 2) - 100 + 8, windowHeight / 2 + 50);
    if (nameCount >= 3) ofDrawBitmapString(name[2], (windowWidth / 2) - 100 + 16, windowHeight / 2 + 50);
}

// stageclear : 마지막 스테이지 이전 스테이지를 클리어했을 시 화면 그리기
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
    // start : 게임 시작 메뉴 화면
    if (gameState == "start") 
        drawStart();

    // rank : 스테이지 별 랭크 정보 열람 화면
    else if (gameState == "rank") 
        drawRank();
    
    // ready : 공이 발사되기 전 게임 화면
    else if (gameState == "ready") 
        drawReady();
    
    // playing : 공이 발사되어 게임이 진행 중인 화면
    else if (gameState == "playing") 
        drawPlaying();

    // clear : 모든 스테이지를 클리어 했을 시 화면
    else if (gameState == "clear") 
        drawClear();

    // stageclear : 마지막 스테이지 이전 스테이지를 클리어 했을 시 화면
    else if (gameState == "stageclear") 
        drawStageClear();

    // gameover : 게임 오버 시 화면
    else if (gameState == "gameover") 
        drawGameOver();
    
    // exit : 게임 종료 
    else if (gameState == "exit") 
        ofExit();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // 랭크 열람 시 a키로 이전 페이지, d키로 다음 페이지, q로 메뉴 이동 기능
    if (gameState == "rank") {
        if ((key == 'a' || key == 'A') && rankPage != 1)
            rankPage--;
        else if ((key == 'd' || key == 'D') && rankPage != stages.size())
            rankPage++;
        else if (key == 'q' || key == 'Q')
            gameState = "start";
    }

    // 게임 오버, 스테이지 클리어나, 전체 클리어 시 이름 입력
    else if (gameState == "gameover" || gameState == "stageclear" || gameState == "clear") {
        // 이름 3글자 입력 후 아무 키 입력 시 다음 스테이지 또는 시작 화면
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
    // 마우스가 움직일 때마다 패들 위치 조정
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
