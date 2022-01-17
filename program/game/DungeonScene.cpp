#include "DungeonScene.h"
#include"Map.h"
#include"GameManager.h"
#include "../library/t2klib.h"
#include"../support/Support.h"
#include "Player.h"

extern GameManager* gManager;

DungeonScene::DungeonScene()
{



}

DungeonScene::~DungeonScene()
{
}

void DungeonScene::Update()
{
	//デバッグ用マップ再生成
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_SPACE)) {
		gManager->ReCreate();
	}

	//スクリーン全体をズーム,ズームアウトしたかった
	//失敗コード
	if (t2k::Input::isKeyDown(t2k::Input::KEYBORD_1)) {
		gManager->graphEx += 0.02;
	}
	else if (t2k::Input::isKeyDown(t2k::Input::KEYBORD_2)) {
		gManager->graphEx -= 0.02;
	}
	
}

void DungeonScene::Draw()
{
	gManager->map->MapDraw();
	gManager->player->Draw();
}
