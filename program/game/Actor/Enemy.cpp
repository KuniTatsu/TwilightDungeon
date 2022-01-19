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
	//	//����enemy�Ɠ������W�łȂ���Δ�����
	//	
	//	
	//}
}

Enemy::~Enemy()
{
}

void Enemy::Move()
{
	//�{���͍���@�œ�����
	// 
	// �����̈ʒu���擾
	// �����������̒��ɂ���Ȃ畔������Ȃ���ʘH���擾
	// ��������Ȃ���ʘH��2�ȏ゠��Ƃ���
	// �����̈ʒu���猩�č����̒ʘH�֌������Đi��
	// 
	// ��������Ȃ���ʘH������������Ƃ��͐܂�Ԃ� //������G���m�̓����蔻������̂ł��̂Ƃ��͋l�܂��Ă��܂�
	// 
	
	//�����ɂ���Ȃ畔����Id,���Ȃ����-1
	int nowPos = gManager->CheckIsThere(pos);
	//�����̂ǂ����ɂ���Ȃ�
	if (nowPos != -1) {
		//�����̒��̒ʘH������


	}



	//�L�����̈ʒu���}�b�v��̂ǂ̃`�b�v�����肷��
	t2k::Vector3 myNowPos = gManager->WorldToLocal(pos);
	if (--moveTimer > 0)return;
	moveTimer = MOVETIME;

	int rand = GetRand(3);

	if (rand == 3) {
		//�L�����̃`�b�v�̍��̃`�b�v��WALL�Ȃ�ړ����Ȃ�
		//int hoge = gManager->GetMapChip(myNowPos + t2k::Vector3(-1, 0, 0));
		if ((gManager->GetMapChip(myNowPos + t2k::Vector3(-1, 0, 0))) == 0)return;
		//�L�����̃`�b�v�̍��̃`�b�v��PASSWAY�Ȃ�ړ�����
		mydir = LEFT;
		pos.x -= 20;

	}
	else if (rand == 0) {

		//�L�����̃`�b�v�̍��̃`�b�v��WALL�Ȃ�ړ����Ȃ�
		if ((gManager->GetMapChip(myNowPos + t2k::Vector3(0, -1, 0))) == 0)return;
		//�L�����̃`�b�v�̍��̃`�b�v��PASSWAY�Ȃ�ړ�����
		mydir = UP;
		pos.y -= 20;
	}
	else if (rand == 1) {
		//�L�����̃`�b�v�̍��̃`�b�v��WALL�Ȃ�ړ����Ȃ�
		if ((gManager->GetMapChip(myNowPos + t2k::Vector3(1, 0, 0))) == 0)return;
		//�L�����̃`�b�v�̍��̃`�b�v��PASSWAY�Ȃ�ړ�����
		mydir = RIGHT;
		pos.x += 20;
	}
	else if (rand == 2) {
		//�L�����̃`�b�v�̍��̃`�b�v��WALL�Ȃ�ړ����Ȃ�
		if ((gManager->GetMapChip(myNowPos + t2k::Vector3(0, 1, 0))) == 0)return;
		//�L�����̃`�b�v�̍��̃`�b�v��PASSWAY�Ȃ�ړ�����
		mydir = DOWN;
		pos.y += 20;
	}
}


