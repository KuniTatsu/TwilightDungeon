#include "Actor.h"
#include"DxLib.h"
#include"../GameManager.h"
#include"../Camera.h"

extern GameManager* gManager;

Actor::Actor()
{
}

Actor::~Actor()
{
}

void Actor::Atack()
{



}

void Actor::Move()
{
	//�{���͍���@�œ�����
	// 
	// 
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

void Actor::Update()
{
	//Move();
}

void Actor::Draw()
{
	Anim();
	DrawRotaGraph(pos.x - gManager->camera->cameraPos.x, pos.y-10 - gManager->camera->cameraPos.y, 1, 0, drawGh, true);
}

void Actor::Anim()
{
	if (--act_wait <= 0) {
		act_index++;
		act_wait = ACT_SPEED;
		act_index %= MAX_MOTION_INDEX;
	}
	drawGh = gh[act_index + 3 * mydir];
}
