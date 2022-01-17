#include "GameManager.h"
#include"SceneManager.h"
#include"DxLib.h"

#include"../support/Support.h"
#include<string>
#include"map.h"
#include<time.h>
#include"Player.h"
#include"Camera.h"
//#include"game_main.h"

//#include"Item.h"
//#include "FadeControl.h"
//#include "SoundManager.h"



GameManager::GameManager()
{

}

GameManager::~GameManager()
{

}

void GameManager::Update()
{
	camera->CameraMove();
	SceneManager::Update();

}
void GameManager::Draw()
{
	SceneManager::Render();
}

void GameManager::initGameManager()
{
	SRand(time(0));
	camera = new Camera();
	map = new Map(MAPWIDTH, MAPHEIGHT);
	map->DivideStart(MAPWIDTH, MAPHEIGHT, map);

	player = new Player(SetStartPos());
	camera->cameraPos = player->pos - t2k::Vector3(512, 384, 0);

	//sound = new Sound();
	//fControl = new FadeControl();

	deitatime_ = 0;
	SceneManager::ChangeScene(SceneManager::SCENE::DUNGEON);

	/*itemList.resize(3);
	loadItem();

	haveItem.resize(38);

	haveItemInit();*/

}

int GameManager::LoadGraphEx(std::string gh)
{

	auto it = ghmap.find(gh);
	if (it != ghmap.end()) {
		return ghmap[gh];
	}

	else {
		int loadgh = LoadGraph(gh.c_str());
		ghmap.insert(std::make_pair(gh, loadgh));
	}


	return ghmap[gh];
}

void GameManager::haveItemInit()
{
	for (int i = 0; i < haveItem.size(); ++i) {
		//アイテムi番を〇〇個で初期化する
		haveItem[i].emplace_back(1);
	}
}

void GameManager::setitem(int ItemId, int addNum)
{
	haveItem[ItemId][0] += addNum;
}

int GameManager::GetRandEx(int a, int b)
{
	if (a > b) {
		int hoge = a - b;
		int random = GetRand(hoge) + b;
		return random;
	}
	else {
		int hoge = b - a;
		int random = GetRand(hoge) + a;
		return random;
	}
	return 0;
}



void GameManager::loadItem()
{
	loadItemCsv = t2k::loadCsv("Csv/Item.csv");
	for (int i = 1; i < loadItemCsv.size(); ++i) {

		//id
		int a = std::atoi(loadItemCsv[i][0].c_str());
		//ItemType
		int b = std::atoi(loadItemCsv[i][1].c_str());
		//setDay
		int c = std::atoi(loadItemCsv[i][2].c_str());
		//addStatus
		int d = std::atoi(loadItemCsv[i][3].c_str());
		//addStatusNum
		int e = std::atoi(loadItemCsv[i][4].c_str());
		//setAbility
		int f = std::atoi(loadItemCsv[i][5].c_str());
		//setAbilityType
		int g = std::atoi(loadItemCsv[i][6].c_str());
		//num
		int h = std::atoi(loadItemCsv[i][10].c_str());


		//Item* abi = new Item(a, b, c, d, e, f, g, loadItemCsv[i][7], loadItemCsv[i][8], loadItemCsv[i][9], h);

		//abilitytypeごとにリストに格納
		//itemList[b].emplace_back(abi);
	}
}

t2k::Vector3 GameManager::SetStartPos()
{
	//ランダムな部屋番号を取得
	int rand = GetRand(map->GetRoomNum());
	//部屋番号から部屋を取得 0:左 1:上 2:右 3:下 帰ってくるのはマップ座標
	vector<int> room = map->GetRoom(rand);
	//部屋の中のランダムなマップ座標を取得する
	int x = GetRandEx(room[0], room[2]);
	int y = GetRandEx(room[1], room[3]);

	//取得したマップ座標を描画座標に変換する
	t2k::Vector3 playerPos = map->MapToWorld(x, y);

	return playerPos;
}

void GameManager::Zoom(double* zoomEx)
{
	//if (t2k::Input::isKeyDown(t2k::Input::KEYBORD_Z)) {
	//	*zoomEx -= 0.01;
	//}
	//else if (t2k::Input::isKeyDown(t2k::Input::KEYBORD_X)) {
	//	*zoomEx += 0.01;
	//}

}

void GameManager::ReCreate()
{
	delete map;
	map = nullptr;
	delete player;
	player = nullptr;

	map = new Map(MAPWIDTH, MAPHEIGHT);
	map->DivideStart(MAPWIDTH, MAPHEIGHT, map);

	player = new Player(SetStartPos());
	camera->cameraPos = player->pos - t2k::Vector3(512, 384, 0);
}

t2k::Vector3 GameManager::WorldToLocal(t2k::Vector3 Pos)
{
	return map->WorldToMap(Pos.x, Pos.y);
}

t2k::Vector3 GameManager::LocalToWorld(int MapX, int MapY)
{
	return map->MapToWorld(MapX, MapY);
}

int GameManager::GetMapChip(t2k::Vector3 PInChip)
{
	return map->GetChip(PInChip.x, PInChip.y);
}
//なんか微妙
void GameManager::CameraMove(Player* p)
{

	camera->cameraPos = player->pos - t2k::Vector3(512, 384, 0);

	/*if (p->pos.x < 200) {
		camera->cameraPos.x = player->pos.x - 512;
	}
	if (p->pos.x > 800) {
		camera->cameraPos.x = player->pos.x - 512;
	}

	if (p->pos.y < 150) {
		camera->cameraPos.y = player->pos.y - 384;
	}
	if (p->pos.y > 570) {
		camera->cameraPos.y = player->pos.y - 384;
	}*/
}

