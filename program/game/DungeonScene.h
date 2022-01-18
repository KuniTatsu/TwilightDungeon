#pragma once
#include"Scene.h"
#include "../library/t2klib.h"

class Map;
class Menu;

class DungeonScene :public BaseScene {

public:
	DungeonScene();
	~DungeonScene();



	void Update();

	void Draw();
private:
	//�K�w
	int dungeonLevel = 1;

	//���̉�ɍs����window
	Menu* nextLevelWindow = nullptr;

	//**debug
	t2k::Vector3 playerPos = {};

};