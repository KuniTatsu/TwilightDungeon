#include "GameManager.h"
#include"SceneManager.h"
#include"DxLib.h"
#include<algorithm>

#include"../support/Support.h"
#include<string>
#include"map.h"
#include<time.h>
#include"Player.h"
#include"Camera.h"
//#include"game_main.h"

//#include"Item.h"
//#include "FadeControl.h"
#include "SoundManager.h"



GameManager::GameManager()
{

}

GameManager::~GameManager()
{

}

void GameManager::Update()
{

	SceneManager::Update();


}
void GameManager::Draw()
{
	SceneManager::Render();
}

void GameManager::initGameManager()
{
	SRand(time(0));
	// 
	//debug
	//SRand(1);

	camera = new Camera();
	map = new Map(MAPWIDTH, MAPHEIGHT);
	map->DivideStart(MAPWIDTH, MAPHEIGHT, map);
	//階段のマップ座標の取得
	t2k::Vector3 stairsPos = SetStartPos(1);
	//階段設置
	map->SetChip(stairsPos.x, stairsPos.y, map->STAIRS);

	//wayPoint.resize(map->GetRoomNum() + 1);

	/*for (int i = 0; i < map->sumRoomNum; ++i) {
		CheckRoomWayPoint(i);
	}*/

	player = new Player(SetStartPos(0));
	camera->cameraPos = player->pos - t2k::Vector3(512, 384, 0);

	sound = new Sound();
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

t2k::Vector3 GameManager::SetStartPos(int setType)
{
	//ランダムな部屋番号を取得
	int rand = GetRand(map->GetRoomNum());
	//部屋番号から部屋を取得 0:左 1:上 2:右 3:下 帰ってくるのはマップ座標
	vector<int> room = map->GetRoom(rand);
	//部屋の中のランダムなマップ座標を取得する
	int x = GetRandEx(room[0], room[2]);
	int y = GetRandEx(room[1], room[3]);
	//敵の生成だったら座標被りチェックを行う
	if (setType == 2) {
		int player_x = player->pos.x;
		int player_y = player->pos.y;
		while (1) {
			if (player_x != x && player_y != y)break;
			x = GetRandEx(room[0], room[2]);
			y = GetRandEx(room[1], room[3]);
		}
	}

	//0:playerじゃなければマップ座標を返す
	if (setType == 1)return t2k::Vector3(x, y, 0);

	//取得したマップ座標を描画座標に変換する
	t2k::Vector3 Pos = map->MapToWorld(x, y);

	return Pos;
}

void GameManager::InitWayPointVector(int initroomNum)
{
	wayPoint.resize(initroomNum + 1);
}

//void GameManager::CheckRoomWayPoint(int roomId)
//{
//	//roomIdの部屋の左上から右下まで見て通路があればemplase_backする
//	std::vector<int> room = map->GetRoom(roomId);
//	for (int k = room[1] - 1; k < room[3] + 2; ++k) {
//		for (int i = room[0] - 1; i < room[2] + 2; ++i) {
//
//			t2k::Vector3 chip = t2k::Vector3(i, k, 0);
//			//壁だったらcontinue
//			if (GetMapChip(chip) == 0)continue;
//			test++;//通ってる
//			//もし部屋の中ならcontinue
//			if (chip.x >= room[0] && chip.x <= room[2])continue;
//			if (chip.y >= room[1] && chip.y <= room[3])continue;
//			wayPoint[roomId].emplace_back(chip);//増えない
//
//		}
//	}
//}

void GameManager::SetRoomWayPoint(t2k::Vector3 pos, int roomId)
{
	wayPoint[roomId].emplace_back(pos);
}

t2k::Vector3 GameManager::GetFarPoint(int roomId, t2k::Vector3 pos)
{
	//roomIdから出口の座標を取得する
	//一応出口が3個以上あるものとして書く(たぶん2個しか無いはず)
	std::vector<t2k::Vector3> outPoint;
	for (int i = 0; i < wayPoint[roomId].size(); ++i) {
		outPoint.emplace_back(wayPoint[roomId][i]);
	}

	std::vector<int> Distances;

	//outPointの各座標とposを比べて距離を出す
	for (auto point : outPoint) {
		int disX = point.x - pos.x;
		int disY = point.y - pos.y;
		int distance = disX * disX + disY * disY;//2乗の状態で比べる
		Distances.emplace_back(distance);
		//Distances[num].emplace_back(point);	
	}
	//Distancesの添字番号とoutPointの添字番号は一致するはず
	//最大の距離になるときのoutPointの番号がほしい
	int max = 0;//添字番号
	int buf = 0;//distanceの一時保管用
	for (int i = 0; i < Distances.size(); ++i) {
		if (buf == 0)buf = Distances[i];//比較用のdistanceがなければ自分を入れる
		//もしbufの中身よりi番のdistanceが大きければ
		if (buf < Distances[i]) {
			//配列番号を取得する
			max = i;
		}
	}
	//この時点でmaxに距離が一番遠いoutPointの添字番号が入っているはず
	//一番遠いoutPointを返す
	return outPoint[max];
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
//いずれexcelから階層だけのデータを読み込み、事前に全てのフロアを作成しておく
void GameManager::ReCreate()
{
	delete map;
	map = nullptr;
	delete player;
	player = nullptr;
	map = new Map(MAPWIDTH, MAPHEIGHT);
	map->DivideStart(MAPWIDTH, MAPHEIGHT, map);

	//階段のマップ座標の取得
	t2k::Vector3 stairsPos = SetStartPos(1);
	//階段設置
	map->SetChip(stairsPos.x, stairsPos.y, map->STAIRS);

	/*for (int i = 0; i < map->sumRoomNum; ++i) {
		CheckRoomWayPoint(i);
	}*/

	player = new Player(SetStartPos(0));
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
int GameManager::CheckIsThereFromWorld(t2k::Vector3 Pos)
{
	t2k::Vector3 PosMap = WorldToLocal(Pos);
	return map->CheckIsThere(PosMap.x, PosMap.y);
}
int GameManager::CheckIsThere(t2k::Vector3 Pos)
{
	return map->CheckIsThere(Pos.x, Pos.y);
}
//なんか微妙
void GameManager::CameraMove(Player* p)
{
	camera->cameraPos = player->pos - t2k::Vector3(512, 384, 0);
}

