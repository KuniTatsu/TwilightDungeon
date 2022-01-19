#include "Enemy.h"
#include"DxLib.h"
#include "../GameManager.h"
#include"../Player.h"

extern GameManager* gManager;
Enemy::Enemy(int Id, int Type, std::string Name, int Hp, int Atack, int Defence, int Speed, std::string Gh, int Exp)
{
	id = Id;
	type = Type;
	name = Name;

	hp = Hp;
	atack = Atack;
	defence = Defence;
	speed = Speed;

	LoadDivGraph(Gh.c_str(), 12, 3, 4, 24, 32, gh);

	exp = Exp;
	//while (1) {
	//	pos = gManager->SetStartPos(0);
	//	//他のenemyと同じ座標でなければ抜ける
	//	
	//	
	//}
}

Enemy::~Enemy()
{
}

void Enemy::Move()
{
	//本来は左手法で動かす
	// 
	// 自分の位置を取得
	// 自分が部屋の中にいるなら部屋からつながる通路を取得
	// 部屋からつながる通路が2つ以上あるときは
	// 自分の位置から見て左側の通路へ向かって進む
	// 
	// 部屋からつながる通路が一つしか無いときは折り返す //いずれ敵同士の当たり判定も取るのでそのときは詰まってしまう
	// 
	
	//部屋にいるなら部屋のId,いなければ-1
	int nowPos = gManager->CheckIsThere(pos);
	//部屋のどこかにいるなら
	if (nowPos != -1) {
		//部屋の中の通路を検索


	}



	//キャラの位置がマップ上のどのチップか特定する
	t2k::Vector3 myNowPos = gManager->WorldToLocal(pos);
	if (--moveTimer > 0)return;
	moveTimer = MOVETIME;

	int rand = GetRand(3);

	if (rand == 3) {
		//キャラのチップの左のチップがWALLなら移動しない
		//int hoge = gManager->GetMapChip(myNowPos + t2k::Vector3(-1, 0, 0));
		if ((gManager->GetMapChip(myNowPos + t2k::Vector3(-1, 0, 0))) == 0)return;
		//キャラのチップの左のチップがPASSWAYなら移動する
		mydir = LEFT;
		pos.x -= 20;

	}
	else if (rand == 0) {

		//キャラのチップの左のチップがWALLなら移動しない
		if ((gManager->GetMapChip(myNowPos + t2k::Vector3(0, -1, 0))) == 0)return;
		//キャラのチップの左のチップがPASSWAYなら移動する
		mydir = UP;
		pos.y -= 20;
	}
	else if (rand == 1) {
		//キャラのチップの左のチップがWALLなら移動しない
		if ((gManager->GetMapChip(myNowPos + t2k::Vector3(1, 0, 0))) == 0)return;
		//キャラのチップの左のチップがPASSWAYなら移動する
		mydir = RIGHT;
		pos.x += 20;
	}
	else if (rand == 2) {
		//キャラのチップの左のチップがWALLなら移動しない
		if ((gManager->GetMapChip(myNowPos + t2k::Vector3(0, 1, 0))) == 0)return;
		//キャラのチップの左のチップがPASSWAYなら移動する
		mydir = DOWN;
		pos.y += 20;
	}
}


