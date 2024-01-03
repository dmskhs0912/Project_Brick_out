#include "stage.h"
#include <algorithm>
#include <fstream>
#include <sstream>

// stage ���� �߰�
void Stage::addBrick(const Brick& brick)
{
	bricks.push_back(brick);
}

// stage ��ũ �߰�
void Stage::addRank(const Rank& rank)
{
	ranks.push_back(rank);
}

// �ǹ��� �������� ū ���� ����, ���� ���� ���� �ʿ� ��ġ �� �ǹ��� �ε��� ����
int partition(vector<Rank>& arr, int low, int high)
{
	int pivot = arr[high].score; // ���� ������ ���Ҹ� �ǹ����� ����
	int i = low - 1;

	for (int j = low; j < high; j++) {
		if (arr[j].score > pivot) { // ���ʺ��� Ž���ϸ� �ǹ����� ū ���� �߽߰� 
			i++;
			swap(arr[i], arr[j]); // i �ϳ� ���� �� �ش� ���ҿ� �ڸ� �ٲ� -> ū ���Ҵ� ����, ���� ���Ҵ� ���������� �з���.
		}
	}
	swap(arr[i + 1], arr[high]); // i+1 ��° ���ҿ� �ǹ� �ڸ� �ٲ� -> �ǹ��� i+1��° ��ġ
	return (i + 1);
}

// Divide and conquer ������� �����ϴ� ������ �˰���.
void quickSort(vector<Rank>& arr, int low, int high)
{
	if (low < high) {
		int pivotIndex = partition(arr, low, high);
		// �ǹ� �������� Divide -> ���� quicksort(conquer)
		quickSort(arr, low, pivotIndex - 1);
		quickSort(arr, pivotIndex + 1, high);
	}
}

// Quick sort�� �̿��� ��ũ ������ ����
void Stage::sortRanks()
{
	quickSort(ranks, 0, ranks.size() - 1);
}

// stage(��ȣ)_ranks.txt ������ �о� ranks�� �߰�
void Stage::readRanks()
{
	ostringstream fileName;
	fileName << "data/stage" << stageNumber << "_ranks.txt"; // ���ϸ� ����
	ifstream inFile(fileName.str());

	if (inFile.is_open()) {
		string line;
		// �� �پ� ���Ͽ��� �о����
		while (getline(inFile, line)) {
			istringstream lineStream(line);
			string name;
			int score;

			// ���� ���� �̸��� ���� �Ľ�
			lineStream >> name >> score;
			if (!lineStream.fail()) {
				addRank(Rank(name, score));
			}
		}
		inFile.close();
	}
	
}

// stage(��ȣ)_ranks.txt ���Ϸ� ��ũ ���� ����ϴ� �Լ�
void Stage::writeRanks()
{
	ostringstream fileName;
	fileName << "data/stage" << stageNumber << "_ranks.txt"; // ���ϸ� ����
	ofstream outFile(fileName.str());

	if (outFile.is_open()) {
		for (const auto& rank : ranks) {
			outFile << rank.name << " " << rank.score << endl; // "�̸�" "����" ������ �� �پ� ���
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