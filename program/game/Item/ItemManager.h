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
	//アイテム総数を返す関数
	inline int GetSumItemNum() {
		return itemSumNum;
	}
	

	//指定したアイテムIDに該当するアイテムのポインタを返す関数
	Item* GetItemData(int ItemId);

	//指定したアイテムタイプの中からランダムでアイテムIDを返す関数
	int GetRamdomTypeItemId(int ItemType);

	//playerのレベルからドロップウェイトに応じたアイテムIDを返す関数
	int GetRandomItemWithWeight(int PlayerLevel);

private:
	//アイテムマスターデータのロード
	void LoadItem();
	//アイテムのドロップウェイトのロード
	void LoadDropWeight();

	//ロードしたアイテムのウェイト配列
	std::vector<std::vector<std::string>> loadWeight;

	std::vector<int>itemWeightList;

	//アイテム総数
	int itemSumNum = 0;

};