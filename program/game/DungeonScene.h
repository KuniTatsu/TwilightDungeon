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
	//ŠK‘w
	int dungeonLevel = 1;

	//Ÿ‚Ì‰ñ‚És‚¯‚éwindow
	Menu* nextLevelWindow = nullptr;

	//**debug
	t2k::Vector3 playerPos = {};

};