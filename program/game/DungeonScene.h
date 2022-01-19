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


	//num‘Ì“G‚ğì‚éŠÖ”
	void RandEnemyCreate(int num);

	void Update();

	void Draw();
private:
	//ŠK‘w
	int dungeonLevel = 1;

	//Ÿ‚Ì‰ñ‚És‚¯‚éwindow
	Menu* nextLevelWindow = nullptr;

	std::shared_ptr<EnemyManager>eManager = nullptr;

	//**debug
	t2k::Vector3 playerPos = {};

};