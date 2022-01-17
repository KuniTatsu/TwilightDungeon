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
	//�f�o�b�O�p�}�b�v�Đ���
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_SPACE)) {
		gManager->ReCreate();
	}

	//�X�N���[���S�̂��Y�[��,�Y�[���A�E�g����������
	//���s�R�[�h
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
