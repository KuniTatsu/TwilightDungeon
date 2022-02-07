#pragma once
#include"Item.h"

class equipItem :public Item {

public:
	//HP(int)	Atack(int)	Defence(int)	Speed(int)

	equipItem(int Id, int ItemType, std::string ItemName, int Saturation, int Heal, int HitDamage, std::string Gh, int Hp, int Attack, int Defence, int Speed);

	~equipItem()override;

	int getItemData(int ColumnId)override;
private:

	int hp = 0;
	int attack = 0;
	int defence = 0;
	int speed = 0;


};