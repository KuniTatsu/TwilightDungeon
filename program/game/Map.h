#pragma once
#define _HAS_STD_BYTE 0
#include<vector>
#include"../library/t2klib.h"
#include"DxLib.h"

using namespace std;

class Camera;

class Map {

public:
	//�}�b�v���̏�����(���ׂĂ�ǂɂ���)
	Map(int Width, int Height);

	//�}�b�v�̔w�i
	vector<vector<int>> ground;

	//�����̐�

	int sumRoomNum = 0;

	enum mapElements {
		WALL,
		PASSWAY,
		CHARACTOR,
		STAIRS,
		EXPASSWAY,
	};

	//�}�b�v�`�b�v���擾
	int GetChip(int x, int y);

	//�͈͓��̃}�b�v�`�b�v���擾
	void GetAllChip(int roomNum, std::vector<std::vector<int>>& chips);

	//�}�b�v�`�b�v��SetChip�ŏ�������
	void SetChip(int x, int y, int SetChip);
	//���͈͂��ׂĂ𓹂ŏ�������
	void SetAllChip(int Left, int Up, int Right, int Down);

	void DivideStart(int Width, int Height, Map* map);

	t2k::Vector3 WorldToMap(int WorldX, int WorldY);
	t2k::Vector3 MapToWorld(int MapX, int MapY);

	//�O������"�����̐�"���擾����Ƃ��̊֐�
	int GetRoomNum();

	//�O������"����̕���"���擾����Ƃ��̊֐�
	vector<int> GetRoom(int roomNum);

	//�O������"����̕����̑傫��"���擾����Ƃ��̊֐�
	t2k::Vector3 GetRoomValue(int roomNum);

	//�������W�n��0,0���擾����֐�
	t2k::Vector3 GetRoomStartPos(int roomNum);


	//����̃}�b�v���W�������̂ǂ����ɑ��݂��邩�m�F����֐�
	int CheckIsThere(int x, int y);

	//�}�b�v�S�̂̕`��
	void MapDraw();

	//�~�j�}�b�v�`��
	void MiniMapDraw();

	//���\������Ă���S�Ă̕����̏㉺���E���W������`�悷��Debug�֐�
	void DrawAllRoomPos(vector<vector<int>>RoomList);

private:

	int color_red = GetColor(255, 0, 0);
	int color_green = GetColor(0, 255, 0);
	int color_blue = GetColor(0, 0, 255);
	int color_yellow = GetColor(200, 200, 0);
	int color_purple = GetColor(200, 0, 200);

	int colors[5] = { color_red,color_green ,color_blue,color_yellow,color_purple };


	bool doneFirstDivide = false;

	//�����̍ŏ���
	const int roomMinWidth = 8;
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
	int mapChip[4] = {};



	//��ԕ���(���)
	vector<vector<int>> divideArea;

	//����������Ԃ���U�ۑ�����vector
	vector<vector<int>> dumpDivideArea;

	//��ԕ���(�敪��)
	vector<vector<int>> divideLine;
	//��ԕ���(����) ��,��,�E,��,id
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
	void SetDivideRoom(int Left, int Up, int Right, int Down,int RoomId);


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
	//arg4:�����̂ǂ��������̕ǂ� 0:��,1:�E,2:��,3:��
	bool CheckPassWay(int roomPos_set, int roomPos_moveStart, int roomPos_moveGoal,int dir);

	//�ŏ��ƍŌ�̕����������̕����������ւ̓���������֐� 
	t2k::Vector3 Map::RandomPoint(int roomId, int& dir);
	//t2k::Vector3 Map::RandomPoint(int roomId, int& dir,vector<int>setDivideLine);

	//���������אڂ��镔���ւƒʘH��L�΂��֐�
	bool Map::CreateSecondWay(int x, int y, int dir, int roomId);

	//�Z��(x,y)����݂�dir�����ɐ����ȃZ��2��dir�����̃Z�����ǂ��ʘH���m�F����֐�
	bool Map::CheckChip(int x, int y, int nextDir);

	//�F�R�[�h����string��Ԃ��֐�
	std::string GetColorName(int code);
	
};
