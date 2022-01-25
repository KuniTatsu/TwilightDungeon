#include "Item.h"
#include"GameManager.h"

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
