#include "Player.h"
#include"GameManager.h"
#include"DxLib.h"
#include"Camera.h"
#include"Item/equipItem.h"

extern GameManager* gManager;

Player::Player(t2k::Vector3 StartPos, float Hp, int Atack, int Defence, int Speed, int ActId)
{
	actId = ActId;
	pos = StartPos;
	//p_gh = gManager->LoadGraphEx("graphics/PlayerTest.png");

	LoadDivGraph("graphics/Character_1.png", 12, 3, 4, 32, 32, gh);

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
}

Player::~Player()
{
}
//
//void Player::TakeHpEffect(int HpMove)
//{
//	if (nowHp <= 0)return;
//	nowHp += HpMove;
//	if (nowHp < 0)nowHp = 0;
//	if (nowHp > hp)nowHp = hp;
//
//	nowHpVarWidth = nowHp / hp;
//}

void Player::ChangeBaseStatus(int ManpukuMove, int HpMove)
{
	manpuku += ManpukuMove;
	if (manpuku < 0)manpuku = 0;
	else if (manpuku > 100)manpuku = 100;

	TakeHpEffect(HpMove);

}

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


/*
void Player::Draw()
{
	//t2k::Vector3 gp(0, 0, 0);
	//t2k::Vector3 fix;
	//fix.x = pos.x ;
	//fix.y = pos.y ;
	////gp += (fix - gp) * 0.1f;
	//t2k::Vector3 move= (fix - gp) * 0.1f;//10f�őS�Ĉړ�����
	//gp += move;


	//DrawRotaGraph(pos.x - gManager->camera->cameraPos.x, pos.y - gManager->camera->cameraPos.y, 1, 0, gh, true);
	//DrawRotaGraph(gp.x, gp.y, 1, 0, p_gh, true);
	//DrawRotaGraph(gp.x - gManager->camera->cameraPos.x, gp.y - gManager->camera->cameraPos.y, 1, 0, p_gh, true);
}
*/

void Player::HpVarDraw()
{
	DrawRotaGraph(pos.x - gManager->camera->cameraPos.x, pos.y - gManager->camera->cameraPos.y - 20, 1, 0, hpVar_gh, false);
	DrawRotaGraph3(pos.x - gManager->camera->cameraPos.x - 15, pos.y - gManager->camera->cameraPos.y - 20 - 5, 0, 0,
		nowHpVarWidth, 1, 0, nowHpVar_gh, false);
	DrawStringEx(pos.x - gManager->camera->cameraPos.x - 10, pos.y - gManager->camera->cameraPos.y - 40, -1, "%.0f", nowHp);
}

void Player::DashToDir(int dir, t2k::Vector3 mapPos)
{
	if (dir == LEFT) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(-1, 0, 0))) == 0 || gManager->CheckNearByPlayerToAllEnemy(2))
		{
			left = false;
			return;
		}
		pos.x -= 20;
		gManager->CameraMove();
	}
	else if (dir == UP) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(0, -1, 0))) == 0 || gManager->CheckNearByPlayerToAllEnemy(2))
		{
			up = false;
			return;
		}
		pos.y -= 20;
		gManager->CameraMove();
	}
	else if (dir == RIGHT) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(1, 0, 0))) == 0 || gManager->CheckNearByPlayerToAllEnemy(2))
		{
			right = false;
			return;
		}
		pos.x += 20;
		gManager->CameraMove();
	}
	else if (dir == DOWN) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(0, 1, 0))) == 0 || gManager->CheckNearByPlayerToAllEnemy(2))
		{
			down = false;
			return;
		}
		pos.y += 20;
		gManager->CameraMove();
	}

	gManager->setPlayerRoomNum(gManager->CheckIsThere(mapPos));
}

void Player::MoveToDir(int dir, t2k::Vector3 mapPos)
{
	if (dir == LEFT) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(-1, 0, 0))) == 0)return;
		pos.x -= 20;
		gManager->CameraMove();
	}
	else if (dir == UP) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(0, -1, 0))) == 0)return;

		pos.y -= 20;
		gManager->CameraMove();
	}
	else if (dir == RIGHT) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(1, 0, 0))) == 0)return;
		pos.x += 20;
		gManager->CameraMove();
	}
	else if (dir == DOWN) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(0, 1, 0))) == 0)return;
		pos.y += 20;
		gManager->CameraMove();
	}

	gManager->setPlayerRoomNum(gManager->CheckIsThere(mapPos));
}

void Player::DrawPlayerStatus()
{
	DrawStringEx(20, 30, -1, "���݂̃��x��:%d", level);
	DrawStringEx(20, 50, -1, "�̗�:%.0f", nowHp);
	DrawStringEx(20, 70, -1, "�U����:%d", atack);
	DrawStringEx(20, 90, -1, "�h���:%d", defence);
	DrawStringEx(20, 110, -1, "�f����:%d", speed);
	for (int i = 0; i < 6; ++i) {
		DrawStringEx(170, 30 + 30 * i, -1, "%s:", equipName[i].c_str());
	}
	for (int i = 0; i < 6; ++i) {
		if (myEquip[i] == nullptr) {
			DrawStringEx(240, 30 + 30 * i, -1, "�����Ȃ�");
		}
		else {
			DrawStringEx(240, 30 + 30 * i, -1, "%s", myEquip[i]->getItemName().c_str());
		}
	}

}
