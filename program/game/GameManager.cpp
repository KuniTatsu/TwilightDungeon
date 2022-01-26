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
	//�K�i�̃}�b�v���W�̎擾
	t2k::Vector3 stairsPos = SetStartPos(1);
	//�K�i�ݒu
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
		//�A�C�e��i�Ԃ��Z�Z�ŏ���������
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

		//abilitytype���ƂɃ��X�g�Ɋi�[
		//itemList[b].emplace_back(abi);
	}
}

t2k::Vector3 GameManager::SetStartPos(int setType)
{
	//�����_���ȕ����ԍ����擾
	int rand = GetRand(map->GetRoomNum());
	//�����ԍ����畔�����擾 0:�� 1:�� 2:�E 3:�� �A���Ă���̂̓}�b�v���W
	vector<int> room = map->GetRoom(rand);
	//�����̒��̃����_���ȃ}�b�v���W���擾����
	int x = GetRandEx(room[0], room[2]);
	int y = GetRandEx(room[1], room[3]);
	//�G�̐�������������W���`�F�b�N���s��
	if (setType == 2) {
		int player_x = player->pos.x;
		int player_y = player->pos.y;
		while (1) {
			if (player_x != x && player_y != y)break;
			x = GetRandEx(room[0], room[2]);
			y = GetRandEx(room[1], room[3]);
		}
	}

	//0:player����Ȃ���΃}�b�v���W��Ԃ�
	if (setType == 1)return t2k::Vector3(x, y, 0);

	//�擾�����}�b�v���W��`����W�ɕϊ�����
	t2k::Vector3 Pos = map->MapToWorld(x, y);

	return Pos;
}

void GameManager::InitWayPointVector(int initroomNum)
{
	wayPoint.resize(initroomNum + 1);
}

//void GameManager::CheckRoomWayPoint(int roomId)
//{
//	//roomId�̕����̍��ォ��E���܂Ō��ĒʘH�������emplase_back����
//	std::vector<int> room = map->GetRoom(roomId);
//	for (int k = room[1] - 1; k < room[3] + 2; ++k) {
//		for (int i = room[0] - 1; i < room[2] + 2; ++i) {
//
//			t2k::Vector3 chip = t2k::Vector3(i, k, 0);
//			//�ǂ�������continue
//			if (GetMapChip(chip) == 0)continue;
//			test++;//�ʂ��Ă�
//			//���������̒��Ȃ�continue
//			if (chip.x >= room[0] && chip.x <= room[2])continue;
//			if (chip.y >= room[1] && chip.y <= room[3])continue;
//			wayPoint[roomId].emplace_back(chip);//�����Ȃ�
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

void GameManager::Zoom(double* zoomEx)
{
	//if (t2k::Input::isKeyDown(t2k::Input::KEYBORD_Z)) {
	//	*zoomEx -= 0.01;
	//}
	//else if (t2k::Input::isKeyDown(t2k::Input::KEYBORD_X)) {
	//	*zoomEx += 0.01;
	//}

}
//������excel����K�w�����̃f�[�^��ǂݍ��݁A���O�ɑS�Ẵt���A���쐬���Ă���
void GameManager::ReCreate()
{
	delete map;
	map = nullptr;
	delete player;
	player = nullptr;
	map = new Map(MAPWIDTH, MAPHEIGHT);
	map->DivideStart(MAPWIDTH, MAPHEIGHT, map);

	//�K�i�̃}�b�v���W�̎擾
	t2k::Vector3 stairsPos = SetStartPos(1);
	//�K�i�ݒu
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
//�Ȃ񂩔���
void GameManager::CameraMove(Player* p)
{
	camera->cameraPos = player->pos - t2k::Vector3(512, 384, 0);
}

