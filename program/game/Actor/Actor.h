#pragma once
#include"../../library/t2klib.h"
#include<string>

class Actor {
public:
	Actor();
	virtual ~Actor();

	bool isLive = true;

	enum dir {
		UP,
		RIGHT,
		DOWN,
		LEFT
	};
	dir mydir = UP;

	const dir dires[4]{ dir::UP,dir::RIGHT,dir::DOWN,dir::LEFT };

	void setDir(const int dir);

	t2k::Vector3 pos = {};
	t2k::Vector3 skill = {};

	//外からステータスを取得する関数 StatusNum:0->HP,1->Atack,2->Defence,3->Speed,4->level
	int GetStatus(const int StatusNum);

	//名前の取得関数
	inline std::string GetName() {
		return name;
	};

	const enum  ChangeStatusType {
		EFFECT,
		EQUIP
	};

	//ステータス上昇の関数 StatusNum:0->HP,1->Atack,2->Defence,3->Speed,StatusType->0:effect,1:equipment
	void ChangeStatus(const int StatusNum, const int MoveNum, const int StatusType);
	//actId取得
	inline int GetActId() {
		return actId;
	}
	////mydir取得
	//inline dir getDir() {
	//	return mydir;
	//}

	//アニメーション座標の設定
	void SetAnimPos();

	t2k::Vector3 effectPos;

	//攻撃アニメーションの決定
	bool SetDrawAnim();
	//攻撃アニメーションの描画
	void DrawAttackAnim();

	//Hpの変動が外部から与えられた時に使う関数
	void TakeHpEffect(const int HpMove);

	virtual void Atack();
	virtual bool Move();

	const enum class AnimPattern {
		WALK,
		EFFECT
	};

	//アニメーションさせたいgh配列,描画スピード,描画最大枚数を引数に持つアニメーション関数
	void Anim(int* DrawGhArr, int Speed, int MaxIndex, int& DrawGh);

	virtual void Update();
	virtual void Draw();
protected:
	int actId;
	//enum dir {
	//	UP,
	//	RIGHT,
	//	DOWN,
	//	LEFT
	//};
	//dir mydir = UP;

	int id = 0;
	int type = 0;
	std::string name = "";
	//装備, アイテムによる追加ステータス
	int equipHp = 0;
	int equipAtack = 0;
	int equipDefence = 0;
	int equipSpeed = 0;

	//その他要因による追加ステータス
	int exHp = 0;
	int exAtack = 0;
	int exDefence = 0;
	int exSpeed = 0;

	//基礎ステータス
	int baseHp = 0;
	int baseAtack = 0;
	int baseDefence = 0;
	int baseSpeed = 0;
	//描画ステータス
	int hp = 0;
	int atack = 0;
	int defence = 0;
	int speed = 0;

	//上右下左の順で3枚ずつ キャラ画像
	int gh[12];

	//攻撃エフェクト
	int effectGh[5];

	int level = 1;

	float nowHp = hp;

	int nowHpVar_gh = 0;
	int hpVar_gh = 0;

	const float VARWIDTH = 30;
	double nowHpVarWidth = nowHp / hp;


	int actSpeed = 20;
	int maxMotionIndex = 3;


	//通常時の描画するアニメーション画像
	int drawGh = 0;
	int actWait = actSpeed;
	int actIndex = 0;

	t2k::Vector3 animPos = { 0,0,0 };


	int moveTimer = 0;
	const int MOVETIME = 60;



};