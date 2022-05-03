#include "DungeonScene.h"
#include"Map.h"
#include"GameManager.h"
#include"SoundManager.h"
#include"../support/Support.h"
#include "Player.h"
#include"MenuWindow.h"
#include"Actor/EnemyManager.h"
#include"Actor/Enemy.h"
#include"Skill.h"
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
	delete gameOver;
	delete menuOpen;
	delete topUI;

	delete shop;
	delete shopDesc;
	//delete inventory;	�V�[����ς��Ă������A�C�e���̃f�[�^�͈����@�����p�������������\��:�D��x��
	delete log;
	delete desc;
	delete status;	delete playerStatus;
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
	//gManager->SetLiveEnemyList(eManager->liveEnemyList);
	//y���W�Ń\�[�g
	gManager->SortEntityList();
}

void DungeonScene::Update()
{
	//debug �L�����v�V�[���ֈړ�
	if (dungeonClear) {
		SceneManager::ChangeScene(SceneManager::SCENE::CAMP);
		return;
	}

	//�}�E�X�ʒu�擾
	GetMousePoint(&mouseX, &mouseY);
	//���݂̃V�[�N�G���X��Update���������s
	mainSequence.update(gManager->deitatime_);

	//�C�x���g�̎��s

	//�G�l���M�[�����܂��Ă�����L�����v�ɖ߂�
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_C)) {
		//�f�o�b�O
		if (gManager->isDebug) {
			ReturnCamp();
			return;
		}

		if (gManager->IsOverMax()) {
			ReturnCamp();
			return;
		}
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

	//�v���C���[�ƃA�C�e���̏Փ˔���
	if (DetectItem()) {
		for (auto item : dropItems) {
			if (item->GetIsLive())continue;
			if (gManager->PopDetectItem(item, dropItems))break;
		}
	}

	//�A�j���[�V�����X�V
	UpdateAnimation();
	//�A�j���[�V�����̐�������
	CheckAnimLive();

	//player��������
	if (gManager->player->GetStatus(0) <= 0) {
		WhenDeadPlayer();
	}
}

void DungeonScene::Draw()
{
	//�K�w�Ԃ̃t�F�[�h���łȂ���Ε`�悷��
	if (nowSeq != sequence::FADEDESC) {
		//�}�b�v�̕`��
		gManager->MapDraw();
		//�h���b�v���Ă���A�C�e���̕`��
		DrawPopItem();
		//player��enemy�̕`��
		for (auto actor : gManager->liveEntityList) {
			actor->Draw();
		}

		//HP�o�[�̕`��
		player->HpVarDraw();

		//�~�j�}�b�v�̕`��
		gManager->MiniMapDraw();
		//�~�j�}�b�v�̓G�̕`��
		DrawMiniEnemy();

		//debugMode
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
		//�v���C���[���V���b�v�̏�ɂ���Ȃ�E�B���h�E��\������
		else if (gManager->GetMapChip(playerPos) == 4 && nowSeq != sequence::SHOP) {
			shopIn->Menu_Draw();
			DrawStringEx(shopIn->menu_x + 70, shopIn->menu_y + 60, -1, "�V���b�v��������");

			DrawStringEx(shopIn->menu_x + 70, shopIn->menu_y + 100, -1, "���蕨�����܂����H");

			SetFontSize(30);
			DrawStringEx(shopIn->menu_x + 20, shopIn->menu_y + 140, -1, "Enter�ŃV���b�v������");
			SetFontSize(16);

		}
		//�A�j���[�V�������X�g�̕`��
		DrawAnimation();

		if (gManager->isDebug) {
			//���̃V�[�N�G���X����\������
			DrawNowSequence(nowSeq);
		}

		//****��ʏ㕔��UI�w�i�`��
		topUI->Menu_Draw();

		DrawTopUI();

		//****

	}


	//�������牺�̓V�[�N�G���X���Ƃ̕`��

	firstMenu->All();
	//�t�F�[�h���ȊO�ŕ`�� ��ʍ����"���j���[���J��"UI��\��
	if (nowSeq == sequence::MAIN || nowSeq == sequence::ENEMYACT || nowSeq == sequence::PLAYERATTACK || nowSeq == sequence::ENEMYATTACK || nowSeq == sequence::ANIMATION) {
		menuOpen->Menu_Draw();
		DrawStringEx(menuOpen->menu_x + 10, menuOpen->menu_y + 10, -1, "Menu���J��");
		DrawStringEx(menuOpen->menu_x + 10, menuOpen->menu_y + menuOpen->menu_height * 5 / 9, -1, "Press");
		DrawRotaGraph(menuOpen->menu_x + menuOpen->menu_width - 20, menuOpen->menu_y + menuOpen->menu_height * 2 / 3, 1, 0, EButton, false);

		//�����摜�`��
		gManager->DrawHowTo();
	}
	//�C���x���g�����J���Ă��鎞�ɕ`��
	else if (nowSeq == sequence::INVENTORY_OPEN || nowSeq == sequence::INVENTORY_USE) {
		inventory->Menu_Draw();
		DrawInventory(inventory->menu_x, inventory->menu_y);
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
	else if (nowSeq == sequence::SHOP) {
		//�V���b�v�C���x���g���̕`��
		shop->Menu_Draw();

		//�����T�C�Y�ύX
		SetFontSize(25);
		//�V���b�v�C���x���g�����e�̕`��
		shopPages[currentDrawPage]->DrawInventory(shop->menu_x + 10, shop->menu_y + 10);
		//�����T�C�Y�ύX
		SetFontSize(16);

		//�K�v�R�C���̕`��
		shopCoin->Menu_Draw();
		shopPages[currentDrawPage]->DrawNeedCoin(shopCoin->menu_x, shopCoin->menu_y);
		//�A�C�e���X�e�[�^�X�̔�r�\��
		shopDesc->Menu_Draw();
		SetFontSize(23);
		DrawStringEx(shopDesc->menu_x + 10, shopDesc->menu_y + 10, -1, "���ݑ������̑����Ɣ�r");
		shopPages[currentDrawPage]->DrawEquipItemStatus(shopDesc->menu_x + 10, shopDesc->menu_y + 50);
		SetFontSize(16);
		//�����̃C���x���g���\��
		shopMyInv->Menu_Draw();
		DrawInventory(shopMyInv->menu_x, shopMyInv->menu_y);
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
	if (!gManager->liveEnemyList.empty()) {
		gManager->liveEnemyList.clear();
	}
	//player�ȊO��entity��actor���X�g�����������
	if (!gManager->liveEntityList.empty()) {

		std::list<std::shared_ptr<Actor>>::iterator entity;

		for (entity = gManager->liveEntityList.begin(); entity != gManager->liveEntityList.end(); ) {
			if ((*entity)->GetActId() != 0) {
				entity = gManager->liveEntityList.erase(entity);
				continue;
			}
			entity++;
		}
	}

	//�A�C�e���̏���
	dropItems.clear();

	//�V���b�v�A�C�e�������ׂ�delete����
	for (int i = 0; i < shopPages.size(); ++i) {
		shopPages[i]->inventoryList.clear();
	}

	//�_���W�����̊K�w���ړ�����
	dungeonLevel += addLevel;
	//map�̍Đ���
	gManager->ReCreate();
	//�G�̍Đ���
	RandEnemyCreate(5);
	//�A�C�e���̍Đ���
	for (int i = 0; i < spawnItemNum; ++i) {
		//int random = rand() % gManager->GetItemNum();
		int random = gManager->GetItemWithWeight(player->GetPlayerLevel());
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
	gManager->ScaleChange();
	gManager->CalcScale();

	EButton = gManager->LoadGraphEx("graphics/button_E.png");
	button_1 = gManager->LoadGraphEx("graphics/button_1.png");
	button_2 = gManager->LoadGraphEx("graphics/button_2.png");
	button_3 = gManager->LoadGraphEx("graphics/button_3.png");

	numButtons[0] = button_1;
	numButtons[1] = button_2;
	numButtons[2] = button_3;

	miniEnemy = gManager->LoadGraphEx("graphics/mini_Enemy.png");


	//std::bind(static_cast<void(&)()>(DungeonScene::InventoryOpen));

	////���j���[�֐��i�[
	//std::bind(&hoge::aaa, this)

	//menues.emplace_back(std::bind(&DungeonScene::InventoryOpen, this));
	//menues.emplace_back(std::bind(&DungeonScene::CheckFootPoint, this));
	//menues.emplace_back(std::bind(&DungeonScene::Save, this));
	//menues.emplace_back(std::bind(&DungeonScene::BackTitle, this));
	//menues.emplace_back(std::bind(&DungeonScene::MenuClose, this));



	//*************UI�֘A�̃C���X�^���X�m��***************
	nextLevelWindow = new Menu(300, 300, 300, 200, "graphics/WindowBase_01.png");
	gameOver = new Menu(100, 100, 924, 668, "graphics/WindowBase_01.png");

	shopIn = new Menu(300, 300, 380, 200, "graphics/WindowBase_01.png");
	shopMyInv = new Menu(60, 90, 320, 340, "graphics/WindowBase_01.png");
	shop = new Menu(380, 90, 320, 340, "graphics/WindowBase_01.png");
	shopDesc = new Menu(720, 90, 300, 180, "graphics/WindowBase_01.png");
	shopCoin = new Menu(720, 280, 300, 200, "graphics/WindowBase_01.png");

	menuOpen = new Menu(20, 160, 100, 100, "graphics/WindowBase_01.png");
	topUI = new Menu(10, 10, 800, 150, "graphics/WindowBase_01.png");

	inventory = new Menu(255, 50, 420, 340, "graphics/WindowBase_01.png");
	desc = new Menu(680, 300, 320, 90, "graphics/WindowBase_01.png");

	log = new Menu(12, 560, 1000, 200, "graphics/WindowBase_01.png");
	status = new Menu(680, 100, 320, 190, "graphics/WindowBase_01.png");
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
	use_equip = new MenuWindow(640, 440, 120, 100, "graphics/WindowBase_02.png", menu_equip, 3, 0.2);

	MenuWindow::MenuElement_t* menu_nowEquip = new MenuWindow::MenuElement_t[]{
		{670,450,"�͂���",0},
		{670,480,"������",1},
		{670,510,"��߂�",2}
	};
	use_nowEquip = new MenuWindow(640, 440, 90, 100, "graphics/WindowBase_02.png", menu_nowEquip, 3, 0.15);




	MenuWindow::MenuElement_t* menu_0 = new MenuWindow::MenuElement_t[]{
		{70,80,"������",0},
		{70,110,"����",1},
		{70,140,"�Z�[�u",2},
		{70,170,"�^�C�g���֖߂�",3},
		{70,200,"Esc|���j���[�����",4}
	};
	// ���j���[�E�B���h�E�̃C���X�^���X��
	firstMenu = new MenuWindow(30, 50, 220, 210, "graphics/WindowBase_02.png", menu_0, 5, 0.45);

	//**********************************************************************//

	eManager = std::make_shared<EnemyManager>();
	//player�̃C���X�^���X���Ȃ���ΐ���(debug)
	gManager->MakePlayer(GameManager::SpawnScene::Dungeon);

	//player�̃|�C���^���擾
	player = gManager->GetPlayer();

	//�G�������_���Ő������z�u
	RandEnemyCreate(5);

	//�A�C�e���������_���ɐ������z�u
	for (int i = 0; i < 5; ++i) {
		//int random = rand() % gManager->GetItemNum();
		int random = gManager->GetItemWithWeight(player->GetPlayerLevel());
		SpawnItem(random);
	}
	gManager->RunDungeonBgm();
	dungeonClear = false;
	//�V���b�v�p�C���x���g���𐶐�����
	if (shopPages.empty()) {
		//�V����inventory�̃C���X�^���X�𐶐�����
		Inventory* newPage = new Inventory(shopPage + 1);
		//�V���b�v�̃y�[�W��o�^
		shopPages.emplace_back(newPage);
	}
}

void DungeonScene::DrawAnimation()
{
	if (drawAnimationList.empty())return;
	auto itr = drawAnimationList.front();
	itr->Draw();

	/*for (auto anim : drawAnimationList) {
		anim->Draw();
	}*/
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


void DungeonScene::SelectItemUseMenu(int SelectNum)
{
	//�g����Enter����������
	if (SelectNum == 0) {
		gManager->sound->System_Play(gManager->sound->system_select);
		ItemUse(drawInventoryPage);
		use_usable->menu_live = false;
		itemBuf = nullptr;
		ChangeSequence(sequence::INVENTORY_OPEN);
	}
	//�������Enter����������
	else if (SelectNum == 1) {
		gManager->sound->System_Play(gManager->sound->system_select);
		firstMenu->menu_live = false;
		ItemThrow(drawInventoryPage);
		ChangeSequence(sequence::THROWITEMMOVE);
	}
	//��߂��Enter����������
	else if (SelectNum == 2) {
		gManager->sound->System_Play(gManager->sound->system_cancel);
		use_usable->menu_live = false;
		itemBuf = nullptr;
		ChangeSequence(sequence::INVENTORY_OPEN);
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
		for (auto enemy : gManager->liveEnemyList) {
			enemy->ChangeStatus(2, 10, 0);
		}
	}
	////�V���b�v�V�[�N�G���X�Ɉړ�
	//if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_S)) {
	//	ChangeSequence(sequence::SHOP);
	//	return true;
	//}

	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_L)) {
		player->LevelUp();
		//�������x���A�b�v������

			//Animation�N���X��new
		std::shared_ptr<Animation>anim = std::make_shared<Animation>("graphics/levelUpEffect_new.png", player->pos);
		//�`�惊�X�g�ɓo�^
		drawAnimationList.emplace_back(anim);

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
	//�K�i,�V���b�v�̏�Ƀv���C���[�����邩�ǂ����m�F
	if (CheckExtraOnTile())return true;

	//���N���b�N��R�{�^���ōU��
	//�v���C���[�̍U���͖ڂ̑O�ɑΏۂ����Ȃ������ꍇskip�Ɠ����������s��
	if (t2k::Input::isMouseTrigger(t2k::Input::MOUSE_RELEASED_LEFT) || t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_R)) {
		//�������A�j���[�V�����V�[�N�G���X�ɔ�΂�

		ChangeSequence(sequence::ANIMATION);
		return true;

	}
	else
	{
		//�X�L���̔����`�F�b�N
		//�X�L�������������炱�̃V�[�N�G���X���X�L�b�v����
		if (ActivateSkillCheck())return true;
	}


	//����Player���������� �������̓X�L�b�v������
	if (gManager->player->Move() || player->skip) {


		itemGetFlag = true;
		playerPos = gManager->WorldToLocal(gManager->player->pos);
		//�����̒��ɓ������Ȃ�~�j�}�b�v�̕�����\��������
		gManager->UpdateMiniMap(playerPos);

		//�A�Ґ΂̃G�l���M�[�[�U
		gManager->AddEnergyToStone(CHARGEENERGY[static_cast<uint32_t>(CHARGE::WALK)]);

		ChangeSequence(sequence::ENEMYACT);
		if (player->skip)player->skip = false;
		return true;
	}

	return true;
}

bool DungeonScene::SeqPlayerAttack(const float deltatime)
{
	if (lastUseSkill != nullptr) {
		player->SkillAttack(lastUseSkill);
	}
	else {
		player->Attack();
	}
	//�G���S�`�F�b�N
	for (auto enemy : gManager->liveEnemyList) {
		if (enemy->GetStatus(0) <= 0) {
			//�������x���A�b�v������
			if (player->AddExp(enemy->GetExp())) {
				//Animation�N���X��new
				std::shared_ptr<Animation>anim = std::make_shared<Animation>("graphics/levelUpEffect_new.png", player->pos);
				//�`�惊�X�g�ɓo�^
				drawAnimationList.emplace_back(anim);
			}
			gManager->addLog(enemy->GetName() + "��|����!!");
			//�R�C���̃h���b�v
			player->ChangeHaveCoin(DROPCOIN);

			//�A�Ґ΂̃G�l���M�[�[�U
			gManager->AddEnergyToStone(CHARGEENERGY[static_cast<uint32_t>(CHARGE::ENEMY)]);

			//�A�C�e���̃|�b�v����
			int odds = rand() % 100;
			if (odds < DROPODDS) {
				/*
				//������A�C�e���̔���
				int random = rand() % gManager->GetItemNum();
				//SpawnItem(random);
				*/

				int random = gManager->GetItemWithWeight(player->GetPlayerLevel());
				t2k::Vector3 dropPos = enemy->pos;
				DropItem(random, dropPos);
			}
			enemy->isLive = false;
		}
	}

	//�����X�L���g�p�ゾ������
	if (lastUseSkill != nullptr)lastUseSkill = nullptr;

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
		for (auto liveEnemy : gManager->liveEnemyList) {
			//player��enemy���ׂ荇���Ă���Ȃ烊�X�g�ɂ���ď��ԂɎ��s����
			if (gManager->CheckNearByPlayer(liveEnemy))
			{
				//�����v���C���[�̕����������Ă��Ȃ��ꍇ�͌�������
				//�v���C���[�̈ʒu��enemy�̈ʒu�Ƃ̑��Ε������擾����
				int vec = gManager->GetPlayerVec(liveEnemy);

				if (liveEnemy->mydir != vec)liveEnemy->setDir(vec);
				//���X�g�ɓ����
				attackEnemies.emplace_back(liveEnemy);
			}
			else {
				//�����֐��͑S�ē����ł���
				liveEnemy->Move();
			}
		}
		itr = attackEnemies.begin();

		//y���W��actor���\�[�g
		gManager->SortEntityList();
	}
	/*
	if (mainSequence.getProgressTime() > 1.0f) {
		for (auto liveEnemy : gManager->liveEnemyList) {
			//player��enemy���ׂ荇���Ă���Ȃ烊�X�g�ɂ���ď��ԂɎ��s����
			if (gManager->CheckNearByPlayer(liveEnemy))
			{
				//�����v���C���[�̕����������Ă��Ȃ��ꍇ�͌�������
				//�v���C���[�̈ʒu��enemy�̈ʒu�Ƃ̑��Ε������擾����
				int vec = gManager->GetPlayerVec(liveEnemy);

				if (liveEnemy->mydir != vec)liveEnemy->setDir(vec);
				//���X�g�ɓ����
				attackEnemies.emplace_back(liveEnemy);
			}
			else {
				//�����֐��͑S�ē����ł���
				liveEnemy->Move();
			}
		}
		itr = attackEnemies.begin();

		//y���W��actor���\�[�g
		gManager->SortEntityList();
		*/

		//�U������G���X�g�����炶��Ȃ��Ȃ�
	if (!attackEnemies.empty()) {

		//�A�j���[�V�����V�[�N�G���X�ɔ�΂�
		ChangeSequence(sequence::ANIMATION);
		return true;
	}
	ChangeSequence(sequence::MAIN);
	return true;
}

bool DungeonScene::SeqEnemyAttack(const float deltatime)
{
	//�����G����̂����Ȃ�C���^�[�o����0�ɂ���
	if (attackEnemies.size() == 1)enemyActTimer = ENEMYACTINTERVAL;
	//��̂��U�������邽�߂̃C���^�[�o���v��
	if (++enemyActTimer > ENEMYACTINTERVAL) {
		(*itr)->Attack();
		enemyActTimer = 0;
		itr = attackEnemies.erase(itr);
	}
	//���ׂĂ̓G���U�����I����܂ł��̃V�[�N�G���X���o�Ȃ�
	if (!attackEnemies.empty())return true;


	//���ׂĂ̍U�����I������MainSequence�ɖ߂�
	ChangeSequence(sequence::MAIN);
	return true;
}


bool DungeonScene::SeqFirstMenu(const float deltatime)
{
	////std::function���g���ă��j���[���Ǘ�����������
	//if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
	//	//menues[firstMenu->SelectNum] ();
	//	return true;
	//}

	//�C���x���g�����J��
	if (firstMenu->SelectNum == 0 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
		gManager->sound->System_Play(gManager->sound->system_select);
		//menu�̏㉺�𑀍�o���Ȃ�����
		firstMenu->manageSelectFlag = false;

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
		if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
			SelectItemUseMenu(use_usable->SelectNum);
			return true;
		}
	}
	//�����A�C�e���Ȃ�
	else {
		equipItem* eItem = static_cast<equipItem*> (itemBuf);
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
				//���ʉ��Đ�
				gManager->sound->System_Play(gManager->sound->system_select);
				//�A�C�e���g�p����
				ItemUse(drawInventoryPage);
				//���j���[�����
				use_equip->menu_live = false;
				//�A�C�e���̈ꎞ����j��
				itemBuf = nullptr;
				//�V�[�N�G���X���ړ�
				ChangeSequence(sequence::INVENTORY_OPEN);
				return true;
			}//�������Enter����������
			//�������Ă��Ȃ��@���@�������Enter����������
			else if (use_equip->SelectNum == 1 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
				//���ʉ��Đ�
				gManager->sound->System_Play(gManager->sound->system_select);
				//���j���[�����
				firstMenu->menu_live = false;
				//�A�C�e����������
				ItemThrow(drawInventoryPage);
				//�A�C�e���������V�[�N�G���X�Ɉړ�
				ChangeSequence(sequence::THROWITEMMOVE);
				return true;
			}
			//��߂��Enter����������
			else if (use_equip->SelectNum == 2 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
				//���ʉ��Đ�
				gManager->sound->System_Play(gManager->sound->system_cancel);
				//���j���[�����
				use_equip->menu_live = false;
				//�A�C�e���̈ꎞ���j��
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


void DungeonScene::DrawMiniEnemy()
{

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	for (auto liveEnemy : gManager->liveEnemyList) {
		t2k::Vector3 localPos = gManager->WorldToLocal(liveEnemy->pos);
		if (gManager->CheckCanDraw(localPos)) {
			//�~�j�}�b�v�Ƀv���C���[�̈ʒu��`��
			DrawRotaGraph(localPos.x * 8 + 150, localPos.y * 8 + 130, 0.4, 0, miniEnemy, true);
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
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

			//�X�L���Ȃ�X�L���̏����擾����
			if (lastUseSkill != nullptr) {
				int index = lastUseSkill->GetGraphicAllNum();

				//Animation�N���X��new
				std::shared_ptr<Animation>anim = std::make_shared<Animation>(lastUseSkill->GetGraphicHandle(), player->effectPos, lastUseSkill->GetActSpeed(), index);
				//�`�惊�X�g�ɓo�^
				drawAnimationList.emplace_back(anim);
				return true;
			}

			//�A�j���[�V�����摜�̍ő�Index�ԍ��̎擾
			int index = gManager->GetMaxIndex(GameManager::index::ATTACK);
			//Animation�N���X��new
			std::shared_ptr<Animation>anim = std::make_shared<Animation>("graphics/AttackAnim_30.png", player->effectPos, ATTACKEFFECTSPEED, index, 5, 1, 30, 30);
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
			for (auto attackEnemy : attackEnemies) {
				//Animation�N���X��new
				std::shared_ptr<Animation>anim = std::make_shared<Animation>("graphics/AttackAnim_30.png", player->pos, ATTACKEFFECTSPEED, index, 5, 1, 30, 30);
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

bool DungeonScene::SeqShopMain(const float deltatime)
{
	if (mainSequence.isStart()) {
		if (gManager->GetDoneBuy())return true;
		if (shopPages.empty()) {
			//�V����inventory�̃C���X�^���X�𐶐�����
			Inventory* newPage = new Inventory(shopPage + 1);
			//�V���b�v�̃y�[�W��o�^
			shopPages.emplace_back(newPage);
		}
		//�V���b�v�y�[�W�ɃA�C�e����o�^(6��,2��,2�œo�^)
		SetShopItem(6, static_cast<uint32_t>(ItemType::CONSUME));
		SetShopItem(2, static_cast<uint32_t>(ItemType::WEAPON));
		SetShopItem(2, static_cast<uint32_t>(ItemType::ARMOR));
	}
	//ESCAPE�ŃV���b�v�����
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_ESCAPE)) {

		//�V���b�v���g�����t���O���X�V
		gManager->SetDoneBuy();
		//�J�[�\���ʒu�̃��Z�b�g
		shopPages[currentDrawPage]->CursorReset();

		//�V���b�v�����
		ChangeSequence(sequence::MAIN);
		return true;
	}

	//�y�[�W���̃A�C�e������Ȃ�return
	if (shopPages[currentDrawPage]->inventoryList.empty())return true;
	//�V���b�v�A�C�e���̑I���ړ�
	shopPages[currentDrawPage]->CursorMove();
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
		int cursorNum = shopPages[currentDrawPage]->GetCursorNum();

		//�\�����̃C���x���g���̖`���̃A�C�e�����擾
		auto itr = shopPages[currentDrawPage]->inventoryList.begin();

		//�I�����ꂽ�A�C�e���܂ŃC�e���[�^�ړ�
		for (int i = 0; i < cursorNum; ++i) {
			itr++;
		}
		int price = 0;
		if ((*itr)->getItemData(1) >= 2) {
			auto item = static_cast<equipItem*>((*itr));
			price = item->getItemData(10);
		}
		else {
			price = (*itr)->getItemData(5);
		}
		//������������Ȃ��Ƃ��͔���Ȃ�
		if (player->GetHaveCoin() < price) {
			gManager->addLog("������������܂���!");
			return true;
		}

		int itemId = gManager->GetItemId((*itr));
		//�C���x���g���ɕ\������Ă����X�e�[�^�X�̂܂ܒǉ�����
		//gManager->AddItemToInventory(itemId, gManager->inventories, gManager->inventoryNum, 1);
		gManager->AddItemFromShop((*itr));
		//�R�C�������炷
		player->ChangeHaveCoin(price * (-1));

		//shop�ɕ���ł���Y���A�C�e����delete����
		delete (*itr);
		itr = shopPages[currentDrawPage]->inventoryList.erase(itr);

		shopPages[currentDrawPage]->SetCursorNum(-1);

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
bool DungeonScene::CheckExtraOnTile()
{
	//����ł���^�C�������ʂ̒ʘH��������return false;
	if (gManager->GetMapChip(playerPos) == 1) return false;

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
	//����ł���̂��V���b�v��������
	else if (gManager->GetMapChip(playerPos) == 4) {
		//enter�ŃV���b�v�V�[�N�G���X�ɓ���
		if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
			gManager->sound->System_Play(gManager->sound->system_select);
			ChangeSequence(sequence::SHOP);
			return true;
		}
	}
	return false;
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
		//�S�[���K�w�Ȃ物���̂������t�^���ăV�[���`�F���W
		if (dungeonLevel >= 49) {
			DungeonClear();
			return true;
		}

		//����map�𐶐�
		MoveLevel(1);
		playerPos = gManager->WorldToLocal(gManager->player->pos);
		//ChangeSequence(sequence::FADEIN);
		ChangeSequence(sequence::FADEDESC);
		return true;
	}
	return true;
}

bool DungeonScene::ActivateSkillCheck()
{

	//int t[5] = { t2k::Input::KEYBORD_1, t2k::Input::KEYBORD_2 };
	//�e�L�[�������ꂽ���m�F
	for (int i = 0; i < 3; ++i) {
		if (t2k::Input::isKeyDownTrigger(static_cast<t2k::Input::eKeys>(skillKeys[i]))) {
			//�����X�L�����X�g���󂶂�Ȃ��Ȃ�
			if (!player->GetSkillList().empty()) {
				//�X�L����o�^
				lastUseSkill = player->GetSkillList()[i];
				//�A�j���[�V�����V�[�N�G���X�ɔ�΂�
				ChangeSequence(sequence::ANIMATION);
				return true;
			}
			//�X�L�����Ȃ���ԂŃX�L�����g�����Ƃ���ƒʏ�U������
			else {
				ChangeSequence(sequence::ANIMATION);
				return true;
			}
		}
	}

	return false;
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
	else if (seq == sequence::SHOP) {
		mainSequence.change(&DungeonScene::SeqShopMain);
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
	for (auto enemy : gManager->liveEnemyList) {

		DrawStringEx(450 + 120 * i, 480, -1, "%s", enemy->GetName().c_str());
		DrawStringEx(450 + 120 * i, 500, -1, "%d", enemy->GetStatus(0));
		DrawStringEx(450 + 120 * i, 520, -1, "%d", enemy->GetStatus(1));
		DrawStringEx(450 + 120 * i, 540, -1, "%d", enemy->GetStatus(2));
		DrawStringEx(450 + 120 * i, 560, -1, "%d", enemy->GetStatus(3));
		DrawStringEx(450 + 120 * i, 580, -1, "%d", enemy->GetExp());
		++i;
	}

}

void DungeonScene::DrawInventory(int x, int y)
{
	//�C���x���g������Ȃ�return
	if (gManager->inventories[drawInventoryPage]->inventoryList.empty())return;
	//�����T�C�Y�ύX
	SetFontSize(25);
	//�C���x���g�����̃A�C�e����`��
	gManager->inventories[drawInventoryPage]->DrawInventory(x, y);

	//�V���b�v�V�[�N�G���X�ł̓C���x���g�����̃A�C�e���f�[�^�͕\�����Ȃ�
	if (nowSeq != sequence::SHOP) {
		//�C���x���g���y�[�W����`��
		DrawStringEx(x + 300, y + 10, -1, "�y�[�W:%d", gManager->inventories[drawInventoryPage]->GetInventoryNum());
		//�A�C�e�������`��
		desc->Menu_Draw();
		gManager->inventories[drawInventoryPage]->DrawItemDesc(desc->menu_x + 10, desc->menu_y + 10);
		//�X�e�[�^�X��r�`��
		status->Menu_Draw();
		gManager->inventories[drawInventoryPage]->DrawEquipItemStatus(status->menu_x + 10, status->menu_y + 10);
	}
	SetFontSize(16);
}

void DungeonScene::ChangeInventory()
{
	//�㉺�ړ�
	gManager->inventories[drawInventoryPage]->CursorMove();

	//�C���x���g����������Ȃ�return
	if (gManager->inventoryNum == 0)return;
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

}

void DungeonScene::SpawnItem(int ItemId)
{
	//�A�C�e���f�[�^���}�X�^�[����擾
	Item* popItem = gManager->GetItemData(ItemId);
	if (popItem == nullptr)return;

	std::vector<int> intData = popItem->GetAllIntData();
	std::vector<std::string> stringData = popItem->GetAllStringData();


	//�擾�����f�[�^�ŃA�C�e����V�K����
	Item* newItem = new Item(intData[0], intData[1], stringData[0], intData[2], intData[3], intData[4], intData[5], stringData[1], stringData[2]);

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
	newItem->SetPos(popPos);
	dropItems.emplace_back(newItem);
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
			//�ŏ��̃y�[�W�Ȃ��߂�
			if (drawInventoryPage == 0) {
				gManager->isDeleteInventory = false;
				return;
			}
			//�������ܕ`�悵�Ă���C���x���g�����Ō�̃y�[�W�Ȃ�`��y�[�W��1���炷
			if (drawInventoryPage == gManager->inventoryNum + 1) {
				drawInventoryPage--;
			}
			//drawInventoryPage--;
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

	//�����ŐV����itemBuf�Ɠ����f�[�^�ŃA�C�e���𐶐�����
	std::vector<int> intData = itemBuf->GetAllIntData();
	std::vector<string> stringData = itemBuf->GetAllStringData();

	std::shared_ptr<Item>throwedItem = std::make_shared<Item>(intData[0], intData[1], stringData[0], intData[2], intData[3], intData[4], intData[5], stringData[1], stringData[2]);

	throwedItem->SetPos(player->pos);
	throwedItem->SetGoalPos(player->mydir);

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
	auto itr1 = gManager->liveEnemyList.begin();
	auto itr2 = gManager->liveEntityList.begin();

	for (int i = 0; i < gManager->liveEnemyList.size(); ++i) {
		if ((*itr1)->isLive == false) {
			itr1 = gManager->liveEnemyList.erase(itr1);
		}
		else {
			itr1++;
		}
	}
	for (int i = 0; i < gManager->liveEntityList.size(); ++i) {
		if ((*itr2)->isLive == false) {
			itr2 = gManager->liveEntityList.erase(itr2);
		}
		else {
			itr2++;
		}
	}
}

void DungeonScene::WhenDeadPlayer()
{
	//�v���C���[�ƃC���x���g���̏�����
	gManager->PlayerDead();
	//�_���W�����̃��x���̏�����
	dungeonLevel = 1;
	//�L�����v�V�[���ɖ߂�
	SceneManager::ChangeScene(SceneManager::SCENE::CAMP);
	return;
}

void DungeonScene::SetShopItem(int SetNum, int ItemType)
{
	//SetNum�Ɠ������������_���ŃA�C�e���𐶐����C���x���g���ɓ����
	for (int i = 0; i < SetNum; ++i) {
		//�A�C�e���^�C�v�̒����烉���_���ȃA�C�e����Id���擾
		int itemId = gManager->GetRandItemData(ItemType);
		//�V���b�v�A�C�e���y�[�W�ɃA�C�e����ǉ�
		gManager->AddItemToInventory(itemId, shopPages, shopPage);
	}
}

//firstMenu��select�֐��S
void DungeonScene::InventoryOpen()
{
	t2k::debugTrace("\nmenu1\n");
	//gManager->sound->System_Play(gManager->sound->system_select);
	////menu�̏㉺�𑀍�o���Ȃ�����
	//firstMenu->manageSelectFlag = false;

	////InventoryOpen�V�[�N�G���X�Ɉړ�����
	//ChangeSequence(sequence::INVENTORY_OPEN);
}

void DungeonScene::CheckFootPoint()
{
	t2k::debugTrace("\nmenu2\n");
}

void DungeonScene::Save()
{
	t2k::debugTrace("\nmenu3\n");
}

void DungeonScene::BackTitle()
{
	t2k::debugTrace("\nmenu4\n");
}

void DungeonScene::MenuClose()
{
	t2k::debugTrace("\nmenu5\n");
	/*gManager->sound->System_Play(gManager->sound->system_cancel);
	firstMenu->menu_live = false;
	ChangeSequence(sequence::MAIN);*/
}
void DungeonScene::menu5Select()
{
	t2k::debugTrace("hoge");
}

bool DungeonScene::DetectItem()
{

	//�A�C�e�������蔻�芴�m
	for (auto item : dropItems) {
		//�A�C�e���ƃv���C���[���d�Ȃ�����
		if (item->DetectOnPlayer(playerPos)) {
			//�A�C�e�������łɏE���ĂȂ����
			if (itemGetFlag) {
				gManager->AddItemToInventory(item->GetItemId(), gManager->inventories, gManager->inventoryNum);
				item->SetIsLiveFalse();
				itemGetFlag = false;

				return true;
			}
		}
	}
	return false;
}

void DungeonScene::DropItem(const int ItemId, const t2k::Vector3 DropPos)
{
	Item* dropItem = gManager->GetItemData(ItemId);

	dropItem->SetPos(DropPos);
	dropItems.emplace_back(dropItem);
}

void DungeonScene::DrawTopUI()
{
	std::vector<Skill*>playerSkills = player->GetSkillList();

	DrawStringEx(topUI->menu_x + 300, topUI->menu_y + 15, -1, "�X�L��");

	for (int i = 0; i < playerSkills.size(); ++i) {
		//�X�L�����`��
		DrawStringEx(topUI->menu_x + 380 + i * 130, topUI->menu_y + 15, -1, "%s", playerSkills[i]->GetSkillName().c_str());
		//�A�C�R���摜�`��
		DrawRotaGraph(topUI->menu_x + 400 + i * 150, topUI->menu_y + topUI->menu_height / 2, 1.5, 0, playerSkills[i]->GetSkillIconGh(), true);
	}
	//�L�[�摜�`��
	for (int i = 0; i < 3; ++i) {
		DrawRotaGraph(topUI->menu_x + 370 + i * 130, topUI->menu_y + 23, 1, 0, numButtons[i], false);
	}

	//��ʏ㕔�̃v���C���[HP�o�[
	//player->TopHpVarDraw(100, 80);
	player->TopHpVarDraw(topUI->menu_x + 20, topUI->menu_y + 80);

}

void DungeonScene::DungeonClear()
{
	//������t�^
	gManager->SetFragmentNum(1);
	//�N���A�t���O
	dungeonClear = true;
	//���O���Z�b�g
	gManager->ResetLog();
	player->pos = gManager->SpawnPlayerCamp();
	player->mydir = player->dir::UP;
}


