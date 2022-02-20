#include "CampScene.h"
#include"GameManager.h"
#include"DxLib.h"
#include"Player.h"
#include"Camera.h"
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
}

CampScene::~CampScene()
{
}

void CampScene::Update()
{
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_LEFT)) {
		//移動予定の座標を取得する
		t2k::Vector3 nextPos = gManager->GetPlayerLocalPos() + gManager->GetVecter(GameManager::LEFT);
		//左一歩先が動けないなら動かない
		if (!canMoveThisPoint(nextPos.x, nextPos.y)) {
			player->TownMove(Player::LEFT);
		}
	}
}

void CampScene::Draw()
{
	DrawMap(groundMapData);
	DrawMap(surfaceMapData);

	DrawStringEx(500, 500, -1, "ここはCampシーンです");
	player->Draw();

}
int CampScene::GetGraphicHandle(int num)
{
	return campGraphic[num];
}
//Csvフォルダの中の特定のマップをロードする関数
void CampScene::LoadMap(std::string fileName, std::vector<std::vector<std::string>>& stringData, std::vector<std::vector<int>>& intData)
{
	stringData = t2k::loadCsv(fileName);

	intData.resize(stringData.size());

	for (int i = 0; i < stringData.size(); ++i) {
		for (int k = 0; k < stringData[i].size(); ++k) {
			intData[i].emplace_back(atoi(stringData[i][k].c_str()));
		}
	}
}


void CampScene::DrawMap(std::vector<std::vector<int>>intData)
{
	int x = 0;
	int y = 0;
	for (int i = 0; i < intData.size(); ++i) {
		for (int k = 0; k < intData[i].size(); ++k) {
			int gh = GetGraphicHandle(intData[i][k]);
			DrawRotaGraph(x - gManager->camera->cameraPos.x, y - gManager->camera->cameraPos.y, 1, 0, gh, true);
			x += 20;
		}
		x = 0;
		y += 20;
	}
}
