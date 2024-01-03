#include "stage.h"
#include <algorithm>
#include <fstream>
#include <sstream>

// stage 벽돌 추가
void Stage::addBrick(const Brick& brick)
{
	bricks.push_back(brick);
}

// stage 랭크 추가
void Stage::addRank(const Rank& rank)
{
	ranks.push_back(rank);
}

// 피벗을 기준으로 큰 것은 왼쪽, 작은 것은 오른 쪽에 배치 후 피벗의 인덱스 리턴
int partition(vector<Rank>& arr, int low, int high)
{
	int pivot = arr[high].score; // 가장 오른쪽 원소를 피벗으로 설정
	int i = low - 1;

	for (int j = low; j < high; j++) {
		if (arr[j].score > pivot) { // 왼쪽부터 탐색하며 피벗보다 큰 원소 발견시 
			i++;
			swap(arr[i], arr[j]); // i 하나 증가 후 해당 원소와 자리 바꿈 -> 큰 원소는 왼쪽, 작은 원소는 오른쪽으로 밀려감.
		}
	}
	swap(arr[i + 1], arr[high]); // i+1 번째 원소와 피벗 자리 바꿈 -> 피벗은 i+1번째 위치
	return (i + 1);
}

// Divide and conquer 방식으로 정렬하는 퀵정렬 알고리즘.
void quickSort(vector<Rank>& arr, int low, int high)
{
	if (low < high) {
		int pivotIndex = partition(arr, low, high);
		// 피벗 기준으로 Divide -> 각각 quicksort(conquer)
		quickSort(arr, low, pivotIndex - 1);
		quickSort(arr, pivotIndex + 1, high);
	}
}

// Quick sort를 이용해 랭크 정보를 정렬
void Stage::sortRanks()
{
	quickSort(ranks, 0, ranks.size() - 1);
}

// stage(번호)_ranks.txt 파일을 읽어 ranks에 추가
void Stage::readRanks()
{
	ostringstream fileName;
	fileName << "data/stage" << stageNumber << "_ranks.txt"; // 파일명 설정
	ifstream inFile(fileName.str());

	if (inFile.is_open()) {
		string line;
		// 한 줄씩 파일에서 읽어들임
		while (getline(inFile, line)) {
			istringstream lineStream(line);
			string name;
			int score;

			// 라인 별로 이름과 점수 파싱
			lineStream >> name >> score;
			if (!lineStream.fail()) {
				addRank(Rank(name, score));
			}
		}
		inFile.close();
	}
	
}

// stage(번호)_ranks.txt 파일로 랭크 정보 출력하는 함수
void Stage::writeRanks()
{
	ostringstream fileName;
	fileName << "data/stage" << stageNumber << "_ranks.txt"; // 파일명 설정
	ofstream outFile(fileName.str());

	if (outFile.is_open()) {
		for (const auto& rank : ranks) {
			outFile << rank.name << " " << rank.score << endl; // "이름" "점수" 순으로 한 줄씩 출력
		}
		outFile.close();
	}

	else {
		cerr << fileName.str() << " File open Error" << endl;
	}
}


Stage::Stage(int number)
{
	this->stageNumber = number;
	this->aliveBrick = 0;
}

Stage::~Stage()
{

}