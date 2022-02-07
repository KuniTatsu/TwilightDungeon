#include "Inventory.h"
#include"DxLib.h"
#include"../../library/t2klib.h"
#include"../../support/Support.h"
#include"Item.h"

Inventory::Inventory()
{
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
		break;
	}
}

void Inventory::DrawInventory(int x, int y)
{
	for (int i = 0; i < 10; ++i) {
		if (inventory[i] == nullptr)break;
		DrawStringEx(x + 10, y + 10 + 20 * i, -1, "%s", inventory[i]->getItemName().c_str());
	}
}
