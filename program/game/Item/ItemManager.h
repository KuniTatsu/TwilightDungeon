#pragma once
#include"../../library/t2klib.h"
#include<vector>
#include<string>
class Item;


class ItemManager {

public:
	std::vector<std::vector<std::string>> loadItem;

	ItemManager();
	//ItemType���ƂɐU�蕪����ꂽItemMaster�f�[�^
	std::vector < std::vector<Item*>> itemMaster;

	Item* getItemData(int ItemId);

private:
	void LoadItem();

};