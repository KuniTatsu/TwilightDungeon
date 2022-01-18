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



	void Update();

	void Draw();
private:
	//階層
	int dungeonLevel = 1;

	//次の回に行けるwindow
	Menu* nextLevelWindow = nullptr;

	std::shared_ptr<EnemyManager>eManager = nullptr;

	//**debug
	t2k::Vector3 playerPos = {};

};