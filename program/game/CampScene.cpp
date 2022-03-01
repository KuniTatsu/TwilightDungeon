#include "CampScene.h"
#include"GameManager.h"
#include"DxLib.h"
#include"Player.h"
#include"Camera.h"
#include"SoundManager.h"
#include"MenuWindow.h"
#include"FadeControl.h"
#include"SceneManager.h"
#include"../support/Support.h"

using namespace std;

extern GameManager* gManager;

CampScene::CampScene()
{
	gManager->sound->BGM_Play(gManager->sound->bgm_town);

	LoadMap("Csv/start_map_floor.csv", loadGroundMap, groundMapData);
	LoadMap("Csv/start_map_top.csv", loadSurfaceMap, surfaceMapData);
	LoadMap("Csv/start_map_cannotMove.csv", loadCollision, collisionData);

	LoadDivGraph("graphics/mapchip_night_20.png", 480, 30, 16, 20, 20, campGraphic);
	gManager->MakePlayer(GameManager::SpawnScene::Camp);
	player = gManager->GetPlayer();
	gManager->CameraReset();

	MenuWindow::MenuElement_t* menu_usable = new MenuWindow::MenuElement_t[]{
	{650,480,"�_���W�����ɓ���",0},
	{650,510,"��߂�",1}
	};
	dungeonIn = new MenuWindow(640, 440, 150, 100, "graphics/WindowBase_02.png", menu_usable, 2, 0.35);
}

CampScene::~CampScene()
{
	delete dungeonIn;
	StopSoundMem(gManager->sound->bgm_town);
}

void CampScene::Update()
{
	//���݂̃V�[�N�G���X�̏���
	mainSequence.update(gManager->deitatime_);

	//�t�F�[�h����������܂Ńt�F�[�h�A�E�g�����s
	if (nowFade) {
		gManager->fControl->FadeOut();

		if (gManager->fControl->doneFade) {
			t2k::debugTrace("\n�_���W�����ɓ��ꂵ�܂�\n");

			//int�^��GameManager::Dungeon�^�ɃL���X�g
			GameManager::Dungeon dungeonName = static_cast<GameManager::Dungeon>(selectDungeon);

			//�_���W�����̎�������
			gManager->CreateDungeon(dungeonName);


			SceneManager::ChangeScene(SceneManager::SCENE::DUNGEON);
			return;
		}
	}
}

void CampScene::Draw()
{
	DrawMap(groundMapData);
	DrawMap(surfaceMapData);

	//DrawStringEx(500, 500, -1, "������Camp�V�[���ł�");
	player->Draw();
	if (nowSeq == sequence::DUNGEONIN) {
		dungeonIn->All();
		DrawStringEx(650, 450, GetColor(0, 0, 0), "%s", DUNGEONNAME[selectDungeon].c_str());
	}
}
int CampScene::GetGraphicHandle(int num)
{
	return campGraphic[num];
}
//Csv�t�H���_�̒��̓���̃}�b�v�����[�h����֐�
void CampScene::LoadMap(string fileName, vector<vector<string>>& stringData, vector<vector<int>>& intData)
{
	stringData = t2k::loadCsv(fileName);

	intData.resize(stringData.size());

	for (int i = 0; i < stringData.size(); ++i) {
		for (int k = 0; k < stringData[i].size(); ++k) {
			intData[i].emplace_back(atoi(stringData[i][k].c_str()));
		}
	}
}


void CampScene::DrawMap(vector<vector<int>>intData)
{
	int x = 0;
	int y = 0;
	for (int i = 0; i < intData.size(); ++i) {
		for (int k = 0; k < intData[i].size(); ++k) {
			if (intData[i][k] != -1) {
				int gh = GetGraphicHandle(intData[i][k]);
				DrawRotaGraph(x - gManager->camera->cameraPos.x, y - gManager->camera->cameraPos.y, 1, 0, gh, true);
			}
			x += 20;
		}
		x = 0;
		y += 20;
	}
}

bool CampScene::SeqMain(const float deltatime)
{
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
		//469��������V�[�N�G���X���ړ�����
		if (GetSurfaceGraphicNum(gManager->WorldToLocal(player->pos).x, gManager->WorldToLocal(player->pos).y - 1) == 469) {
			gManager->sound->System_Play(gManager->sound->system_select);
			for (int i = 0; i < 5; ++i) {
				if (GetGraphicNum(gManager->WorldToLocal(player->pos).x, gManager->WorldToLocal(player->pos).y) == PORTALPOINTNUM[i]) {
					selectDungeon = i;
					dungeonIn->Open();
					ChangeSequence(sequence::DUNGEONIN);
					return true;
				}
			}
		}
	}
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_UP)) {
		player->mydir = Player::UP;
		//�ړ��\��̍��W���擾����
		t2k::Vector3 nextPos = gManager->WorldToLocal(player->pos) + gManager->GetVecter(GameManager::UP);
		//�͈͊O�Ȃ瓮���Ȃ�
		if (!checkOutOfRange(nextPos))return true;
		//������悪�����Ȃ��Ȃ瓮���Ȃ�
		if (canMoveThisPoint(nextPos.x, nextPos.y) == 0) {
			player->TownMove(Player::UP);
			gManager->CameraMove(groundMapData[0].size() * 20, groundMapData.size() * 20, GameManager::UP);
		}
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RIGHT)) {
		player->mydir = Player::RIGHT;
		//�ړ��\��̍��W���擾����
		t2k::Vector3 nextPos = gManager->WorldToLocal(player->pos) + gManager->GetVecter(GameManager::RIGHT);
		//�͈͊O�Ȃ瓮���Ȃ�
		if (!checkOutOfRange(nextPos))return true;
		//������悪�����Ȃ��Ȃ瓮���Ȃ�
		if (canMoveThisPoint(nextPos.x, nextPos.y) == 0) {
			player->TownMove(Player::RIGHT);
			gManager->CameraMove(groundMapData[0].size() * 20, groundMapData.size() * 20, GameManager::RIGHT);
		}
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_DOWN)) {
		player->mydir = Player::DOWN;
		//�ړ��\��̍��W���擾����
		t2k::Vector3 nextPos = gManager->WorldToLocal(player->pos) + gManager->GetVecter(GameManager::DOWN);
		//�͈͊O�Ȃ瓮���Ȃ�
		if (!checkOutOfRange(nextPos))return true;
		//������悪�����Ȃ��Ȃ瓮���Ȃ�
		if (canMoveThisPoint(nextPos.x, nextPos.y) == 0) {
			player->TownMove(Player::DOWN);
			gManager->CameraMove(groundMapData[0].size() * 20, groundMapData.size() * 20, GameManager::DOWN);
		}
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_LEFT)) {
		player->mydir = Player::LEFT;
		//�ړ��\��̍��W���擾����
		t2k::Vector3 nextPos = gManager->WorldToLocal(player->pos) + gManager->GetVecter(GameManager::LEFT);
		//�͈͊O�Ȃ瓮���Ȃ�
		if (!checkOutOfRange(nextPos))return true;
		//������悪�����Ȃ��Ȃ瓮���Ȃ�
		if (canMoveThisPoint(nextPos.x, nextPos.y) == 0) {
			player->TownMove(Player::LEFT);
			gManager->CameraMove(groundMapData[0].size() * 20, groundMapData.size() * 20, GameManager::LEFT);
		}
	}
	return true;
}

bool CampScene::SeqDungeonInMenu(const float deltatime)
{

	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_ESCAPE)) {
		gManager->sound->System_Play(gManager->sound->system_cancel);
		dungeonIn->menu_live = false;
		ChangeSequence(sequence::MAIN);
		return true;
	}
	//�����I��������
	if (dungeonIn->SelectNum == 0 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
		gManager->sound->System_Play(gManager->sound->system_select);
		//menu�̏㉺�𑀍�o���Ȃ�����
		dungeonIn->manageSelectFlag = false;
		//gManager->sound->System_Play(gManager->sound->system_select);

		//�V�[���؂�ւ��t���O��on�ɂ���
		nowFade = true;
		return true;
	}
	//SetStartPos(setStartPosType::PLAYER) player�̈ʒu���X�V���邱��


	return true;
}

bool CampScene::SeqFadeIn(const float deltatime)
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



void CampScene::ChangeSequence(const sequence seq)
{
	nowSeq = seq;
	if (seq == sequence::MAIN) {
		mainSequence.change(&CampScene::SeqMain);
	}
	else if (seq == sequence::DUNGEONIN) {
		mainSequence.change(&CampScene::SeqDungeonInMenu);
	}
	else if (seq == sequence::FADEIN) {
		mainSequence.change(&CampScene::SeqFadeIn);
	}
}
