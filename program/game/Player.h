#pragma once
#include"../library/t2klib.h"
#include"Actor/Actor.h"

class Player:public Actor
{
public:
	Player(t2k::Vector3 StartPos,float Hp,int Atack,int Defence,int Speed);
	~Player();

	t2k::Vector3 pos = {};

	//Hp�̕ϓ����O������^����ꂽ���Ɏg���֐�
	void TakeHpEffect(int HpMove);

	bool Move()override;

	void Draw()override;
private:

	int p_gh = 0;

	//std::string name = "Player";

	////����,�A�C�e��,���̑��v���ɂ��ǉ��X�e�[�^�X
	//int exHp = 0;
	//int exAtack = 0;
	//int exHDefence = 0;
	//int exSpeed = 0;

	////��b�X�e�[�^�X
	//float hp = 100 + exHp;
	//int atack = 10 + exAtack;
	//int defence = 10 + exHDefence;
	//int speed = 10 + exSpeed;

	/*float nowHp = hp;

	int nowHpVar_gh = 0;
	int hpVar_gh = 0;

	const float VARWIDTH = 30;
	double nowHpVarWidth = nowHp / hp;*/

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
