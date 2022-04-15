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

//�R���X�g���N�^�̈�������̏ꍇ�͓��� explicit�����邱��

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

	//map��񗘗p�̂��߃_���W��������
	CreateDungeon(Dungeon::FOREST);

	//loadDivGraph��index�擾
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
	//����inventory�̎��A�C�e���z�񂪂����ς��Ȃ�
	if (Inventories[InventoryNum]->inventoryList.size() >= 10) {
		//if (sharedInventories[inventoryNum]->inventoryList.size() >= 10) {

			//�V����inventory�̃C���X�^���X�𐶐�����
		Inventory* newInventory = new Inventory(InventoryNum + 1);
		//inventory�z��ɓo�^
		Inventories.emplace_back(newInventory);

		//�o�^����inventory���X�V����
		InventoryNum++;
	}
	Item* item = iManager->GetItemData(ItemId);
	//�����A�C�e����������
	if (item->getItemData(1) >= 2) {
		equipItem* eItem = (equipItem*)item;
		//�����f�[�^�̎擾
		std::vector<int> intData = eItem->GetAllIntData();
		//������f�[�^�̎擾
		std::vector<std::string> stringData = item->GetAllStringData();

		//�����A�C�e���𐶐� �������ɃX�e�[�^�X�������_���ɕύX
		equipItem* newItem = new equipItem(intData[0], intData[1], stringData[0], intData[2], intData[3], intData[4],
			stringData[1], intData[5], intData[6], stringData[2], intData[7], intData[8], intData[9], intData[10], 0);
		//�C���x���g���ǉ�
		Inventories[InventoryNum]->AddInventory(newItem);
	}
	else {
		std::vector<int> intData = item->GetAllIntData();
		std::vector<std::string> stringData = item->GetAllStringData();

		Item* newItem = new Item(intData[0], intData[1], stringData[0], intData[2], intData[3], intData[4], intData[5], stringData[1], stringData[2]);

		Inventories[InventoryNum]->AddInventory(newItem);

	}

}
//�V���b�v����v���C���[�C���x���g���ɃA�C�e����ǉ�����֐�
void GameManager::AddItemFromShop(Item* ShopItem)
{
	//����inventory�̎��A�C�e���z�񂪂����ς��Ȃ�
	if (inventories[inventoryNum]->inventoryList.size() >= 10) {
		//if (sharedInventories[inventoryNum]->inventoryList.size() >= 10) {

			//�V����inventory�̃C���X�^���X�𐶐�����
		Inventory* newInventory = new Inventory(inventoryNum + 1);
		//inventory�z��ɓo�^
		inventories.emplace_back(newInventory);

		//�o�^����inventory���X�V����
		inventoryNum++;
	}
	//�����A�C�e����������
	if (ShopItem->getItemData(1) >= 2) {
		equipItem* eItem = static_cast<equipItem*>(ShopItem);
		//�����f�[�^�̎擾
		std::vector<int> intData = eItem->GetAllIntData();
		//������f�[�^�̎擾
		std::vector<std::string> stringData = ShopItem->GetAllStringData();

		//�����A�C�e���𐶐� �������ɃX�e�[�^�X�������_���ɕύX
		equipItem* newItem = new equipItem(intData[0], intData[1], stringData[0], intData[2], intData[3], intData[4],
			stringData[1], intData[5], intData[6], stringData[2], intData[7], intData[8], intData[9], intData[10], 1);
		addLog(newItem->getItemName()+"����肵��");
		//�C���x���g���ǉ�
		inventories[inventoryNum]->AddInventory(newItem);
	}
	else {
		std::vector<int> intData = ShopItem->GetAllIntData();
		std::vector<std::string> stringData = ShopItem->GetAllStringData();

		Item* newItem = new Item(intData[0], intData[1], stringData[0], intData[2], intData[3], intData[4], intData[5], stringData[1], stringData[2]);

		addLog(newItem->getItemName() + "����肵��");
		//�C���x���g���ǉ�
		inventories[inventoryNum]->AddInventory(newItem);

	}
}

void GameManager::PopItemFromInventory(const int NowInventoryId)
{
	//���̈ʒu�̔z��ԍ����擾
	int selectNum = inventories[NowInventoryId]->GetCursorNum();
	//�\�����̃C���x���g�����擾
	auto itr = inventories[NowInventoryId]->inventoryList.begin();

	//�I�����ꂽ�A�C�e���܂ŃC�e���[�^�ړ�
	for (int i = 0; i < selectNum; ++i) {
		itr++;
	}
	//�A�C�e��������
	delete((*itr));
	itr = inventories[NowInventoryId]->inventoryList.erase(itr);
	//�J�[�\���̈ʒu���ЂƂ�Ɉړ�
	inventories[NowInventoryId]->SetCursorNum(-1);

	//pop����A�C�e��������ꏊ=������C���x���g�����Ō�̃C���x���g���ł͂Ȃ��ꍇ
	if (NowInventoryId != inventoryNum) {
		int checkInventoryNum = NowInventoryId;
		while (1) {
			if (inventories[checkInventoryNum + 1]->inventoryList.empty())break;

			//if (sharedInventories[checkInventoryNum + 1]->inventoryList.empty())break;

		//���̃y�[�W�̍ŏ��̃A�C�e�����R�s�[���ď������A�C�e���̃��X�g�̖����ɉ�����
			auto item = inventories[checkInventoryNum + 1]->inventoryList.begin();

			//�A�C�e���ǉ�
			inventories[checkInventoryNum]->inventoryList.emplace_back((*item));

			//���̃y�[�W�̍ŏ��̃A�C�e����pop����
			inventories[checkInventoryNum + 1]->inventoryList.pop_front();

			//�Ō�̃C���x���g���y�[�W�ɂ��ǂ蒅������break
			if (checkInventoryNum + 1 == inventoryNum)break;
			checkInventoryNum++;
		}
	}
	//�ŏ��̃C���x���g�����Ȃ�
	else {
		//�C���x���g�����̃A�C�e������1���炷
		inventories[NowInventoryId]->SetItemNum(-1);
	}
	//��̃C���x���g������������
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
	//�J�[�\���̈ʒu����ԏ�Ƀ��Z�b�g
	if (inventories[NowInventoryId]->inventoryList.empty())inventories[NowInventoryId]->CursorReset();
}
//X�L�[�ŉ�ʂ�2�{�Ɋg��k������֐�
void GameManager::ScaleChange()
{
	//�v���C���[�̃��[�J�����W���擾����
	playerPosBuf = WorldToLocal(player->pos);
	//���̕`�悪�ʏ�{���Ȃ�
	if (nowScale == ScaleMode::NORMAL) {
		//�`����@���g��ɂ���
		nowScale = ScaleMode::WIDE;
	}
	//���̕`�悪2�{�Ȃ�
	else {
		//�`����@��ʏ�ɂ���
		nowScale = ScaleMode::NORMAL;
	}
	//�`��`�b�v�̃T�C�Y��ύX����
	nowGraphicSize = scale[static_cast<uint32_t>(nowScale)];
	//�`��{�����C������
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
	//�v���C���[�̕`����W���C������
	player->pos = LocalToWorld(playerPosBuf.x, playerPosBuf.y);
	//�J�������v���C���[���S�ɏC������
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
//�h���b�v�A�C�e�����X�g�������̃A�C�e�����폜
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

//a����b�܂ł̒l���烉���_���Ɏ擾����
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
		//�^�C�}�[���񂵁A�������ŉ��Ɋg�傷��
		//����̑傫���ɂȂ�����c�Ɋg�傷��
		//�c������̑傫���ɂȂ�����g�����߁A���̂܂ܕ\������
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


void GameManager::PlayerDead()
{
	//�v���C���[�̃X�e�[�^�X�֌W����
	player->DeadPlayer();
	//���O����
	ResetLog();

	//�C���x���g���̃A�C�e����S�ď�������
	for (auto inventory : inventories) {
		inventory->inventoryList.clear();
	}
	//�C���x���g�����폜
	for (int i = inventoryNum; i >= 0; --i) {
		delete inventories[i];
		inventories[i] = nullptr;
		inventories.pop_back();
	}
	//�C���x���g���̌������Z�b�g
	inventoryNum = 0;
	//�C���x���g�������������
	inventory = new Inventory(0);
	inventories.emplace_back(inventory);
	//�L�����v�V�[���X�|�[���ʒu
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
	//�v���C���[���U������Ȃ�
	if (actor->GetActId() == 0) {
		//�S�ẴG�l�~�[����O�ɂ���G�l�~�[���擾
		std::shared_ptr<Enemy>frontEnemy = GetIsThereEnemyToDir(Pos);
		//������U��Ȃ�
		if (frontEnemy == nullptr) {
			//player->skip = true;
			return;
		}
		//�U���͂Ɩh��͂��擾,�_���[�W�v�Z
		float damage = CalcDamage(player->GetStatus(1), frontEnemy->GetStatus(2));
		//�_���[�W�𔽉f
		RunDamageEvent((-1) * damage, frontEnemy);
		//���O�ǉ�
		addLog(frontEnemy->GetName() + "��" + std::to_string(static_cast<int>(damage)) + "�_���[�W���󂯂�");
	}
	//�G�l�~�[���U������Ȃ�
	else {
		//�U���͂Ɩh��͂��擾,�_���[�W�v�Z
		float damage = CalcDamage(actor->GetStatus(1), player->GetStatus(2));
		//�_���[�W�𔽉f
		RunDamageEvent((-1) * damage, player);
		addLog("Player��" + std::to_string(static_cast<int>(damage)) + "�_���[�W���󂯂�");
	}
}

void GameManager::DealSkillDamageToTarget(Actor* Actor, const t2k::Vector3 Pos, Skill* ActivateSkill)
{
	//�X�L���̐��l�f�[�^���擾
	const float* skillData = ActivateSkill->GetSkillAmount();
	//�X�L�������擾
	std::string skillName = ActivateSkill->GetSkillName();

	//�v���C���[���U������Ȃ�
	if (Actor->GetActId() == 0) {
		//�S�ẴG�l�~�[����O�ɂ���G�l�~�[���擾
		std::shared_ptr<Enemy>frontEnemy = GetIsThereEnemyToDir(Pos);
		//������U��Ȃ�
		if (frontEnemy == nullptr) {
			//player->skip = true;
			return;
		}

		//�X�L���̔{��,�U���͂Ɩh��͂��擾,�_���[�W�v�Z
		float damage = CalcSkillDamage(skillData[0], player->GetStatus(1), frontEnemy->GetStatus(2));
		//�_���[�W�𔽉f
		RunDamageEvent((-1) * damage, frontEnemy);
		//���O�ǉ�
		addLog(frontEnemy->GetName() + "��" + skillName + "�ɂ����" + std::to_string(static_cast<int>(damage)) + "�_���[�W���󂯂�");
	}
	//�G�l�~�[���U������Ȃ�
	else {
		//�U���͂Ɩh��͂��擾,�_���[�W�v�Z
		float damage = CalcSkillDamage(skillData[0], Actor->GetStatus(1), player->GetStatus(2));
		//�_���[�W�𔽉f
		RunDamageEvent((-1) * damage, player);
		addLog("Player��" + skillName + "�ɂ����" + std::to_string(static_cast<int>(damage)) + "�_���[�W���󂯂�");
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

float GameManager::CalcSkillDamage(const float SkillRate, const int Attack, const int Defence)
{
	//�X�L���̃_���[�W�{�����g�p�҂�Attack�Ɋ|�������l�Ń_���[�W�v�Z���s��
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
//������excel����K�w�����̃f�[�^��ǂݍ��݁A���O�ɑS�Ẵt���A���쐬���Ă���
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
	t2k::debugTrace("\n�_���W�����쐬�O�f�o�b�O\n");

	CreateMap(dungeonName);
	t2k::debugTrace("\n�_���W�����쐬��f�o�b�O\n");

	//�}�b�v��������
	map->DivideStart(MAPWIDTH, MAPHEIGHT, map);
	t2k::debugTrace("\n�����I���f�o�b�O\n");

	//�K�i�̃}�b�v���W�̎擾
	t2k::Vector3 stairsPos = SetStartPos(setStartPosType::STAIR);
	t2k::debugTrace("\n�K�i���W�w��f�o�b�O\n");

	//�K�i�ݒu
	map->SetChip(stairsPos.x, stairsPos.y, map->STAIRS);

	//�f�o�b�O�p�K�i���W�o�^
	map->AddStairList(stairsPos);

	//�V���b�v�������_���Őݒu(�f�t�H���g�m����20%)
	map->SetShop();
	t2k::debugTrace("\n�V���b�v�ݒu�f�o�b�O\n");

	if (player == nullptr)return;
	//�_���W�������̃����_���Ȉʒu�Ƀv���C���[���ړ�
	player->pos = SetStartPos(setStartPosType::PLAYER);
	t2k::debugTrace("\n�v���C���[�ݒu�f�o�b�O\n");

	map->player = player;
	map->ChangeRoomVisit(WorldToLocal(player->pos));
	t2k::debugTrace("\n�ʒu���o�^�f�o�b�O\n");

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
	camera->cameraPos = player->pos - WINDOWCENTER;
}
//�Ȃ񂩔���
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


//�����������O��\������֐�
void GameManager::LogDraw(int x, int y)
{
	for (int i = 0; i < 7; ++i) {
		DrawStringEx(x + 20, y + 20 + (i * 20), -1, "%s", Log[i].c_str());
	}
}


void GameManager::DrawHowTo()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	//�{�^�������w�i
	DrawRotaGraph(870, 250, 1, 0, howToPlayBack, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	//�{�^�������摜
	DrawRotaGraph(930, 150, 0.1, 0, arrowButton, true);
	DrawRotaGraph(930, 210, 1, 0, RButton, true);
	DrawRotaGraph(930, 255, 1, 0, EnterButton, true);

	//������
	DrawStringEx(780, 150, -1, "�ړ��{�^��");
	DrawStringEx(780, 200, -1, "�U���{�^��");
	DrawStringEx(780, 250, -1, "����{�^��");
}