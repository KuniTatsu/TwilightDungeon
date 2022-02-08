#pragma once
#include<list>
class Item;

class Inventory
{
public:
	Inventory();
	~Inventory();

	Item* inventory[10] = { nullptr,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr };

	std::list<Item*> inventoryList;
	//インベントリにアイテムを追加する関数
	void AddInventory(Item* item);
	//カーソルを上下に動かす関数
	void CursorMove();
	//カーソルを一番上に戻す関数
	void CursorReset();
	//インベントリ内のアイテム名を描画する関数
	void DrawInventory(int x,int y);
	//カーソルで選択中のアイテムの説明を描画する関数
	void DrawItemData(int x,int y);

private:
	//選択中のアイテムを指すカーソルの位置
	int selectCursor = 0;
	//カーソルgh
	int cursorGh = 0;
	//インベントリ内のアイテム数
	int itemNum = 0;


};

