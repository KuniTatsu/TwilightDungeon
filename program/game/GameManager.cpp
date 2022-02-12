#include "GameManager.h"
#include"SceneManager.h"
#include"DxLib.h"
#include<algorithm>
#include<memory>

#include"../support/Support.h"
#include<string>
#include"map.h"
#include<time.h>
#include"Player.h"
#include"Camera.h"
#include"Actor/Enemy.h"
#include"Actor/Actor.h"
#include"Item/ItemManager.h"
#include"Item/HaveItem.h"
#include"Item/Item.h"
#include"Item/equipItem.h"
#include"Item//Inventory.h"

//#include"game_main.h"

//#include"Item.h"
//#include "FadeControl.h"
#include "SoundManager.h"

//コンストラクタの引数が一つの場合は頭に explicitをつけること

GameManager::GameManager()
{

}

GameManager::~GameManager()
{

}

void GameManager::AddItemToInventory(int itemId)
{
	//今のinventoryの持つアイテム配列がいっぱいなら
	if (inventories[inventoryNum]->inventoryList.size() >= 10) {
		//新しくinventoryのインスタンスを生成する
		Inventory* newInventory = new Inventory();
		//inventory配列に登録
		inventories.emplace_back(newInventory);
		//登録するinventoryを更新する
		inventoryNum++;
	}
	Item* item = iManager->getItemData(itemId);
	//装備アイテムだったら
	if (item->getItemData(1) >= 2) {
		equipItem* eItem = (equipItem*)item;
		std::vector<int> intData = eItem->GetAllIntData();
		std::vector<std::string> stringData = item->GetAllStringData();
		//int Id, int ItemType, std::string ItemName, int Saturation, int Heal, int HitDamage, std::string Gh, int SubId, std::string Desc, int Hp, int Attack, int Defence, int Speed
		equipItem* newItem = new equipItem(intData[0], intData[1], stringData[0], intData[2], intData[3], intData[4],
			stringData[1], intData[5], stringData[2], intData[6], intData[7], intData[8], intData[9]);
		inventories[inventoryNum]->AddInventory(newItem);
	}
	else {
		std::vector<int> intData = item->GetAllIntData();
		std::vector<std::string> stringData = item->GetAllStringData();

		Item* newItem = new Item(intData[0], intData[1], stringData[0], intData[2], intData[3], intData[4], stringData[1], stringData[2]);
		//std::shared_ptr<Item>nItem= std::make_shared<Item>(intData[0], intData[1], stringData[0], intData[2], intData[3], intData[4], stringData[1], stringData[2]);

		inventories[inventoryNum]->AddInventory(newItem);
	}

#if 0
	//今のinventoryの持つアイテム配列がいっぱいなら
	if (inventories[inventoryNum]->inventory[9] != nullptr) {
		//新しくinventoryのインスタンスを生成する
		Inventory* newInventory = new Inventory();
		//inventory配列に登録
		inventories.emplace_back(newInventory);
		//登録するinventoryを更新する
		inventoryNum++;
	}
	//debug itemId:2のアイテムをインベントリに追加
	inventories[inventoryNum]->AddInventory(iManager->getItemData(itemId));
#endif
}

void GameManager::PopItemFromInventory(int NowInventoryId)
{
	int selectNum = inventories[NowInventoryId]->GetCursorNum();

	auto itr = inventories[NowInventoryId]->inventoryList.begin();
	for (int i = 0; i < selectNum; ++i) {
		itr++;
	}
	delete((*itr));
	itr = inventories[NowInventoryId]->inventoryList.erase(itr);
	//popするアイテムがいる場所=今いるインベントリが最後のインベントリではない場合
	if (NowInventoryId != inventoryNum) {
		int checkInventoryNum = NowInventoryId;
		while (1) {
			//次のページの最初のアイテムをコピーして消したアイテムのリストの末尾に加える
			auto item = inventories[checkInventoryNum + 1]->inventoryList.begin();
			//アイテム追加
			inventories[checkInventoryNum]->inventoryList.emplace_back((*item));
			//次のページの最初のアイテムをpopする
			inventories[checkInventoryNum + 1]->inventoryList.pop_front();
			//最後のインベントリページにたどり着いたらbreak
			if (checkInventoryNum + 1 == inventoryNum)break;
			checkInventoryNum++;
		}
	}
}

Item* GameManager::GetItemData(int ItemId)
{
	Item* hoge = iManager->getItemData(ItemId);
	return hoge;
}

bool GameManager::OutOfRangeInItem(int ItemId)
{
	if (ItemId > 0 && ItemId < iManager->itemSumNum + 1)return true;
	return false;
}
bool GameManager::PopDetectItem(Item* item, std::list<Item*>& list) {
	bool hoge = false;
	for (auto itr = list.begin(); itr != list.end();) {
		if ((*itr) == item) {
			itr = list.erase(itr);
			hoge = true;
			break;
		}
		else {
			itr++;
		}
	}
	return hoge;
}
void GameManager::Update()
{
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_I)) {
		AddItemToInventory(2);
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_O)) {
		AddItemToInventory(3);
	}
	SceneManager::Update();

	/*if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_F4) && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_F5)) {
		t2k::debugTrace("同時押し成功");
	}*/
}
void GameManager::Draw()
{
	for (int i = 0; i < map->GetRoomNum() + 1; ++i) {
		for (int k = 0; k < wayPoint[i].size(); ++k) {

			DrawStringEx(100 + 100 * k, 400 + 80 * i, -1, "部屋番号:%d", i);
			DrawStringEx(100 + 100 * k, 420 + 80 * i, -1, "x:%.1f", wayPoint[i][k].x);
			DrawStringEx(100 + 100 * k, 440 + 80 * i, -1, "y:%.1f", wayPoint[i][k].y);
		}
	}

	SceneManager::Render();

	//debug

	if (haveItemList.empty())return;

	int k = 0;
	for (auto id : haveItemList) {
		Item* haveItem = iManager->getItemData(id);
		DrawStringEx(500 + 100 * k, 100, -1, "ItemId:%d", haveItem->getItemData(0));
		DrawStringEx(500 + 100 * k, 120, -1, "ItemName:%s", haveItem->getItemName().c_str());
		++k;
	}
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
	//playerのactidは0
	player = std::make_shared<Player>(SetStartPos(0), 100, 10, 10, 10, 0);
	camera->cameraPos = player->pos - t2k::Vector3(512, 384, 0);
	iManager = new ItemManager();

	haveItem = new HaveItem();
	inventory = new Inventory();
	inventories.emplace_back(inventory);

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



void GameManager::setPlayerRoomNum(int roomNum)
{
	playerRoomNum = roomNum;
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

	//階段ならマップ座標を返す
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

void GameManager::SetChip(int Left, int Up, int Right, int Down)
{
	map->SetAllChip(Left, Up, Right, Down);
}

t2k::Vector3 GameManager::GetRoomValue(int roomNum)
{
	return map->GetRoomValue(roomNum);
}

bool GameManager::CheckNearByPlayer(std::shared_ptr<Enemy>enemy)
{
	//enemyPosはマップ座標
	t2k::Vector3 enemyPos = WorldToLocal(enemy->pos);
	t2k::Vector3 hidari;
	t2k::Vector3 ue;
	t2k::Vector3 migi;
	t2k::Vector3 shita;

	if (enemyPos.x > 0)hidari = enemyPos + t2k::Vector3(-1, 0, 0);
	if (enemyPos.y > 0)ue = enemyPos + t2k::Vector3(0, -1, 0);
	if (enemyPos.x < MAPWIDTH)migi = enemyPos + t2k::Vector3(1, 0, 0);
	if (enemyPos.y < MAPHEIGHT)shita = enemyPos + t2k::Vector3(0, 1, 0);

	t2k::Vector3 enemyPosNear[4] = { hidari,ue,migi,shita };

	t2k::Vector3 playerPos = WorldToLocal(player->pos);

	bool isNear = false;
	for (int i = 0; i < 4; ++i) {
		//if (playerPos == enemyPosNear[i])
		if (playerPos.x == enemyPosNear[i].x && playerPos.y == enemyPosNear[i].y) {
			isNear = true;
			break;
		}
	}
	if (isNear)return true;
	return false;
}

t2k::Vector3 GameManager::GetRoomStartPos(int roomNum)
{
	return map->GetRoomStartPos(roomNum);
}


bool GameManager::CheckNearByPlayerToAllEnemy(int range)
{
	bool isNear = false;
	for (auto enemy : hoge) {
		//enemyPosはマップ座標
		t2k::Vector3 enemyPos = WorldToLocal(enemy->pos);
		t2k::Vector3 hidari;
		t2k::Vector3 ue;
		t2k::Vector3 migi;
		t2k::Vector3 shita;
		for (int m = 0; m < range; ++m) {
			if (enemyPos.x > 0)hidari = enemyPos + t2k::Vector3(-1 + (-1 * m), 0, 0);
			if (enemyPos.y > 0)ue = enemyPos + t2k::Vector3(0, -1 + (-1 * m), 0);
			if (enemyPos.x < MAPWIDTH)migi = enemyPos + t2k::Vector3(1 + 1 * m, 0, 0);
			if (enemyPos.y < MAPHEIGHT)shita = enemyPos + t2k::Vector3(0, 1 + 1 * m, 0);

			t2k::Vector3 enemyPosNear[4] = { hidari,ue,migi,shita };

			t2k::Vector3 playerPos = WorldToLocal(player->pos);

			for (int i = 0; i < 4; ++i) {
				//if (playerPos == enemyPosNear[i])
				//int xDis = abs(playerPos.x - enemyPosNear[i].x);
				//int yDis = abs(playerPos.y - enemyPosNear[i].y);

				////xの差の絶対値がrange以下またはyの差の絶対値がrange以下だったら
				//if (xDis < 3 || yDis < 3) {
				//	isNear = true;
				//	break;
				//}

				if (playerPos.x == enemyPosNear[i].x && playerPos.y == enemyPosNear[i].y) {
					isNear = true;
					break;
				}
			}
		}
	}
	//range範囲内にいたらtrueを返す
	if (isNear)return true;
	return false;
}

bool GameManager::CheckIsThereEnemyToDir(t2k::Vector3 Pos)
{
	bool isThere = false;
	for (auto enemy : hoge) {
		t2k::Vector3 enemyPos = WorldToLocal(enemy->pos);
		if (enemyPos.x == Pos.x && enemyPos.y == Pos.y) {
			isThere = true;
			break;
		}
	}
	return isThere;
}
std::shared_ptr<Enemy> GameManager::GetIsThereEnemyToDir(t2k::Vector3 Pos)
{
	std::shared_ptr<Enemy>thereEnemy = nullptr;
	for (auto enemy : hoge) {
		t2k::Vector3 enemyPos = WorldToLocal(enemy->pos);
		if (enemyPos.x == Pos.x && enemyPos.y == Pos.y) {
			thereEnemy = enemy;
			break;
		}
	}
	return thereEnemy;
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
void GameManager::TakeDamageToTarget(Actor* hoge, t2k::Vector3 Pos)
{
	//プレイヤーが攻撃するなら
	if (hoge->GetActId() == 0) {
		//全てのエネミーから前にいるエネミーを取得
		std::shared_ptr<Enemy>hoge = GetIsThereEnemyToDir(Pos);

		float damage = CalcDamage(player->GetStatus(1), hoge->GetStatus(2));
		RunDamageEvent(damage, hoge);
	}
	//エネミーが攻撃するなら
	else {
		float damage = (-1) * CalcDamage(hoge->GetStatus(1), player->GetStatus(2));
		RunDamageEvent(damage, player);
	}
}

std::shared_ptr<Player> GameManager::GetPlayer()
{
	return player;
}
int GameManager::GetItemNum()
{
	return itemNum;
}
float GameManager::CalcDamage(int Attack, int Defence)
{
	//ダメージ=攻撃側の攻撃力 ÷ 2^(攻撃を受ける側の防御力/10)
	//std::pow(2,(enemyDef/10))

	float rand = (float)GetRandEx(-5, 5);
	float damage = Attack * std::pow(2, (Defence / 10)) + rand;

	return damage;
}

void GameManager::RunDamageEvent(float damage, std::shared_ptr<Actor>actor)
{
	actor->TakeHpEffect(damage);
}
void GameManager::SetItemNum(int num)
{
	itemNum = num;
}
//いずれexcelから階層だけのデータを読み込み、事前に全てのフロアを作成しておく
void GameManager::ReCreate()
{
	delete map;
	map = nullptr;
	//playerをnewするのはやめる
	/*
	delete player;
	player = nullptr;
	*/
	wayPoint.clear();

	map = new Map(MAPWIDTH, MAPHEIGHT);
	map->DivideStart(MAPWIDTH, MAPHEIGHT, map);

	//階段のマップ座標の取得
	t2k::Vector3 stairsPos = SetStartPos(1);
	//階段設置
	map->SetChip(stairsPos.x, stairsPos.y, map->STAIRS);

	/*for (int i = 0; i < map->sumRoomNum; ++i) {
		CheckRoomWayPoint(i);
	}*/
	player->pos = SetStartPos(0);
	//player = new Player(SetStartPos(0), 100, 10, 10, 10);
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
void GameManager::CameraMove()
{
	camera->cameraPos = player->pos - t2k::Vector3(512, 384, 0);
}

bool GameManager::CheckMousePointToRect(int MouseX, int MouseY, int RectLeftTopX, int RectWidth, int RectTopY, int RectHeight)
{
	if (MouseX >= RectLeftTopX && MouseX <= RectLeftTopX + RectWidth
		&& MouseY >= RectTopY && MouseY <= RectTopY + RectHeight) {
		return true;
	}
	return false;
}