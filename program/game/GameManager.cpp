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
#include"ResourcesManager.h"
#include"Actor/Enemy.h"
#include"Actor/Actor.h"
#include"Item/ItemManager.h"
#include"Item/HaveItem.h"
#include"Item/Item.h"
#include"Item/equipItem.h"
#include"Item//Inventory.h"

using namespace std;

//#include"Item.h"
#include "FadeControl.h"
#include "SoundManager.h"

//�R���X�g���N�^�̈�������̏ꍇ�͓��� explicit�����邱��

GameManager::GameManager()
{

}

GameManager::~GameManager()
{

}

void GameManager::AddItemToInventory(int itemId)
{
	//����inventory�̎��A�C�e���z�񂪂����ς��Ȃ�
	if (inventories[inventoryNum]->inventoryList.size() >= 10) {
		//if (sharedInventories[inventoryNum]->inventoryList.size() >= 10) {

			//�V����inventory�̃C���X�^���X�𐶐�����
		Inventory* newInventory = new Inventory(inventoryNum + 1);
		//inventory�z��ɓo�^
		inventories.emplace_back(newInventory);

		/*
		std::shared_ptr<Inventory> newShared_inventory = std::make_shared<Inventory>(inventoryNum+1);
		sharedInventories.emplace_back(newShared_inventory);
		*/

		//�o�^����inventory���X�V����
		inventoryNum++;
	}
	Item* item = iManager->getItemData(itemId);
	//�����A�C�e����������
	if (item->getItemData(1) >= 2) {
		equipItem* eItem = (equipItem*)item;
		std::vector<int> intData = eItem->GetAllIntData();
		std::vector<std::string> stringData = item->GetAllStringData();
		//int Id, int ItemType, std::string ItemName, int Saturation, int Heal, int HitDamage, std::string Gh, int SubId, std::string Desc, int Hp, int Attack, int Defence, int Speed
		equipItem* newItem = new equipItem(intData[0], intData[1], stringData[0], intData[2], intData[3], intData[4],
			stringData[1], intData[5], stringData[2], intData[6], intData[7], intData[8], intData[9]);
		inventories[inventoryNum]->AddInventory(newItem);

		//sharedInventories[inventoryNum]->AddInventory(newItem);
	}
	else {
		std::vector<int> intData = item->GetAllIntData();
		std::vector<std::string> stringData = item->GetAllStringData();

		Item* newItem = new Item(intData[0], intData[1], stringData[0], intData[2], intData[3], intData[4], stringData[1], stringData[2]);
		//std::shared_ptr<Item>nItem= std::make_shared<Item>(intData[0], intData[1], stringData[0], intData[2], intData[3], intData[4], stringData[1], stringData[2]);

		inventories[inventoryNum]->AddInventory(newItem);

		//sharedInventories[inventoryNum]->AddInventory(newItem);

	}

}

void GameManager::PopItemFromInventory(int NowInventoryId)
{
	int selectNum = inventories[NowInventoryId]->GetCursorNum();

	//int selectNum = sharedInventories[NowInventoryId]->GetCursorNum();

	auto itr = inventories[NowInventoryId]->inventoryList.begin();

	//auto itr = sharedInventories[NowInventoryId]->inventoryList.begin();

	for (int i = 0; i < selectNum; ++i) {
		itr++;
	}
	delete((*itr));
	itr = inventories[NowInventoryId]->inventoryList.erase(itr);

	//itr = sharedInventories[NowInventoryId]->inventoryList.erase(itr);

	inventories[NowInventoryId]->SetCursorNum(-1);
	inventories[NowInventoryId]->SetItemNum(-1);

	//sharedInventories[NowInventoryId]->SetCursorNum(-1);

//pop����A�C�e��������ꏊ=������C���x���g�����Ō�̃C���x���g���ł͂Ȃ��ꍇ
	if (NowInventoryId != inventoryNum) {
		int checkInventoryNum = NowInventoryId;
		while (1) {
			if (inventories[checkInventoryNum + 1]->inventoryList.empty())break;

			//if (sharedInventories[checkInventoryNum + 1]->inventoryList.empty())break;

		//���̃y�[�W�̍ŏ��̃A�C�e�����R�s�[���ď������A�C�e���̃��X�g�̖����ɉ�����
			auto item = inventories[checkInventoryNum + 1]->inventoryList.begin();

			//auto item = sharedInventories[checkInventoryNum + 1]->inventoryList.begin();

		//�A�C�e���ǉ�
			inventories[checkInventoryNum]->inventoryList.emplace_back((*item));

			//sharedInventories[checkInventoryNum]->inventoryList.emplace_back((*item));

		//���̃y�[�W�̍ŏ��̃A�C�e����pop����
			inventories[checkInventoryNum + 1]->inventoryList.pop_front();

			//sharedInventories[checkInventoryNum + 1]->inventoryList.pop_front();

		//�Ō�̃C���x���g���y�[�W�ɂ��ǂ蒅������break
			if (checkInventoryNum + 1 == inventoryNum)break;
			checkInventoryNum++;
		}
	}
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
	if (inventories[NowInventoryId]->inventoryList.empty())inventories[NowInventoryId]->CursorReset();
	//if (sharedInventories[NowInventoryId]->inventoryList.empty())sharedInventories[NowInventoryId]->CursorReset();
}

void GameManager::LoadMaxIndex()
{
	maxIndex = t2k::loadCsv("Csv/GraphicMaxIndex.csv");
}

Item* GameManager::GetItemData(int ItemId)
{
	Item* item = iManager->getItemData(ItemId);
	return item;
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
		t2k::debugTrace("������������");
	}*/
}
void GameManager::Draw()
{
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
	//SRand(time(0));//dxlib randInit
	//srand(time(0));//C randInit
	// 
	//debug
	//SRand(1);
	resource = new ResourceManager();
	resource->LoadResource();

	camera = new Camera();

	CreateDungeon(Dungeon::FOREST);

	//loadDivGraph��index�擾
	LoadMaxIndex();

	iManager = new ItemManager();

	haveItem = new HaveItem();
	inventory = new Inventory(0);
	inventories.emplace_back(inventory);

	SceneManager::ChangeScene(SceneManager::SCENE::CAMP);
	camera->cameraPos = player->pos - WINDOWCENTER;


	//shared_inventory = std::make_shared<Inventory>(0);
	//sharedInventories.emplace_back(shared_inventory);


	sound = new Sound();
	fControl = new FadeControl();


	deitatime_ = 0;

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
//a����b�܂ł̒l���烉���_���Ɏ擾����
int GameManager::GetRandEx(int a, int b)
{
	if (a > b) {
		int hoge = a - b;
		int random = GetRand(hoge) + b;
		//int random = (hoge) ? ( rand() % hoge ) + b : b ;
		//
		return random;
	}
	else {
		int hoge = b - a;
		int random = GetRand(hoge) + a;
		//int random = (hoge) ? ( rand() % hoge ) + a : a ;
		return random;
	}
	return 0;
}



void GameManager::setPlayerRoomNum(int roomNum)
{
	playerRoomNum = roomNum;
}

t2k::Vector3 GameManager::SetStartPos(setStartPosType num)
{
	//�����_���ȕ����ԍ����擾
	int random = rand() % (map->GetRoomNum());
	//�����ԍ����畔�����擾 0:�� 1:�� 2:�E 3:�� �A���Ă���̂̓}�b�v���W
	vector<int> room = map->GetRoom(random);
	//�����̒��̃����_���ȃ}�b�v���W���擾����
	int x = GetRandEx(room[0], room[2]);
	int y = GetRandEx(room[1], room[3]);
	//�G�̐�������������W���`�F�b�N���s��
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
			t2k::debugTrace("\n���W����%d���\n", count);
		}
	}

	//�K�i�Ȃ�}�b�v���W��Ԃ�
	if (num == setStartPosType::STAIR)return t2k::Vector3(x, y, 0);

	//�擾�����}�b�v���W��`����W�ɕϊ�����
	t2k::Vector3 Pos = map->MapToWorld(x, y);

	return Pos;
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
	//roomId����o���̍��W���擾����
	//�ꉞ�o����3�ȏ゠����̂Ƃ��ď���(���Ԃ�2���������͂�)
	std::vector<t2k::Vector3> outPoint;
	for (int i = 0; i < wayPoint[roomId].size(); ++i) {
		outPoint.emplace_back(wayPoint[roomId][i]);
	}

	std::vector<int> Distances;

	//outPoint�̊e���W��pos���ׂċ������o��
	for (auto point : outPoint) {
		int disX = point.x - pos.x;
		int disY = point.y - pos.y;
		int distance = disX * disX + disY * disY;//2��̏�ԂŔ�ׂ�
		Distances.emplace_back(distance);
		//Distances[num].emplace_back(point);	
	}
	//Distances�̓Y���ԍ���outPoint�̓Y���ԍ��͈�v����͂�
	//�ő�̋����ɂȂ�Ƃ���outPoint�̔ԍ����ق���
	int max = 0;//�Y���ԍ�
	int buf = 0;//distance�̈ꎞ�ۊǗp
	for (int i = 0; i < Distances.size(); ++i) {
		if (buf == 0)buf = Distances[i];//��r�p��distance���Ȃ���Ύ���������
		//����buf�̒��g���i�Ԃ�distance���傫�����
		if (buf < Distances[i]) {
			//�z��ԍ����擾����
			max = i;
		}
	}
	//���̎��_��max�ɋ�������ԉ���outPoint�̓Y���ԍ��������Ă���͂�
	//��ԉ���outPoint��Ԃ�
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
	//enemyPos�̓}�b�v���W
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
bool GameManager::CheckNearByPlayerToAllEnemy(int range)
{
	bool isNear = false;
	for (auto enemy : liveEnemyList) {
		//enemyPos�̓}�b�v���W
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

				////x�̍��̐�Βl��range�ȉ��܂���y�̍��̐�Βl��range�ȉ���������
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
	//range�͈͓��ɂ�����true��Ԃ�
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

void GameManager::SetLiveEnemyList(std::list<std::shared_ptr<Enemy>> list)
{
	liveEnemyList = list;
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
void GameManager::TakeDamageToTarget(Actor* hoge, t2k::Vector3 Pos)
{
	//�v���C���[���U������Ȃ�
	if (hoge->GetActId() == 0) {
		//�S�ẴG�l�~�[����O�ɂ���G�l�~�[���擾
		std::shared_ptr<Enemy>hoge = GetIsThereEnemyToDir(Pos);
		//������U��Ȃ�
		if (hoge == nullptr) {
			player->skip = true;
			return;
		}

		float damage = CalcDamage(player->GetStatus(1), hoge->GetStatus(2));
		RunDamageEvent((-1) * damage, hoge);
		addLog(hoge->GetName() + "��" + std::to_string(static_cast<int>(damage)) + "�_���[�W���󂯂�");
	}
	//�G�l�~�[���U������Ȃ�
	else {
		float damage = CalcDamage(hoge->GetStatus(1), player->GetStatus(2));
		RunDamageEvent((-1) * damage, player);
		addLog("Player��" + std::to_string(static_cast<int>(damage)) + "�_���[�W���󂯂�");
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
	//�_���[�W=�U�����̍U���� �� 2^(�U�����󂯂鑤�̖h���/10)
	//std::pow(2,(enemyDef/10))

	float rand = (float)GetRandEx(-5, 5);
	float damage = Attack / std::pow(2, (Defence / 10)) + rand;
	//�Œ�ۏ؃_���[�W
	if (damage < 0)damage = 1;

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
//������excel����K�w�����̃f�[�^��ǂݍ��݁A���O�ɑS�Ẵt���A���쐬���Ă���
void GameManager::ReCreate()
{
	delete map;
	map = nullptr;

	wayPoint.clear();

	CreateDungeon(nowDungeon);


	player->pos = SetStartPos(setStartPosType::PLAYER);
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

	if (!minimapDraw)return;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	map->MiniMapDraw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}
void GameManager::CreateDungeon(Dungeon dungeonName) {

	if (map != nullptr)delete map;
	//InitWayPointVector();
	wayPoint.clear();
	nowDungeon = dungeonName;
	CreateMap(dungeonName);
	//�}�b�v��������
	map->DivideStart(MAPWIDTH, MAPHEIGHT, map);
	//�K�i�̃}�b�v���W�̎擾
	t2k::Vector3 stairsPos = SetStartPos(setStartPosType::STAIR);
	//�K�i�ݒu
	map->SetChip(stairsPos.x, stairsPos.y, map->STAIRS);

	if (player == nullptr)return;
	//�_���W�������̃����_���Ȉʒu�Ƀv���C���[���ړ�
	player->pos = SetStartPos(setStartPosType::PLAYER);
	CameraReset();

	map->player = player;
}
void GameManager::CreateMap(Dungeon dungeonName)
{
	std::vector<int> handles = GetGraphicHandles(dungeonName);
	map = new Map(MAPWIDTH, MAPHEIGHT, handles);

}

void GameManager::MakePlayer(SpawnScene nowScene)
{
	if (player != nullptr)return;
	if (nowScene == SpawnScene::Camp) {
		player = std::make_shared<Player>(SpawnPlayerCamp(), 100.0f, 30, 30, 30, 0);
		//map->player = player;
	}
	else if (nowScene == SpawnScene::Dungeon) {
		player = std::make_shared<Player>(SetStartPos(setStartPosType::PLAYER), 100.0f, 30, 30, 30, 0);
		map->player = player;
	}
}

std::vector<int>& GameManager::GetGraphicHandles(Dungeon dungeonName)
{
	//�_���W�����l�[���ɑΉ�����摜�n���h����2�����z�񂩂�摜�n���h���z����擾����
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
//�J�������L�����𒆐S�Ƀ��Z�b�g
void GameManager::CameraReset() {
	camera->cameraPos = player->pos - t2k::Vector3(512, 384, 0);
}
//�Ȃ񂩔���
void GameManager::CameraMove(int width, int height, int dir)
{
	//	if (player->pos.x+ width / 2 > width)return;
	//	camera->cameraPos = player->pos - t2k::Vector3(512, 384, 0);
	//	//camera->cameraPos = player->pos - GetMultipleVector(dir, 20);
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
//0:��, 1 : �E, 2 : ��, 3 : ��
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

//���O�𐶐�����֐�,�Â������������
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

void GameManager::DrawStringLows(int lowNum)
{
	for (int i = 0; i < 3; ++i) {
		DrawStringEx(100, 100 * i + 20, -1, (char*)strings[lowNum][i].c_str());
	}
}
//�����������O��\������֐�
void GameManager::LogDraw(int x, int y)
{
	for (int i = 0; i < 7; ++i) {
		DrawStringEx(x + 20, y + 20 + (i * 20), -1, "%s", Log[i].c_str());
	}
}


