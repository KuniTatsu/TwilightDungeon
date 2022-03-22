#include "Inventory.h"
#include"DxLib.h"
#include"../../library/t2klib.h"
#include"../../support/Support.h"
#include"Item.h"
#include"equipItem.h"
#include"../GameManager.h"
#include"../SoundManager.h"

extern GameManager* gManager;
Inventory::Inventory(int MyInventoryNum)
{
	cursorGh = gManager->LoadGraphEx("graphics/menuCursor.png");
	selectItemBackGh = gManager->LoadGraphEx("graphics/selectItemBack.png");
	myInventoryNum = MyInventoryNum + 1;
}

Inventory::~Inventory()
{
}

void Inventory::AddInventory(Item* item)
{
	if (inventoryList.size() == 10)return;
	inventoryList.emplace_back(item);
	itemNum++;

}

void Inventory::CursorMove()
{
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_UP)) {
		itemNum = GetItemCount();
		gManager->sound->System_Play(gManager->sound->system_move);
		selectCursor = (selectCursor + (itemNum - 1)) % itemNum;
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_DOWN)) {
		itemNum = GetItemCount();
		gManager->sound->System_Play(gManager->sound->system_move);
		selectCursor = (selectCursor + 1) % itemNum;
	}
}
//インベントリのカーソル位置を一番上にリセットする関数
void Inventory::CursorReset()
{
	selectCursor = 0;
}
//インベントリ内のアイテム名を描画する関数
void Inventory::DrawInventory(int x, int y)
{
	int i = 0;

	/*
	ソート
	inventoryList.sort([](Item* l, Item* r) {
		return (l->getItemData(0) > r->getItemData(0));
	});
	*/

	for (auto item : inventoryList) {
		//装備アイテムなら
		if (item->getItemData(1) >= 2) {
			equipItem* eItem = static_cast<equipItem*>(item);
			//装備中のアイテムなら'E'を頭に描画する
			if (eItem->GetIsEquiped())DrawStringEx(x + 40, y + 10 + 30 * i, -1, "[E]");
			//アイテム名の描画
			DrawStringEx(x + 80, y + 10 + 30 * i, -1, "%s", item->getItemName().c_str());
		}
		else {
			//アイテム名の描画
			DrawStringEx(x + 80, y + 10 + 30 * i, -1, "%s", item->getItemName().c_str());
		}
		++i;
	}
	//選択中アイテムの横にカーソルを描画する
	DrawRotaGraph(x + 30, y + 20 + selectCursor * 30, 0.7, 0, cursorGh, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 30);
	DrawRotaGraph(x + 220, y + 20 + selectCursor * 30, 1, 0, selectItemBackGh, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}
//アイテムの説明を描画する関数
void Inventory::DrawItemDesc(int x, int y)
{
	if (inventoryList.empty())return;
	auto itr = inventoryList.begin();
	for (int i = 0; i < selectCursor; ++i) {
		if (itr == inventoryList.end()) {
			break;
		}
		itr++;
	}
	(*itr)->DrawItemStringData(x, y);
	//inventory[selectCursor]->DrawItemData(x, y);
}

void Inventory::DrawEquipItemStatus(const int x, const int y)
{
	if (inventoryList.empty())return;

	auto itr = inventoryList.begin();
	for (int i = 0; i < selectCursor; ++i) {
		if (itr == inventoryList.end()) {
			break;
		}
		itr++;
	}
	//装備アイテムならデータを取得して描画する
	if ((*itr)->getItemData(1) >= 2) {
		auto item = static_cast<equipItem*>((*itr));
		item->DrawEquipItemStatus(x, y, item->GetSubId());
	}

}

int Inventory::GetCursorNum()
{
	return selectCursor;
}

void Inventory::SetCursorNum(int MoveNum)
{
	if (selectCursor != inventoryList.size())return;
	selectCursor += MoveNum;
	if (selectCursor < 0)selectCursor = 0;

}
