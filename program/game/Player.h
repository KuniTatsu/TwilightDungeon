///*****Description*****
///プレイヤーに関するクラス
///プレイヤーの移動と装備やステータスの変更を行う
/// Actorを継承している。
///*********************

#pragma once
#include"../library/t2klib.h"
#include"Actor/Actor.h"

class equipItem;
class Player : public Actor
{
public:
	Player(){}
	Player(const t2k::Vector3& StartPos, float Hp, int Attack, int Defence, int Speed, int ActId);
	~Player();

	//足踏みフラグ
	bool skip = false;

	t2k::Vector3 localPos = {};


	//満腹度とHPの変更関数(主に食料アイテムの使用で使う)
	void ChangeBaseStatus(const int ManpukuMove, const int HpMove);

	//装備欄の変更関数
	void ChangeEquipItem(equipItem* item);

	//装備を外す関数
	void RemoveEquipItem(equipItem* item);

	//装備しているかどうか返す関数
	inline bool CheckEquip(int subId) {
		if (myEquip[subId - 1] != nullptr)return true;
		return false;
	}

	//装備によるステータス上昇値を獲得する関数
	void GetSumStatusFromEquipment();

	//特定の装備のステータスを取得する関数
	//arg2:0->hp,1->attack,2->defence,3->speed
	int GetStatusFromEquipment(int subId,int statusType);

	inline t2k::Vector3 GetPlayerLocalPos() {
		return playerInMap;
	}

	void SetPlayerLocalPos();

	//移動関数
	bool Move()override;
	//Campでの移動関数
	void TownMove(dir nextDir);
	//HPバーの描画
	void HpVarDraw();

	//経験値の獲得関数
	bool AddExp(const int num);

	//デバッグ用レベルアップ関数
	void LevelUp();

	//プレイヤーのステータス,装備を描画する関数
	void DrawPlayerStatus(int x,int y,int width,int height);

	//プレイヤー死亡時の処理
	void DeadPlayer();

	//所持金を変化させる関数
	inline void ChangeHaveCoin(int MoveNum) {
		coin += MoveNum;
		if (coin < 0)coin = 0;
	}
	//所持金を取得する関数
	inline int GetHaveCoin() {
		return coin;
	}
private:

	t2k::Vector3 playerInMap;
	//満腹度 機能未実装
	int manPuku = 100;
	//今保持している経験値
	int nowExp = 0;
	//次のレベルまでの必要経験値 だんだん多くなる
	int nextLevelExp = 100;
	//レベルごとの必要経験値倍率
	const int needExpParLevel = 50;

	//レベルごとの基本上昇ステータス倍率
	const int statusParLevel = 2;
	//レベルごとの体力上昇倍率
	const int hpParLevel = 20;

	//表示のy座標間隔
	const int yBuf = 20;

	//レベルによる上昇ステータスの設定関数
	void SetLevelStatus();
	//次のレベルまでの必要経験値を更新する関数
	void SetNextExp();
	//装備アイテム
	equipItem* myEquip[6] = { nullptr,nullptr, nullptr, nullptr, nullptr, nullptr };

	const std::string equipName[6] = { "WEAPON","HEAD","CHEST","GLOVE","BOOTS","SHIELD" };
	//hp,attack,defence,speed
	int statuses[4] = { 0,0,0,0 };

	int coin = 10000000;

	//突き当りまで走るフラグ
	bool left = false;
	bool up = false;
	bool right = false;
	bool down = false;
	bool isDash = false;


	//ダッシュ中なら移動する関数
	//bool DetectDash();

	void DashToDir(const int dir, const t2k::Vector3 mapPos);
	void MoveToDir(const int dir, const t2k::Vector3 mapPos);

	//装備をすべて外す関数
	void RemoveAllEquip();

};
