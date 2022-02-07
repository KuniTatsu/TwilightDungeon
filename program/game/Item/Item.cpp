#include "Item.h"
#include"../GameManager.h"

extern GameManager* gManager;

Item::Item(int Id, int ItemType, std::string ItemName, int Saturation, int Heal, int HitDamage, std::string Gh)
{
	id = Id;
	itemType = ItemType;
	itemName = ItemName;
	amountOfSaturation = Saturation;
	amountOfHeal = Heal;
	hitDamage = HitDamage;

	gh = gManager->LoadGraphEx(Gh);

}

Item::~Item()
{
}

int Item::getItemId()
{
	return id;
}

int Item::getItemData(int ColumnId)
{
	if (ColumnId == 0)return id;
	else if (ColumnId == 1)return itemType;
	else if (ColumnId == 2)return amountOfSaturation;
	else if (ColumnId == 3)return amountOfHeal;
	else if (ColumnId == 4)return hitDamage;
}

std::string Item::getItemName()
{
	return itemName;
}

//void Item::DrawItemData()
//{
//
//}
