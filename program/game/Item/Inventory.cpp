#include "Inventory.h"
#include"DxLib.h"
#include"../../library/t2klib.h"
#include"../../support/Support.h"
#include"Item.h"
#include"../GameManager.h"

extern GameManager* gManager;
Inventory::Inventory()
{
	cursorGh = gManager->LoadGraphEx("graphics/menuCursor.png");
}

Inventory::~Inventory()
{
}

void Inventory::AddInventory(Item* item)
{
	if (inventory[9] != nullptr)return;

	for (int i = 0; i < 10; ++i) {
		if (inventory[i] != nullptr)continue;
		inventory[i] = item;
		itemNum++;
		break;
	}
}

void Inventory::CursorMove()
{
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_UP)) {
		selectCursor = (selectCursor + (itemNum-1)) % itemNum;
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_DOWN)) {
		selectCursor = (selectCursor + 1) % itemNum;
	}
}

void Inventory::CursorReset()
{
	selectCursor = 0;
}

void Inventory::DrawInventory(int x, int y)
{
	for (int i = 0; i < 10; ++i) {
		if (inventory[i] == nullptr)break;
		DrawStringEx(x + 50, y + 10 + 30 * i, -1, "%s", inventory[i]->getItemName().c_str());
	}
	DrawRotaGraph(x + 30, y + 20 + selectCursor * 30, 0.7, 0, cursorGh, true);
}

void Inventory::DrawItemData(int x, int y)
{
	if (inventory[selectCursor] == nullptr)return;
	inventory[selectCursor]->DrawItemData(x, y);
}
