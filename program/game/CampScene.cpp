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
	if (player == nullptr)player = gManager->GetPlayer();
	gManager->CameraReset();

	fragmentsGh[0] = gManager->LoadGraphEx("graphics/fragment_0.png");
	fragmentsGh[1] = gManager->LoadGraphEx("graphics/fragment_1.png");
	fragmentsGh[2] = gManager->LoadGraphEx("graphics/fragment_2.png");
	fragmentsGh[3] = gManager->LoadGraphEx("graphics/fragment_3.png");
	fragmentsGh[4] = gManager->LoadGraphEx("graphics/fragment_4.png");
	fragmentsGh[5] = gManager->LoadGraphEx("graphics/fragment_Full.png");

	spaceButton = gManager->LoadGraphEx("graphics/spaceButton.png");
	EnterButton = gManager->LoadGraphEx("graphics/button_Enter.png");

	fragBackGround = gManager->LoadGraphEx("graphics/fragment_Back.png");

	dungeonEnterUi = new Menu(345, 245, 330, 260, "graphics/WindowBase_01.png");
	/*Menu* hoge=new Menu(342,246,330,258,gh(int))
Menu* hoge=new Menu(494,521,505,234,gh(int))
*/
	miniFragment = new Menu(50, 520, 220, 248, "graphics/WindowBase_02.png");

	MenuWindow::MenuElement_t* menu_usable = new MenuWindow::MenuElement_t[]{
	{650,480,"�_���W�����ɓ���",0},
	{650,510,"��߂�",1}
	};
	dungeonIn = new MenuWindow(640, 440, 150, 100, "graphics/WindowBase_02.png", menu_usable, 2, 0.35);

	ChangeSequence(sequence::FADEIN);
}

CampScene::~CampScene()
{
	delete dungeonIn;
	StopSoundMem(gManager->sound->bgm_town);
}

void CampScene::Update()
{
	//debug
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_F)) {
		gManager->SetFragmentNum(1);
		t2k::debugTrace("\n�����̂������1�����܂�\n");
		t2k::debugTrace("\n�����̂������:%d\n", gManager->GetFragmentNum());
	}


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
	gManager->DrawHowTo();
	if (nowSeq == sequence::DUNGEONIN) {
		dungeonIn->All();
		DrawStringEx(650, 450, GetColor(0, 0, 0), "%s", DUNGEONNAME[selectDungeon].c_str());
	}
	if (drawFrag) {
		DrawRotaGraph(512, 384, 1, 0, fragBackGround, false);
		DrawRotaGraph(512, 384, 1, 0, fragmentsGh[gManager->GetFragmentNum()], true);
		SetFontSize(25);
		DrawStringEx(300, 100, -1, "���ݏ������Ă��鉩���̂�����:%d��", gManager->GetFragmentNum());
		SetFontSize(16);
	}
	else {
		miniFragment->Menu_Draw();
		DrawStringEx(miniFragment->menu_x + 40, miniFragment->menu_y + 15, GetColor(0, 0, 0), "�����̂�����ꗗ");
		DrawRotaGraph(miniFragment->menu_x + miniFragment->menu_width / 2, miniFragment->menu_y + miniFragment->menu_height / 2, 0.3, 0, fragBackGround, false);
		DrawRotaGraph(miniFragment->menu_x + miniFragment->menu_width / 2, miniFragment->menu_y + miniFragment->menu_height / 2, 0.3, 0, fragmentsGh[gManager->GetFragmentNum()], true);
		DrawRotaGraph(miniFragment->menu_x + miniFragment->menu_width / 2, miniFragment->menu_y + miniFragment->menu_height - 20, 1, 0, spaceButton, true);

	}
	if (nowSeq != sequence::MAIN)return;
	//���_���̑O�ɂ��鎞�Ƀ_���W��������𑣂�UI��\������
	if (GetSurfaceGraphicNum(gManager->WorldToLocal(player->pos).x, gManager->WorldToLocal(player->pos).y - 1) == 469) {

		dungeonEnterUi->Menu_Draw();
		DrawStringEx(dungeonEnterUi->menu_x + 20, dungeonEnterUi->menu_y + 10, -1, "Enter�������ă_���W�����ɓ��ꂷ��");
		DrawRotaGraph(510, 480, 1, 0, EnterButton, true);
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
	//�����̂������ʂ�`��
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_SPACE)) {
		if (!drawFrag)drawFrag = true;
		else drawFrag = false;
	}


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
