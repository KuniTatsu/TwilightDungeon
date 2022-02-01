#pragma once
#include"../../library/t2klib.h"
#include<string>

class Actor {

public:
	Actor();
	virtual ~Actor();

	//id(int)	EnemyType(int)	Enemy_Name(std::string)	HP(int)	Atack(int)	Defence(int)	Speed(int)	Gh(std::string)	Exp(int)


	bool isLive = true;

	enum dir {
		UP,
		RIGHT,
		DOWN,
		LEFT
	};
	dir mydir = UP;

	t2k::Vector3 pos = {};
	t2k::Vector3 skill = {};

	//外からステータスを取得する関数 StatusNum:0->HP,1->Atack,2->Defence,3->Speed,4->level
	int GetStatus(int StatusNum);
	//名前の取得関数
	std::string GetName();

	//ステータス上昇のテスト関数 StatusNum:0->HP,1->Atack,2->Defence,3->Speed
	void ChangeStatus(int StatusNum, int MoveNum);

	virtual void Atack();
	virtual bool Move();
	void Anim();

	virtual void Update();
	virtual void Draw();
protected:
	int id = 0;
	int type = 0;
	std::string name = "";

	//装備,アイテム,その他要因による追加ステータス
	int exHp = 0;
	int exAtack = 0;
	int exDefence = 0;
	int exSpeed = 0;

	//基礎ステータス
	int baseHp = hp + exHp;
	int baseAtack = atack + exAtack;
	int baseDefence = defence + exDefence;
	int baseSpeed = speed + exSpeed;


	//描画ステータス
	int hp = hp + exHp;
	int atack = atack + exAtack;
	int defence = defence + exDefence;
	int speed = speed + exSpeed;

	//上右下左の順で3枚ずつ
	int gh[12];

	int level = 1;

	float nowHp = hp;

	int nowHpVar_gh = 0;
	int hpVar_gh = 0;

	const float VARWIDTH = 30;
	double nowHpVarWidth = nowHp / hp;


	const int ACT_SPEED = 20;
	const int MAX_MOTION_INDEX = 3;
	int act_wait = ACT_SPEED;
	int act_index = 0;

	int drawGh = 0;


	int moveTimer = 0;
	const int MOVETIME = 60;

};