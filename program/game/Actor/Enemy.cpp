#include "Enemy.h"
#include"DxLib.h"
#include "../GameManager.h"
#include"../Player.h"
#include"../Map.h"

extern GameManager* gManager;
Enemy::Enemy(int Id, int Type, std::string Name, int Hp, int Attack, int Defence, int Speed, std::string Gh, int Exp, int Floor, int ActId)
{
	actId = ActId;
	id = Id;
	type = Type;
	name = Name;
	//�␳�l const�ɂ���
	exHp = (Floor - 1) * 10;
	exAttack = (Floor - 1) * 2;
	exDefence = (Floor - 1) * 2;
	exSpeed = (Floor - 1) * 2;
	exExp = (Floor - 1) * 5;

	//csv����ǂݍ��ފ�b�l
	baseHp = Hp;
	baseAttack = Attack;
	baseDefence = Defence;
	baseSpeed = Speed;

	//�`��X�e�[�^�X�̎Z�o
	hp = baseHp + exHp;
	attack = baseAttack + exAttack;
	defence = baseDefence + exDefence;
	speed = baseSpeed + exSpeed;

	LoadDivGraph(Gh.c_str(), 12, 3, 4, 24, 32, gh);
	LoadDivGraph("graphics/AttackAnim_30.png", 5, 5, 1, 30, 30, effectGh);

	baseExp = Exp;
	exp = baseExp + exExp;
	nowHp = hp;

	maxMotionIndex = gManager->GetMaxIndex(GameManager::index::ENEMY);
}

Enemy::~Enemy()
{
	t2k::debugTrace("\n�G�����S���܂���\n");
}

void Enemy::TimeUpdate()
{
	--moveTimer;
	if (moveTimer < 0)moveTimer = 0;
}

bool Enemy::Move()
{
	//�L�����̈ʒu���}�b�v��̂ǂ̃`�b�v�����肷��
	myNowPos = gManager->WorldToLocal(pos);

	//������ꏊ�������̂ǂ����Ȃ畔���̔ԍ����擾����
	roomNum = gManager->CheckIsThere(myNowPos);

	//���������ɂ��邩�`�F�b�N
	if (roomNum == gManager->playerRoomNum && roomNum != -1) {
		sameRoom = true;
	}
	else {
		sameRoom = false;
	}
	//�v���C���[�������Ă�����
	if (detectFrag) {
		//�v���C���[�̍��W�Ɍ�����
		ChasePoint = gManager->WorldToLocal(gManager->player->pos);
		//chasepoint�Ɍ������Ĉړ�����
		MoveChasePoint();
		//chasepoint��������
		CheckDoneChase();
		//�ǐՔ͈͔���
		ResetDetectFrag();
		return true;

	}

	//�ړI�n���Z�b�g����Ă���΂�����֌�����
	if (isSetChasePoint && !sameRoom) {
		MoveChasePoint();
		CheckDoneChase();
		return true;
	}
	//���������ɂ���Ȃ�
	else if (isSetChasePoint && sameRoom) {
		//player��ǔ�����悤�ɂȂ�
		ChasePoint = gManager->WorldToLocal(gManager->player->pos);
		isSetChasePoint = true;
		detectFrag = true;

		MoveChasePoint();
		CheckDoneChase();
		return true;
	}

#if 0
	//enemy��player�����������ɂ��邩�ʘH�ł͂Ȃ��Ȃ�
	if (sameRoom) {


		//A*�Ōo�H�T��
		//�o�H��list��n�ȏ�c���Ă���΍s��Ȃ�
		if (willMove.size() < 5) {
			//�o�H�T�� ����͕������ɏ�Q�����Ȃ����߂܂��͒P���Ȍo�H�T������������
			MoveToPlayer();
		}
		//willMove���X�g�̈�ԏ��߂�Node�Ɍ�����

		//willMove���X�g�ɂ�Node*�^�̕ϐ��������Ă���
		//Node*�̕ϐ����璆�g��Pos(�}�b�v���W�n)���擾���A�ړI�n�ɂ���


		t2k::Vector3 chasePoint = willMove.front();


		//���Z�b�g
		ChasePoint = { 0,0,0 };
		//ChasePoint�̍X�V
		ChasePoint = chasePoint;
		//�ړ����I������烊�X�g�̐擪��pop����
		willMove.pop_front();

		isSetChasePoint = true;
		//MoveChasePoint();
		//return;
	}
#endif


	//�����̂ǂ����ɂ���Ȃ�
	if (roomNum != -1) {
		//���̕����̏o���̒����玩�������ԉ����o�����擾����
		t2k::Vector3 wayPoint = gManager->GetFarPoint(roomNum, myNowPos);

		//CHasePoint�̃��Z�b�g
		ChasePoint = (0, 0, 0);
		//�擾�����o����ړI�n�ɃZ�b�g����
		ChasePoint = wayPoint;

		isSetChasePoint = true;
		MoveChasePoint();
		return true;
	}
	//�ʘH�ɂ���Ȃ�
	else {
		//�i�߂�Ȃ玩����dir�̕����ɐi��
		bool canMove = MoveToDir(mydir, myNowPos);

		//�����i�߂Ȃ����dir���猩�č����ɍs���Ȃ����m�F����
		if (!canMove) {
			//�������E�ɂ����߂�Ȃ烉���_���Ői��
			if (CheckCanMoveToDir(mydir, myNowPos, CheckDir::LEFT) && CheckCanMoveToDir(mydir, myNowPos, CheckDir::RIGHT))
			{
				if (rand() % 2 == 0)DegradedMoveToDir(GetDir(mydir, CheckDir::LEFT));
				else DegradedMoveToDir(GetDir(mydir, CheckDir::RIGHT));
			}
			//���ɂ����߂邩�m�F
			else if (CheckCanMoveToDir(mydir, myNowPos, CheckDir::LEFT))DegradedMoveToDir(GetDir(mydir, CheckDir::LEFT));
			//�������ɐi�߂Ȃ��Ȃ�E���m�F���Đi�߂�Ȃ�i��
			//else DegradedMoveToDir(GetDir(mydir, CheckDir::RIGHT));
			else if (CheckCanMoveToDir(mydir, myNowPos, CheckDir::RIGHT))DegradedMoveToDir(GetDir(mydir, CheckDir::RIGHT));
			//����ł��i�߂Ȃ��Ȃ�
			else {
				//�߂�
				DegradedMoveToDir(GetDir(mydir, CheckDir::DOWN));
			}
		}
	}


	return true;
}


void Enemy::MoveChasePoint()
{
	//�ړI�n�Ɍ������Ĉړ�����
	//�ӂ̒��������ɐi��

	//�ړI�n�܂ł̋������v�Z(��Βl�ŋ��߂�)
	int disX = abs(ChasePoint.x - myNowPos.x);
	int disY = abs(ChasePoint.y - myNowPos.y);

	//t2k::Vector3 v[4] ;
	//v[dir::RIGHT]	= { 1, 0, 0 };
	//v[dir::LEFT]	= { -1, 0, 0 };
	//v[dir::UP]		= { 0, 1, 0 };
	//v[dir::DOWN]	= { 0, -1, 0 };

	//bool is_not_wall[dir::MAX] ;
	//for (int i = 0; i < dir::MAX; ++i) is_not_wall[i] = gManager->GetMapChip(myNowPos + v[i]);

	//bool is_there_enemy[dir::MAX];
	//for (int i = 0; i < dir::MAX; ++i) is_there_enemy[i] = gManager->CheckIsThereEnemyToDir(myNowPos + v[i]);

	//int Dir = (disX > disY) ? (myNowPos.x < ChasePoint.x) ? dir::RIGHT : dir::LEFT : (myNowPos.y < ChasePoint.y) ? dir::DOWN : dir::UP ;


	//x�̂ق��������ꍇ
	if (disX > disY) {
		//x�����ɐi��
		//������x���W���ړI�n��荶�Ȃ�E�ɐi��
		if (myNowPos.x < ChasePoint.x) {

			//�E���ǂ���Ȃ����G�����Ȃ��Ȃ�
			if (gManager->GetMapChip(myNowPos + t2k::Vector3(1, 0, 0)) != 0 && !gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(1, 0, 0))) {
				//���ɐi��
				pos.x += gManager->nowGraphicSize;
				mydir = dir::RIGHT;
				return;
			}
			//�E�ɐi�߂Ȃ��Ȃ�㉺���l����
			else {
				//enemy����ɂ���Ȃ灨���ɐi�݂���
				if (ChasePoint.y > myNowPos.y) {
					//���ɓG�����Ȃ����
					if (!gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(0, 1, 0))) {
						//�ǂ��G������Ȃ�i�܂Ȃ�
						if (gManager->GetMapChip(myNowPos + t2k::Vector3(0, 1, 0)) == 0 || gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(0, 1, 0)))return;
						pos.y += gManager->nowGraphicSize;
						mydir = dir::DOWN;
						return;
					}
					//�������߂Ȃ牽�����Ȃ�
					else {
						return;
					}
				}
				//enemy�����ɂ���Ȃ灨��ɐi�݂���
				else {
					if (!gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(0, -1, 0))) {
						//�ǂ��G������Ȃ�i�܂Ȃ�
						if (gManager->GetMapChip(myNowPos + t2k::Vector3(0, -1, 0)) == 0 || gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(0, -1, 0)))return;
						pos.y -= gManager->nowGraphicSize;
						mydir = dir::UP;
						return;
					}
					//������߂Ȃ牽�����Ȃ�
					else {
						return;
					}
				}
			}
		}//���ɐi��
		else {
			//�����ǂ���Ȃ����G�����Ȃ��Ȃ�
			if (gManager->GetMapChip(myNowPos + t2k::Vector3(-1, 0, 0)) != 0 && !gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(-1, 0, 0))) {
				//���ɐi��
				pos.x -= gManager->nowGraphicSize;
				mydir = dir::LEFT;
				return;
			}
			//���ɐi�߂Ȃ��Ȃ�㉺���l����s
			else {
				//enemy����ɂ���Ȃ灨���ɐi�݂���
				if (ChasePoint.y > myNowPos.y) {
					//���ɓG�����Ȃ����
					if (!gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(0, 1, 0))) {
						//�ǂ��G������Ȃ�i�܂Ȃ�
						if (gManager->GetMapChip(myNowPos + t2k::Vector3(0, 1, 0)) == 0 || gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(0, 1, 0)))return;
						pos.y += gManager->nowGraphicSize;
						mydir = dir::DOWN;
						return;
					}
					//�������߂Ȃ牽�����Ȃ�
					else {
						return;
					}
				}
				//enemy�����ɂ���Ȃ灨��ɐi�݂���
				else {
					if (!gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(0, -1, 0))) {
						//�ǂ��G������Ȃ�i�܂Ȃ�
						if (gManager->GetMapChip(myNowPos + t2k::Vector3(0, -1, 0)) == 0 || gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(0, -1, 0)))return;
						pos.y -= gManager->nowGraphicSize;
						mydir = dir::UP;
						return;
					}
					//������߂Ȃ牽�����Ȃ�
					else {
						return;
					}
				}
			}
			/*pos.x -= gManager->nowGraphicSize;
			mydir = dir::LEFT;*/
		}
	}//y�̂ق��������ꍇ
	else {
		//����x��y�������ł����ɓ���
		//�ړI�n�Ɍ������ĉ������㑤���ǂ̎�
		//x�����ɐi�܂���


		//y�����ɐi��
		//������y���W���ړI�n����Ȃ牺�ɐi��
		if (myNowPos.y < ChasePoint.y) {
			//�����ǂ���Ȃ����G�����Ȃ��Ȃ�
			if (gManager->GetMapChip(myNowPos + t2k::Vector3(0, 1, 0)) != 0 && !gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(0, 1, 0))) {
				//���ɐi��
				pos.y += gManager->nowGraphicSize;
				mydir = dir::DOWN;
				return;
			}
			//���ɐi�߂Ȃ��Ȃ獶�E���l����
			else {
				//������x���W���ړI�n��荶�Ȃ�E�ɐi��
				if (myNowPos.x < ChasePoint.x) {
					//�ǂ��G������Ȃ牽�����Ȃ�
					if (gManager->GetMapChip(myNowPos + t2k::Vector3(1, 0, 0)) == 0 || gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(1, 0, 0)))return;

					pos.x += gManager->nowGraphicSize;
					mydir = dir::RIGHT;
					return;
				}//���ɐi��
				else {
					//�ǂ��G������Ȃ牽�����Ȃ�
					if (gManager->GetMapChip(myNowPos + t2k::Vector3(-1, 0, 0)) == 0 || gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(-1, 0, 0)))return;

					pos.x -= gManager->nowGraphicSize;
					mydir = dir::LEFT;
					return;
				}

			}
		}
		//��ɐi��
		else {
			//�オ�ǂ���Ȃ����G�����Ȃ��Ȃ�
			if (gManager->GetMapChip(myNowPos + t2k::Vector3(0, -1, 0)) != 0 && !gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(0, -1, 0))) {
				//���ɐi��
				pos.y -= gManager->nowGraphicSize;
				mydir = dir::UP;
				return;
			}
			//��ɐi�߂Ȃ��Ȃ獶�E���l����
			else {
				//������x���W���ړI�n��荶�Ȃ�E�ɐi��
				if (myNowPos.x < ChasePoint.x) {
					//�ǂ��G������Ȃ牽�����Ȃ�
					if (gManager->GetMapChip(myNowPos + t2k::Vector3(1, 0, 0)) == 0 || gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(1, 0, 0)))return;

					pos.x += gManager->nowGraphicSize;
					mydir = dir::RIGHT;
					return;
				}//���ɐi��
				else {
					//�ǂ��G������Ȃ牽�����Ȃ�
					if (gManager->GetMapChip(myNowPos + t2k::Vector3(-1, 0, 0)) == 0 || gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(-1, 0, 0)))return;

					pos.x -= gManager->nowGraphicSize;
					mydir = dir::LEFT;
					return;
				}

			}
		}
	}


}
int Enemy::GetDir(const int dir, const int getDir)
{
	//���̌�������
	if (dir == 0) {
		//���͍���
		if (getDir == CheckDir::LEFT)return CheckDir::LEFT;
		//�E�͉E��
		else if (getDir == CheckDir::RIGHT)return CheckDir::RIGHT;

		else if (getDir == CheckDir::DOWN)return CheckDir::DOWN;
	}
	//���̌������E
	else if (dir == 1) {
		//���͏㑤
		if (getDir == CheckDir::LEFT)return CheckDir::UP;
		//�E�͉���
		else if (getDir == CheckDir::RIGHT)return CheckDir::DOWN;
		else if (getDir == CheckDir::DOWN)return CheckDir::LEFT;
	}
	//���̌�������
	else if (dir == 2) {
		//���͉E��
		if (getDir == CheckDir::LEFT)return CheckDir::RIGHT;
		//�E�͍���
		else if (getDir == CheckDir::RIGHT)return CheckDir::LEFT;
		else if (getDir == CheckDir::DOWN)return CheckDir::UP;
	}
	//���̌�������
	else if (dir == 3) {
		//���͉���
		if (getDir == CheckDir::LEFT)return CheckDir::DOWN;
		//�E�͏㑤
		else if (getDir == CheckDir::RIGHT)return CheckDir::UP;
		else if (getDir == CheckDir::DOWN)return CheckDir::RIGHT;
	}
	return -1;
}

bool Enemy::MoveToDir(const int dir, t2k::Vector3& nowPos)
{
	//dir������1�}�X�悪�ړ��s�Ȃ�ړ����Ȃ�
	if (!CheckCanMove(dir, nowPos))return false;
	//��
	if (dir == 0) {
		pos.y -= gManager->nowGraphicSize;
	}
	//�E
	else if (dir == 1) {
		pos.x += gManager->nowGraphicSize;
	}
	//��
	else if (dir == 2) {
		pos.y += gManager->nowGraphicSize;
	}
	//��
	else if (dir == 3) {
		pos.x -= gManager->nowGraphicSize;
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
		pos.y -= gManager->nowGraphicSize;
		mydir = dir::UP;
	}
	//�E
	else if (dir == 1) {
		pos.x += gManager->nowGraphicSize;
		mydir = dir::RIGHT;
	}
	//��
	else if (dir == 2) {
		pos.y += gManager->nowGraphicSize;
		mydir = dir::DOWN;
	}
	//��
	else if (dir == 3) {
		pos.x -= gManager->nowGraphicSize;
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
	if (gManager->GetMapChip(nextPos) == 0 || gManager->CheckIsThereEnemyToDir(nextPos))return false;

	return true;
}

void Enemy::CheckDoneChase()
{
	//�������̎����̈ʒu�ƖړI�n����v���Ă����
	if (pos.x / gManager->nowGraphicSize == ChasePoint.x && pos.y / gManager->nowGraphicSize == ChasePoint.y) {
		//isSetChasePoint��false�ɂ���
		isSetChasePoint = false;
		//�ړI�n��j������
		ChasePoint = { 0,0,0 };
	}
}

void Enemy::ResetDetectFrag()
{
	t2k::Vector3 PlayerLocalPos = gManager->WorldToLocal(gManager->player->pos);

	float x = abs(PlayerLocalPos.x - myNowPos.x);
	float y = abs(PlayerLocalPos.y - myNowPos.y);

	if (x + y > DETECTRANGE)detectFrag = false;
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

#if 0

//�o�H�T��
void Enemy::MoveToPlayer()
{
	Point goal;
	Point start;


	if (nodes != nullptr)
	{
		for (int i = 0; i < MH; i++)
		{
			delete[] nodes[i];
		}

		delete[] nodes;
		nodes = nullptr;
	}

	//�����������镔�����擾���� ��roomNum

	std::vector<std::vector<int>>chips;
	//chips�ɍ����镔���̒���chip�f�[�^�����[
	gManager->map->GetAllChip(roomNum, chips);
	//�����̑傫��
	t2k::Vector3 hoge = gManager->GetRoomValue(roomNum);

	MH = hoge.y;
	MW = hoge.x;


	//�����̍���̍��W���擾��nodes[0][0]
	t2k::Vector3 LeftTop = gManager->GetRoomStartPos(roomNum);
	int x = myNowPos.x - LeftTop.x;
	int y = myNowPos.y - LeftTop.y;

	// �X�^�[�g�ƃS�[���̈ʒu���擾
	start = Point(x, y);

	nodes = new Node * [MH];
	for (int i = 0; i < MH; i++)
	{
		nodes[i] = new Node[MW];
	}


	//�X�^�[�g�͎������g,�S�[����player
	//for (int i = 0; i < chips.size(); ++i) {
	//	for (int k = 0; k < chips[i].size(); ++k) {
	//		//2==Player��������S�[���ɂ��� ���{���Ɏ��Ă邩�s��
	//		if (2 == chips[i][k]) goal = Point(k, i);
	//	}
	//}

	//�v���C���[�̍��W���璼�ڃS�[�����w�肷��
	t2k::Vector3 playerPos = gManager->WorldToLocal(gManager->player->pos);
	int goalx = playerPos.x - LeftTop.x;
	int goaly = playerPos.y - LeftTop.y;
	goal = Point(goalx, goaly);


	// �m�[�h�f�[�^�̏����ݒ�
	for (int i = 0; i < MH; ++i) {
		for (int k = 0; k < MW; ++k) {
			nodes[i][k].pos = Point(k, i);
			if (k == start.x && i == start.y)nodes[i][k].status = 4;
			else if (k == goal.x && i == goal.y)nodes[i][k].status = 5;
			else nodes[i][k].status = chips[i][k];
			nodes[i][k].cost_guess = abs((goal.x + goal.y) - (i + k));
		}
	}

	// �񎟌��z��̃A�h���X�������ɓn���ׂ̏���
	//Node* tmp_nodes[MH];

	// nodes
	//TODO: �J�����邱��
	Node** tmp_nodes;
	tmp_nodes = new Node * [MH];
	for (int i = 0; i < MH; i++)
	{
		tmp_nodes[i] = new Node[MW];
		for (int j = 0; j < MW; j++)
		{
			tmp_nodes[i][j] = nodes[i][j];
		}
	}

	auto a = &nodes[start.y][start.x];

	if (a->status == START) {
		int b = 0;
		++b;
	}

	// �o�H�T�����s
	bool is_success = aster(tmp_nodes, &nodes[start.y][start.x], &willMove, LeftTop);

	// false ���A���Ă����瓞�B�s�\
	if (!is_success) {
		printf("���B�s�\\n");
		return;
	}
	if (nodes != nullptr)
	{
		for (int i = 0; i < MH; i++)
		{
			delete[] nodes[i];
		}

		delete[] nodes;
		nodes = nullptr;
	}
	if (tmp_nodes != nullptr)
	{
		for (int i = 0; i < MH; i++)
		{
			delete[] tmp_nodes[i];
		}

		delete[] tmp_nodes;
		tmp_nodes = nullptr;
	}
}

//void Enemy::ChangeSequence(sequence seq)
//{
//	nowSeq = seq;
//	if (seq == sequence::MOVE) {
//		main_sequence.change(&Enemy::SeqMove);
//	}
//	else if (seq == sequence::ATTACK) {
//		main_sequence.change(&Enemy::SeqAttack);
//	}
//}

bool Enemy::isEnableMapPosition(Point pos, Node** const _nodes)
{
	//�T���͈͊O�Ȃ�false
	if (pos.x < 0) return false;
	if (pos.y < 0) return false;
	if (pos.x >= MW) return false;
	if (pos.y >= MH) return false;
	//�X�^�[�g�͍ēx���ׂȂ�
	if (START == _nodes[pos.y][pos.x].status) return false;
	//��Ԃ܂��̓S�[���Ȃ�T���\
	if (SPACE == _nodes[pos.y][pos.x].status) return true;
	if (GOAL == _nodes[pos.y][pos.x].status) return true;
	//�ǂ͒T���s��
	return false;
}
Enemy::Node* Enemy::getSmallScoreNodeFromOpenNodes()
{
	Node* p = nullptr;

	std::list<Node*>::iterator itr = openNodes.begin();
	for (auto node : openNodes) {
		//���������̍ŏ��Ȃ�͂��߂̃m�[�h������
		if (nullptr == p) {
			p = *itr;
			++itr;
			continue;
		}
		//�����X�R�A�����������̂��������p�ɂ����
		if (p->score > (*itr)->score) p = *itr;
		//�����X�R�A�������Ȃ�
		else if (p->score == (*itr)->score) {
			//���R�X�g���ׂď������Ȃ�p�ɓ����
			if (p->cost_real > (*itr)->cost_real) p = *itr;
		}
		++itr;
	}
	return p;
}

bool Enemy::aster(Node** _nodes, Node* _now, std::list<t2k::Vector3>* _route, t2k::Vector3 LeftTop)
{
	// �X�^�[�g�n�_�̃X�R�A�v�Z
	if (START == _now->status) {
		_now->score = _now->cost_real + _now->cost_guess;
	}

	// �S�����̍��W
	Point dir[4] = { Point(0, 1), Point(1, 0), Point(0, -1), Point(-1, 0) };

	// ����S�����𒲂ׂĉ\�Ȃ�I�[�v��
	for (int i = 0; i < 4; ++i) {
		Point next = _now->pos + dir[i];

		// ���א悪�I�[�v���\���ǂ���
		if (!isEnableMapPosition(next, _nodes)) continue;

		// �I�[�v���\��̍��W���S�[��������
		if (GOAL == _nodes[next.y][next.x].status) {

			// �S�[����ۑ�����
			auto goal = &_nodes[next.y][next.x];
			/*_route->push_back(&_nodes[next.y][next.x]);*/
			int x = next.x + LeftTop.x;
			int y = next.y + LeftTop.y;

			(*_route).push_back(t2k::Vector3(x, y, 0));

			// �S�[�������O���玩���̐e�m�[�h��k���ċL�^
			// ���̋L�^���ŒZ�o�H�ƂȂ�
			auto p = _now;
			while (nullptr != p) {
				/*_route->push_back(p);*/
				int x = p->pos.x + LeftTop.x;
				int y = p->pos.y + LeftTop.y;
				(*_route).push_back(t2k::Vector3(x, y, 0));
				p = p->parent;
			}

			// �S�[�������������̂� true
			return true;
		}

		// �S�����̃m�[�h�ɑ΂���I�[�v���ƃX�R�A�v�Z����
		_nodes[next.y][next.x].status = OPEN;
		//openvector�Ɋi�[
		openNodes.emplace_back(&_nodes[next.y][next.x]);
		_nodes[next.y][next.x].parent = _now;
		_nodes[next.y][next.x].cost_real = _now->cost_real + 1;
		_nodes[next.y][next.x].score = _nodes[next.y][next.x].cost_real + _nodes[next.y][next.x].cost_guess;
	}

	// ����̃I�[�v�����I������̂Ŏ����̓N���[�Y
	if (START != _now->status) {
		_now->status = CLOSED;

		//opennodes�̂Ȃ�����_now�Ɠ������̂�������pop����
		for (auto itr = openNodes.begin(); itr != openNodes.end();) {
			if (*itr == _now) {
				itr = openNodes.erase(itr);
				break;
			}
			++itr;
		}
	}

	// ���݃I�[�v�����Ă���m�[�h�ň�ԃX�R�A�̏��������̂���m�[�h
	Node* node = getSmallScoreNodeFromOpenNodes();

	// �m�[�h��������Ȃ���Γ��B�s�\
	if (nullptr == node) return false;

	// �ċA�I�ɒ��ׂĂ���
	return aster(_nodes, node, _route, LeftTop);
	}

#endif