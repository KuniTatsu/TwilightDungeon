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

void Player::Move()
{

	//�L�����̈ʒu���}�b�v��̂ǂ̃`�b�v�����肷��
	t2k::Vector3 playerInMap = gManager->WorldToLocal(pos);

	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_LEFT)) {
		//�L�����̃`�b�v�̍��̃`�b�v��WALL�Ȃ�ړ����Ȃ�
		if ((gManager->GetMapChip(playerInMap + t2k::Vector3(-1, 0, 0))) == 0)return;
		//�L�����̃`�b�v�̍��̃`�b�v��PASSWAY�Ȃ�ړ�����
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

void Player::Draw()
{
	DrawRotaGraph(pos.x- gManager->camera->cameraPos.x, pos.y - gManager->camera->cameraPos.y, 1, 0, gh, true);
}
