#pragma once
#include"Actor.h"

class Enemy :public Actor {

public:
	//id(int)	EnemyType(int)	Enemy_Name(std::string)	HP(int)	Atack(int)	Defence(int)	Speed(int)	Gh(std::string)	Exp(int) level(int)

	Enemy(int Id, int Type, std::string Name, int Hp, int Atack, int Defence, int Speed, std::string Gh, int Exp);
	~Enemy()override;

	void Move()override;

	//今の向きから見て左側の方向を取得する関数
	int GetMyLeft(int MyDir);

private:
	//目的地
	t2k::Vector3 ChasePoint = {};

	//目的地セットフラグ
	bool isSetChasePoint = false;

	t2k::Vector3 myNowPos = {};
	//設定された目的地に向かう関数
	void MoveChasePoint();

	//dirからみて特定の方向が絶対座標系のどの方向か取得する関数 とりあえず左右だけほしい
	int GetDir(const int dir, const int getDir);

	//dir方向に1進む関数(移動可能判定あり)
	bool MoveToDir(const int dir, t2k::Vector3& nowPos);

	//使用注意
	//dir方向に1進む関数(移動可能判定なし 使用する際は移動可能なことを保証すること)
	void DegradedMoveToDir(const int dir);

	//dirの方向に1進んだマスが移動可能かどうか確認する関数
	bool CheckCanMove(const int dir, const t2k::Vector3 nowPos);


	enum /*class*/ CheckDir {
		UP,
		RIGHT,
		DOWN,
		LEFT
	};

	//自分のdirから見て特定の方向に1進んだマスが移動可能かどうか確認する関数
	bool CheckCanMoveToDir(const int dir, const t2k::Vector3 nowPos, const int checkDir);

	//キャラクターに向かって移動するAI関数
	void MoveToPlayer();



};