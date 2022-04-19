///*****Description*****
///�A�C�e�����Ɋւ���N���X
///�S�ẴA�C�e���̃}�X�^�[�f�[�^��ێ�����
/// �A�C�e���̏������̃N���X����󂯎��
///*********************
#pragma once
#include"../../library/t2klib.h"
#include<vector>
#include<string>
class Item;


class ItemManager {

public:
	std::vector<std::vector<std::string>> loadItem;

	ItemManager();
	~ItemManager();

	//ItemType���ƂɐU�蕪����ꂽItemMaster�f�[�^
	std::vector < std::vector<Item*>> itemMaster;
	//�A�C�e��������Ԃ��֐�
	inline int GetSumItemNum() {
		return itemSumNum;
	}
	

	//�w�肵���A�C�e��ID�ɊY������A�C�e���̃|�C���^��Ԃ��֐�
	Item* GetItemData(int ItemId);

	//�w�肵���A�C�e���^�C�v�̒����烉���_���ŃA�C�e��ID��Ԃ��֐�
	int GetRamdomTypeItemId(int ItemType);

	//player�̃��x������h���b�v�E�F�C�g�ɉ������A�C�e��ID��Ԃ��֐�
	int GetRandomItemWithWeight(int PlayerLevel);

private:
	//�A�C�e���}�X�^�[�f�[�^�̃��[�h
	void LoadItem();
	//�A�C�e���̃h���b�v�E�F�C�g�̃��[�h
	void LoadDropWeight();

	//���[�h�����A�C�e���̃E�F�C�g�z��
	std::vector<std::vector<std::string>> loadWeight;

	std::vector<int>itemWeightList;

	//�A�C�e������
	int itemSumNum = 0;

};