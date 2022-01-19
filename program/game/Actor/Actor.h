#pragma once
#include"../../library/t2klib.h"
#include<string>

class Actor {

public:
	Actor();
	virtual ~Actor();

	//id(int)	EnemyType(int)	Enemy_Name(std::string)	HP(int)	Atack(int)	Defence(int)	Speed(int)	Gh(std::string)	Exp(int)

	int id = 0;
	int type = 0;
	std::string name = "";

	int hp = 0;
	int atack = 0;
	int defence = 0;
	int speed = 0;

	//è„âEâ∫ç∂ÇÃèáÇ≈3ñáÇ∏Ç¬
	int gh[12];

	int exp = 0;

	int level = 1;

	bool isLive = true;

	enum dir{
		UP,
		RIGHT,
		DOWN,
		LEFT
	};
	dir mydir = UP;
	
	t2k::Vector3 pos = {};
	t2k::Vector3 skill = {};


	virtual void Atack();
	virtual void Move();
	void Anim();

	virtual void Update();
	virtual void Draw();
protected:
	const int ACT_SPEED = 20;
	const int MAX_MOTION_INDEX = 3;
	int act_wait = ACT_SPEED;
	int act_index = 0;

	int drawGh = 0;


	int moveTimer = 0;
	const int MOVETIME = 60;

};