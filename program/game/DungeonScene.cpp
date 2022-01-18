#include "DungeonScene.h"
#include"Map.h"
#include"GameManager.h"

#include"../support/Support.h"
#include "Player.h"
#include"MenuWindow.h"

extern GameManager* gManager;

DungeonScene::DungeonScene()
{
	nextLevelWindow = new Menu(300, 300, 300, 200, "graphics/WindowBase_01.png");


}

DungeonScene::~DungeonScene()
{
}

void DungeonScene::Update()
{
	//playerPos = gManager->GetMapChip(gManager->player->pos);
	playerPos = gManager->WorldToLocal(gManager->player->pos);

	//もしplayerが階段の上にいたら
	//windowを出す
	//enterで次の階へ
	if (gManager->GetMapChip(playerPos)== 3) {
		//nextLevelWindow->Menu_Draw();
		if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
			dungeonLevel += 1;
			gManager->ReCreate();
		}
	}


	//デバッグ用マップ再生成
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_SPACE)) {
		gManager->ReCreate();
	}
	gManager->player->Move();
	////スクリーン全体をズーム,ズームアウトしたかった
	////失敗コード
	//if (t2k::Input::isKeyDown(t2k::Input::KEYBORD_1)) {
	//	gManager->graphEx += 0.02;
	//}
	//else if (t2k::Input::isKeyDown(t2k::Input::KEYBORD_2)) {
	//	gManager->graphEx -= 0.02;
	//}
	
}

void DungeonScene::Draw()
{
	gManager->map->MapDraw();
	gManager->player->Draw();
	DrawStringEx(100, 100, -1, "%d", dungeonLevel);

	DrawStringEx(100, 120, -1, "PlayerMapChipX:%d", (int)playerPos.x);
	DrawStringEx(100, 140, -1, "PlayerMapChipY:%d", (int)playerPos.y);
	if (gManager->GetMapChip(playerPos) == 3) {
		nextLevelWindow->Menu_Draw();
		DrawStringEx(nextLevelWindow->menu_x + 10, nextLevelWindow->menu_y + 100, -1, "Enterで次の階へ");
	}
}
