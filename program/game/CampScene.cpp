#include "CampScene.h"
#include"GameManager.h"
#include"DxLib.h"
#include"Player.h"
#include"Camera.h"
#include"MenuWindow.h"
#include"FadeControl.h"
#include"SceneManager.h"
#include"../support/Support.h"

using namespace std;

extern GameManager* gManager;
CampScene::CampScene()
{
	LoadMap("Csv/start_map_floor.csv", loadGroundMap, groundMapData);
	LoadMap("Csv/start_map_top.csv", loadSurfaceMap, surfaceMapData);
	LoadMap("Csv/start_map_cannotMove.csv", loadCollision, collisionData);

	LoadDivGraph("graphics/mapchip_night_20.png", 480, 30, 16, 20, 20, campGraphic);
	gManager->MakePlayer(GameManager::SpawnScene::Camp);
	player = gManager->GetPlayer();

	MenuWindow::MenuElement_t* menu_usable = new MenuWindow::MenuElement_t[]{
	{650,480,"ダンジョンに入る",0},
	{650,510,"やめる",1}
	};
	dungeonIn = new MenuWindow(640, 440, 150, 100, "graphics/WindowBase_02.png", menu_usable, 2, 0.35);
}

CampScene::~CampScene()
{
}

void CampScene::Update()
{
	//現在のシークエンスの処理
	mainSequence.update(gManager->deitatime_);

	//フェードが完了するまでフェードアウトを実行
	if (nowFade) {
		gManager->fControl->FadeOut();
	}
	//ゲームの開始処理
	//フェードが完了したらDungeonSceneへ飛ばす
	if (!gManager->fControl->doneFade)return;

	t2k::debugTrace("\nダンジョンに入場します\n");

	//int型をGameManager::Dungeon型にキャスト
	GameManager::Dungeon dungeonName = static_cast<GameManager::Dungeon>(selectDungeon);

	//ダンジョンの自動生成
	gManager->CreateDungeon(dungeonName);

	
	SceneManager::ChangeScene(SceneManager::SCENE::DUNGEON);
	return;

}

void CampScene::Draw()
{
	DrawMap(groundMapData);
	DrawMap(surfaceMapData);

	DrawStringEx(500, 500, -1, "ここはCampシーンです");
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
//Csvフォルダの中の特定のマップをロードする関数
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
		//469だったらシークエンスを移動する
		if (GetSurfaceGraphicNum(gManager->WorldToLocal(player->pos).x, gManager->WorldToLocal(player->pos).y - 1) == 469) {
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
		//移動予定の座標を取得する
		t2k::Vector3 nextPos = gManager->WorldToLocal(player->pos) + gManager->GetVecter(GameManager::UP);
		//範囲外なら動かない
		if (!checkOutOfRange(nextPos))return true;
		//左一歩先が動けないなら動かない
		if (canMoveThisPoint(nextPos.x, nextPos.y) == 0) {
			player->TownMove(Player::UP);
			gManager->CameraMove(groundMapData[0].size() * 20, groundMapData.size() * 20, GameManager::UP);
		}
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RIGHT)) {
		player->mydir = Player::RIGHT;
		//移動予定の座標を取得する
		t2k::Vector3 nextPos = gManager->WorldToLocal(player->pos) + gManager->GetVecter(GameManager::RIGHT);
		//範囲外なら動かない
		if (!checkOutOfRange(nextPos))return true;
		//左一歩先が動けないなら動かない
		if (canMoveThisPoint(nextPos.x, nextPos.y) == 0) {
			player->TownMove(Player::RIGHT);
			gManager->CameraMove(groundMapData[0].size() * 20, groundMapData.size() * 20, GameManager::RIGHT);
		}
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_DOWN)) {
		player->mydir = Player::DOWN;
		//移動予定の座標を取得する
		t2k::Vector3 nextPos = gManager->WorldToLocal(player->pos) + gManager->GetVecter(GameManager::DOWN);
		//範囲外なら動かない
		if (!checkOutOfRange(nextPos))return true;
		//左一歩先が動けないなら動かない
		if (canMoveThisPoint(nextPos.x, nextPos.y) == 0) {
			player->TownMove(Player::DOWN);
			gManager->CameraMove(groundMapData[0].size() * 20, groundMapData.size() * 20, GameManager::DOWN);
		}
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_LEFT)) {
		player->mydir = Player::LEFT;
		//移動予定の座標を取得する
		t2k::Vector3 nextPos = gManager->WorldToLocal(player->pos) + gManager->GetVecter(GameManager::LEFT);
		//範囲外なら動かない
		if (!checkOutOfRange(nextPos))return true;
		//左一歩先が動けないなら動かない
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
		dungeonIn->menu_live = false;
		ChangeSequence(sequence::MAIN);
		return true;
	}
	//入るを選択したら
	if (dungeonIn->SelectNum == 0 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
		//menuの上下を操作出来なくする
		dungeonIn->manageSelectFlag = false;
		//gManager->sound->System_Play(gManager->sound->system_select);

		//シーン切り替えフラグをonにする
		nowFade = true;
		return true;
	}
	//SetStartPos(setStartPosType::PLAYER) playerの位置を更新すること


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
}
