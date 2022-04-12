#include "Actor.h"
#include"DxLib.h"
#include"../GameManager.h"
#include"../Camera.h"
#include"../Skill.h"

extern GameManager* gManager;

Actor::Actor()
{
}

Actor::~Actor()
{
}

void Actor::setDir(int dir)
{
	mydir = dires[dir];
}

int Actor::GetStatus(int StatusNum)
{
	//StatusNum:0->HP,1->Atack,2->Defence,3->Speed,4->Name
	if (StatusNum == 0) {
		return nowHp;
	}
	else if (StatusNum == 1) {
		return attack;
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
			exAttack += MoveNum;
			attack = baseAttack + equipAttack + exAttack;
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
	//装備アイテム
	else {
		
		if (StatusNum == 0) {
			equipHp += MoveNum;
			hp = baseHp + equipHp + exHp;
		}
		else if (StatusNum == 1) {
			equipAttack += MoveNum;
			attack = baseAttack + equipAttack + exAttack;
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
//装備アイテムによって付与されていたステータスを消去する関数
void Actor::RemoveStatus(const int StatusNum, const int MoveNum)
{
	if (StatusNum == 0) {
		equipHp -= MoveNum;
		hp = baseHp + equipHp + exHp;
	}
	else if (StatusNum == 1) {
		equipAttack -= MoveNum;
		attack = baseAttack + equipAttack + exAttack;
	}
	else if (StatusNum == 2) {
		equipDefence -= MoveNum;
		defence = baseDefence + equipDefence + exDefence;
	}
	else if (StatusNum == 3) {
		equipSpeed -= MoveNum;
		speed = baseSpeed + equipSpeed + exSpeed;
	}
}
//今の向きの1マス先の座標を取得する
void Actor::SetAnimPos()
{
	//ローカル座標の取得
	t2k::Vector3 localEffectPos = gManager->WorldToLocal(pos) + gManager->GetVecter(mydir);

	//描画座標がほしい posは描画座標
	effectPos = gManager->LocalToWorld(localEffectPos.x, localEffectPos.y);
}
//bool Actor::SetDrawAnim()
//{
//	//プレイヤーの座標を一定分方向の向きに移動し、その後もとに戻る
//
//	//今向いている向きに発生させるエフェクトを取得
//	Anim(effectGh, 5, 5, attackDrawGh, AnimPattern::EFFECT);
//	//もしアニメーションが終わっていれば
//	if (attackDrawGh == effectGh[4])
//	{
//		actIndexEffect = 0;
//		return true;
//	}
//
//	return false;
//}
//void Actor::DrawAttackAnim()
//{
//	DrawRotaGraph(effectPos.x - gManager->camera->cameraPos.x, effectPos.y - gManager->camera->cameraPos.y, 1, 0, attackDrawGh, true);
//
//}
void Actor::TakeHpEffect(int HpMove)
{
	if (nowHp <= 0)return;
	nowHp += HpMove;
	if (nowHp < 0)nowHp = 0;
	if (nowHp > hp)nowHp = hp;

	nowHpVarWidth = nowHp / hp;
}
void Actor::Attack()
{
	//方向を引数にVector3を返す関数を用意する

	t2k::Vector3 front;
	//目の前の対象を取得
	front = gManager->WorldToLocal(pos) + gManager->GetVecter(mydir);

	//目の前の対象を取得,ダメージ処理を行う
	gManager->DealDamageToTarget(this, front);

}

void Actor::SkillAttack(Skill* usedSkill)
{
	t2k::Vector3 front;
	//目の前の対象を取得
	front = gManager->WorldToLocal(pos) + gManager->GetVecter(mydir);

	//目の前の対象を取得,ダメージ処理を行う
	gManager->DealSkillDamageToTarget(this, front, usedSkill);

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
	Anim(gh, actSpeed, maxMotionIndex, drawGh);
	//描画座標+アニメーション位置補正(補完座標)-カメラ補正
	DrawRotaGraph(pos.x  - gManager->camera->cameraPos.x, pos.y  - 10 - gManager->camera->cameraPos.y, gManager->graphEx, 0, drawGh, true);
}

void Actor::Anim(int* DrawGhArr, int Speed, int MaxIndex, int& DrawGh)
{
	if (--actWait <= 0) {
		actIndex++;
		actWait = Speed;
		actIndex %= MaxIndex;
	}
	DrawGh = DrawGhArr[actIndex + 3 * mydir];
}
