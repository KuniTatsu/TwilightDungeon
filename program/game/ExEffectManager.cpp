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
	//作った効果の処理関数をmapに収納する
	exEffects.insert(std::make_pair(key, hoge));
}

void ExEffectManager::SetPosMap(std::string key, void hoge(t2k::Vector3))
{
	//作った効果の処理関数をmapに収納する
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
	Pos = gManager->SetStartPos(GameManager::setStartPosType::PLAYER);
}

void ExEffectManager::NextFloor()
{
	//階層を一つ移動させたかった
	DungeonScene* dungeon = SceneManager::CastScene<DungeonScene>();
	if (dungeon == nullptr)return;
	dungeon->MoveLevel(1);
}

void CreateWay(t2k::Vector3 Pos)
{
	//使った場所を中心に縦横3マスずつ通路にする
	gManager->SetChip(Pos.x - 2, Pos.y - 2, Pos.x + 2, Pos.y + 2);
}

