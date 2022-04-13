///*****Description*****
///アイテム情報に関するクラス
///全てのアイテムのマスターデータを保持する
/// アイテムの情報をこのクラスから受け取る
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

	//ItemTypeごとに振り分けられたItemMasterデータ
	std::vector < std::vector<Item*>> itemMaster;

	int itemSumNum = 0;

	Item* GetItemData(int ItemId);

	int GetRamdomTypeItemId(int ItemType);

private:
	void LoadItem();

};