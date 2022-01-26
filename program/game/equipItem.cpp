#include "equipItem.h"


equipItem::equipItem(int Id, int ItemType, std::string ItemName, int Saturation, int Heal, int HitDamage, std::string Gh, int Hp, int Attack, int Defence, int Speed)
{
	hp = Hp;
	attack = Attack;
	defence = Defence;
	speed = Speed;

}

equipItem::~equipItem()
{
}
