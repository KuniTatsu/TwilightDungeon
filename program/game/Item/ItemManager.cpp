#include "ItemManager.h"
#include"Item.h"
#include"equipItem.h"
#include"../GameManager.h"
#include"DxLib.h"
#include<random>

extern GameManager* gManager;
ItemManager::ItemManager()
{
	itemMaster.resize(4);
	LoadItem();
	LoadDropWeight();

}

ItemManager::~ItemManager()
{
	for (int i = 0; i < itemMaster.size(); ++i) {
		for (int k = 0; k < itemMaster[i].size(); ++k) {
			if (itemMaster[i].empty())continue;
			delete itemMaster[i][k];
		}
	}
}

Item* ItemManager::GetItemData(int ItemId)
{
	for (auto master : itemMaster) {
		for (auto item : master) {
			if (item->GetItemId() == ItemId) {
				return item;
			}
		}
	}
	return nullptr;
}

int ItemManager::GetRamdomTypeItemId(int ItemType)
{
	int num = itemMaster[ItemType].size() - 1;
	int rand = gManager->GetRandEx(0, num);

	return itemMaster[ItemType][rand]->getItemData(0);
}
//�A�C�e�����E�F�C�g�ɉ����Ē��I����֐�
int ItemManager::GetRandomItemWithWeight(int PlayerLevel)
{
	std::random_device rnd;				 // �񌈒�I�ȗ���������->�����V�[�h�Ɏg��
	//�����_���Ȑ������߂邽�߂̊֐��������߂�
	std::mt19937 GetRandom(rnd());       // �����Z���k�E�c�C�X�^��32�r�b�g�ŁA�����͏����V�[�h

	int totalWeight = 0;
	int selectedItemId = 0;

	//totalWeight�����߂�->������Level�ɂ���ďo������A�C�e����ω������A���̏�ō�����A�C�e�����X�g��weight�Ōv�Z����悤�ɂ���
	for (int i = 0; i < itemSumNum; ++i) {
		totalWeight += itemWeightList[i];
	}

	//���͈͂̈�l���z�����擾
	std::uniform_int_distribution<> Weight(0, totalWeight);

	//�A�C�e���������_���Ō��߂�
	int rand = Weight(GetRandom);

	//���I
	for (int i = 0; i < itemSumNum; i++) {
		if (rand < itemWeightList[i]) {
			//�A�C�e������
			selectedItemId = i;
			break;
		}

		// ���̑Ώۂ𒲂ׂ�
		rand -= itemWeightList[i];
	}
	return selectedItemId;
}

void ItemManager::LoadItem()
{
	//id(int)	ItemType(int)	Item_Name(std::string)	AmountOfSaturation(int)	AmountOfHeal(int)	HitDamage(int)	HP(int)	Atack(int)	Defence(int)	Speed(int)	Gh(std::string)	
	loadItem = t2k::loadCsv("Csv/Item.csv");
	for (int i = 1; i < loadItem.size(); ++i) {

		int id = std::atoi(loadItem[i][0].c_str());
		int type = std::atoi(loadItem[i][1].c_str());

		int manpuku = std::atoi(loadItem[i][3].c_str());
		int heal = std::atoi(loadItem[i][4].c_str());
		int damage = std::atoi(loadItem[i][5].c_str());
		int equipHp = std::atoi(loadItem[i][6].c_str());
		int equipAtack = std::atoi(loadItem[i][7].c_str());
		int equipDefence = std::atoi(loadItem[i][8].c_str());
		int equipSpeed = std::atoi(loadItem[i][9].c_str());
		int subId = std::atoi(loadItem[i][11].c_str());
		int price = std::atoi(loadItem[i][12].c_str());

		//����A�C�e���Ȃ�
		if (type < 2) {
			Item* newItem = new Item(id, type, loadItem[i][2], manpuku, heal, damage, price, loadItem[i][10], loadItem[i][13]);
			itemMaster[type].emplace_back(newItem);
		}
		else {
			equipItem* newItem = new equipItem(id, type, loadItem[i][2], manpuku, heal, damage, loadItem[i][10], subId, price, loadItem[i][13], equipHp, equipAtack, equipDefence, equipSpeed, 1);
			itemMaster[type].emplace_back(newItem);
		}
		itemSumNum++;
	}
	gManager->SetItemNum(itemSumNum);
}

void ItemManager::LoadDropWeight()
{
	loadWeight = t2k::loadCsv("Csv/ItemDropWeight.csv");

	for (int i = 1; i < loadWeight.size(); ++i) {
		//�A�C�e���̃E�F�C�g�݂̂��i�[����vector�ɓo�^
		itemWeightList.emplace_back(stoi(loadWeight[i][3]));
	}

}
