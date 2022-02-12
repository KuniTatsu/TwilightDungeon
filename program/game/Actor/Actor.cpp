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

void Actor::setDir(int dir)
{
	if (dir == 0)mydir = dir::UP;
	else if (dir == 1)mydir = dir::RIGHT;
	else if (dir == 2)mydir = dir::DOWN;
	else if (dir == 3)mydir = dir::LEFT;
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

//装備を変更するとこれが呼ばれるため、永遠にステータスが増えてしまう→装備品変更時は別の処理を行う必要あり exステータスを0にする
void Actor::ChangeStatus(int StatusNum, int MoveNum, int StatusType)
{
	if (StatusType == EFFECT) {
		if (StatusNum == 0) {
			exHp += MoveNum;
			hp = baseHp + equipHp + exHp;
		}
		else if (StatusNum == 1) {
			exAtack += MoveNum;
			atack = baseAtack + equipAtack + exAtack;
		}
		else if (StatusNum == 2) {
			exDefence += MoveNum;
			defence = baseDefence + equipDefence + exDefence;
		}
		else if (StatusNum == 3) {
			exSpeed += MoveNum;
			speed = baseSpeed + equipSpeed + exSpeed;
		}
	}
	else {
		//装備アイテムを付け替える場合は装備アイテムによって付与されていたステータスを消去してから加算する
		equipHp = 0;
		equipAtack = 0;
		equipDefence = 0;
		equipSpeed = 0;
		if (StatusNum == 0) {
			equipHp += MoveNum;
			hp = baseHp + equipHp + exHp;
		}
		else if (StatusNum == 1) {
			equipAtack += MoveNum;
			atack = baseAtack + equipAtack + exAtack;
		}
		else if (StatusNum == 2) {
			equipDefence += MoveNum;
			defence = baseDefence + equipDefence + exDefence;
		}
		else if (StatusNum == 3) {
			equipSpeed += MoveNum;
			speed = baseSpeed + equipSpeed + exSpeed;
		}
	}
}
void Actor::TakeHpEffect(int HpMove)
{
	if (nowHp <= 0)return;
	nowHp += HpMove;
	if (nowHp < 0)nowHp = 0;
	if (nowHp > hp)nowHp = hp;

	nowHpVarWidth = nowHp / hp;
}
void Actor::Atack()
{
	t2k::Vector3 front;
	//目の前の対象を取得
	if (mydir == dir::UP) {
		front = gManager->WorldToLocal(pos) + t2k::Vector3(0, -1, 0);
	}
	else if (mydir == dir::RIGHT) {
		front = gManager->WorldToLocal(pos) + t2k::Vector3(1, 0, 0);
	}
	else if (mydir == dir::DOWN) {
		front = gManager->WorldToLocal(pos) + t2k::Vector3(0, 1, 0);
	}
	else if (mydir == dir::LEFT) {
		front = gManager->WorldToLocal(pos) + t2k::Vector3(-1, 0, 0);
	}

	//前にダメージを与えられる対象がいるなら
	//gManager->CalcDamage()
	gManager->TakeDamageToTarget(this, front);

}

bool Actor::Move()
{
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
