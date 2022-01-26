#include "DungeonScene.h"
#include"Map.h"
#include"GameManager.h"

#include"../support/Support.h"
#include "Player.h"
#include"MenuWindow.h"
#include"Actor/EnemyManager.h"
#include"Actor/Enemy.h"
#include"Camera.h"

extern GameManager* gManager;

DungeonScene::DungeonScene()
{
	alfa = gManager->LoadGraphEx("graphics/old/test.png");
	nextLevelWindow = new Menu(300, 300, 300, 200, "graphics/WindowBase_01.png");
	eManager = std::make_shared<EnemyManager>();
	/*for (int i = 0; i < 5; ++i) {
		int rand = GetRand(100) % 6+100;
		eManager->CreateEnemy(rand);
	}*/
	RandEnemyCreate(5);
}

DungeonScene::~DungeonScene()
{
}

void DungeonScene::RandEnemyCreate(int num)
{
	for (int i = 0; i < num; ++i) {
		int rand = GetRand(100) % 6 + 100;
		eManager->CreateEnemy(rand);
	}
}

void DungeonScene::Update()
{
	main_sequence.update(gManager->deitatime_);

	//�f�o�b�O�؂�ւ�
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_LSHIFT)) {
		if (gManager->isDebug)gManager->isDebug = false;
		else gManager->isDebug = true;
	}

	//�f�o�b�O�p�}�b�v�Đ���
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_SPACE)) {

		/*for (auto enemy : eManager->liveEnemyList) {
			enemy->isLive = false;
		}*/
		if (!eManager->liveEnemyList.empty())eManager->liveEnemyList.clear();
		dungeonLevel++;
		gManager->ReCreate();
		RandEnemyCreate(5);
	}

	//isLive��false�ȓG�����X�g����O������
	/*std::list<std::shared_ptr<Enemy>>::iterator hoge = eManager->liveEnemyList.begin();
	for (int i = 0; i < eManager->liveEnemyList.size(); ++i) {
		if ((*it)->isLive == false) {
			eManager->liveEnemyList.erase(it);
		}
		it++;
	}*/


	////�X�N���[���S�̂��Y�[��,�Y�[���A�E�g����������

}

void DungeonScene::Draw()
{
	gManager->map->MapDraw();
	gManager->player->Draw();
	for (auto hoge : eManager->liveEnemyList) {
		hoge->Draw();
	}

	if (gManager->isDebug) {
		DrawStringEx(100, 280, -1, "���݂̊K�w:%d", dungeonLevel);

		DrawStringEx(100, 300, -1, "PlayerMapChipX:%d", (int)playerPos.x);
		DrawStringEx(100, 320, -1, "PlayerMapChipY:%d", (int)playerPos.y);

	}
	if (gManager->GetMapChip(playerPos) == 3) {
		nextLevelWindow->Menu_Draw();
		DrawStringEx(nextLevelWindow->menu_x + 10, nextLevelWindow->menu_y + 100, -1, "Enter�Ŏ��̊K��");
	}
	for (auto hoge : gManager->wayPoint) {
		for (auto hage : hoge) {
			int X = (int)hage.x;
			int Y = (int)hage.y;
			DrawCircle(X * 20 - gManager->camera->cameraPos.x, Y * 20 - gManager->camera->cameraPos.y, 10, -1, true);
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawRotaGraph(gManager->player->pos.x - gManager->camera->cameraPos.x, gManager->player->pos.y - gManager->camera->cameraPos.y, 1, 0, alfa, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	//gManager->map->DrawAllRoomPos(gManager->map.)
}

bool DungeonScene::Seq_Main(const float deltatime)
{
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_ESCAPE)) {
		ChangeSequence(sequence::CAMERA);
		return true;
	}


	//���t���[���擾����K�v�͂Ȃ��������ړ���Ɋm�F,�G���A�ړ����1�񂾂��m�F����悤�ɕύX������
	playerPos = gManager->WorldToLocal(gManager->player->pos);

	//����player���K�i�̏�ɂ�����
	//window���o��
	//enter�Ŏ��̊K��
	if (gManager->GetMapChip(playerPos) == 3) {

		if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
			dungeonLevel += 1;
			if (!eManager->liveEnemyList.empty())eManager->liveEnemyList.clear();
			gManager->ReCreate();
			RandEnemyCreate(5);
		}
	}

	//enemy�̈ړ�
	auto it = eManager->liveEnemyList.begin();
	for (auto hoge : eManager->liveEnemyList) {
		hoge->Move();
	}

	gManager->player->Move();

	return true;
}

bool DungeonScene::Seq_CameraMove(const float deltatime)
{
	gManager->camera->CameraMove();


	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_ESCAPE)) {
		ChangeSequence(sequence::MAIN);
		gManager->CameraMove(gManager->player);
		return true;
	}

	return true;
}

void DungeonScene::ChangeSequence(sequence seq)
{
	nowSeq = seq;
	if (seq == sequence::MAIN) {
		main_sequence.change(&DungeonScene::Seq_Main);
	}
	else if (seq == sequence::CAMERA) {
		main_sequence.change(&DungeonScene::Seq_CameraMove);
	}

}
