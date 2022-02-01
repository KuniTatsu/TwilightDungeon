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
	//�L�����̈ʒu���}�b�v��̂ǂ̃`�b�v�����肷��
	t2k::Vector3 playerInMap = gManager->WorldToLocal(pos);

	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_LEFT)) {
		//�L�����̃`�b�v�̍��̃`�b�v��WALL�Ȃ�ړ����Ȃ�
		if ((gManager->GetMapChip(playerInMap + t2k::Vector3(-1, 0, 0))) == 0)return false;
		//����shift���ꏏ�ɉ����Ă�����ǂ��G�ɓ�����܂Ń_�b�V������
		if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_LSHIFT)) {
			while (1) {
				if ((gManager->GetMapChip(playerInMap + t2k::Vector3(-1, 0, 0))) == 0 || gManager->CheckNearByPlayerToAllEnemy(2))break;
				pos.x -= 20;
				gManager->CameraMove(this);
			}
			return true;
		}
		//�L�����̃`�b�v�̍��̃`�b�v��PASSWAY�Ȃ�ړ�����
		pos.x -= 20;
		gManager->CameraMove(this);
		return true;
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_UP)) {

		//�L�����̃`�b�v�̍��̃`�b�v��WALL�Ȃ�ړ����Ȃ�
		if ((gManager->GetMapChip(playerInMap + t2k::Vector3(0, -1, 0))) == 0)return false;
		//����shift���ꏏ�ɉ����Ă�����ǂ��G�ɓ�����܂Ń_�b�V������
		if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_LSHIFT)) {
			while (1) {
				if ((gManager->GetMapChip(playerInMap + t2k::Vector3(0, -1, 0))) == 0 || gManager->CheckNearByPlayerToAllEnemy(2))break;
				pos.y -= 20;
				gManager->CameraMove(this);
			}
			return true;
		}
		//�L�����̃`�b�v�̍��̃`�b�v��PASSWAY�Ȃ�ړ�����
		pos.y -= 20;
		gManager->CameraMove(this);
		return true;
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RIGHT)) {
		//�L�����̃`�b�v�̍��̃`�b�v��WALL�Ȃ�ړ����Ȃ�
		if ((gManager->GetMapChip(playerInMap + t2k::Vector3(1, 0, 0))) == 0)return false;

		//����shift���ꏏ�ɉ����Ă�����ǂ��G�ɓ�����܂Ń_�b�V������
		if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_LSHIFT)) {
			while (1) {
				if ((gManager->GetMapChip(playerInMap + t2k::Vector3(1, 0, 0))) == 0 || gManager->CheckNearByPlayerToAllEnemy(2))break;
				pos.x += 20;
				gManager->CameraMove(this);
			}
			return true;
		}
		//�L�����̃`�b�v�̍��̃`�b�v��PASSWAY�Ȃ�ړ�����
		pos.x += 20;
		gManager->CameraMove(this);
		return true;
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_DOWN)) {
		//�L�����̃`�b�v�̍��̃`�b�v��WALL�Ȃ�ړ����Ȃ�
		if ((gManager->GetMapChip(playerInMap + t2k::Vector3(0, 1, 0))) == 0)return false;
		//����shift���ꏏ�ɉ����Ă�����ǂ��G�ɓ�����܂Ń_�b�V������
		if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_LSHIFT)) {
			while (1) {
				if ((gManager->GetMapChip(playerInMap + t2k::Vector3(0, 1, 0))) == 0 || gManager->CheckNearByPlayerToAllEnemy(2))break;
				pos.y += 20;
				gManager->CameraMove(this);
			}
			return true;
		}
		//�L�����̃`�b�v�̍��̃`�b�v��PASSWAY�Ȃ�ړ�����
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
