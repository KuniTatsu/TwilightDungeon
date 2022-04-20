///*****Description*****
///生き物に関するクラス
///プレイヤーとエネミーを派生先に持つ。
/// 各種ステータスと描画関数を持つ
///*********************
#pragma once
#include"../../library/t2klib.h"
#include<string>

class Skill;
class PassiveMod;

class Actor {
public:
	Actor();
	virtual ~Actor();

	bool isLive = true;

	enum dir {
		UP,
		RIGHT,
		DOWN,
		LEFT,
		MAX
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

	//装備ステータスの除去
	void RemoveStatus(const int StatusNum, const int MoveNum);

	//装備ステータスの全除去
	inline void RemoveAllEquipStatus() {
		equipHp = 0;
		equipAttack = 0;
		equipDefence = 0;
		equipSpeed = 0;
	}

	//レベルステータスの全除去
	inline void RemoveAllLevelStatus() {
		exHp = 0;
		exAttack = 0;
		exDefence = 0;
		exSpeed = 0;
	}

	//actId取得
	inline int GetActId() {
		return actId;
	}

	//アニメーション座標の設定
	void SetAnimPos();

	t2k::Vector3 effectPos;

	//攻撃アニメーションの決定
	bool SetDrawAnim();
	//攻撃アニメーションの描画
	void DrawAttackAnim();

	//Hpの変動が外部から与えられた時に使う関数
	void TakeHpEffect(const int HpMove);

	virtual void Attack();
	virtual void SkillAttack(Skill* usedSkill);

	virtual bool Move();

	const enum class AnimPattern {
		WALK,
		EFFECT
	};

	//アニメーションさせたいgh配列,描画スピード,描画最大枚数を引数に持つアニメーション関数
	void Anim(int* DrawGhArr, int Speed, int MaxIndex, int& DrawGh);

	//所持スキルリストを取得する
	inline std::vector<Skill*>& GetSkillList() {
		return mySkill;
	}

	virtual void Update();
	virtual void Draw();
protected:
	int actId;

	int id = 0;
	int type = 0;
	std::string name = "";
	//装備, アイテムによる追加ステータス
	int equipHp = 0;
	int equipAttack = 0;
	int equipDefence = 0;
	int equipSpeed = 0;

	//その他要因による追加ステータス
	int exHp = 0;
	int exAttack = 0;
	int exDefence = 0;
	int exSpeed = 0;

	//基礎ステータス
	int baseHp = 0;
	int baseAttack = 0;
	int baseDefence = 0;
	int baseSpeed = 0;
	//描画ステータス
	int hp = 0;
	int attack = 0;
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
	double nowHpVarWidth = static_cast<double>(nowHp / hp);


	int actSpeed = 20;
	int maxMotionIndex = 3;


	//通常時の描画するアニメーション画像
	int drawGh = 0;
	int actWait = actSpeed;
	int actIndex = 0;

	t2k::Vector3 animPos = { 0,0,0 };


	int moveTimer = 0;
	const int MOVETIME = 60;

	//スキル関連
	//所持スキル
	std::vector<Skill*>mySkill = {};

	//所持パッシブ効果
	std::vector<PassiveMod*>myMod = {};

};