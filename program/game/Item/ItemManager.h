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

	int itemSumNum = 0;

	Item* GetItemData(int ItemId);

	int GetRamdomTypeItemId(int ItemType);

private:
	void LoadItem();

};