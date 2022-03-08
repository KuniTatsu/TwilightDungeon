///*****Description*****
///�_���W��������O�̑S�Ă��s���V�[���N���X
///Update��Draw�֐���SceneManager�ōs����
///Update��Draw�֐��̒��ł̓V�[�N�G���X�Ŗ��t���[���s���鏈�������򂷂�
///�e�V�[�N�G���X�Ԃ�ChangeSequence�֐��ɂ���Ď��t���[�����瑖��V�[�N�G���X���ړ�����
///*********************

#pragma once
#include"Scene.h"
#include "../library/t2klib.h"
#include<memory>
#include<string>

class Player;
class MenuWindow;

class CampScene :public BaseScene {

public:
	CampScene();
	~CampScene();


	void Update();

	void Draw();

	inline int canMoveThisPoint(int x, int y) {
		return collisionData[y][x];
	}

private:
	//�����̂�����̃O���t�B�b�N�n���h��
	int fragmentsGh[6];
	//�w�i
	int fragBackGround = 0;

	std::shared_ptr<Player>player;

	std::vector<std::vector<std::string>>loadGroundMap;
	std::vector<std::vector<std::string>>loadSurfaceMap;
	std::vector<std::vector<std::string>>loadCollision;

	std::vector<std::vector<int>>groundMapData;
	std::vector<std::vector<int>>surfaceMapData;
	std::vector<std::vector<int>>collisionData;

	MenuWindow* dungeonIn = nullptr;

	bool nowFade = false;

	//mapchip_night_20.png�̓ǂݍ��� ��x�c=30x20
	int campGraphic[600];
	//�摜�n���h���擾�֐�
	int GetGraphicHandle(int num);

	//����̍��W�̉摜�ԍ���Ԃ��֐�
	inline int GetGraphicNum(int x, int y) {
		return groundMapData[y][x];
	}
	inline int GetSurfaceGraphicNum(int x, int y) {
		return surfaceMapData[y][x];
	}

	//�͈͊O�G���[�h�~�p�֐�
	inline bool checkOutOfRange(t2k::Vector3 nowLocalPos) {
		if (nowLocalPos.x < 1)return false;
		else if (nowLocalPos.y < 1)return false;
		else if (nowLocalPos.x > groundMapData[0].size()-1)return false;
		else if (nowLocalPos.y > groundMapData.size()-1)return false;
		else return true;
	}

	void LoadMap(std::string fileName, std::vector<std::vector<std::string>>& stringData, std::vector<std::vector<int>>& intData);
	void DrawMap(std::vector<std::vector<int>>intData);

	const int PORTALPOINTNUM[5] = { 67,187,307,421,427 };

	/*TOWER,
		FOREST,
		WATERWAY,
		DUNGEONMAX*/
	//�N�����邩�o�������j���[�̃_���W����
	int selectDungeon = 0;

	const std::string DUNGEONNAME[5] = { "���̃_���W����","�X�̃_���W����","�ܔM�̃_���W����","��̃_���W����","�΂̃_���W����" };
	
	t2k::Sequence<CampScene*> mainSequence =
		t2k::Sequence<CampScene*>(this, &CampScene::SeqFadeIn);

	bool SeqMain(const float deltatime);
	bool SeqDungeonInMenu(const float deltatime);
	//�t�F�[�h�C���V�[�N�G���X
	bool SeqFadeIn(const float deltatime);

	//�V�[�N�G���X�̗񋓑�
	enum class sequence {
		MAIN,
		DUNGEONIN,
		FADEIN,
	};
	sequence nowSeq = sequence::MAIN;
	//Sequence���ړ�������֐�,enum���ꏏ�ɕύX����
	void ChangeSequence(const sequence seq);

	bool drawFrag = false;
};