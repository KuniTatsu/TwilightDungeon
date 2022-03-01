#include "DungeonScene.h"
#include"Map.h"
#include"GameManager.h"
#include"SoundManager.h"
#include"../support/Support.h"
#include "Player.h"
#include"MenuWindow.h"
#include"Actor/EnemyManager.h"
#include"Actor/Enemy.h"
#include"Camera.h"
#include"Item/Item.h"
#include"Item/equipItem.h"
#include"Item/Inventory.h"
#include"Animation.h"
#include"FadeControl.h"
#include"SceneManager.h"

using namespace std;

extern GameManager* gManager;

DungeonScene::DungeonScene()
{
	initDungeonScene();
}

DungeonScene::~DungeonScene()
{
	delete nextLevelWindow;
	delete menuOpen;
	//delete inventory;	�V�[����ς��Ă������A�C�e���̃f�[�^�͈����p�������@�����p�������������\��:�D��x��
	delete log;
	delete desc;
	delete playerStatus;
	delete use_usable;
	delete use_equip;
	delete use_nowEquip;
	delete firstMenu;
}

void DungeonScene::RandEnemyCreate(int num)
{
	for (int i = 0; i < num; ++i) {
		int random = rand() % 6 + 100;
		eManager->CreateEnemy(random, dungeonLevel);
	}
	gManager->SetLiveEnemyList(eManager->liveEnemyList);
}

void DungeonScene::Update()
{
	GetMousePoint(&mouseX, &mouseY);

	mainSequence.update(gManager->deitatime_);

	//�f�o�b�O
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_C)) {
		ReturnCamp();
		return;
	}

	//�f�o�b�O�؂�ւ�
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_F2)) {
		if (gManager->isDebug)gManager->isDebug = false;
		else gManager->isDebug = true;
	}

	//�f�o�b�O�p�}�b�v�Đ���
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_F5)) {
		MoveLevel(1);
		return;
	}
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_M)) {
		if (gManager->minimapDraw)gManager->minimapDraw = false;
		else gManager->minimapDraw = true;
	}
	//�A�C�e�������蔻�芴�m

	for (auto item : dropItems) {
		//�A�C�e���ƃv���C���[���d�Ȃ�����
		if (item->DetectOnPlayer(playerPos)) {
			gManager->AddItemToInventory(item->GetItemId());
			item->SetIsLiveFalse();
			//gManager->PopDetectItem(item, dropItems);
		}
	}
	for (auto item : dropItems) {
		if (item->GetIsLive())continue;
		if (gManager->PopDetectItem(item, dropItems))break;
	}
	UpdateAnimation();
	CheckAnimLive();
}

void DungeonScene::Draw()
{
	//�K�w�Ԃ̃t�F�[�h���łȂ���Ε`�悷��
	if (nowSeq != sequence::FADEDESC) {
		gManager->MapDraw();

		DrawPopItem();
		gManager->player->Draw();
		gManager->player->HpVarDraw();

		for (auto enemy : eManager->liveEnemyList) {
			enemy->Draw();
		}

		if (gManager->isDebug) {
			DrawStringEx(100, 280, -1, "���݂̊K�w:%d", dungeonLevel);

			DrawStringEx(100, 300, -1, "PlayerMapChipX:%d", (int)playerPos.x);
			DrawStringEx(100, 320, -1, "PlayerMapChipY:%d", (int)playerPos.y);
			DrawEnemyData();
		}
		//�v���C���[�̈ʒu���K�i�̏�Ȃ�E�B���h�E��\������
		if (gManager->GetMapChip(playerPos) == 3) {
			nextLevelWindow->Menu_Draw();

			DrawStringEx(nextLevelWindow->menu_x + 60, nextLevelWindow->menu_y + 60, -1, "�K�i��������");

			DrawStringEx(nextLevelWindow->menu_x + 60, nextLevelWindow->menu_y + 100, -1, "���̊K�֐i�݂܂����H");

			DrawStringEx(nextLevelWindow->menu_x + 60, nextLevelWindow->menu_y + 140, -1, "Enter�Ŏ��̊K��");

		}
		//�A�j���[�V�������X�g�̕`��
		DrawAnimation();

		if (gManager->isDebug) {
			//���̃V�[�N�G���X����\������
			DrawNowSequence(nowSeq);
		}
	}
	//�������牺�̓V�[�N�G���X���Ƃ̕`��

	firstMenu->All();
	//�t�F�[�h���ȊO�ŕ`��
	if (nowSeq == sequence::MAIN || nowSeq == sequence::ENEMYACT || nowSeq == sequence::PLAYERATTACK || nowSeq == sequence::ENEMYATTACK || nowSeq == sequence::ANIMATION) {
		menuOpen->Menu_Draw();
		DrawStringEx(menuOpen->menu_x + 10, menuOpen->menu_y + 10, -1, "Menu���J��");
		DrawStringEx(menuOpen->menu_x + 10, menuOpen->menu_y + menuOpen->menu_height * 5 / 9, -1, "Press");
		DrawRotaGraph(menuOpen->menu_x + menuOpen->menu_width - 20, menuOpen->menu_y + menuOpen->menu_height * 2 / 3, 1, 0, EButton, false);
	}
	//�C���x���g�����J���Ă��鎞�ɕ`��
	else if (nowSeq == sequence::INVENTORY_OPEN || nowSeq == sequence::INVENTORY_USE) {
		inventory->Menu_Draw();
		DrawInventory();
		if (nowSeq == sequence::INVENTORY_USE) {
			if (usetype == USABLE)use_usable->All();
			else if (usetype == EQUIP)use_equip->All();
			else if (usetype == NOWEQUIP)use_nowEquip->All();
		}
	}
	//�A�C�e�����������A���ł��鎞�ɕ`��
	else if (nowSeq == sequence::THROWITEMMOVE) {
		if (!throwedItemList.empty()) {
			for (auto item : throwedItemList) {
				item->DrawThrowItem();
			}
		}
	}
	//�t�F�[�h���ɕ`��
	else if (nowSeq == sequence::FADEDESC) {
		SetFontSize(30);
		DrawFadeDesc();
		SetFontSize(16);
	}
	if (nowSeq == sequence::FADEDESC)return;
	//���O�̔w�i�`��
	log->Menu_Draw();
	//���O�̕`��
	gManager->LogDraw(log->menu_x, log->menu_y);
	//�v���C���[�X�e�[�^�X�w�i�̕`��
	playerStatus->Menu_Draw();
	//�v���C���[�X�e�[�^�X�̕`��
	player->DrawPlayerStatus(playerStatus->menu_x, playerStatus->menu_y, playerStatus->menu_width, playerStatus->menu_height);

}

int DungeonScene::GetDungeonLevel()
{
	return dungeonLevel;
}

void DungeonScene::SetDungeonLevel(int addLevel)
{
	dungeonLevel += addLevel;
}

void DungeonScene::MoveLevel(int addLevel)
{
	//�󂶂�Ȃ��Ȃ�enemy��S�ď�������
	if (!eManager->liveEnemyList.empty())eManager->liveEnemyList.clear();
	if (!gManager->liveEnemyList.empty())gManager->liveEnemyList.clear();
	//�A�C�e���̏���
	dropItems.clear();

	//�_���W�����̊K�w���ړ�����
	dungeonLevel += addLevel;
	//map�̍Đ���
	gManager->ReCreate();
	//�G�̍Đ���
	RandEnemyCreate(5);
	//�A�C�e���̍Đ���
	for (int i = 0; i < spawnItemNum; ++i) {
		int random = rand() % gManager->GetItemNum();
		SpawnItem(random);
	}
}

void DungeonScene::UpdateAnimation()
{
	if (drawAnimationList.empty())return;
	for (auto anim : drawAnimationList) {
		anim->Update();
	}
}

void DungeonScene::initDungeonScene()
{
	EButton = gManager->LoadGraphEx("graphics/button_E.png");

	nextLevelWindow = new Menu(300, 300, 300, 200, "graphics/WindowBase_01.png");
	menuOpen = new Menu(20, 20, 100, 100, "graphics/WindowBase_01.png");
	inventory = new Menu(255, 50, 420, 340, "graphics/WindowBase_01.png");
	log = new Menu(12, 560, 1000, 200, "graphics/WindowBase_01.png");
	desc = new Menu(680, 300, 320, 90, "graphics/WindowBase_01.png");
	playerStatus = new Menu(512, 560, 500, 200, "graphics/WindowBase_01.png");

	MenuWindow::MenuElement_t* menu_usable = new MenuWindow::MenuElement_t[]{
		{670,450,"�g��",0},
		{670,480,"������",1},
		{670,510,"��߂�",2}
	};
	use_usable = new MenuWindow(640, 440, 90, 100, "graphics/WindowBase_02.png", menu_usable, 3, 0.15);

	MenuWindow::MenuElement_t* menu_equip = new MenuWindow::MenuElement_t[]{
		{670,450,"��������",0},
		{670,480,"������",1},
		{670,510,"��߂�",2}
	};
	use_equip = new MenuWindow(640, 440, 90, 100, "graphics/WindowBase_02.png", menu_equip, 3, 0.15);

	MenuWindow::MenuElement_t* menu_nowEquip = new MenuWindow::MenuElement_t[]{
		{670,450,"�͂���",0},
		{670,480,"������",1},
		{670,510,"��߂�",2}
	};
	use_nowEquip = new MenuWindow(640, 440, 90, 100, "graphics/WindowBase_02.png", menu_nowEquip, 3, 0.15);


	eManager = std::make_shared<EnemyManager>();

	MenuWindow::MenuElement_t* menu_0 = new MenuWindow::MenuElement_t[]{
		{70,80,"������",0},
		{70,110,"����",1},
		{70,140,"�Z�[�u",2},
		{70,170,"�^�C�g���֖߂�",3},
		{70,200,"Esc|���j���[�����",4}
	};
	// ���j���[�E�B���h�E�̃C���X�^���X��
	firstMenu = new MenuWindow(30, 50, 220, 210, "graphics/WindowBase_02.png", menu_0, 5, 0.45);
	gManager->MakePlayer(GameManager::SpawnScene::Dungeon);
	RandEnemyCreate(5);

	for (int i = 0; i < 5; ++i) {
		//int rand = GetRand(100) % gManager->GetItemNum();
		int random = rand() % gManager->GetItemNum();
		SpawnItem(random);
	}
	player = gManager->GetPlayer();
	gManager->RunDungeonBgm();
}

void DungeonScene::DrawAnimation()
{
	if (drawAnimationList.empty())return;
	for (auto anim : drawAnimationList) {
		anim->Draw();
	}
}
void DungeonScene::CheckAnimLive()
{
	auto itr = drawAnimationList.begin();
	while (itr != drawAnimationList.end()) {
		if (!(*itr)->GetIsAlive()) {
			itr = drawAnimationList.erase(itr);
			continue;
		}
		++itr;
	}
}



bool DungeonScene::SeqMain(const float deltatime)
{
	//debug
	//�J�����ړ����[�h
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_ESCAPE)) {
		ChangeSequence(sequence::CAMERA);
		return true;
	}
	//�_���[�W���󂯂�
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_D)) {
		gManager->player->TakeHpEffect(-20);
	}
	//�X�e�[�^�X�㏸
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_U)) {
		gManager->player->ChangeStatus(1, 50, 0);
		for (auto enemy : eManager->liveEnemyList) {
			enemy->ChangeStatus(2, 10, 0);
		}
	}
	//

	//menu���J��
	//����menu�{�^���̏�Ƀ}�E�X��������
	if (gManager->CheckMousePointToRect(mouseX, mouseY, menuOpen->menu_x, menuOpen->menu_width, menuOpen->menu_y, menuOpen->menu_height)) {
		//�}�E�X�N���b�N���Ă�����
		if (t2k::Input::isMouseTrigger(t2k::Input::MOUSE_RELEASED_LEFT)) {
			t2k::debugTrace("\n�����ꂽ��\n");//ok
			gManager->sound->System_Play(gManager->sound->system_select);
			firstMenu->Open();
			ChangeSequence(sequence::FIRSTMENU);
			return true;
		}
	}
	//E�������Ă����j���[���J��
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_E)) {
		gManager->sound->System_Play(gManager->sound->system_select);
		firstMenu->Open();
		ChangeSequence(sequence::FIRSTMENU);
		return true;
	}



	//������(Player�͈ړ������Ƀ^�[���͌o�߂���
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_SPACE)) {
		player->skip = true;
	}

	//����player���K�i�̏�ɂ�����
	//window���o��
	//enter�Ŏ��̊K��
	if (gManager->GetMapChip(playerPos) == 3) {

		if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
			gManager->sound->System_Play(gManager->sound->system_select);
			ChangeSequence(sequence::FADEOUT);
			return true;
		}
	}
	//���N���b�N��R�{�^���ōU��
	//�v���C���[�̍U���͖ڂ̑O�ɑΏۂ����Ȃ������ꍇskip�Ɠ����������s��
	if (t2k::Input::isMouseTrigger(t2k::Input::MOUSE_RELEASED_LEFT) || t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_R)) {
		//�������A�j���[�V�����V�[�N�G���X�ɔ�΂�

		ChangeSequence(sequence::ANIMATION);
		return true;

	}

	//����Player���������� �������̓X�L�b�v������
	if (gManager->player->Move() || player->skip) {

		playerPos = gManager->WorldToLocal(gManager->player->pos);

		ChangeSequence(sequence::ENEMYACT);
		if (player->skip)player->skip = false;
		return true;
	}

	return true;
}

bool DungeonScene::SeqPlayerAttack(const float deltatime)
{
	player->Atack();
	//���S�`�F�b�N
	for (auto enemy : eManager->liveEnemyList) {
		if (enemy->GetStatus(0) <= 0) {
			player->AddExp(enemy->GetExp());
			enemy->isLive = false;
		}
	}

	DeleteDeadEnemy();
	if (!player->skip) {
		ChangeSequence(sequence::ENEMYACT);
		return true;
	}
	ChangeSequence(sequence::ENEMYACT);
	return true;
}

void DungeonScene::ReturnCamp()
{
	gManager->StopBgm();
	player->pos = gManager->SpawnPlayerCamp();
	SceneManager::ChangeScene(SceneManager::CAMP);
}

bool DungeonScene::SeqEnemyAct(const float deltatime)
{
	if (mainSequence.isStart()) {
		for (auto liveEnemy : eManager->liveEnemyList) {
			//player��enemy���ׂ荇���Ă���Ȃ�
			if (gManager->CheckNearByPlayer(liveEnemy))
			{
				//�����v���C���[�̕����������Ă��Ȃ��ꍇ�͌�������
				//�v���C���[�̈ʒu��enemy�̈ʒu�Ƃ̑��Ε������擾����
				int vec = gManager->GetPlayerVec(liveEnemy);

				if (liveEnemy->mydir != vec)liveEnemy->setDir(vec);
				//���X�g�ɓ����
				atackEnemies.emplace_back(liveEnemy);
			}
			else {
				//�����֐��͑S�ē����ł���
				liveEnemy->Move();
			}
		}
		itr = atackEnemies.begin();
	}
	//�U������G���X�g�����炶��Ȃ��Ȃ�
	if (!atackEnemies.empty()) {

		//�A�j���[�V�����V�[�N�G���X�ɔ�΂�
		ChangeSequence(sequence::ANIMATION);
		return true;

		/*
		//*****enemyAttackSeq�ɏ������ړ�

		////�����G����̂����Ȃ�C���^�[�o����0�ɂ���
		//if (atackEnemies.size() == 1)enemyActTimer = ENEMYACTINTERVAL;
		////��̂��U�������邽�߂̃C���^�[�o���v��
		//if (++enemyActTimer > ENEMYACTINTERVAL) {
		//	(*itr)->Atack();
		//	enemyActTimer = 0;
		//	itr = atackEnemies.erase(itr);
		//}
		////���ׂĂ̓G���U�����I����܂ł��̃V�[�N�G���X���o�Ȃ�
		//if (!atackEnemies.empty())return true;
		//*****
		*/
	}
	ChangeSequence(sequence::MAIN);
	return true;
}

bool DungeonScene::SeqEnemyAttack(const float deltatime)
{
	//�����G����̂����Ȃ�C���^�[�o����0�ɂ���
	if (atackEnemies.size() == 1)enemyActTimer = ENEMYACTINTERVAL;
	//��̂��U�������邽�߂̃C���^�[�o���v��
	if (++enemyActTimer > ENEMYACTINTERVAL) {
		(*itr)->Atack();
		enemyActTimer = 0;
		itr = atackEnemies.erase(itr);
	}
	//���ׂĂ̓G���U�����I����܂ł��̃V�[�N�G���X���o�Ȃ�
	if (!atackEnemies.empty())return true;


	//���ׂĂ̍U�����I������MainSequence�ɖ߂�
	ChangeSequence(sequence::MAIN);
	return true;
}

bool DungeonScene::SeqFirstMenu(const float deltatime)
{
	//�C���x���g�����J��
	if (firstMenu->SelectNum == 0 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
		gManager->sound->System_Play(gManager->sound->system_select);
		//menu�̏㉺�𑀍�o���Ȃ�����
		firstMenu->manageSelectFlag = false;
		//gManager->sound->System_Play(gManager->sound->system_select);

		//InventoryOpen�V�[�N�G���X�Ɉړ�����
		ChangeSequence(sequence::INVENTORY_OPEN);
		return true;
	}
	//���j���[�����
	else if (firstMenu->SelectNum == 4 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
		gManager->sound->System_Play(gManager->sound->system_cancel);
		firstMenu->menu_live = false;
		ChangeSequence(sequence::MAIN);
		return true;
	}

	//Esc�L�[�ł����j���[�����
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_ESCAPE))
	{
		gManager->sound->System_Play(gManager->sound->system_cancel);
		firstMenu->menu_live = false;
		ChangeSequence(sequence::MAIN);
		return true;
	}
	return true;
}

bool DungeonScene::SeqInventoryOpen(const float deltatime)
{

	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_ESCAPE))
	{
		gManager->sound->System_Play(gManager->sound->system_cancel);
		firstMenu->manageSelectFlag = true;
		ChangeSequence(sequence::FIRSTMENU);
		return true;
	}
	//�C���x���g���̑���
	ChangeInventory();
	//�����C���x���g������Ȃ�return
	if (gManager->inventories[drawInventoryPage]->inventoryList.empty())return true;
	//���ݑI�𒆂̃J�[�\���ʒu���擾
	int selectNum = gManager->inventories[drawInventoryPage]->GetCursorNum();


	//�����C���x���g�����J���Ă��鎞��enter�������ꂽ��
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
		gManager->sound->System_Play(gManager->sound->system_select);
		//���݂̃J�[�\���̈ʒu�̃A�C�e�����擾����
		auto itr = gManager->inventories[drawInventoryPage]->inventoryList.begin();
		for (int i = 0; i < selectNum; ++i) {
			itr++;
		}
		Item* selectedItem = (*itr);
		//�ꎞ�I�ɃA�C�e���f�[�^��ۊǂ���
		itemBuf = selectedItem;
		//���̃A�C�e����type���擾����
		int type = selectedItem->getItemData(1);
		if (type < 2) {
			use_usable->Open();
			usetype = USABLE;
			ChangeSequence(sequence::INVENTORY_USE);
			return true;
		}
		else {
			equipItem* eItem = (equipItem*)itemBuf;
			if (eItem->GetIsEquiped()) {
				use_nowEquip->Open();
				usetype = NOWEQUIP;
				ChangeSequence(sequence::INVENTORY_USE);
				return true;
			}
			use_equip->Open();
			usetype = EQUIP;
			ChangeSequence(sequence::INVENTORY_USE);
			return true;
		}
	}

	return true;
}

bool DungeonScene::SeqInventoryUse(const float deltatime)
{
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_ESCAPE))
	{
		gManager->sound->System_Play(gManager->sound->system_cancel);
		use_usable->menu_live = false;
		use_equip->menu_live = false;
		use_nowEquip->menu_live = false;
		itemBuf = nullptr;
		ChangeSequence(sequence::INVENTORY_OPEN);
		return true;
	}

	int type = itemBuf->getItemData(1);
	//�����A�C�e���ƕ�����ׂ�
	//����A�C�e���Ȃ�
	if (type < 2) {
		//�g����Enter����������
		if (use_usable->SelectNum == 0 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
			gManager->sound->System_Play(gManager->sound->system_select);
			ItemUse(drawInventoryPage);
			use_usable->menu_live = false;
			itemBuf = nullptr;
			ChangeSequence(sequence::INVENTORY_OPEN);
			return true;
		}
		//�������Enter����������
		else if (use_usable->SelectNum == 1 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
			gManager->sound->System_Play(gManager->sound->system_select);
			firstMenu->menu_live = false;
			ItemThrow(drawInventoryPage);
			ChangeSequence(sequence::THROWITEMMOVE);
			return true;
		}
		//��߂��Enter����������
		else if (use_usable->SelectNum == 2 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
			gManager->sound->System_Play(gManager->sound->system_cancel);
			use_usable->menu_live = false;
			itemBuf = nullptr;
			ChangeSequence(sequence::INVENTORY_OPEN);
			return true;
		}
	}
	//�����A�C�e���Ȃ�
	else {
		equipItem* eItem = (equipItem*)itemBuf;
		//�������Ă��鎞
		if (eItem->GetIsEquiped()) {
			//�g����Enter����������
			if (use_nowEquip->SelectNum == 0 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
				gManager->sound->System_Play(gManager->sound->system_select);
				ItemUse(drawInventoryPage);
				use_equip->menu_live = false;
				itemBuf = nullptr;
				ChangeSequence(sequence::INVENTORY_OPEN);
				return true;
			}
			//�������Ă���@���@�������Enter����������
			else if (use_nowEquip->SelectNum == 1 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
				gManager->sound->System_Play(gManager->sound->system_cancel);
				gManager->addLog("�������̃A�C�e���͓������܂���");
				ChangeSequence(sequence::INVENTORY_OPEN);
				return true;
			}
			//��߂��Enter����������
			else if (use_nowEquip->SelectNum == 2 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
				gManager->sound->System_Play(gManager->sound->system_cancel);
				use_equip->menu_live = false;
				itemBuf = nullptr;
				ChangeSequence(sequence::INVENTORY_OPEN);
				return true;
			}
		}
		else {
			//�g����Enter����������
			if (use_equip->SelectNum == 0 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
				gManager->sound->System_Play(gManager->sound->system_select);
				ItemUse(drawInventoryPage);
				use_equip->menu_live = false;
				itemBuf = nullptr;
				ChangeSequence(sequence::INVENTORY_OPEN);
				return true;
			}//�������Enter����������
			//�������Ă��Ȃ��@���@�������Enter����������
			else if (use_equip->SelectNum == 1 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
				gManager->sound->System_Play(gManager->sound->system_select);
				firstMenu->menu_live = false;
				ItemThrow(drawInventoryPage);
				ChangeSequence(sequence::THROWITEMMOVE);
				return true;
			}
			//��߂��Enter����������
			else if (use_equip->SelectNum == 2 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
				gManager->sound->System_Play(gManager->sound->system_cancel);
				use_equip->menu_live = false;
				itemBuf = nullptr;
				ChangeSequence(sequence::INVENTORY_OPEN);
				return true;
			}
		}
	}
	return true;
}

bool DungeonScene::SeqThrowItemMove(const float deltatime)
{
	if (throwedItemList.empty())return true;
	auto itr = throwedItemList.begin();

	if ((*itr)->ThrowItem(player->mydir)) {
		throwedItemList.erase(itr);
		ChangeSequence(sequence::MAIN);
		return true;
	}
	return true;
}

bool DungeonScene::SeqAnimation(const float deltatime)
{
	//�A�j���[�V����������֐������s����
	//�A�j���[�V�������I��莟��N����Ă΂ꂽ�����m�F���ăV�[�N�G���X���ړ�����
	//�v���C���[�̍U���Ȃ�
	if (lastSeq == sequence::MAIN) {
		//�ŏ��̃t���[���̂ݎ��s
		if (mainSequence.isStart()) {
			gManager->sound->System_Play(gManager->sound->system_attack);
			//�A�j���[�V�����|�W�V�����̌���
			player->SetAnimPos();
			//�A�j���[�V�����摜�̍ő�Index�ԍ��̎擾
			int index = gManager->GetMaxIndex(GameManager::index::ATTACK);
			//Animation�N���X��new
			std::shared_ptr<Animation>anim = std::make_shared<Animation>("graphics/AttackAnim_30.png", player->effectPos, ATTACKEFFECTSPEED, index);
			//�`�惊�X�g�ɓo�^
			drawAnimationList.emplace_back(anim);
		}
		//�����A�j���[�V�������I����Ă���Ȃ�
		if (drawAnimationList.empty()) {
			//�U���V�[�N�G���X�Ɉړ�
			ChangeSequence(sequence::PLAYERATTACK);
			return true;
		}
	}
	//�G�l�~�[�̍U���Ȃ�
	else if (lastSeq == sequence::ENEMYACT) {
		if (mainSequence.isStart()) {
			gManager->sound->System_Play(gManager->sound->system_attack);
			//�A�j���[�V�����摜�̍ő�Index�ԍ��̎擾
			int index = gManager->GetMaxIndex(GameManager::index::ATTACK);
			for (auto attackEnemy : atackEnemies) {
				//Animation�N���X��new
				std::shared_ptr<Animation>anim = std::make_shared<Animation>("graphics/AttackAnim_30.png", player->pos, ATTACKEFFECTSPEED, index);
				//�`�惊�X�g�ɓo�^
				drawAnimationList.emplace_back(anim);
			}
		}


		if (drawAnimationList.empty()) {
			//�����A�j���[�V�������I����Ă���Ȃ�
			ChangeSequence(sequence::ENEMYATTACK);
			return true;
		}
	}

	return true;
}


//�K�w�ړ����Ȃǂɕ�����`�悷��V�[�N�G���X
bool DungeonScene::SeqDescFade(const float deltatime)
{
	//�����`�掞�Ԃ��I���܂Ńt�F�[�h�A�E�g���n�߂Ȃ�

		//***�t�F�[�h�C������
	if (gManager->fControl->doneFade && descFadeCount < DESCFADETIME) {
		gManager->fControl->FadeIn();
		return true;
	}
	//***
	//�����`�掞�Ԃ̍X�V
	descFadeCount += deltatime;

	if (descFadeCount < DESCFADETIME)return true;

	if (!gManager->fControl->doneFade) {
		gManager->fControl->FadeOut();
		return true;
	}
	else {
		ChangeSequence(sequence::FADEIN);
		descFadeCount = 0;
		return true;
	}

	return true;
}

bool DungeonScene::SeqCameraMove(const float deltatime)
{
	gManager->camera->CameraMove();

	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_ESCAPE)) {
		ChangeSequence(sequence::MAIN);
		gManager->CameraReset();
		return true;
	}

	return true;
}
void DungeonScene::DrawFadeDesc() {


	if (mainSequence.isStart()) {
		nowDungeonName = gManager->GetDungeonName(gManager->nowDungeon);
	}
	if (lastSeq == sequence::FADEOUT) {
		DrawStringEx(gManager->WINDOWCENTER.x - OFFSET, gManager->WINDOWCENTER.y, -1, "%s", nowDungeonName.c_str());
		DrawStringEx(gManager->WINDOWCENTER.x - OFFSET, gManager->WINDOWCENTER.y + 50, -1, "%d�K", dungeonLevel);
	}
	else {
		DrawStringEx(gManager->WINDOWCENTER.x - OFFSET, gManager->WINDOWCENTER.y, -1, "%s", nowDungeonName.c_str());

	}

}
bool DungeonScene::SeqFadeIn(const float deltatime)
{
	if (gManager->fControl->doneFade) {
		gManager->fControl->FadeIn();
		return true;
	}
	else {
		ChangeSequence(sequence::MAIN);
		return true;
	}
	return true;
}

bool DungeonScene::SeqFadeOut(const float deltatime)
{
	if (!gManager->fControl->doneFade) {
		gManager->fControl->FadeOut();
		return true;
	}
	else {
		//����map�𐶐�
		MoveLevel(1);
		playerPos = gManager->WorldToLocal(gManager->player->pos);
		//ChangeSequence(sequence::FADEIN);
		ChangeSequence(sequence::FADEDESC);
		return true;
	}
	return true;
}

void DungeonScene::ChangeSequence(sequence seq)
{
	lastSeq = nowSeq;
	nowSeq = seq;
	if (seq == sequence::MAIN) {
		mainSequence.change(&DungeonScene::SeqMain);
	}
	else if (seq == sequence::PLAYERATTACK) {
		mainSequence.change(&DungeonScene::SeqPlayerAttack);
	}
	else if (seq == sequence::ENEMYACT) {
		mainSequence.change(&DungeonScene::SeqEnemyAct);
	}
	else if (seq == sequence::ENEMYATTACK) {
		mainSequence.change(&DungeonScene::SeqEnemyAttack);
	}
	else if (seq == sequence::FIRSTMENU) {
		mainSequence.change(&DungeonScene::SeqFirstMenu);
	}
	else if (seq == sequence::INVENTORY_OPEN) {
		mainSequence.change(&DungeonScene::SeqInventoryOpen);
	}
	else if (seq == sequence::INVENTORY_USE) {
		mainSequence.change(&DungeonScene::SeqInventoryUse);
	}
	else if (seq == sequence::THROWITEMMOVE) {
		mainSequence.change(&DungeonScene::SeqThrowItemMove);
	}
	else if (seq == sequence::ANIMATION) {
		mainSequence.change(&DungeonScene::SeqAnimation);
	}
	else if (seq == sequence::FADEIN) {
		mainSequence.change(&DungeonScene::SeqFadeIn);
	}
	else if (seq == sequence::FADEOUT) {
		mainSequence.change(&DungeonScene::SeqFadeOut);
	}
	else if (seq == sequence::FADEDESC) {
		mainSequence.change(&DungeonScene::SeqDescFade);
	}
	else if (seq == sequence::CAMERA) {
		mainSequence.change(&DungeonScene::SeqCameraMove);
	}

}

void DungeonScene::DrawNowSequence(sequence seq)
{
	if (seq == sequence::MAIN) {
		DrawStringEx(800, 300, -1, "MAINSequence");
	}
	else if (seq == sequence::ENEMYACT) {
		DrawStringEx(800, 300, -1, "PLAYERATTACKSequence");
	}
	else if (seq == sequence::ENEMYACT) {
		DrawStringEx(800, 300, -1, "ENEMYACTSequence");
	}
	else if (seq == sequence::ENEMYACT) {
		DrawStringEx(800, 300, -1, "ENEMYATTACKSequence");
	}
	else if (seq == sequence::FIRSTMENU) {
		DrawStringEx(800, 300, -1, "FIRSTMENUSequence");
	}
	else if (seq == sequence::INVENTORY_OPEN) {
		DrawStringEx(800, 300, -1, "INVENTORYOPENSequence");
	}
	else if (seq == sequence::INVENTORY_USE) {
		DrawStringEx(800, 300, -1, "INVENTORYUSESequence");
	}
	else if (seq == sequence::THROWITEMMOVE) {
		DrawStringEx(800, 300, -1, "ITEMMOVESequence");
	}
	else if (seq == sequence::ANIMATION) {
		DrawStringEx(800, 300, -1, "ANIMATIONSequence");
	}
	else if (seq == sequence::CAMERA) {
		DrawStringEx(800, 300, -1, "CAMERASequence");
	}
}

void DungeonScene::DrawEnemyData()
{
	int i = 0;
	for (auto enemy : eManager->liveEnemyList) {

		DrawStringEx(450 + 120 * i, 480, -1, "%s", enemy->GetName().c_str());
		DrawStringEx(450 + 120 * i, 500, -1, "%d", enemy->GetStatus(0));
		DrawStringEx(450 + 120 * i, 520, -1, "%d", enemy->GetStatus(1));
		DrawStringEx(450 + 120 * i, 540, -1, "%d", enemy->GetStatus(2));
		DrawStringEx(450 + 120 * i, 560, -1, "%d", enemy->GetStatus(3));
		DrawStringEx(450 + 120 * i, 580, -1, "%d", enemy->GetExp());
		++i;
	}

}

void DungeonScene::DrawInventory()
{
	DrawStringEx(inventory->menu_x + 300, inventory->menu_y + 10, -1, "�y�[�W:%d", gManager->inventories[drawInventoryPage]->GetInventoryNum());
	if (gManager->inventories[drawInventoryPage]->inventoryList.empty())return;
	desc->Menu_Draw();
	SetFontSize(25);
	gManager->inventories[drawInventoryPage]->DrawInventory(inventory->menu_x, inventory->menu_y);
	gManager->inventories[drawInventoryPage]->DrawItemData(desc->menu_x + 10, desc->menu_y + 10);

	SetFontSize(16);
}

void DungeonScene::ChangeInventory()
{
	//�C���x���g����؂�ւ���
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RIGHT)) {
		drawInventoryPage = (drawInventoryPage + 1) % (gManager->inventoryNum + 1);
		gManager->inventories[drawInventoryPage]->CursorReset();
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_LEFT)) {
		drawInventoryPage = (drawInventoryPage + (gManager->inventoryNum)) % (gManager->inventoryNum + 1);
		gManager->inventories[drawInventoryPage]->CursorReset();
	}
	if (gManager->inventories[drawInventoryPage]->inventoryList.empty())return;
	//�㉺�ړ�
	gManager->inventories[drawInventoryPage]->CursorMove();
}

void DungeonScene::SpawnItem(int ItemId)
{
	Item* popItem = gManager->GetItemData(ItemId);
	if (popItem == nullptr)return;
	//�|�b�v��������W
	t2k::Vector3 popPos;
	//�ݒu�ς݂̃A�C�e�����Ȃ�
	if (dropItems.empty()) {
		popPos = gManager->SetStartPos(GameManager::setStartPosType::ITEM);
	}
	//���łɐݒu�ς݂̃A�C�e��������
	else {
		bool set = false;
		int count = 0;//debug
		//���łɃ��X�g���ɂ���A�C�e���̃|�W�V�����Ƃ��Ԃ��Ă����������x���W���擾����
		while (!set) {
			popPos = gManager->SetStartPos(GameManager::setStartPosType::ITEM);
			//bool set = false;
			//�ݒu�ς݂̑S�ẴA�C�e���Ɣ�ׂ�
			for (auto item : dropItems) {
				t2k::Vector3 alreadySetPos = item->GetPos();
				if (popPos.x == alreadySetPos.x && popPos.y == alreadySetPos.y) {
					t2k::debugTrace("\n�|�W�V�������Ԃ�%d���\n", count);
					count++;
					continue;
				}
				else {
					set = true;
					break;
				}
			}
			//if (set)break;

		}
	}
	popItem->SetPos(popPos);
	dropItems.emplace_back(popItem);
}

void DungeonScene::DrawPopItem()
{
	if (dropItems.empty())return;
	for (auto popItem : dropItems) {
		popItem->DrawPopItem();
	}
}

void DungeonScene::ItemUse(/*int selectNum, Inventory* inventory,*/ int inventoryPage)
{
	int type = itemBuf->getItemData(1);

	//�A�C�e���̏������s��
	//�����g�����A�C�e�����񕜏���A�C�e����������
	if (type == 0) {
		int manpuku = itemBuf->getItemData(2);
		int heal = itemBuf->getItemData(3);
		player->ChangeBaseStatus(manpuku, heal);
		gManager->addLog(itemBuf->getItemName() + "���g����");
		//�C���x���g������̏���
		gManager->PopItemFromInventory(inventoryPage);
		//�y�[�W���̃A�C�e����S�ď���ăy�[�W�������������Ƃ�������
		if (gManager->isDeleteInventory) {
			//gManager->ForceInventoryChange(inventoryPage);
			if (drawInventoryPage == 0) {
				gManager->isDeleteInventory = false;
				return;
			}
			drawInventoryPage--;
			gManager->isDeleteInventory = false;
		}
	}//��������A�C�e����������
	else if (type == 1) {
		//������A�C�e����pop�A�C�e���Ƃ��ĕ`�悷��
		//������֐����Ă�
		ItemThrow(inventoryPage);


		//�����A�C�e���͎g���ł�������ł��A�C�e�����ˏo����

	}//�����A�C�e����������s
	else if (type == 2 || type == 3) {
		equipItem* item = (equipItem*)itemBuf;
		if (item->GetIsEquiped()) {
			player->RemoveEquipItem(item);
			return;
		}
		player->ChangeEquipItem(item);
	}
}

void DungeonScene::ItemThrow(int inventoryPage)
{
	//itemBuf->SetPos(player->pos);

	//Item* throwedItem = itemBuf;
	//�����ŐV����itemBuf�Ɠ����f�[�^�ŃA�C�e���𐶐�����
	std::vector<int> intData = itemBuf->GetAllIntData();
	std::vector<string> stringData = itemBuf->GetAllStringData();

	//Item* throwedItem = new Item(intData[0], intData[1], stringData[0], intData[2], intData[3], intData[4], stringData[1], stringData[2]);
	std::shared_ptr<Item>throwedItem = std::make_shared<Item>(intData[0], intData[1], stringData[0], intData[2], intData[3], intData[4], stringData[1], stringData[2]);

	throwedItem->SetPos(player->pos);
	throwedItem->SetGoalPos(player->mydir);
	//throwItem.emplace_back(throwedItem);

	throwedItemList.emplace_back(throwedItem);
	//�C���x���g������̏���
	gManager->PopItemFromInventory(inventoryPage);
	//�y�[�W���̃A�C�e����S�ď���ăy�[�W�������������Ƃ�������
	if (gManager->isDeleteInventory) {
		gManager->ForceInventoryChange(inventoryPage);
	}
}
void DungeonScene::DeleteDeadEnemy()
{
	auto itr = eManager->liveEnemyList.begin();

	for (int i = 0; i < eManager->liveEnemyList.size(); ++i) {
		if ((*itr)->isLive == false) {
			itr = eManager->liveEnemyList.erase(itr);
			gManager->SetLiveEnemyList(eManager->liveEnemyList);
		}
		else {
			itr++;
		}
	}
}




