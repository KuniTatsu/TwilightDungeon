#pragma once
#include"Scene.h"
#include "../library/t2klib.h"
#include<memory>

class Map;
class Menu;
class MenuWindow;
class EnemyManager;

class DungeonScene :public BaseScene {

public:
	DungeonScene();
	~DungeonScene();


	//num�̓G�����֐�
	void RandEnemyCreate(int num);

	void Update();

	void Draw();

	int GetDungeonLevel();
	void SetDungeonLevel(int addLevel);

	void MoveLevel(int addLevel);

private:
	//�K�w
	int dungeonLevel = 1;

	int alfa = 0;

	bool skip = false;

	//���̉�ɍs����window
	Menu* nextLevelWindow = nullptr;

	MenuWindow* firstMenu = nullptr;
	
	/*
	Menu* hoge=new Menu(30,50,175,390,gh(int))
	Menu* hoge=new Menu(220,50,440,390,gh(int))
	*/

	std::shared_ptr<EnemyManager>eManager = nullptr;

	//**debug
	t2k::Vector3 playerPos = {};

	t2k::Sequence<DungeonScene*> main_sequence =
		t2k::Sequence<DungeonScene*>(this, &DungeonScene::Seq_Main);

	bool Seq_Main(const float deltatime);
	bool Seq_EnemyAct(const float deltatime);



	//debug
	bool Seq_CameraMove(const float deltatime);
	//�V�[�N�G���X�̗񋓑�
	enum class sequence {
		MAIN,
		ENEMYACT,
		CAMERA

	};
	sequence nowSeq = sequence::MAIN;
	//Sequence���ړ�������֐�,enum���ꏏ�ɕύX����
	void ChangeSequence(sequence seq);
	//�V�[�P���X�m�F�֐�
	void DrawNowSequence(sequence seq);

	//enemy�̃f�[�^�̊m�F�֐�
	void DrawEnemyData();

};