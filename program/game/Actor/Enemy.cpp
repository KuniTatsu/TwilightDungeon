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


