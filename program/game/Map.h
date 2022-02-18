#pragma once
//#define _HAS_STD_BYTE 0
#include<vector>
#include"../library/t2klib.h"
#include"DxLib.h"
#include<memory>


class Camera;
class Player;

class Map {

public:
	//�}�b�v���̏�����(���ׂĂ�ǂɂ���)
	Map(const int Width, const int Height);

	//�}�b�v�̔w�i
	std::vector<std::vector<int>> ground;

	std::shared_ptr<Player> player = nullptr;

	//�`�b�v��̑傫��
	const int SIZE = 20;

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
	inline int GetChip(const int x, const int y) {
		if (IsOutOfRange(x, y))return OUTOFRANGE;
		return ground[y][x];
	}

	//�͈͓��̃}�b�v�`�b�v���擾
	void GetAllChip(const int roomNum, std::vector<std::vector<int>>& chips);

	//�}�b�v�`�b�v��SetChip�ŏ�������
	void SetChip(const int x, const int y, const int SetChip);
	//���͈͂��ׂĂ𓹂ŏ�������
	void SetAllChip(const int Left, const int Up, const int Right, const int Down);

	void DivideStart(const int Width, const int Height, Map* map);

	inline t2k::Vector3 WorldToMap(const int WorldX, const int WorldY) {
		int mapX = WorldX / SIZE;
		int mapY = WorldY / SIZE;

		return t2k::Vector3(mapX, mapY, 0);
	}
	inline t2k::Vector3 MapToWorld(const int MapX, const int MapY) {
		int worldX = MapX * SIZE;
		int worldY = MapY * SIZE;

		return t2k::Vector3(worldX, worldY, 0);
	}

	//�O������"�����̐�"���擾����Ƃ��̊֐�
	inline int GetRoomNum() {
		return divideRoom.size() - 1;
	}

	//�O������"����̕���"���擾����Ƃ��̊֐�
	inline std::vector<int> GetRoom(int roomNum) {
		return divideRoom[roomNum];
	}

	//�O������"����̕����̑傫��"���擾����Ƃ��̊֐�
	t2k::Vector3 GetRoomValue(const int roomNum);

	//�������W�n��0,0���擾����֐�
	t2k::Vector3 GetRoomStartPos(const int roomNum);


	//����̃}�b�v���W�������̂ǂ����ɑ��݂��邩�m�F����֐�
	int CheckIsThere(const int x, const int y);

	//�}�b�v�S�̂̕`��
	void MapDraw();

	//�~�j�}�b�v�`��
	void MiniMapDraw();

	//���\������Ă���S�Ă̕����̏㉺���E���W������`�悷��Debug�֐�
	void DrawAllRoomPos(const std::vector<std::vector<int>>RoomList);

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
	const int OUTOFRANGE = -1;

	//�}�b�v�`�b�v
	int mapChip[3] = {};

	//�I�[�g�^�C���p�񋓑�
	enum graphicPattern
	{
		ROOMWALL,
		ROOMTOP,
		ROOMRIGHT,
		ROOMBOTTOM,
		ROOMLEFT,
		ROOMLEFTTOP,
		ROOMRIGHTTOP,
		ROOMLEFTBOTTOM,
		ROOMRIGHTBOTTOM,
		MAXNUM
	};

	//�I�[�g�^�C���p�`�b�v
	int autoTileChip[graphicPattern::MAXNUM];

	int ue = 0;
	int migi = 0;
	int shita = 0;
	int hidari = 0;

	int voidGh = 0;

	//

	//�~�j�}�b�v�`��p�}�b�v�`�b�v
	int miniMapChip[3] = {};
	//�~�j�}�b�v�v���C���[gh
	int miniPlayer = 0;

	//��ԕ���(���)
	std::vector<std::vector<int>> divideArea;

	//����������Ԃ���U�ۑ�����vector
	std::vector<std::vector<int>> dumpDivideArea;

	//��ԕ���(�敪��)
	std::vector<std::vector<int>> divideLine;
	//��ԕ���(����) ��,��,�E,��,id
	std::vector<std::vector<int>> divideRoom;



	enum mapDir {
		VERTICAL,
		HORIZONTAL,
	};

	bool IsOutOfRange(const int x, const int y);

	//��ԕ����@
	//�㉺���E�̍��W�ƕ����ԍ����擾����vector�Ɋi�[
	void SetDivideArea(const int Left, const int Up, const int Right, const int Down, const int Id);

	//���������G���A�̑傫�������i�[����֐�
	void SetLargeDivideArea(const int Left, const int Up, const int Right, const int Down, const int Id);

	//�S�G���A����ŕ���
	//�n�_,�I�_��xy,����
	void SetDivideLine(const int Start_x, const int Start_y, const int Goal_x, const int Goal_y, const int Dir);

	//�����̊i�[
	void SetDivideRoom(const int Left, const int Up, const int Right, const int Down, const int RoomId);


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
	bool CheckPassWay(const int roomPos_set, const int roomPos_moveStart, const int roomPos_moveGoal, const int dir);

	//�ŏ��ƍŌ�̕����������̕����������ւ̓���������֐� 
	t2k::Vector3 RandomPoint(const int roomId, int& dir);
	//t2k::Vector3 Map::RandomPoint(int roomId, int& dir,vector<int>setDivideLine);

	//���������אڂ��镔���ւƒʘH��L�΂��֐�
	bool CreateSecondWay(const int x, const int y, const int dir, const int roomId);


	//�Z��������͂̒ʘH���m�F���A�`�悷�ׂ��摜�����Ԃ��擾����֐� 
	int CheckAroundWay(const int x, const int y);

	//�Z�����猩�Ď��͂ɒʘH�����邩�ǂ����m���߂�֐�
	bool CheckAround(const int x, const int y);
	//���̍��W�̃Z�����ʘH��������true��Ԃ��֐�
	bool CheckThisCell(const int x, const int y);

	//�Z��(x,y)����݂�dir�����ɐ����ȃZ��2��dir�����̃Z�����ǂ��ʘH���m�F����֐�
	bool CheckChip(const int x, const int y, const int nextDir);

	//�F�R�[�h����string��Ԃ��֐�
	std::string GetColorName(const int code);


};
