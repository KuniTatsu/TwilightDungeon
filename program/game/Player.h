#pragma once
#include"../library/t2klib.h"

class Player
{
public:
	Player(t2k::Vector3 StartPos);
	~Player();

	t2k::Vector3 pos = {};

	void Move();
	

private:

	int gh = 0;



};
