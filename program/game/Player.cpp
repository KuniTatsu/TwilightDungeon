#include "Player.h"
#include"GameManager.h"
#include"DxLib.h"
#include"Camera.h"
#include"Item/equipItem.h"

extern GameManager* gManager;

Player::Player(t2k::Vector3 StartPos, float Hp, int Atack, int Defence, int Speed)
{
	pos = StartPos;
	p_gh = gManager->LoadGraphEx("graphics/PlayerTest.png");
	nowHpVar_gh = gManager->LoadGraphEx("graphics/haveHpVar.png");
	hpVar_gh = gManager->LoadGraphEx("graphics/AllHpVar.png");

	name = "Player";

	hp = Hp;
	atack = Atack;
	defence = Defence;
	speed = Speed;

	nowHp = hp;
}

Player::~Player()
{
}

void Player::TakeHpEffect(int HpMove)
{
	if (nowHp <= 0)return;
	nowHp += HpMove;
	if (nowHp < 0)nowHp = 0;
	if (nowHp > hp)nowHp = hp;

	nowHpVarWidth = nowHp / hp;
}

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
	//1->武器,2->head,3->chest,4->glove,5->boots,6->shield
	if (subId == 1) {
		//もうすでに装備しているものがあれば
		if (myEquip[0] != nullptr) {
			//入れ替える
			

		}
	}else if (subId == 2) {

	}
	else if (subId == 3) {

	}
	else if (subId == 4) {

	}
	else if (subId == 5) {

	}
	else if (subId == 6) {

	}
}

bool Player::Move()
{
	//キャラの位置がマップ上のどのチップか特定する
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

	//もしshiftも一緒に押していたら壁か敵に当たるまでダッシュする
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_LEFT) && t2k::Input::isKeyDown(t2k::Input::KEYBORD_LSHIFT)) {
		DashToDir(LEFT, playerInMap);
		left = true;
		gManager->setPlayerRoomNum(gManager->CheckIsThere(playerInMap));
		return true;
		//もしshiftも一緒に押していたら壁か敵に当たるまでダッシュする
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_UP) && t2k::Input::isKeyDown(t2k::Input::KEYBORD_LSHIFT)) {

		DashToDir(UP, playerInMap);
		up = true;
		gManager->setPlayerRoomNum(gManager->CheckIsThere(playerInMap));
		return true;
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RIGHT) && t2k::Input::isKeyDown(t2k::Input::KEYBORD_LSHIFT)) {
		DashToDir(RIGHT, playerInMap);
		right = true;
		gManager->setPlayerRoomNum(gManager->CheckIsThere(playerInMap));
		return true;
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_DOWN) && t2k::Input::isKeyDown(t2k::Input::KEYBORD_LSHIFT)) {
		DashToDir(DOWN, playerInMap);
		down = true;
		gManager->setPlayerRoomNum(gManager->CheckIsThere(playerInMap));
		return true;
	}


	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_LEFT)) {
		//キャラのチップの左のチップがWALLなら移動しない
		if ((gManager->GetMapChip(playerInMap + t2k::Vector3(-1, 0, 0))) == 0)return false;

		//キャラのチップの左のチップがPASSWAYなら移動する
		MoveToDir(LEFT, playerInMap);
		return true;
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_UP)) {

		//キャラのチップの左のチップがWALLなら移動しない
		if ((gManager->GetMapChip(playerInMap + t2k::Vector3(0, -1, 0))) == 0)return false;

		//キャラのチップの左のチップがPASSWAYなら移動する
		MoveToDir(UP, playerInMap);
		return true;
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RIGHT)) {
		//キャラのチップの左のチップがWALLなら移動しない
		if ((gManager->GetMapChip(playerInMap + t2k::Vector3(1, 0, 0))) == 0)return false;

		//キャラのチップの左のチップがPASSWAYなら移動する
		MoveToDir(RIGHT, playerInMap);
		return true;
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_DOWN)) {
		//キャラのチップの左のチップがWALLなら移動しない
		if ((gManager->GetMapChip(playerInMap + t2k::Vector3(0, 1, 0))) == 0)return false;
		//キャラのチップの左のチップがPASSWAYなら移動する
		MoveToDir(DOWN, playerInMap);
		return true;
	}
	return false;
}

void Player::Draw()
{
	DrawRotaGraph(pos.x - gManager->camera->cameraPos.x, pos.y - gManager->camera->cameraPos.y, 1, 0, p_gh, true);

	//Hpバー関連

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
		gManager->CameraMove(this);
	}
	else if (dir == UP) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(0, -1, 0))) == 0 || gManager->CheckNearByPlayerToAllEnemy(2))
		{
			up = false;
			return;
		}
		pos.y -= 20;
		gManager->CameraMove(this);
	}
	else if (dir == RIGHT) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(1, 0, 0))) == 0 || gManager->CheckNearByPlayerToAllEnemy(2))
		{
			right = false;
			return;
		}
		pos.x += 20;
		gManager->CameraMove(this);
	}
	else if (dir == DOWN) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(0, 1, 0))) == 0 || gManager->CheckNearByPlayerToAllEnemy(2))
		{
			down = false;
			return;
		}
		pos.y += 20;
		gManager->CameraMove(this);
	}

	gManager->setPlayerRoomNum(gManager->CheckIsThere(mapPos));
}

void Player::MoveToDir(int dir, t2k::Vector3 mapPos)
{
	if (dir == LEFT) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(-1, 0, 0))) == 0)return;
		pos.x -= 20;
		gManager->CameraMove(this);
	}
	else if (dir == UP) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(0, -1, 0))) == 0)return;

		pos.y -= 20;
		gManager->CameraMove(this);
	}
	else if (dir == RIGHT) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(1, 0, 0))) == 0)return;
		pos.x += 20;
		gManager->CameraMove(this);
	}
	else if (dir == DOWN) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(0, 1, 0))) == 0)return;
		pos.y += 20;
		gManager->CameraMove(this);
	}

	gManager->setPlayerRoomNum(gManager->CheckIsThere(mapPos));
}
