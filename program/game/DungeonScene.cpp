#include "DungeonScene.h"
#include"Map.h"
#include"GameManager.h"

#include"../support/Support.h"
#include "Player.h"
#include"MenuWindow.h"
#include"Actor/EnemyManager.h"
#include"Actor/Enemy.h"

extern GameManager* gManager;

DungeonScene::DungeonScene()
{
	nextLevelWindow = new Menu(300, 300, 300, 200, "graphics/WindowBase_01.png");
	eManager = std::make_shared<EnemyManager>();
	for (int i = 0; i < 5; ++i) {
		int rand = GetRand(100) % 6+100;
		eManager->CreateEnemy(rand);
	}
}

DungeonScene::~DungeonScene()
{
}

void DungeonScene::Update()
{
	//playerPos = gManager->GetMapChip(gManager->player->pos);
	playerPos = gManager->WorldToLocal(gManager->player->pos);

	//����player���K�i�̏�ɂ�����
	//window���o��
	//enter�Ŏ��̊K��
	if (gManager->GetMapChip(playerPos) == 3) {
		//nextLevelWindow->Menu_Draw();
		if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
			dungeonLevel += 1;
			gManager->ReCreate();
		}
	}

	//enemy�̈ړ�
	auto it = eManager->liveEnemyList.begin();
	for (auto hoge : eManager->liveEnemyList) {
		hoge->Move();
		//hoge->Draw();
	}


	//�f�o�b�O�p�}�b�v�Đ���
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_SPACE)) {
		gManager->ReCreate();
	}
	gManager->player->Move();
	////�X�N���[���S�̂��Y�[��,�Y�[���A�E�g����������
	////���s�R�[�h
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
	for (auto hoge : eManager->liveEnemyList) {
		hoge->Draw();
	}


	DrawStringEx(100, 100, -1, "%d", dungeonLevel);

	DrawStringEx(100, 120, -1, "PlayerMapChipX:%d", (int)playerPos.x);
	DrawStringEx(100, 140, -1, "PlayerMapChipY:%d", (int)playerPos.y);
	if (gManager->GetMapChip(playerPos) == 3) {
		nextLevelWindow->Menu_Draw();
		DrawStringEx(nextLevelWindow->menu_x + 10, nextLevelWindow->menu_y + 100, -1, "Enter�Ŏ��̊K��");
	}
}
