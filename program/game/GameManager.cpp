#include "GameManager.h"
#include"SceneManager.h"
#include"DxLib.h"
#include<algorithm>
#include<memory>

#include"../support/Support.h"
#include<string>
#include"map.h"
#include<time.h>
#include"Actor/Actor.h"
#include"Player.h"
#include"Camera.h"
#include"ResourcesManager.h"
#include"Actor/Enemy.h"
#include"Actor/Actor.h"
#include"Item/ItemManager.h"
#include"Item/Item.h"
#include"Item/equipItem.h"
#include"Item//Inventory.h"

#include"Skill.h"
#include"SkillManager.h"

using namespace std;

//#include"Item.h"
#include "FadeControl.h"
#include "SoundManager.h"

//コンストラクタの引数が一つの場合は頭に explicitをつけること

GameManager::GameManager()
{

}

GameManager::~GameManager()
{
	if (player != nullptr)player->GetSkillList().clear();
	if (!liveEnemyList.empty()) liveEnemyList.clear();
	if (!liveEntityList.empty())liveEntityList.clear();

	delete resource;
	//delete camera;
	delete sManager;
	delete iManager;
	if (!inventories.empty()) {
		for (int i = 0; i < inventories.size(); ++i) {
			delete inventories[i];
		}
	}
	delete sound;
	delete fControl;
	InitGraph();

}
void GameManager::SetActorSkill(std::vector<Skill*>& SkillList, int SkillType, int SkillId)
{
	SkillList.emplace_back(sManager->GetSkill(SkillType, SkillId));
}
void GameManager::initGameManager()
{
	SRand(time(0));//dxlib randInit
	//srand(time(0));//C randInit
	// 
	//debug
	//SRand(1);
	//
	resource = new ResourceManager();
	resource->LoadResource();

	camera = new Camera();
	sManager = new SkillManager();

	//map情報利用のためダンジョン生成
	CreateDungeon(Dungeon::FOREST);

	//loadDivGraphのindex取得
	LoadMaxIndex();

	arrowButton = LoadGraphEx("graphics/arrowButtons.png");
	RButton = LoadGraphEx("graphics/button_R.png");
	EnterButton = LoadGraphEx("graphics/button_Enter.png");
	howToPlayBack = LoadGraphEx("graphics/howToPlayBack.png");

	iManager = new ItemManager();
	inventory = new Inventory(0);
	inventories.emplace_back(inventory);

	bankInventory = new Inventory(0);
	bankInventories.emplace_back(bankInventory);

	sound = new Sound();
	fControl = new FadeControl();

	SceneManager::ChangeScene(SceneManager::SCENE::TITLE);
	//shared_inventory = std::make_shared<Inventory>(0);
	//sharedInventories.emplace_back(shared_inventory);
	deitatime_ = 0;

	red = GetColor(255, 0, 0);
	blue = GetColor(0, 0, 255);
}

void GameManager::Update()
{
	//debug
	DebugKey();
	//
	SceneManager::Update();

}
void GameManager::Draw()
{
	SceneManager::Render();
}

void GameManager::AddItemToInventory(const int ItemId, std::vector<Inventory*>& Inventories,
	int& InventoryNum)
{
	//今のinventoryの持つアイテム配列がいっぱいなら
	if (Inventories[InventoryNum]->inventoryList.size() >= 10) {
		//if (sharedInventories[inventoryNum]->inventoryList.size() >= 10) {

			//新しくinventoryのインスタンスを生成する
		Inventory* newInventory = new Inventory(InventoryNum + 1);
		//inventory配列に登録
		Inventories.emplace_back(newInventory);

		//登録するinventoryを更新する
		InventoryNum++;
	}
	Item* item = iManager->GetItemData(ItemId);
	//装備アイテムだったら
	if (item->getItemData(1) >= 2) {
		equipItem* eItem = (equipItem*)item;
		//整数データの取得
		std::vector<int> intData = eItem->GetAllIntData();
		//文字列データの取得
		std::vector<std::string> stringData = item->GetAllStringData();

		//装備アイテムを生成 生成時にステータスをランダムに変更
		equipItem* newItem = new equipItem(intData[0], intData[1], stringData[0], intData[2], intData[3], intData[4],
			stringData[1], intData[5], intData[6], stringData[2], intData[7], intData[8], intData[9], intData[10], 0);
		//インベントリ追加
		Inventories[InventoryNum]->AddInventory(newItem);
	}
	else {
		std::vector<int> intData = item->GetAllIntData();
		std::vector<std::string> stringData = item->GetAllStringData();

		Item* newItem = new Item(intData[0], intData[1], stringData[0], intData[2], intData[3], intData[4], intData[5], stringData[1], stringData[2]);

		Inventories[InventoryNum]->AddInventory(newItem);

	}

}
//ショップからプレイヤーインベントリにアイテムを追加する関数
void GameManager::AddItemFromShop(Item* ShopItem)
{
	//今のinventoryの持つアイテム配列がいっぱいなら
	if (inventories[inventoryNum]->inventoryList.size() >= 10) {
		//if (sharedInventories[inventoryNum]->inventoryList.size() >= 10) {

			//新しくinventoryのインスタンスを生成する
		Inventory* newInventory = new Inventory(inventoryNum + 1);
		//inventory配列に登録
		inventories.emplace_back(newInventory);

		//登録するinventoryを更新する
		inventoryNum++;
	}
	//装備アイテムだったら
	if (ShopItem->getItemData(1) >= 2) {
		equipItem* eItem = static_cast<equipItem*>(ShopItem);
		//整数データの取得
		std::vector<int> intData = eItem->GetAllIntData();
		//文字列データの取得
		std::vector<std::string> stringData = ShopItem->GetAllStringData();

		//装備アイテムを生成 生成時にステータスをランダムに変更
		equipItem* newItem = new equipItem(intData[0], intData[1], stringData[0], intData[2], intData[3], intData[4],
			stringData[1], intData[5], intData[6], stringData[2], intData[7], intData[8], intData[9], intData[10], 1);
		addLog(newItem->getItemName()+"を入手した");
		//インベントリ追加
		inventories[inventoryNum]->AddInventory(newItem);
	}
	else {
		std::vector<int> intData = ShopItem->GetAllIntData();
		std::vector<std::string> stringData = ShopItem->GetAllStringData();

		Item* newItem = new Item(intData[0], intData[1], stringData[0], intData[2], intData[3], intData[4], intData[5], stringData[1], stringData[2]);

		addLog(newItem->getItemName() + "を入手した");
		//インベントリ追加
		inventories[inventoryNum]->AddInventory(newItem);

	}
}

void GameManager::PopItemFromInventory(const int NowInventoryId)
{
	//今の位置の配列番号を取得
	int selectNum = inventories[NowInventoryId]->GetCursorNum();
	//表示中のインベントリを取得
	auto itr = inventories[NowInventoryId]->inventoryList.begin();

	//選択されたアイテムまでイテレータ移動
	for (int i = 0; i < selectNum; ++i) {
		itr++;
	}
	//アイテムを消去
	delete((*itr));
	itr = inventories[NowInventoryId]->inventoryList.erase(itr);
	//カーソルの位置をひとつ上に移動
	inventories[NowInventoryId]->SetCursorNum(-1);

	//popするアイテムがいる場所=今いるインベントリが最後のインベントリではない場合
	if (NowInventoryId != inventoryNum) {
		int checkInventoryNum = NowInventoryId;
		while (1) {
			if (inventories[checkInventoryNum + 1]->inventoryList.empty())break;

			//if (sharedInventories[checkInventoryNum + 1]->inventoryList.empty())break;

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
	//最初のインベントリ内なら
	else {
		//インベントリ内のアイテム数を1減らす
		inventories[NowInventoryId]->SetItemNum(-1);
	}
	//空のインベントリを消す処理
	if (inventories[inventoryNum]->inventoryList.empty()) {
		if (inventoryNum != 0) {
			delete inventories[inventoryNum];
			inventories[inventoryNum] = nullptr;
			inventories.pop_back();
			inventoryNum--;
			isDeleteInventory = true;
		}
	}
	if (isDeleteInventory)return;
	//カーソルの位置を一番上にリセット
	if (inventories[NowInventoryId]->inventoryList.empty())inventories[NowInventoryId]->CursorReset();
}
//Xキーで画面を2倍に拡大縮小する関数
void GameManager::ScaleChange()
{
	//プレイヤーのローカル座標を取得する
	playerPosBuf = WorldToLocal(player->pos);
	//今の描画が通常倍率なら
	if (nowScale == ScaleMode::NORMAL) {
		//描画方法を拡大にする
		nowScale = ScaleMode::WIDE;
	}
	//今の描画が2倍なら
	else {
		//描画方法を通常にする
		nowScale = ScaleMode::NORMAL;
	}
	//描画チップのサイズを変更する
	nowGraphicSize = scale[static_cast<uint32_t>(nowScale)];
	//描画倍率を修正する
	graphEx = nowGraphicSize / GRAPHICSIZE;

}

void GameManager::DebugKey()
{
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_I)) {
		AddItemToInventory(2, inventories, inventoryNum);
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_O)) {
		AddItemToInventory(3, inventories, inventoryNum);
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_W)) {
		AddItemToInventory(11, inventories, inventoryNum);
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_G)) {
		player->TakeHpEffect(-100);
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_X)) {
		ScaleChange();
	}
}

void GameManager::CalcScale()
{
	//プレイヤーの描画座標を修正する
	player->pos = LocalToWorld(playerPosBuf.x, playerPosBuf.y);
	//カメラをプレイヤー中心に修正する
	CameraReset();
}

void GameManager::LoadMaxIndex()
{
	maxIndex = t2k::loadCsv("Csv/GraphicMaxIndex.csv");
}

Item* GameManager::GetItemData(const int ItemId)
{
	Item* item = iManager->GetItemData(ItemId);
	return item;
}

bool GameManager::OutOfRangeInItem(const int ItemId)
{
	if (ItemId > 0 && ItemId < iManager->itemSumNum + 1)return true;
	return false;
}
//ドロップアイテムリストから特定のアイテムを削除
bool GameManager::PopDetectItem(Item* item, std::list<Item*>& list) {
	bool erase = false;
	for (auto itr = list.begin(); itr != list.end();) {
		if ((*itr) == item) {
			itr = list.erase(itr);
			erase = true;
			break;
		}
		else {
			itr++;
		}
	}
	return erase;
}


void GameManager::RunDungeonBgm()
{
	if (nowDungeon == Dungeon::TOWER)sound->BGM_Play(sound->bgm_tower);
	else if (nowDungeon == Dungeon::FOREST)sound->BGM_Play(sound->bgm_forest);
}

void GameManager::StopBgm()
{
	if (nowDungeon == Dungeon::TOWER)StopSoundMem(sound->bgm_tower);
	else if (nowDungeon == Dungeon::FOREST)StopSoundMem(sound->bgm_forest);
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

void GameManager::LoadDivGraphEx(const std::string Gh, const int AllNum, const int WidthNum, const int HeightNum, int XSize, int YSize, std::vector<int>& GhVector)
{
	int* buf = new int[AllNum];
	LoadDivGraph(Gh.c_str(), AllNum, WidthNum, HeightNum, XSize, YSize, buf);


	for (int i = 0; i < AllNum; ++i) {
		GhVector.emplace_back(buf[i]);
	}
	delete[] buf;

}

//aからbまでの値からランダムに取得する
int GameManager::GetRandEx(int a, int b)
{
	if (a > b) {
		int diff = static_cast<int>(a - b);
		int random = static_cast<int>(GetRand(diff) + b);
		//int random = (diff) ? ( rand() % diff ) + b : b ;
		//
		return random;
	}
	else {
		int diff = static_cast<int>(b - a);
		int random = static_cast<int>(GetRand(diff) + a);
		//int random = (diff) ? ( rand() % diff ) + a : a ;
		return random;
	}
	return 0;
}

void GameManager::RunSystemSound()
{
}



void GameManager::setPlayerRoomNum(int roomNum)
{
	playerRoomNum = roomNum;
}

t2k::Vector3 GameManager::SetStartPos(setStartPosType num)
{
	//ランダムな部屋番号を取得
	int random = rand() % (map->GetRoomNum());
	//部屋番号から部屋を取得 0:左 1:上 2:右 3:下 帰ってくるのはマップ座標
	vector<int> room = map->GetRoom(random);
	//部屋の中のランダムなマップ座標を取得する
	int x = GetRandEx(room[0], room[2]);
	int y = GetRandEx(room[1], room[3]);
	//敵の生成だったら座標被りチェックを行う
	if (num == setStartPosType::ENEMY) {
		int player_x = player->pos.x;
		int player_y = player->pos.y;
		int debugBreak = 0;
		int count = 0;
		while (1) {
			if (player_x != x && player_y != y)break;
			x = GetRandEx(room[0], room[2]);
			y = GetRandEx(room[1], room[3]);
			count++;
			t2k::debugTrace("\n座標生成%d回目\n", count);
		}
	}

	//階段ならマップ座標を返す
	if (num == setStartPosType::STAIR)return t2k::Vector3(x, y, 0);

	//取得したマップ座標を描画座標に変換する
	t2k::Vector3 Pos = map->MapToWorld(x, y);

	return Pos;
}

void GameManager::DrawOpenWindow(float windowX, float windowY, int xCount, float yCount, int gh)
{
	// DrawRotaGraph3( 320, 240, 0, 0, 2.0f, 1.5f, PI / 2, GHandle, TRUE ) ;
	/*DrawRotaGraph3(
	int x, int y,
		int cx, int cy,
		double ExtRateX, double ExtRateY,
		double Angle, int GrHandle,
		int TransFlag, int TurnFlag );
		*/
		//タイマーを回し、一定周期で横に拡大する
		//特定の大きさになったら縦に拡大する
		//縦も特定の大きさになったら拡大をやめ、そのまま表示する
	if (openTimer += deitatime_ > xCount) {
		openTimer = 0;

	}




}

void GameManager::InitWayPointVector(int initroomNum)
{
	wayPoint.resize(initroomNum + 1);
}


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

	if (enemyPos.y > 0)ue = enemyPos + GetVecter(UP);
	if (enemyPos.x < MAPWIDTH)migi = enemyPos + GetVecter(RIGHT);
	if (enemyPos.y < MAPHEIGHT)shita = enemyPos + GetVecter(DOWN);
	if (enemyPos.x > 0)hidari = enemyPos + GetVecter(LEFT);

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
	return isNear;
}

t2k::Vector3 GameManager::GetRoomStartPos(int roomNum)
{
	return map->GetRoomStartPos(roomNum);
}



void GameManager::SortEntityList()
{
	liveEntityList.sort([](std::shared_ptr<Actor> actor1, std::shared_ptr<Actor> actor2) {
		return (actor1->pos.y < actor2->pos.y);
		});
}



bool GameManager::CheckNearByPlayerToAllEnemy(int range)
{
	bool isNear = false;
	for (auto enemy : liveEnemyList) {
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
	return isNear;
}

bool GameManager::CheckIsThereEnemyToDir(t2k::Vector3 Pos)
{
	bool isThere = false;
	for (auto enemy : liveEnemyList) {
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
	for (auto enemy : liveEnemyList) {
		t2k::Vector3 enemyPos = WorldToLocal(enemy->pos);
		if (enemyPos.x == Pos.x && enemyPos.y == Pos.y) {
			thereEnemy = enemy;
			break;
		}
	}
	return thereEnemy;
}


void GameManager::PlayerDead()
{
	//プレイヤーのステータス関係処理
	player->DeadPlayer();
	//ログ消去
	ResetLog();

	//インベントリのアイテムを全て消去する
	for (auto inventory : inventories) {
		inventory->inventoryList.clear();
	}
	//インベントリを削除
	for (int i = inventoryNum; i >= 0; --i) {
		delete inventories[i];
		inventories[i] = nullptr;
		inventories.pop_back();
	}
	//インベントリの個数をリセット
	inventoryNum = 0;
	//インベントリを一つだけ生成
	inventory = new Inventory(0);
	inventories.emplace_back(inventory);
	//キャンプシーンスポーン位置
	player->pos = SpawnPlayerCamp();
}

void GameManager::SetDoneBuy()
{
	map->SetDoneBuy();
}

bool GameManager::GetDoneBuy()
{
	return map->GetDoneBuy();
}

void GameManager::Zoom()
{
	if (t2k::Input::isKeyDown(t2k::Input::KEYBORD_Z)) {
		nowGraphicSize -= 0.01;
	}
	else if (t2k::Input::isKeyDown(t2k::Input::KEYBORD_X)) {
		nowGraphicSize += 0.01;
	}
	graphEx = nowGraphicSize / GRAPHICSIZE;
}
void GameManager::DealDamageToTarget(Actor* actor, t2k::Vector3 Pos)
{
	//プレイヤーが攻撃するなら
	if (actor->GetActId() == 0) {
		//全てのエネミーから前にいるエネミーを取得
		std::shared_ptr<Enemy>frontEnemy = GetIsThereEnemyToDir(Pos);
		//もし空振りなら
		if (frontEnemy == nullptr) {
			//player->skip = true;
			return;
		}
		//攻撃力と防御力を取得,ダメージ計算
		float damage = CalcDamage(player->GetStatus(1), frontEnemy->GetStatus(2));
		//ダメージを反映
		RunDamageEvent((-1) * damage, frontEnemy);
		//ログ追加
		addLog(frontEnemy->GetName() + "が" + std::to_string(static_cast<int>(damage)) + "ダメージを受けた");
	}
	//エネミーが攻撃するなら
	else {
		//攻撃力と防御力を取得,ダメージ計算
		float damage = CalcDamage(actor->GetStatus(1), player->GetStatus(2));
		//ダメージを反映
		RunDamageEvent((-1) * damage, player);
		addLog("Playerが" + std::to_string(static_cast<int>(damage)) + "ダメージを受けた");
	}
}

void GameManager::DealSkillDamageToTarget(Actor* Actor, const t2k::Vector3 Pos, Skill* ActivateSkill)
{
	//スキルの数値データを取得
	const float* skillData = ActivateSkill->GetSkillAmount();
	//スキル名を取得
	std::string skillName = ActivateSkill->GetSkillName();

	//プレイヤーが攻撃するなら
	if (Actor->GetActId() == 0) {
		//全てのエネミーから前にいるエネミーを取得
		std::shared_ptr<Enemy>frontEnemy = GetIsThereEnemyToDir(Pos);
		//もし空振りなら
		if (frontEnemy == nullptr) {
			//player->skip = true;
			return;
		}

		//スキルの倍率,攻撃力と防御力を取得,ダメージ計算
		float damage = CalcSkillDamage(skillData[0], player->GetStatus(1), frontEnemy->GetStatus(2));
		//ダメージを反映
		RunDamageEvent((-1) * damage, frontEnemy);
		//ログ追加
		addLog(frontEnemy->GetName() + "が" + skillName + "によって" + std::to_string(static_cast<int>(damage)) + "ダメージを受けた");
	}
	//エネミーが攻撃するなら
	else {
		//攻撃力と防御力を取得,ダメージ計算
		float damage = CalcSkillDamage(skillData[0], Actor->GetStatus(1), player->GetStatus(2));
		//ダメージを反映
		RunDamageEvent((-1) * damage, player);
		addLog("Playerが" + skillName + "によって" + std::to_string(static_cast<int>(damage)) + "ダメージを受けた");
	}
}

std::shared_ptr<Player> GameManager::GetPlayer()
{
	return player;
}
t2k::Vector3 GameManager::GetPlayerLocalPos()
{
	return player->GetPlayerLocalPos();
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
	float damage = Attack / std::pow(2, (Defence / 10)) + rand;
	//最低保証ダメージ
	if (damage < 0)damage = 1;

	return damage;
}

float GameManager::CalcSkillDamage(const float SkillRate, const int Attack, const int Defence)
{
	//スキルのダメージ倍率を使用者のAttackに掛けた数値でダメージ計算を行う
	float skillDamage = Attack * SkillRate;

	return CalcDamage(skillDamage, Defence);
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

	wayPoint.clear();

	CreateDungeon(nowDungeon);


	//player->pos = SetStartPos(setStartPosType::PLAYER);
	map->player = player;

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

void GameManager::MapDraw()
{

	map->MapDraw();

	/*if (!minimapDraw)return;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	map->MiniMapDraw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);*/
}
void GameManager::MiniMapDraw()
{
	if (!minimapDraw)return;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	map->MiniMapDraw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}
void GameManager::UpdateMiniMap(t2k::Vector3 localPos)
{
	map->ChangeRoomVisit(localPos);
	map->ChangeWayVisit(localPos);
}
bool GameManager::CheckCanDraw(t2k::Vector3 localPos)
{
	return map->CheckCanDraw(localPos);
}
void GameManager::CreateDungeon(Dungeon dungeonName) {

	if (map != nullptr)delete map;
	ResetLog();
	wayPoint.clear();

	nowDungeon = dungeonName;
	t2k::debugTrace("\nダンジョン作成前デバッグ\n");

	CreateMap(dungeonName);
	t2k::debugTrace("\nダンジョン作成後デバッグ\n");

	//マップ自動生成
	map->DivideStart(MAPWIDTH, MAPHEIGHT, map);
	t2k::debugTrace("\n分割終了デバッグ\n");

	//階段のマップ座標の取得
	t2k::Vector3 stairsPos = SetStartPos(setStartPosType::STAIR);
	t2k::debugTrace("\n階段座標指定デバッグ\n");

	//階段設置
	map->SetChip(stairsPos.x, stairsPos.y, map->STAIRS);

	//デバッグ用階段座標登録
	map->AddStairList(stairsPos);

	//ショップをランダムで設置(デフォルト確率は20%)
	map->SetShop();
	t2k::debugTrace("\nショップ設置デバッグ\n");

	if (player == nullptr)return;
	//ダンジョン内のランダムな位置にプレイヤーを移動
	player->pos = SetStartPos(setStartPosType::PLAYER);
	t2k::debugTrace("\nプレイヤー設置デバッグ\n");

	map->player = player;
	map->ChangeRoomVisit(WorldToLocal(player->pos));
	t2k::debugTrace("\n位置情報登録デバッグ\n");

	CameraReset();

}
void GameManager::CreateMap(Dungeon dungeonName)
{
	std::vector<int> handles = GetGraphicHandles(dungeonName);
	map = new Map(MAPWIDTH, MAPHEIGHT, handles);
}

int GameManager::GetDifStatus(int subId, int equipType, int amount)
{
	int dif = amount - player->GetStatusFromEquipment(subId, equipType);
	return dif;
}

bool GameManager::CheckEquipItem(int subId)
{
	return player->CheckEquip(subId);
}

int GameManager::GetRandItemData(int ItemType)
{
	return iManager->GetRamdomTypeItemId(ItemType);
}

int GameManager::GetItemId(Item* Item)
{
	return Item->getItemData(0);
}

void GameManager::MakePlayer(SpawnScene nowScene)
{
	if (player != nullptr)return;
	if (nowScene == SpawnScene::Camp) {
		player = std::make_shared<Player>(SpawnPlayerCamp(), 100.0f, 90, 30, 20, 0);
	}
	else if (nowScene == SpawnScene::Dungeon) {
		player = std::make_shared<Player>(SetStartPos(setStartPosType::PLAYER), 100.0f, 30, 30, 30, 0);
		map->player = player;
	}
	SetActorSkill(player->GetSkillList(), 0, 0);
	SetActorSkill(player->GetSkillList(), 0, 1);
	SetActorSkill(player->GetSkillList(), 0, 2);
	liveEntityList.emplace_back(player);
}

void GameManager::SetCampStartPlayerPos()
{
	const t2k::Vector3 spawnPos = SpawnPlayerCamp();
	player->pos.x = spawnPos.x;
	player->pos.y = spawnPos.y;
}

void GameManager::SetSkill(std::vector<Skill*>SkillList, int SkillId)
{
	SkillList.emplace_back();
}

std::vector<int>& GameManager::GetGraphicHandles(Dungeon dungeonName)
{
	//ダンジョンネームに対応する画像ハンドルの2次元配列から画像ハンドル配列を取得する
	return resource->dungeonMapChipGh[static_cast<uint32_t>(dungeonName)];
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
//カメラをキャラを中心にリセット
void GameManager::CameraReset() {
	camera->cameraPos = player->pos - WINDOWCENTER;
}
//なんか微妙
void GameManager::CameraMove(int width, int height, int dir)
{
	if (player->pos.x > 1024 / 2 && player->pos.x < width - 1024 / 2) {
		camera->cameraPos.x = player->pos.x - (1024 / 2);
	}
	if (player->pos.y > 768 / 2 && player->pos.y < height - 768 / 2) {
		camera->cameraPos.y = player->pos.y - (768 / 2);
	}
}

bool GameManager::CheckMousePointToRect(int MouseX, int MouseY, int RectLeftTopX, int RectWidth, int RectTopY, int RectHeight)
{
	if (MouseX >= RectLeftTopX && MouseX <= RectLeftTopX + RectWidth
		&& MouseY >= RectTopY && MouseY <= RectTopY + RectHeight) {
		return true;
	}
	return false;
}
//0:上, 1 : 右, 2 : 下, 3 : 左
int GameManager::GetPlayerVec(std::shared_ptr<Enemy> enemy)
{
	int playerVec = -1;

	t2k::Vector3 enemyPos = WorldToLocal(enemy->pos);
	t2k::Vector3 hidari = { 0,0,0 };
	t2k::Vector3 ue = { 0,0,0 };
	t2k::Vector3 migi = { 0,0,0 };
	t2k::Vector3 shita = { 0,0,0 };

	if (enemyPos.x > 0)hidari = enemyPos + t2k::Vector3(-1, 0, 0);
	if (enemyPos.y > 0)ue = enemyPos + t2k::Vector3(0, -1, 0);
	if (enemyPos.x < MAPWIDTH)migi = enemyPos + t2k::Vector3(1, 0, 0);
	if (enemyPos.y < MAPHEIGHT)shita = enemyPos + t2k::Vector3(0, 1, 0);

	t2k::Vector3 enemyPosNear[4] = { ue,migi,shita,hidari };

	t2k::Vector3 playerPos = WorldToLocal(player->pos);

	for (int i = 0; i < 4; ++i) {
		if (playerPos.x == enemyPosNear[i].x && playerPos.y == enemyPosNear[i].y) {
			playerVec = i;
			break;
		}
	}
	return playerVec;
}

//ログを生成する関数,古い方から消える
void GameManager::addLog(const std::string log)
{
	if (!Log[6].empty()) {
		Log[0] = Log[1];
		Log[1] = Log[2];
		Log[2] = Log[3];
		Log[3] = Log[4];
		Log[4] = Log[5];
		Log[5] = Log[6];
		Log[6] = log;
		//Log[7] = Log[8];
		//Log[8] = log;
		return;
	}
	for (int i = 0; i < 7; i++) {

		if (Log[i].empty()) {

			Log[i] = log;
			return;
		}
	}

}


//生成したログを表示する関数
void GameManager::LogDraw(int x, int y)
{
	for (int i = 0; i < 7; ++i) {
		DrawStringEx(x + 20, y + 20 + (i * 20), -1, "%s", Log[i].c_str());
	}
}


void GameManager::DrawHowTo()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	//ボタン説明背景
	DrawRotaGraph(870, 250, 1, 0, howToPlayBack, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	//ボタン説明画像
	DrawRotaGraph(930, 150, 0.1, 0, arrowButton, true);
	DrawRotaGraph(930, 210, 1, 0, RButton, true);
	DrawRotaGraph(930, 255, 1, 0, EnterButton, true);

	//説明文
	DrawStringEx(780, 150, -1, "移動ボタン");
	DrawStringEx(780, 200, -1, "攻撃ボタン");
	DrawStringEx(780, 250, -1, "決定ボタン");
}