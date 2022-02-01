#pragma once
#include"Scene.h"
#include "../library/t2klib.h"
#include<memory>

class Map;
class Menu;
class EnemyManager;

class DungeonScene :public BaseScene {

public:
	DungeonScene();
	~DungeonScene();


	//num体敵を作る関数
	void RandEnemyCreate(int num);

	void Update();

	void Draw();
private:
	//階層
	int dungeonLevel = 1;

	int alfa = 0;

	bool skip = false;

	//次の回に行けるwindow
	Menu* nextLevelWindow = nullptr;

	std::shared_ptr<EnemyManager>eManager = nullptr;

	//**debug
	t2k::Vector3 playerPos = {};

	t2k::Sequence<DungeonScene*> main_sequence =
		t2k::Sequence<DungeonScene*>(this, &DungeonScene::Seq_Main);

	bool Seq_Main(const float deltatime);
	bool Seq_EnemyAct(const float deltatime);



	//debug
	bool Seq_CameraMove(const float deltatime);
	//シークエンスの列挙体
	enum class sequence {
		MAIN,
		ENEMYACT,
		CAMERA
		
	};
	sequence nowSeq = sequence::MAIN;
	//Sequenceを移動させる関数,enumも一緒に変更する
	void ChangeSequence(sequence seq);

	void DrawNowSequence(sequence seq);



};