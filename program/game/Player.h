#pragma once
#include"../library/t2klib.h"

class Player
{
public:
	Player(t2k::Vector3 StartPos);
	~Player();

	t2k::Vector3 pos = {};

	bool Move();
	
	void Draw();
private:

	int gh = 0;

	//�˂�����܂ő���t���O
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
