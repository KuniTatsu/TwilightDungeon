#include "equipItem.h"

equipItem::equipItem(int Id, int ItemType, std::string ItemName, int Saturation, int Heal, int HitDamage, std::string Gh, int SubId, std::string Desc, int Hp, int Attack, int Defence, int Speed)
	: Item(Id, ItemType, ItemName, Saturation, Heal, HitDamage, Gh, Desc)
{
	hp = Hp;
	attack = Attack;
	defence = Defence;
	speed = Speed;
	subId = SubId;
}

equipItem::~equipItem()
{
}

int equipItem::getItemData(int ColumnId)
{
	if (ColumnId == 0)return id;
	else if (ColumnId == 1)return itemType;
	else if (ColumnId == 2)return amountOfSaturation;
	else if (ColumnId == 3)return amountOfHeal;
	else if (ColumnId == 4)return hitDamage;
	else if (ColumnId == 5)return hp;
	else if (ColumnId == 6)return attack;
	else if (ColumnId == 7)return defence;
	else if (ColumnId == 8)return speed;
	else if (ColumnId == 9)return subId;
}
