#pragma once
#include <iostream>
#include<vector>
#include<stdlib.h>
#include<algorithm>
using namespace std;

class Map {

public:
	//�}�b�v���̏�����(���ׂĂ�ǂɂ���)
	Map(int Width, int Height);
	//�}�b�v�`�b�v���擾
	int GetChip(int x, int y);
	//�}�b�v�`�b�v��SetChip�ŏ�������
	void SetChip(int x, int y, int SetChip);
	//���͈͂��ׂĂ���������
	void SetAllChip(int Left, int Up, int Right, int Down);

	void DivideStart(int Width, int Height, Map* map);

	//�}�b�v�S�̂̕`��
	void MapDraw();

private:
	//�����̍ŏ���
	const int roomMinWidth = 6;
	//�����̍ŏ�����
	const int roomMinHeight = 6;
	//�����̍ő吔
	const int roomMaxNum = 10;
	

	//�}�b�v�̕�(�R���X�g���N�^�Ŏg�p)
	int width = 0;
	//�}�b�v�̍���(�R���X�g���N�^�Ŏg�p)
	int height = 0;
	//�͈͊O����p
	int outOfRange = -1;

	//�}�b�v�`�b�v
	int mapChip[3] = {};


	//�}�b�v�̔w�i
	vector<vector<int>> ground;
	//��ԕ���(���)
	vector<vector<int>> divideArea;
	//��ԕ���(�敪��)
	vector<vector<int>> divideLine;
	//��ԕ���(����)
	vector<vector<int>> divideRoom;

	enum mapElements {
		WALL,
		ROUTE,
		CHARACTOR,
	};

	enum mapDir {
		VERTICAL,
		HORIZONTAL,
	};

	bool IsOutOfRange(int x, int y);

	//��ԕ����@
	//�㉺���E�̍��W�ƕ����ԍ����擾����vector�Ɋi�[
	void SetDivideArea(int Left, int Up, int Right, int Down, int Id);

	//�S�G���A����ŕ���
	//�n�_,�I�_��xy,����
	void SetDivideLine(int Start_x, int Start_y, int Goal_x, int Goal_y, int Dir);

	//�����̊i�[
	void SetDivideRoom(int Left, int Up, int Right, int Down);


	//���̕���
	void AreaDivide();
	//�����̍쐬
	void CreateRoom();
	//�ʘH�̍쐬
	void CreatePassWay();


	
};
