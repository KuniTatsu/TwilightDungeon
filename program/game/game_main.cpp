#include "DxLib.h"
#include "../library/t2klib.h"
#include "../support/Support.h"
#include "game_main.h"
#include <time.h>
#include <list>
#include <map>
#include <algorithm>
#include"GameManager.h"

bool init = false;

GameManager* gManager = nullptr;

void gameMain(float deltatime) {

	if (!init) {

		gManager = new GameManager();

		gManager->initGameManager();

		init = true;
	}
	
	gManager->deitatime_ = deltatime;
	gManager->Update();
	gManager->Draw();
}

