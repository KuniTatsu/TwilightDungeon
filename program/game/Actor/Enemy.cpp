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
	//�����Ɋ֌W�Ȃ����ɐi�߂�Ȃ�i��


	//}
	//�L�����̈ʒu���}�b�v��̂ǂ̃`�b�v�����肷��
	t2k::Vector3 myNowPos = gManager->WorldToLocal(pos);
	if (--moveTimer > 0)return;
	moveTimer = MOVETIME;

	//t2k::Vector3 Left = t2k::Vector3(myNowPos.x - 1, myNowPos.y, 0);
	////�����ǂ���Ȃ�������
	//if (gManager->GetMapChip(Left) != 0) {
	//	//���ɐi��
	//	pos.x -= 20;
	//	mydir = LEFT;
	//}
	////�����ǂ�������
	//else {
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


#if 1
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
			mydir = UP;
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
				mydir = DOWN;
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
			mydir = RIGHT;
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
				mydir = LEFT;
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
			mydir = DOWN;
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
				mydir = UP;
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
			mydir = LEFT;
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
				mydir = RIGHT;
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


