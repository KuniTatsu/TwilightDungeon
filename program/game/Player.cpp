#include "Player.h"
#include"GameManager.h"
#include"DxLib.h"
#include"Camera.h"
#include"Item/equipItem.h"

extern GameManager* gManager;

Player::Player(const t2k::Vector3& StartPos, float Hp, int Atack, int Defence, int Speed, int ActId)
{
	actId = ActId;
	pos = StartPos;
	//p_gh = gManager->LoadGraphEx("graphics/PlayerTest.png");

	LoadDivGraph("graphics/Character_1.png", 12, 3, 4, 32, 32, gh);
	LoadDivGraph("graphics/AttackAnim_30.png", 5, 5, 1, 30, 30, effectGh);


	nowHpVar_gh = gManager->LoadGraphEx("graphics/haveHpVar.png");
	hpVar_gh = gManager->LoadGraphEx("graphics/AllHpVar.png");

	name = "Player";

	baseHp = Hp;
	baseAtack = Atack;
	baseDefence = Defence;
	baseSpeed = Speed;

	hp = baseHp;
	atack = baseAtack;
	defence = baseDefence;
	speed = baseSpeed;

	nowHp = hp;
	nowHpVarWidth = nowHp / hp;
	maxMotionIndex = gManager->GetMaxIndex(GameManager::index::PLAYER);
	playerInMap = gManager->WorldToLocal(pos);
}

Player::~Player()
{
}
//�̗͂Ɩ����x��ύX����֐�
void Player::ChangeBaseStatus(int ManpukuMove, int HpMove)
{
	manPuku += ManpukuMove;
	if (manPuku < 0)manPuku = 0;
	else if (manPuku > 100)manPuku = 100;

	TakeHpEffect(HpMove);

}
//������ύX����֐�
void Player::ChangeEquipItem(equipItem* item)
{
	int subId = item->getItemData(9);
	//1->����,2->head,3->chest,4->glove,5->boots,6->shield

		//�������łɑ������Ă�����̂������
	if (myEquip[subId - 1] != nullptr) {
		myEquip[subId - 1]->ChangeEquip();
		//myEquip[subId - 1] = nullptr;
		//����ւ���(�{���͂�����inventory�ɂ���A�C�e����[E]�}�[�N��؂�ւ�����)
		item->ChangeEquip();
		myEquip[subId - 1] = item;
	}
	else {
		item->ChangeEquip();
		myEquip[subId - 1] = item;
	}
	for (int i = 0; i < 4; ++i) {
		ChangeStatus(i, item->getItemData(i + 5), 1);
	}
}
//�������O���֐�
void Player::RemoveEquipItem(equipItem* item)
{
	int subId = item->getItemData(9);
	//1->����,2->head,3->chest,4->glove,5->boots,6->shield
	myEquip[subId - 1]->ChangeEquip();
	myEquip[subId - 1] = nullptr;

	GetSumStatusFromEquipment();
	for (int i = 0; i < 4; ++i) {
		ChangeStatus(i, statuses[i], 1);
	}
}
//�����A�C�e���̍��v�X�e�[�^�X���i�[����֐�
void Player::GetSumStatusFromEquipment()
{
	//�������������Ă���A�C�e���� HP/atack/defence/speed�����ꂼ�ꍇ�v���Ĕz��ɂ��܂�����
	//0�ɖ߂�
	for (int k = 0; k < 4; ++k) {
		statuses[k] = 0;
	}
	for (int i = 0; i < 4; ++i) {
		for (int k = 0; k < 6; ++k) {
			if (myEquip[k] == nullptr)continue;
			statuses[i] = myEquip[k]->getItemData(i + 5);
		}
	}
}
int Player::GetStatusFromEquipment(int subId, int statusType)
{
	//subId�ɑΉ����鑕���A�C�e���̎��AstatusType�ɑΉ�����X�e�[�^�X��Ԃ�
	return myEquip[subId - 1]->getItemData(statusType + 5);
}
//���t�@�N�^�����O�K�{�@���Ƃŕς���:�D��x��
bool Player::Move()
{
	//�L�����̈ʒu���}�b�v��̂ǂ̃`�b�v�����肷��
	t2k::Vector3 playerInMap = gManager->WorldToLocal(pos);

	if (left)
	{
		DashToDir(LEFT, playerInMap);
		return true;
	}
	else if (up)
	{
		DashToDir(UP, playerInMap);
		return true;
	}
	else if (right)
	{
		DashToDir(RIGHT, playerInMap);
		return true;
	}
	else if (down)
	{
		DashToDir(DOWN, playerInMap);
		return true;
	}

	//����shift���ꏏ�ɉ����Ă�����ǂ��G�ɓ�����܂Ń_�b�V������
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_LEFT) && t2k::Input::isKeyDown(t2k::Input::KEYBORD_LSHIFT)) {
		mydir = Actor::LEFT;
		DashToDir(LEFT, playerInMap);
		left = true;
		gManager->setPlayerRoomNum(gManager->CheckIsThere(playerInMap));
		return true;
		//����shift���ꏏ�ɉ����Ă�����ǂ��G�ɓ�����܂Ń_�b�V������
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_UP) && t2k::Input::isKeyDown(t2k::Input::KEYBORD_LSHIFT)) {
		mydir = Actor::UP;
		DashToDir(UP, playerInMap);
		up = true;
		gManager->setPlayerRoomNum(gManager->CheckIsThere(playerInMap));
		return true;
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RIGHT) && t2k::Input::isKeyDown(t2k::Input::KEYBORD_LSHIFT)) {
		mydir = Actor::RIGHT;
		DashToDir(RIGHT, playerInMap);
		right = true;
		gManager->setPlayerRoomNum(gManager->CheckIsThere(playerInMap));
		return true;
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_DOWN) && t2k::Input::isKeyDown(t2k::Input::KEYBORD_LSHIFT)) {
		mydir = Actor::DOWN;
		DashToDir(DOWN, playerInMap);
		down = true;
		gManager->setPlayerRoomNum(gManager->CheckIsThere(playerInMap));
		return true;
	}


	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_LEFT)) {
		mydir = Actor::LEFT;
		//�L�����̃`�b�v�̍��̃`�b�v��WALL�Ȃ�ړ����Ȃ�
		if ((gManager->GetMapChip(playerInMap + t2k::Vector3(-1, 0, 0))) == 0)return false;
		//�G�����̈ʒu�ɂ���Έړ����Ȃ�
		if (gManager->CheckIsThereEnemyToDir(playerInMap + t2k::Vector3(-1, 0, 0)))return false;

		//�L�����̃`�b�v�̍��̃`�b�v��PASSWAY�Ȃ�ړ�����
		MoveToDir(LEFT, playerInMap);
		return true;
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_UP)) {
		mydir = Actor::UP;
		//�L�����̃`�b�v�̍��̃`�b�v��WALL�Ȃ�ړ����Ȃ�
		if ((gManager->GetMapChip(playerInMap + t2k::Vector3(0, -1, 0))) == 0)return false;
		//�G�����̈ʒu�ɂ���Έړ����Ȃ�
		if (gManager->CheckIsThereEnemyToDir(playerInMap + t2k::Vector3(0, -1, 0)))return false;

		//�L�����̃`�b�v�̍��̃`�b�v��PASSWAY�Ȃ�ړ�����
		MoveToDir(UP, playerInMap);
		return true;
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RIGHT)) {
		mydir = Actor::RIGHT;
		//�L�����̃`�b�v�̍��̃`�b�v��WALL�Ȃ�ړ����Ȃ�
		if ((gManager->GetMapChip(playerInMap + t2k::Vector3(1, 0, 0))) == 0)return false;
		//�G�����̈ʒu�ɂ���Έړ����Ȃ�
		if (gManager->CheckIsThereEnemyToDir(playerInMap + t2k::Vector3(1, 0, 0)))return false;

		//�L�����̃`�b�v�̍��̃`�b�v��PASSWAY�Ȃ�ړ�����
		MoveToDir(RIGHT, playerInMap);
		return true;
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_DOWN)) {
		mydir = Actor::DOWN;
		//�L�����̃`�b�v�̍��̃`�b�v��WALL�Ȃ�ړ����Ȃ�
		if ((gManager->GetMapChip(playerInMap + t2k::Vector3(0, 1, 0))) == 0)return false;
		//�G�����̈ʒu�ɂ���Έړ����Ȃ�
		if (gManager->CheckIsThereEnemyToDir(playerInMap + t2k::Vector3(0, 1, 0)))return false;
		//�L�����̃`�b�v�̍��̃`�b�v��PASSWAY�Ȃ�ړ�����
		MoveToDir(DOWN, playerInMap);
		return true;
	}
	return false;
}

void Player::TownMove(dir nextDir)
{
	playerInMap = gManager->WorldToLocal(pos);
	mydir = nextDir;
	//MoveToDir(nextDir, playerInMap);
	t2k::Vector3 nextPos = gManager->GetMultipleVector(nextDir, 20);
	pos.x += nextPos.x;
	pos.y += nextPos.y;
}
//HP�o�[�̕`��
void Player::HpVarDraw()
{
	DrawRotaGraph(pos.x - gManager->camera->cameraPos.x, pos.y - gManager->camera->cameraPos.y - 30, 1, 0, hpVar_gh, false);
	DrawRotaGraph3(pos.x - gManager->camera->cameraPos.x - 15, pos.y - gManager->camera->cameraPos.y - 30 - 5, 0, 0,
		nowHpVarWidth, 1, 0, nowHpVar_gh, false);
	DrawStringEx(pos.x - gManager->camera->cameraPos.x - 10, pos.y - gManager->camera->cameraPos.y - 50, -1, "%.0f", nowHp);
}
//�o���l�擾�ƃ��x���A�b�v����
bool Player::AddExp(const int num)
{
	nowExp += num;
	if (nowExp >= nextLevelExp) {
		level++;
		nowExp = nowExp - nextLevelExp;
		SetLevelStatus();
		SetNextExp();
		return true;
	}
	return false;
}

void Player::LevelUp()
{
	level++;
	SetLevelStatus();
	SetNextExp();
}

//�߂��ɓG�����Ȃ���Ύw������ɐi�ފ֐�
void Player::DashToDir(int dir, t2k::Vector3 mapPos)
{
	if (dir == LEFT) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(-1, 0, 0))) == 0 || gManager->CheckNearByPlayerToAllEnemy(2))
		{
			left = false;
			return;
		}
		pos.x -= 20;
		//gManager->CameraMove(gManager->MAPWIDTH * 20, gManager->MAPHEIGHT * 20,dir);
		gManager->CameraReset();
	}
	else if (dir == UP) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(0, -1, 0))) == 0 || gManager->CheckNearByPlayerToAllEnemy(2))
		{
			up = false;
			return;
		}
		pos.y -= 20;
		//gManager->CameraMove(gManager->MAPWIDTH * 20, gManager->MAPHEIGHT * 20, dir);
		gManager->CameraReset();
	}
	else if (dir == RIGHT) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(1, 0, 0))) == 0 || gManager->CheckNearByPlayerToAllEnemy(2))
		{
			right = false;
			return;
		}
		pos.x += 20;
		//gManager->CameraMove(gManager->MAPWIDTH * 20, gManager->MAPHEIGHT * 20, dir);
		gManager->CameraReset();
	}
	else if (dir == DOWN) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(0, 1, 0))) == 0 || gManager->CheckNearByPlayerToAllEnemy(2))
		{
			down = false;
			return;
		}
		pos.y += 20;
		//gManager->CameraMove(gManager->MAPWIDTH * 20, gManager->MAPHEIGHT * 20, dir);
		gManager->CameraReset();
	}

	gManager->setPlayerRoomNum(gManager->CheckIsThere(mapPos));
}
//�w�������1�}�X�i�ފ֐�
void Player::MoveToDir(int dir, t2k::Vector3 mapPos)
{
	if (dir == LEFT) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(-1, 0, 0))) == 0)return;
		pos.x -= 20;
		//gManager->CameraMove(gManager->MAPWIDTH * 20, gManager->MAPHEIGHT * 20, dir);
		gManager->CameraReset();
	}
	else if (dir == UP) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(0, -1, 0))) == 0)return;

		pos.y -= 20;
		//gManager->CameraMove(gManager->MAPWIDTH * 20, gManager->MAPHEIGHT * 20, dir);
		gManager->CameraReset();
	}
	else if (dir == RIGHT) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(1, 0, 0))) == 0)return;
		pos.x += 20;
		//gManager->CameraMove(gManager->MAPWIDTH * 20, gManager->MAPHEIGHT * 20, dir);
		gManager->CameraReset();
	}
	else if (dir == DOWN) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(0, 1, 0))) == 0)return;
		pos.y += 20;
		//gManager->CameraMove(gManager->MAPWIDTH * 20, gManager->MAPHEIGHT * 20, dir);
		gManager->CameraReset();
	}

	gManager->setPlayerRoomNum(gManager->CheckIsThere(mapPos));
}
void Player::RemoveAllEquip()
{
	for (int i = 0; i < 6; ++i) {
		myEquip[i] = nullptr;
	}
	GetSumStatusFromEquipment();
	for (int i = 0; i < 4; ++i) {
		ChangeStatus(i, statuses[i], 1);
	}
}
//���t�@�N�^�����O�K�{�@���Ƃŕς���:�D��x��
//�v���C���[�X�e�[�^�X�̕`��
void Player::DrawPlayerStatus(int x, int y, int width, int height)
{

	DrawStringEx(x + 10, y + yBuf, -1, "���݂̃��x��:%d", level);//20
	DrawStringEx(x + 10, y + yBuf * 2, -1, "�̗�:%.0f/%d", nowHp, hp);	//40
	DrawStringEx(x + 10, y + yBuf * 3, -1, "�U����:%d", atack);	//60
	DrawStringEx(x + 10, y + yBuf * 4, -1, "�h���:%d", defence);	//80
	DrawStringEx(x + 10, y + yBuf * 5, -1, "�f����:%d", speed);	//100
	for (int i = 0; i < 6; ++i) {
		DrawStringEx(x + 160, y + 20 + 30 * i, -1, "%s:", equipName[i].c_str());
	}
	for (int i = 0; i < 6; ++i) {
		if (myEquip[i] == nullptr) {
			DrawStringEx(x + 230, y + 20 + 30 * i, -1, "�����Ȃ�");
		}
		else {
			DrawStringEx(x + 230, y + 20 + 30 * i, -1, "%s", myEquip[i]->getItemName().c_str());
		}
	}

	DrawStringEx(x + 10, y + 120, -1, "�o���l:%d/%d", nowExp, nextLevelExp);

}

void Player::DeadPlayer()
{
	nowExp = 0;
	level = 1;
	//�X�e�[�^�X�̏�����
	SetLevelStatus();
	//�K�v�o���l�̏�����
	SetNextExp();
	//�����A�C�e���̔j��
	RemoveAllEquip();
	//hp�̃��Z�b�g
	nowHp = hp;
	nowHpVarWidth = nowHp / hp;
	mydir = dir::UP;
}

void Player::SetLevelStatus()
{
	int lvHp = (level - 1) * hpParLevel;
	int lvAttack = (level - 1) * statusParLevel;
	int lvDefence = (level - 1) * statusParLevel;
	int lvSpeed = (level - 1) * statusParLevel;
	int lvStatus[4] = { lvHp,lvAttack,lvDefence,lvSpeed };

	for (int i = 0; i < 4; ++i) {
		ChangeStatus(i, lvStatus[i], 0);
	}
}

void Player::SetNextExp()
{
	nextLevelExp = 100 + (level - 1) * needExpParLevel; //1lv�オ�邲�ƂɕK�v�o���l��50������
}
