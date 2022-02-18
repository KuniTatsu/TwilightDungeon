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
	mydir = dires[dir];
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

//������ύX����Ƃ��ꂪ�Ă΂�邽�߁A�i���ɃX�e�[�^�X�������Ă��܂��������i�ύX���͕ʂ̏������s���K�v���� ex�X�e�[�^�X��0�ɂ���
void Actor::ChangeStatus(int StatusNum, int MoveNum, int StatusType)
{
	if (StatusType == EFFECT) {
		exHp = 0;
		exAtack = 0;
		exDefence = 0;
		exSpeed = 0;
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
		//�����A�C�e����t���ւ���ꍇ�͑����A�C�e���ɂ���ĕt�^����Ă����X�e�[�^�X���������Ă�����Z����
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
void Actor::Atack()
{
	//������������Vector3��Ԃ��֐���p�ӂ���

	t2k::Vector3 front;
	//�ڂ̑O�̑Ώۂ��擾
	front = gManager->WorldToLocal(pos) + gManager->GetVecter(mydir);

	//�ڂ̑O�̑Ώۂ��擾,�_���[�W�������s��
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
	Anim(gh, actSpeed, maxMotionIndex, drawGh);
	//�`����W+�A�j���[�V�����ʒu�␳(�⊮���W)-�J�����␳
	DrawRotaGraph(pos.x + animPos.x - gManager->camera->cameraPos.x, pos.y + animPos.y - 10 - gManager->camera->cameraPos.y, 1, 0, drawGh, true);
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
