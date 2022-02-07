#pragma once
#include<list>
#include<vector>

class HaveItem
{
public:
	HaveItem();
	~HaveItem();
	//ItemId n番を持っているアイテムリストに登録
	void setItemToInventory(int ItemId);
	//inventoryの中身を外部のvectorに移す形で取得する関数
	std::vector<int>* getItemFromInventory(std::vector<int>*HaveItemIds);

private:

	std::list<int>inventory;
};

