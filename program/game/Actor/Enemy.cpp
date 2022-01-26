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
	//�������Ԃ���Ȃ���Γ����Ȃ�
	if (--moveTimer > 0)return;
	moveTimer = MOVETIME;

	//�L�����̈ʒu���}�b�v��̂ǂ̃`�b�v�����肷��
	myNowPos = gManager->WorldToLocal(pos);

	//�ړI�n���Z�b�g����Ă���΂�����֌�����
	if (isSetChasePoint) {
		MoveChasePoint();
		return;
	}

	//������ꏊ�������̂ǂ����Ȃ畔���̔ԍ����擾����
	int roomNum = gManager->CheckIsThere(myNowPos);
	//�����̂ǂ����ɂ���Ȃ�
	if (roomNum != -1) {
		//���̕����̏o���̒����玩�������ԉ����o�����擾����
		t2k::Vector3 wayPoint = gManager->GetFarPoint(roomNum, myNowPos);
		//�擾�����o����ړI�n�ɃZ�b�g����
		ChasePoint = wayPoint;

		isSetChasePoint = true;
		return;
	}
	//�ʘH�ɂ���Ȃ�
	else {
		//�i�߂�Ȃ玩����dir�̕����ɐi��
		bool canMove = MoveToDir(mydir, myNowPos);
		//�����i�߂Ȃ����dir���猩�č����ɍs���Ȃ����m�F����
		if (!canMove) {
			//�������ɐi�߂�Ȃ獶�ɐi��
			if (CheckCanMoveToDir(mydir, myNowPos, CheckDir::LEFT)) DegradedMoveToDir(GetDir(mydir, CheckDir::LEFT));
			//�������ɐi�߂Ȃ��Ȃ�E�ɐi��
			else DegradedMoveToDir(GetDir(mydir, CheckDir::RIGHT));
		}
	}




	//	//�������擾����
	//	//2.1 ���������̂Ƃ�
	//	//����������
	//	//�����ǂ��������Ɍ�����

	//	//2.2 ��������̂Ƃ�
	//	//�܂������i�߂邩����
	//	//�܂��������_���Ȃ�E�Ɍ�����

	//	//2.3 �������E�̂Ƃ�
	//	//�܂������i�߂邩����
	//	//�܂��������_���Ȃ�

	//	//2.4 ���������̂Ƃ�
	//	//�܂������i�߂邩����
	//	//�܂��������_���Ȃ�E�Ɍ�����
	//}


#if 0
	int myleft = GetMyLeft(mydir);
	//���������̌����Ă�������̍����ǂ���Ȃ�������
	//�������ɐi��

	//�������Ă���������E�̂Ƃ�
	//����m�F
	if (myleft == UP) {
		//�オ�ǂ���Ȃ������炻�����ɐi��
		t2k::Vector3 nextPos = t2k::Vector3(myNowPos.x, myNowPos.y - 1, 0);
		if (gManager->GetMapChip(nextPos) != 0) {
			pos.y -= 20;
			mydir = dir::UP;
		}
		//�ǂ�������
		else {
			//�O���ǂ���Ȃ�������^�������i��
			t2k::Vector3 Front = t2k::Vector3(myNowPos.x + 1, myNowPos.y, 0);
			if (gManager->GetMapChip(Front) != 0) {
				pos.x += 20;
			}
			//�O���ǂ�������
			else {
				//�E���擾
				t2k::Vector3 right = t2k::Vector3(myNowPos.x, myNowPos.y - 1, 0);
				//�E���ǂȂ�i�܂Ȃ�
				if (gManager->GetMapChip(right) == 0)return;
				//�E�ɐi��
				pos.y += 20;
				mydir = dir::DOWN;
			}
		}
	}
	//�������Ă�����������̂Ƃ�
	//�E���m�F
	else if (myleft == RIGHT) {
		//�E���ǂ���Ȃ������炻�����ɐi��
		t2k::Vector3 nextPos = t2k::Vector3(myNowPos.x + 1, myNowPos.y, 0);
		if (gManager->GetMapChip(nextPos) != 0) {
			pos.x += 20;
			mydir = dir::RIGHT;
		}
		//�ǂ�������
		else {
			//�O���ǂ���Ȃ�������^�������i��
			t2k::Vector3 Front = t2k::Vector3(myNowPos.x, myNowPos.y + 1, 0);
			if (gManager->GetMapChip(Front) != 0) {
				pos.y += 20;
			}
			//�O���ǂ�������
			else {
				//�E���擾
				t2k::Vector3 right = t2k::Vector3(myNowPos.x - 1, myNowPos.y, 0);
				//�E���ǂȂ�i�܂Ȃ�
				if (gManager->GetMapChip(right) == 0)return;
				//�E�ɐi��
				pos.x -= 20;
				mydir = dir::LEFT;
			}
		}
	}
	//�������Ă�����������̂Ƃ�
	//�����m�F
	else if (myleft == DOWN) {
		//�����ǂ���Ȃ������炻�����ɐi��
		t2k::Vector3 nextPos = t2k::Vector3(myNowPos.x, myNowPos.y + 1, 0);
		if (gManager->GetMapChip(nextPos) != 0) {
			pos.y += 20;
			mydir = dir::DOWN;
		}
		//�ǂ�������
		else {
			//�O���ǂ���Ȃ�������^�������i��
			t2k::Vector3 Front = t2k::Vector3(myNowPos.x - 1, myNowPos.y, 0);
			if (gManager->GetMapChip(Front) != 0) {
				pos.x -= 20;
			}
			//�O���ǂ�������
			else {
				//�E���擾
				t2k::Vector3 right = t2k::Vector3(myNowPos.x, myNowPos.y - 1, 0);
				//�E���ǂȂ�i�܂Ȃ�
				if (gManager->GetMapChip(right) == 0)return;
				//�E�ɐi��
				pos.y -= 20;
				mydir = dir::UP;
			}
		}
	}
	//�������Ă����������̂Ƃ�
	//�����m�F
	else if (myleft == LEFT) {
		//�����ǂ���Ȃ������炻�����ɐi��
		t2k::Vector3 nextPos = t2k::Vector3(myNowPos.x - 1, myNowPos.y, 0);
		if (gManager->GetMapChip(nextPos) != 0) {
			pos.x -= 20;
			mydir = dir::LEFT;
		}
		//�ǂ�������
		else {
			//�O���ǂ���Ȃ�������^�������i��
			t2k::Vector3 Front = t2k::Vector3(myNowPos.x, myNowPos.y - 1, 0);
			if (gManager->GetMapChip(Front) != 0) {
				pos.y -= 20;
			}
			//�O���ǂ�������
			else {
				//�E���擾
				t2k::Vector3 right = t2k::Vector3(myNowPos.x + 1, myNowPos.y, 0);
				//�E���ǂȂ�i�܂Ȃ�
				if (gManager->GetMapChip(right) == 0)return;
				//�E�ɐi��
				pos.x += 20;
				mydir = dir::RIGHT;
			}
		}
	}

#endif

#if 0
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
#endif
}

int Enemy::GetMyLeft(int MyDir)
{
	//��,�E,��,�� 0,1,2,3
	//�������܌����Ă���������ゾ������
	if (MyDir == 0) {
		//�����Ă����������݂č��͍���
		return LEFT;
	}
	//�E�������Ă����� ���͏㑤
	//���������Ă����� ���͉E��
	//���������Ă����� ���͉���

	return (MyDir - 1) % 4;
}

void Enemy::MoveChasePoint()
{
	//�ړI�n�Ɍ������Ĉړ�����
	//�ӂ̒��������ɐi��

	//�ړI�n�܂ł̋������v�Z(��Βl�ŋ��߂�)
	int disX = abs(ChasePoint.x - myNowPos.x);
	int disY = abs(ChasePoint.y - myNowPos.y);


	//x�̂ق��������ꍇ
	if (disX > disY) {
		//x�����ɐi��
		//������x���W���ړI�n��荶�Ȃ�E�ɐi��
		if (myNowPos.x < ChasePoint.x) {
			if (gManager->GetMapChip(myNowPos + t2k::Vector3(1, 0, 0)) == 0)return;
			pos.x += 20;
			mydir = dir::RIGHT;
		}//���ɐi��
		else {
			if (gManager->GetMapChip(myNowPos + t2k::Vector3(-1, 0, 0)) == 0)return;
			pos.x -= 20;
			mydir = dir::LEFT;
		}
	}//y�̂ق��������ꍇ
	else {
		//y�����ɐi��
		//������y���W���ړI�n����Ȃ牺�ɐi��
		if (myNowPos.y < ChasePoint.y) {
			if (gManager->GetMapChip(myNowPos + t2k::Vector3(0, 1, 0)) == 0)return;
			pos.y += 20;
			mydir = dir::DOWN;
		}//��ɐi��
		else {
			if (gManager->GetMapChip(myNowPos + t2k::Vector3(0, -1, 0)) == 0)return;
			pos.y -= 20;
			mydir = dir::UP;
		}
	}
	//�������̎����̈ʒu�ƖړI�n����v���Ă����
	if (myNowPos.x == ChasePoint.x && myNowPos.y == ChasePoint.y) {
		//isSetChasePoint��false�ɂ���
		isSetChasePoint = false;
		//�ړI�n��j������
		ChasePoint = { 0,0,0 };
	}
}
int Enemy::GetDir(const int dir, const int getDir)
{
	//���̌�������
	if (dir == 0) {
		//���͍���
		if (getDir == CheckDir::LEFT)return CheckDir::LEFT;
		//�E�͉E��
		if (getDir == CheckDir::RIGHT)return CheckDir::RIGHT;
	}
	//���̌������E
	else if (dir == 1) {
		//���͏㑤
		if (getDir == CheckDir::LEFT)return CheckDir::UP;
		//�E�͉���
		if (getDir == CheckDir::RIGHT)return CheckDir::DOWN;
	}
	//���̌�������
	else if (dir == 2) {
		//���͉E��
		if (getDir == CheckDir::LEFT)return CheckDir::RIGHT;
		//�E�͍���
		if (getDir == CheckDir::RIGHT)return CheckDir::LEFT;
	}
	//���̌�������
	else if (dir == 3) {
		//���͉���
		if (getDir == CheckDir::LEFT)return CheckDir::DOWN;
		//�E�͏㑤
		if (getDir == CheckDir::RIGHT)return CheckDir::UP;
	}
	return -1;
}
/*
enum dir{
		UP,
		RIGHT,
		DOWN,
		LEFT
	};
*/
/*
t2k::Vector3 Enemy::MoveToDir(int dir)
{
	//��
	if (dir == 0) {
		return t2k::Vector3(0, -1, 0);
	}
	//�E
	else if (dir == 1) {
		return t2k::Vector3(1, 0, 0);
	}
	//��
	else if (dir == 2) {
		return t2k::Vector3(0, 1, 0);
	}
	//��
	else if (dir == 3) {
		return t2k::Vector3(-1, 0, 0);
	}
	return t2k::Vector3(-1, -1, -1);
}
*/

bool Enemy::MoveToDir(const int dir, t2k::Vector3& nowPos)
{
	//dir������1�}�X�悪�ړ��s�Ȃ�ړ����Ȃ�
	if (!CheckCanMove(dir, nowPos))return false;
	//��
	if (dir == 0) {
		pos.y -= 20;
	}
	//�E
	else if (dir == 1) {
		pos.x += 20;
	}
	//��
	else if (dir == 2) {
		pos.y += 20;
	}
	//��
	else if (dir == 3) {
		pos.x -= 20;
	}
	else {
		t2k::debugTrace("\n�ړ��G���[\n");
	}
	return true;
}

void Enemy::DegradedMoveToDir(const int dir)
{
	//��
	if (dir == 0) {
		pos.y -= 20;
		mydir = dir::UP;
	}
	//�E
	else if (dir == 1) {
		pos.x += 20;
		mydir = dir::RIGHT;
	}
	//��
	else if (dir == 2) {
		pos.y += 20;
		mydir = dir::DOWN;
	}
	//��
	else if (dir == 3) {
		pos.x -= 20;
		mydir = dir::LEFT;
	}
	else {
		t2k::debugTrace("\n�ړ��G���[\n");
	}
}

bool Enemy::CheckCanMove(const int dir, const t2k::Vector3 nowPos)
{
	t2k::Vector3 nextPos = { 0,0,0 };
	//��
	if (dir == 0) {
		nextPos = nowPos + t2k::Vector3(0, -1, 0);
	}
	//�E
	else if (dir == 1) {
		nextPos = nowPos + t2k::Vector3(1, 0, 0);
	}
	//��
	else if (dir == 2) {
		nextPos = nowPos + t2k::Vector3(0, 1, 0);
	}
	//��
	else if (dir == 3) {
		nextPos = nowPos + t2k::Vector3(-1, 0, 0);
	}
	//�ǂȂ�false��Ԃ�
	if (gManager->GetMapChip(nextPos) == 0)return false;

	return true;
}

bool Enemy::CheckCanMoveToDir(const int dir, const t2k::Vector3 nowPos, const int checkDir)
{
	//�������Ă���̂���̏ꍇ
	if (dir == 0) {
		//�m�F������������dir���猩�ď�̎�
		if (checkDir == CheckDir::UP)return CheckCanMove(CheckDir::UP, nowPos);
		//�E�̎�
		if (checkDir == CheckDir::RIGHT)return CheckCanMove(CheckDir::RIGHT, nowPos);
		//���̎�
		if (checkDir == CheckDir::DOWN)return CheckCanMove(CheckDir::DOWN, nowPos);
		//���̎�
		if (checkDir == CheckDir::LEFT)return CheckCanMove(CheckDir::LEFT, nowPos);
	}
	//�������Ă���̂��E�̏ꍇ
	else if (dir == 1) {
		//�������č����͏����
		if (checkDir == CheckDir::LEFT)return CheckCanMove(CheckDir::UP, nowPos);
		//�������ĉE���͉�����
		if (checkDir == CheckDir::RIGHT)return CheckCanMove(CheckDir::DOWN, nowPos);
	}
	//�������Ă���̂����̏ꍇ
	else if (dir == 2) {
		//�������č����͉E����
		if (checkDir == CheckDir::LEFT)return CheckCanMove(CheckDir::RIGHT, nowPos);
		//�������ĉE���͍�����
		if (checkDir == CheckDir::RIGHT)return CheckCanMove(CheckDir::LEFT, nowPos);
	}
	//�������Ă���̂����̏ꍇ
	else if (dir == 3) {
		//�������č����͉�����
		if (checkDir == CheckDir::LEFT)return CheckCanMove(CheckDir::DOWN, nowPos);
		//�������ĉE���͏����
		if (checkDir == CheckDir::RIGHT)return CheckCanMove(CheckDir::UP, nowPos);
	}

	return false;
}


