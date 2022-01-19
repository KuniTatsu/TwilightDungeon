#pragma once
#define _HAS_STD_BYTE 0
#include<vector>
#include"../library/t2klib.h"

using namespace std;

class Camera;

class Map {

public:
	//�}�b�v���̏�����(���ׂĂ�ǂɂ���)
	Map(int Width, int Height);

	//�}�b�v�̔w�i
	vector<vector<int>> ground;

	enum mapElements {
		WALL,
		PASSWAY,
		CHARACTOR,
		STAIRS,
	};

	//�}�b�v�`�b�v���擾
	int GetChip(int x, int y);

	//�}�b�v�`�b�v��SetChip�ŏ�������
	void SetChip(int x, int y, int SetChip);
	//���͈͂��ׂĂ𓹂ŏ�������
	void SetAllChip(int Left, int Up, int Right, int Down);

	void DivideStart(int Width, int Height, Map* map);

	t2k::Vector3 WorldToMap(int WorldX, int WorldY);
	t2k::Vector3 MapToWorld(int MapX, int MapY);

	//�O�����畔���̐����擾����Ƃ��̊֐�
	int GetRoomNum();

	//�O���������̕������擾����Ƃ��̊֐�
	vector<int> GetRoom(int roomNum);

	//�K�i�̐ݒu
	void SetStairs(int x, int y);

	//�}�b�v�S�̂̕`��
	void MapDraw();

private:


	bool doneFirstDivide = false;

	//�����̍ŏ���
	const int roomMinWidth = 5;
	//�����̍ŏ�����
	const int roomMinHeight = 5;
	//�����̍ő吔
	const int roomMaxNum = 8;


	//�}�b�v�̕�(�R���X�g���N�^�Ŏg�p)
	int width = 0;
	//�}�b�v�̍���(�R���X�g���N�^�Ŏg�p)
	int height = 0;
	//�͈͊O����p
	int outOfRange = -1;

	//�}�b�v�`�b�v
	int mapChip[3] = {};



	//��ԕ���(���)
	vector<vector<int>> divideArea;

	//����������Ԃ���U�ۑ�����vector
	vector<vector<int>> dumpDivideArea;

	//��ԕ���(�敪��)
	vector<vector<int>> divideLine;
	//��ԕ���(����)
	vector<vector<int>> divideRoom;



	enum mapDir {
		VERTICAL,
		HORIZONTAL,
	};

	bool IsOutOfRange(int x, int y);

	//��ԕ����@
	//�㉺���E�̍��W�ƕ����ԍ����擾����vector�Ɋi�[
	void SetDivideArea(int Left, int Up, int Right, int Down, int Id);

	//���������G���A�̑傫�������i�[����֐�
	void SetLargeDivideArea(int Left, int Up, int Right, int Down, int Id);

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


	
	//�����̕ӂɒʘH�����邩�ǂ����m�F����֐�
	//arg1:�������Ȃ����W(�c�Ɍ����Ȃ�x���W,���Ɍ����Ȃ�y���W
	//arg2:��������ӂ̍ŏ��̍��W
	//arg3:��������ӂ̍ő�̍��W
	//arg4:�����c�� ��:0,�c:1
	bool CheckPassWay(int roomPos_set, int roomPos_moveStart, int roomPos_moveGoal,int dir);
};
