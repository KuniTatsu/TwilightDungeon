#pragma once
#include"Actor.h"

class Enemy :public Actor {

public:
	//id(int)	EnemyType(int)	Enemy_Name(std::string)	HP(int)	Atack(int)	Defence(int)	Speed(int)	Gh(std::string)	Exp(int) level(int)

	Enemy(int Id, int Type, std::string Name, int Hp,int Atack, int Defence, int Speed, std::string Gh,int Exp);
	~Enemy()override;

	void Move()override;


};