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
	//ì‚Á‚½Œø‰Ê‚Ìˆ—ŠÖ”‚ğmap‚Éû”[‚·‚é
	exEffects.insert(std::make_pair(key, hoge));
}

void ExEffectManager::SetPosMap(std::string key, void hoge(t2k::Vector3))
{
	//ì‚Á‚½Œø‰Ê‚Ìˆ—ŠÖ”‚ğmap‚Éû”[‚·‚é
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


void ExEffectManager::Warp(t2k::Vector3& Pos)
{
	Pos = gManager->SetStartPos(0);
}

void ExEffectManager::NextFloor()
{
	//ŠK‘w‚ğˆê‚ÂˆÚ“®‚³‚¹‚½‚©‚Á‚½
	DungeonScene* hoge = SceneManager::CastScene<DungeonScene>();
	if (hoge == nullptr)return;
	//hoge->SetDungeonLevel(1);
	hoge->MoveLevel(1);
}

void CreateWay(t2k::Vector3 Pos)
{
	//g‚Á‚½êŠ‚ğ’†S‚Éc‰¡3ƒ}ƒX‚¸‚Â’Ê˜H‚É‚·‚é
	gManager->SetChip(Pos.x - 2, Pos.y - 2, Pos.x + 2, Pos.y + 2);
}

