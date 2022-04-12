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

//������ύX����Ƃ��ꂪ�Ă΂�邽�߁A�i���ɃX�e�[�^�X�������Ă��܂��������i�ύX���͕ʂ̏������s���K�v���� ex�X�e�[�^�X��0�ɂ���
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
	//�����A�C�e��
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
//�����A�C�e���ɂ���ĕt�^����Ă����X�e�[�^�X����������֐�
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
//���̌�����1�}�X��̍��W���擾����
void Actor::SetAnimPos()
{
	//���[�J�����W�̎擾
	t2k::Vector3 localEffectPos = gManager->WorldToLocal(pos) + gManager->GetVecter(mydir);

	//�`����W���ق��� pos�͕`����W
	effectPos = gManager->LocalToWorld(localEffectPos.x, localEffectPos.y);
}
//bool Actor::SetDrawAnim()
//{
//	//�v���C���[�̍��W����蕪�����̌����Ɉړ����A���̌���Ƃɖ߂�
//
//	//�������Ă�������ɔ���������G�t�F�N�g���擾
//	Anim(effectGh, 5, 5, attackDrawGh, AnimPattern::EFFECT);
//	//�����A�j���[�V�������I����Ă����
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
	//������������Vector3��Ԃ��֐���p�ӂ���

	t2k::Vector3 front;
	//�ڂ̑O�̑Ώۂ��擾
	front = gManager->WorldToLocal(pos) + gManager->GetVecter(mydir);

	//�ڂ̑O�̑Ώۂ��擾,�_���[�W�������s��
	gManager->DealDamageToTarget(this, front);

}

void Actor::SkillAttack(Skill* usedSkill)
{
	t2k::Vector3 front;
	//�ڂ̑O�̑Ώۂ��擾
	front = gManager->WorldToLocal(pos) + gManager->GetVecter(mydir);

	//�ڂ̑O�̑Ώۂ��擾,�_���[�W�������s��
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
	//�`����W+�A�j���[�V�����ʒu�␳(�⊮���W)-�J�����␳
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
