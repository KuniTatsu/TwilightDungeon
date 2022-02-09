#include "ExEffectManager.h"
#include"GameManager.h"
#include"Map.h"
#include"SceneManager.h"
#include"DungeonScene.h"
#include"Item/Item.h"

extern GameManager* gManager;

void CreateWay(t2k::Vector3 Pos);

ExEffectManager::ExEffectManager()
{
	//SetMap("Warp",Warp)
	//SetPosMap("CreateWay", CreateWay);
}

ExEffectManager::~ExEffectManager()
{
}

void ExEffectManager::SetMap(std::string key, void hoge())
{
	//��������ʂ̏����֐���map�Ɏ��[����
	exEffects.insert(std::make_pair(key, hoge));
}

void ExEffectManager::SetPosMap(std::string key, void hoge(t2k::Vector3))
{
	//��������ʂ̏����֐���map�Ɏ��[����
	exPosEffect.insert(std::make_pair(key, hoge));

}
std::function<void()> ExEffectManager::DoEvent(std::string key)
{
	auto it = exEffects.find(key);
	if (it != exEffects.end()) {
		return exEffects[key];
	}
	return std::function<void()>();
}
std::function<void(t2k::Vector3)> ExEffectManager::DoPosEvent(std::string key)
{
	auto it = exPosEffect.find(key);
	if (it != exPosEffect.end()) {
		return exPosEffect[key];
	}
	return std::function<void(t2k::Vector3)>();
}
bool ExEffectManager::ThrowItem(int dir, t2k::Vector3& Pos)
{
	//���݂�item�̍��W���擾����(�`����W)
	//t2k::Vector3 drawPos = item->GetItemDrawPos();
	// 
	//�������Ώۂ������Ă���������擾����(dir)
	//�����������Ɍ������Ė��t���[���������ړ�������
	//��
	if (dir == 0) {
		Pos.y -= 0.01;
	}//�E
	else if (dir == 1) {
		Pos.x += 0.01;
	}//��
	else if (dir == 2) {
		Pos.y += 0.01;
	}//��
	else if (dir == 3) {
		Pos.x -= 0.01;
	}


	//�A�C�e����`�悷��
	//�ǂ��G�ɂ������Ă��Ȃ���΍ċA�I�ɌĂ�
	//�}�b�v���W
	t2k::Vector3 mapPos = gManager->WorldToLocal(Pos);
	t2k::Vector3 nextPos;

	//��
	if (dir == 0) {
		nextPos = mapPos + t2k::Vector3(0, -1, 0);
	}//�E
	else if (dir == 1) {
		nextPos = mapPos + t2k::Vector3(1, 0, 0);

	}//��
	else if (dir == 2) {
		nextPos = mapPos + t2k::Vector3(0, 1, 0);

	}//��
	else if (dir == 3) {
		nextPos = mapPos + t2k::Vector3(-1, 0, 0);
	}
	//�ǂȂ�
	if (gManager->GetMapChip(nextPos) ==0) {
		return true;
	}//�G�Ȃ�



		return false;
}
void ExEffectManager::Warp(t2k::Vector3& Pos)
{
	Pos = gManager->SetStartPos(0);
}

void ExEffectManager::NextFloor()
{
	//�K�w����ړ�������������
	DungeonScene* hoge = SceneManager::CastScene<DungeonScene>();
	if (hoge == nullptr)return;
	//hoge->SetDungeonLevel(1);
	hoge->MoveLevel(1);
}

void CreateWay(t2k::Vector3 Pos)
{
	//�g�����ꏊ�𒆐S�ɏc��3�}�X���ʘH�ɂ���
	gManager->SetChip(Pos.x - 2, Pos.y - 2, Pos.x + 2, Pos.y + 2);
}

