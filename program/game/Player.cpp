#include "Player.h"
#include"GameManager.h"
#include"DxLib.h"
#include"Camera.h"

extern GameManager* gManager;

Player::Player(t2k::Vector3 StartPos)
{
	pos = StartPos;
	gh = gManager->LoadGraphEx("graphics/PlayerTest.png");

}

Player::~Player()
{
}

bool Player::Move()
{
	//キャラの位置がマップ上のどのチップか特定する
	t2k::Vector3 playerInMap = gManager->WorldToLocal(pos);

	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_LEFT)) {
		//キャラのチップの左のチップがWALLなら移動しない
		if ((gManager->GetMapChip(playerInMap + t2k::Vector3(-1, 0, 0))) == 0)return false;
		//もしshiftも一緒に押していたら壁か敵に当たるまでダッシュする
		if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_LSHIFT)) {
			while (1) {
				if ((gManager->GetMapChip(playerInMap + t2k::Vector3(-1, 0, 0))) == 0 || gManager->CheckNearByPlayerToAllEnemy(2))break;
				pos.x -= 20;
				gManager->CameraMove(this);
			}
			return true;
		}
		//キャラのチップの左のチップがPASSWAYなら移動する
		pos.x -= 20;
		gManager->CameraMove(this);
		return true;
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_UP)) {

		//キャラのチップの左のチップがWALLなら移動しない
		if ((gManager->GetMapChip(playerInMap + t2k::Vector3(0, -1, 0))) == 0)return false;
		//もしshiftも一緒に押していたら壁か敵に当たるまでダッシュする
		if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_LSHIFT)) {
			while (1) {
				if ((gManager->GetMapChip(playerInMap + t2k::Vector3(0, -1, 0))) == 0 || gManager->CheckNearByPlayerToAllEnemy(2))break;
				pos.y -= 20;
				gManager->CameraMove(this);
			}
			return true;
		}
		//キャラのチップの左のチップがPASSWAYなら移動する
		pos.y -= 20;
		gManager->CameraMove(this);
		return true;
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RIGHT)) {
		//キャラのチップの左のチップがWALLなら移動しない
		if ((gManager->GetMapChip(playerInMap + t2k::Vector3(1, 0, 0))) == 0)return false;

		//もしshiftも一緒に押していたら壁か敵に当たるまでダッシュする
		if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_LSHIFT)) {
			while (1) {
				if ((gManager->GetMapChip(playerInMap + t2k::Vector3(1, 0, 0))) == 0 || gManager->CheckNearByPlayerToAllEnemy(2))break;
				pos.x += 20;
				gManager->CameraMove(this);
			}
			return true;
		}
		//キャラのチップの左のチップがPASSWAYなら移動する
		pos.x += 20;
		gManager->CameraMove(this);
		return true;
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_DOWN)) {
		//キャラのチップの左のチップがWALLなら移動しない
		if ((gManager->GetMapChip(playerInMap + t2k::Vector3(0, 1, 0))) == 0)return false;
		//もしshiftも一緒に押していたら壁か敵に当たるまでダッシュする
		if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_LSHIFT)) {
			while (1) {
				if ((gManager->GetMapChip(playerInMap + t2k::Vector3(0, 1, 0))) == 0 || gManager->CheckNearByPlayerToAllEnemy(2))break;
				pos.y += 20;
				gManager->CameraMove(this);
			}
			return true;
		}
		//キャラのチップの左のチップがPASSWAYなら移動する
		pos.y += 20;
		gManager->CameraMove(this);
		return true;
	}
	gManager->setPlayerRoomNum(gManager->CheckIsThere(playerInMap));
	return false;
}

void Player::Draw()
{
	DrawRotaGraph(pos.x - gManager->camera->cameraPos.x, pos.y - gManager->camera->cameraPos.y, 1, 0, gh, true);
}
