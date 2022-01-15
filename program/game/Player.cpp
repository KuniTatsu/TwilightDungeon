#include "Player.h"
#include"GameManager.h"


extern GameManager* gManager;

Player::Player(t2k::Vector3 StartPos)
{
	pos = StartPos;
	gh = gManager->LoadGraphEx("graphics/PlayerTest.png");

}

Player::~Player()
{
}

void Player::Move()
{

	//キャラの位置がマップ上のどのチップか特定する
	t2k::Vector3 playerInMap = gManager->WorldToLocal(pos);

	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_LEFT)) {
		//キャラのチップの左のチップがWALLなら移動しない

		//キャラのチップの左のチップがPASSWAYなら移動する

		pos.x -= 20;
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_UP)) {
		pos.y -= 20;
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RIGHT)) {
		pos.x += 20;
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_DOWN)) {
		pos.y += 20;
	}


}
