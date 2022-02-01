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

int Actor::GetStatus(int StatusNum)
{
	//StatusNum:0->HP,1->Atack,2->Defence,3->Speed,4->Name
	if (StatusNum == 0) {
		return nowHp;
	}
	else if (StatusNum == 1) {
		return atack;
	}
	else if (StatusNum == 2) {
		return defence;
	}
	else if (StatusNum == 3) {
		return speed;
	}
	return -1;
}

std::string Actor::GetName()
{
	return name;
}

void Actor::ChangeStatus(int StatusNum, int MoveNum)
{
	if (StatusNum == 0) {
		exHp += MoveNum;
		hp = baseHp + exHp;
	}
	else if (StatusNum == 1) {
		exAtack += MoveNum;
		atack = baseAtack + exAtack;
	}
	else if (StatusNum == 2) {
		exDefence += MoveNum;
		defence = baseDefence + exDefence;
	}
	else if (StatusNum == 3) {
		exSpeed += MoveNum;
		speed = baseSpeed + exSpeed;
	}
}

void Actor::Atack()
{



}

bool Actor::Move()
{
	////�{���͍���@�œ�����
	//// 
	//// 
	////�L�����̈ʒu���}�b�v��̂ǂ̃`�b�v�����肷��
	//t2k::Vector3 myNowPos = gManager->WorldToLocal(pos);
	//if (--moveTimer > 0)return true;
	//moveTimer = MOVETIME;

	//int rand = GetRand(3);

	//if (rand == 3) {
	//	//�L�����̃`�b�v�̍��̃`�b�v��WALL�Ȃ�ړ����Ȃ�
	//	//int hoge = gManager->GetMapChip(myNowPos + t2k::Vector3(-1, 0, 0));
	//	if ((gManager->GetMapChip(myNowPos + t2k::Vector3(-1, 0, 0))) == 0)return;
	//	//�L�����̃`�b�v�̍��̃`�b�v��PASSWAY�Ȃ�ړ�����
	//	mydir = LEFT;
	//	pos.x -= 20;

	//}
	//else if (rand == 0) {

	//	//�L�����̃`�b�v�̍��̃`�b�v��WALL�Ȃ�ړ����Ȃ�
	//	if ((gManager->GetMapChip(myNowPos + t2k::Vector3(0, -1, 0))) == 0)return;
	//	//�L�����̃`�b�v�̍��̃`�b�v��PASSWAY�Ȃ�ړ�����
	//	mydir = UP;
	//	pos.y -= 20;
	//}
	//else if (rand == 1) {
	//	//�L�����̃`�b�v�̍��̃`�b�v��WALL�Ȃ�ړ����Ȃ�
	//	if ((gManager->GetMapChip(myNowPos + t2k::Vector3(1, 0, 0))) == 0)return;
	//	//�L�����̃`�b�v�̍��̃`�b�v��PASSWAY�Ȃ�ړ�����
	//	mydir = RIGHT;
	//	pos.x += 20;
	//}
	//else if (rand == 2) {
	//	//�L�����̃`�b�v�̍��̃`�b�v��WALL�Ȃ�ړ����Ȃ�
	//	if ((gManager->GetMapChip(myNowPos + t2k::Vector3(0, 1, 0))) == 0)return;
	//	//�L�����̃`�b�v�̍��̃`�b�v��PASSWAY�Ȃ�ړ�����
	//	mydir = DOWN;
	//	pos.y += 20;
	//}

	return true;
}

void Actor::Update()
{
	//Move();
}

void Actor::Draw()
{
	Anim();
	DrawRotaGraph(pos.x - gManager->camera->cameraPos.x, pos.y - 10 - gManager->camera->cameraPos.y, 1, 0, drawGh, true);
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
