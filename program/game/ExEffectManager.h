///Description
///特殊な効果を処理する関数をひとまとめにしたクラス
///アイテムやスキルなどをここから呼ぶ(予定だった)
///優先順位の都合上開発中断
///

#pragma once
#include<vector>
#include<string>
#include<unordered_map>
#include<functional>
#include"../library/t2klib.h"

class Item;

//**********まだ使用しないクラスのため一旦更新停止*********//

class ExEffectManager
{
public:
	//keyで引数返り値なしの関数を検索し、実行する関数
	std::function<void()> DoEvent(std::string key);
	//keyで引数t2k::Vector3,返り値なしの関数を検索し、実行する関数
	std::function<void(t2k::Vector3)> DoPosEvent(std::string key);

private:
	ExEffectManager();
	~ExEffectManager();

	std::vector<std::string>loadSkill;
	//各種特殊処理の効果を実行する関数を入れたmap(引数返り値なし)
	std::unordered_map<std::string, std::function<void()>> exEffects;

	//各種特殊処理の効果を実行する関数を入れたmap(引数 t2k::vector3)
	std::unordered_map<std::string, std::function<void(t2k::Vector3)>> exPosEffect;

	void SetMap(std::string key, void hoge());
	void SetPosMap(std::string key, void hoge(t2k::Vector3));

	//*****ここから下は特殊効果の実装部分*****//
	//全部屋の中からランダムな位置にワープする関数 ワープ対象の引数を取る
	void Warp(t2k::Vector3& Pos);

	void NextFloor();

	//void CreateWay(t2k::Vector3 Pos);

};

//void CreateWay(t2k::Vector3 Pos) {};