#pragma once
#include<list>
#include<memory>
class Item;

class Inventory
{
public:
	Inventory(int MyInventoryNum);
	~Inventory();

	//Item* inventory[10] = { nullptr,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr };

	//std::shared_ptr<Item>inventory[10] = { nullptr,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr };

	std::list<Item*> inventoryList;
	std::list<std::shared_ptr<Item>> inventorySharedList;

	//インベントリにアイテムを追加する関数
	void AddInventory(Item* item);

	void AddSharedInventory(std::shared_ptr<Item>item);
	//カーソルを上下に動かす関数
	void CursorMove();
	//カーソルを一番上に戻す関数
	void CursorReset();
	//インベントリ内のアイテム名を描画する関数
	void DrawInventory(int x, int y);
	//カーソルで選択中のアイテムの説明を描画する関数
	void DrawItemData(int x, int y);

	//カーソルの位置を取得する関数
	int GetCursorNum();

	//カーソルの位置を変更する関数 type:0->加算移動,1->指定位置移動
	void SetCursorNum(int MoveNum);

	//インベントリ番号を取得する関数
	inline int GetInventoryNum() {
		return myInventoryNum;
	}

	inline void SetItemNum(int num) {
		itemNum += num;
	}

private:
	//選択中のアイテムを指すカーソルの位置
	int selectCursor = 0;
	//カーソルgh
	int cursorGh = 0;
	//選択中アイテムの背景
	int selectItemBackGh = 0;
	//インベントリ内のアイテム数
	int itemNum = 0;

	//インベントリ番号
	int myInventoryNum = 0;


};

