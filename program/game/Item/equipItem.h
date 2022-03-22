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

	//settype:0->RandomRange,1->そのままの値で生成
	equipItem(int Id, int ItemType, std::string ItemName, int Saturation, int Heal, int HitDamage,
		std::string Gh, int SubId, int SellPrice, std::string Desc, int Hp, int Attack, int Defence, int Speed,int SetType);

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
	//subId取得
	inline int GetSubId(){
		return subId;
	}

	//インベントリ内の装備アイテムのステータス描画
	//arg1:描画X座標
	//arg2:描画Y座標
	//arg3:選択中アイテムのsubId
	void DrawEquipItemStatus(int x, int y,int subId);

private:

	int hp = 0;
	int attack = 0;
	int defence = 0;
	int speed = 0;

	int subId = 0;

	bool isEquiped = false;

	//描画用間隔の定数
	const int DIS = 30;

	enum class STATUS : uint32_t {
		HP,
		ATTACK,
		DEFENCE,
		SPEED,
		STATUSMAX
	};
	//アイテムステータスの配列
	int equipStatus[static_cast<uint32_t>(STATUS::STATUSMAX)];


	//アイテムのステータスを一定範囲内でランダムにする関数
	int SetRandomStatus(int CenterNum);

	//装備中アイテムとの差を取得する関数
	void SetDifNumEquipment(int subId);
	//装備中アイテムとの差
	int difNum[static_cast<uint32_t>(STATUS::STATUSMAX)] = {0,0,0,0};
};