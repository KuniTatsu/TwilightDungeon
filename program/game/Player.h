#pragma once
#include"../library/t2klib.h"
#include"Actor/Actor.h"

class equipItem;
class Player : public Actor
{
public:
	Player(){}
	Player(const t2k::Vector3& StartPos, float Hp, int Atack, int Defence, int Speed, int ActId);
	~Player();

	//t2k::Vector3 pos = {};
	bool skip = false;


	//満腹度とHPの変更関数(主に食料アイテムの使用で使う)
	void ChangeBaseStatus(const int ManpukuMove, const int HpMove);

	//装備欄の変更関数
	void ChangeEquipItem(equipItem* item);

	//装備を外す関数
	void RemoveEquipItem(equipItem* item);

	//装備によるステータス上昇値を獲得する関数
	void GetSumStatusFromEquipment();


	bool Move()override;

	//void Draw()override;
	void HpVarDraw();

	//経験値の獲得関数
	void AddExp(const int num);

	//debug
	void DrawPlayerStatus();
private:

	int p_gh = 0;

	int manpuku = 100;

	int nowExp = 0;
	int nextLevelExp = 100;

	void SetLevelStatus();
	void SetNextExp();

	equipItem* myEquip[6] = { nullptr,nullptr, nullptr, nullptr, nullptr, nullptr };

	const std::string equipName[6] = { "WEAPON","HEAD","CHEST","GLOVE","BOOTS","SHIELD" };
	//hp,attack,defence,speed
	int statuses[4] = { 0,0,0,0 };

	//突き当りまで走るフラグ
	bool left = false;
	bool up = false;
	bool right = false;
	bool down = false;

	enum Dir {
		LEFT,
		UP,
		RIGHT,
		DOWN
	};

	void DashToDir(const int dir, const t2k::Vector3 mapPos);
	void MoveToDir(const int dir, const t2k::Vector3 mapPos);



};
