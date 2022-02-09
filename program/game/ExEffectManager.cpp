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
bool ExEffectManager::ThrowItem(int dir, t2k::Vector3& Pos)
{
	//現在のitemの座標を取得する(描画座標)
	//t2k::Vector3 drawPos = item->GetItemDrawPos();
	// 
	//投げた対象が向いている向きを取得する(dir)
	//投げた向きに向かって毎フレーム少しずつ移動させる
	//上
	if (dir == 0) {
		Pos.y -= 0.01;
	}//右
	else if (dir == 1) {
		Pos.x += 0.01;
	}//下
	else if (dir == 2) {
		Pos.y += 0.01;
	}//左
	else if (dir == 3) {
		Pos.x -= 0.01;
	}


	//アイテムを描画する
	//壁か敵にあたっていなければ再帰的に呼ぶ
	//マップ座標
	t2k::Vector3 mapPos = gManager->WorldToLocal(Pos);
	t2k::Vector3 nextPos;

	//上
	if (dir == 0) {
		nextPos = mapPos + t2k::Vector3(0, -1, 0);
	}//右
	else if (dir == 1) {
		nextPos = mapPos + t2k::Vector3(1, 0, 0);

	}//下
	else if (dir == 2) {
		nextPos = mapPos + t2k::Vector3(0, 1, 0);

	}//左
	else if (dir == 3) {
		nextPos = mapPos + t2k::Vector3(-1, 0, 0);
	}
	//壁なら
	if (gManager->GetMapChip(nextPos) ==0) {
		return true;
	}//敵なら



		return false;
}
void ExEffectManager::Warp(t2k::Vector3& Pos)
{
	Pos = gManager->SetStartPos(0);
}

void ExEffectManager::NextFloor()
{
	//階層を一つ移動させたかった
	DungeonScene* hoge = SceneManager::CastScene<DungeonScene>();
	if (hoge == nullptr)return;
	//hoge->SetDungeonLevel(1);
	hoge->MoveLevel(1);
}

void CreateWay(t2k::Vector3 Pos)
{
	//使った場所を中心に縦横3マスずつ通路にする
	gManager->SetChip(Pos.x - 2, Pos.y - 2, Pos.x + 2, Pos.y + 2);
}

