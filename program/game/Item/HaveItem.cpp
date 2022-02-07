#include "HaveItem.h"

HaveItem::HaveItem()
{
}

HaveItem::~HaveItem()
{
}

void HaveItem::setItemToInventory(int ItemId)
{
	inventory.emplace_back(ItemId);
}

std::list<int>* HaveItem::getItemFromInventory(std::list<int>* HaveItemIds)
{
	for (auto items : inventory) {
		HaveItemIds->emplace_back(items);
	}
	return HaveItemIds;
}

