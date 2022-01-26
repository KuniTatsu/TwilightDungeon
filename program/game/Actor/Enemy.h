#pragma once
#include"Actor.h"

class Enemy :public Actor {

public:
	//id(int)	EnemyType(int)	Enemy_Name(std::string)	HP(int)	Atack(int)	Defence(int)	Speed(int)	Gh(std::string)	Exp(int) level(int)

	Enemy(int Id, int Type, std::string Name, int Hp, int Atack, int Defence, int Speed, std::string Gh, int Exp);
	~Enemy()override;

	void Move()override;

	//���̌������猩�č����̕������擾����֐�
	int GetMyLeft(int MyDir);

private:
	//�ړI�n
	t2k::Vector3 ChasePoint = {};

	//�ړI�n�Z�b�g�t���O
	bool isSetChasePoint = false;

	t2k::Vector3 myNowPos = {};
	//�ݒ肳�ꂽ�ړI�n�Ɍ������֐�
	void MoveChasePoint();

	//dir����݂ē���̕�������΍��W�n�̂ǂ̕������擾����֐� �Ƃ肠�������E�����ق���
	int GetDir(const int dir, const int getDir);

	//dir������1�i�ފ֐�(�ړ��\���肠��)
	bool MoveToDir(const int dir, t2k::Vector3& nowPos);

	//�g�p����
	//dir������1�i�ފ֐�(�ړ��\����Ȃ� �g�p����ۂ͈ړ��\�Ȃ��Ƃ�ۏ؂��邱��)
	void DegradedMoveToDir(const int dir);

	//dir�̕�����1�i�񂾃}�X���ړ��\���ǂ����m�F����֐�
	bool CheckCanMove(const int dir, const t2k::Vector3 nowPos);


	enum /*class*/ CheckDir {
		UP,
		RIGHT,
		DOWN,
		LEFT
	};

	//������dir���猩�ē���̕�����1�i�񂾃}�X���ړ��\���ǂ����m�F����֐�
	bool CheckCanMoveToDir(const int dir, const t2k::Vector3 nowPos, const int checkDir);

	//�L�����N�^�[�Ɍ������Ĉړ�����AI�֐�
	void MoveToPlayer();



};