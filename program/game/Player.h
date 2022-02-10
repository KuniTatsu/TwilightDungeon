#pragma once
#include"../library/t2klib.h"
#include"Actor/Actor.h"

class equipItem;
class Player :public Actor
{
public:
	Player(t2k::Vector3 StartPos, float Hp, int Atack, int Defence, int Speed,int ActId);
	~Player();

	t2k::Vector3 pos = {};

	//Hpの変動が外部から与えられた時に使う関数
	void TakeHpEffect(int HpMove);

	//満腹度とHPの変更関数(主に食料アイテムの使用で使う)
	void ChangeBaseStatus(int ManpukuMove, int HpMove);

	//装備欄の変更関数
	void ChangeEquipItem(equipItem* item);

	bool Move()override;

	void Draw()override;


	void DrawPlayerStatus();
private:

	int p_gh = 0;

	int manpuku = 100;

	/*class EquipItem {
		Item* Weapon = nullptr;
		Item* Head = nullptr;
		Item* Chest = nullptr;
		Item* Glove = nullptr;
		Item* Boots = nullptr;
		Item* Shield = nullptr;
	};*/
	equipItem* myEquip[6] = { nullptr,nullptr, nullptr, nullptr, nullptr, nullptr };

	const std::string equipName[6] = { "WEAPON","HEAD","CHEST","GLOVE","BOOTS","SHIELD" };


	//std::string name = "Player";

	////装備,アイテム,その他要因による追加ステータス
	//int exHp = 0;
	//int exAtack = 0;
	//int exHDefence = 0;
	//int exSpeed = 0;

	////基礎ステータス
	//float hp = 100 + exHp;
	//int atack = 10 + exAtack;
	//int defence = 10 + exHDefence;
	//int speed = 10 + exSpeed;

	/*float nowHp = hp;

	int nowHpVar_gh = 0;
	int hpVar_gh = 0;

	const float VARWIDTH = 30;
	double nowHpVarWidth = nowHp / hp;*/

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

	void DashToDir(int dir, t2k::Vector3 mapPos);
	void MoveToDir(int dir, t2k::Vector3 mapPos);



};
