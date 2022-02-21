///*****Description*****
///装備アイテムに関するクラス
///アイテムを継承している
/// 装備アイテム固有のステータス保持と、
/// 装備状況の変更関数を持つ
///*********************
#pragma once
#include"Item.h"

class equipItem :public Item {

public:
	//HP(int)	Atack(int)	Defence(int)	Speed(int)

	equipItem(int Id, int ItemType, std::string ItemName, int Saturation, int Heal, int HitDamage, std::string Gh, int SubId, std::string Desc, int Hp, int Attack, int Defence, int Speed);

	~equipItem()override;
	//ColumnId: 0->id,1->type,2->manpukudo,3->heal,4->hitdamage,5->hp,6->attack,7->defence,8->speed,9->subid,
	int getItemData(int ColumnId)override;

	//生成時に必要なint型アイテムデータをvectorで返す関数 Id,ItemType,Saturation,Heal,HitDamage,subId,hp,attack,defence,speed,
	std::vector<int> GetAllIntData()override;

	//アイテムの装備状況を変更する関数
	void ChangeEquip();
	inline bool GetIsEquiped() {
		return isEquiped;
	}
	
private:

	int hp = 0;
	int attack = 0;
	int defence = 0;
	int speed = 0;

	int subId = 0;

	bool isEquiped = false;
};