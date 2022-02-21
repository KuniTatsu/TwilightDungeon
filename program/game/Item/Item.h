///*****Description*****
///アイテムに関するクラス
///equipItemクラスを派生先に持つ
///アイテムの各種情報と
///現在の各アイテムごとの状況を保持する
///*********************
#pragma once
#include<string>
#include<vector>
#include"../../library/t2klib.h"
#include"../../support/Support.h"

class Item
{
public:
	Item() {};
	Item(int Id,int ItemType,std::string ItemName,int Saturation,int Heal,int HitDamage,std::string Gh,std::string Desc);
	
	virtual ~Item();
	//アイテムIDの取得
	int GetItemId();
	//アイテムデータの描画
	void DrawItemData(int x,int y);
	//popしているアイテムの描画
	void DrawPopItem();
	 
	//ColumnId: 0->id,1->type,2->manpukudo,3->heal,4->hitdamage
	virtual int getItemData(int ColumnId);
	const std::string getItemName();

	//生成時に必要なint型アイテムデータをvectorで返す関数 Id,ItemType,Saturation,Heal,HitDamage
	virtual std::vector<int> GetAllIntData();
	//生成時に必要なstringデータをvectorで返す関数 Name,Gh,Desc
	std::vector<std::string>GetAllStringData();

	void SetPos(t2k::Vector3 Pos);

	t2k::Vector3& GetPos();
	//プレイヤーが踏んでいるかどうかの取得関数
	bool DetectOnPlayer(t2k::Vector3 Pos);

	//ポップ状態かどうか取得
	bool GetIsLive();
	//アイテムを取得した時にポップ状態ではなくする関数
	void SetIsLiveFalse();
	//アイテム座標の取得
	t2k::Vector3 GetItemDrawPos();
	

	//移動補完 移動先設定
	void SetGoalPos(int dir);

	bool ThrowItem(int dir);

	//投げられているアイテムの描画関数
	void DrawThrowItem();
	
protected:

	//id(int)	ItemType(int)	Item_Name(std::string)	AmountOfSaturation(int)	AmountOfHeal(int)	HitDamage(int) Gh(std::string)

	int id = 0;
	int itemType = 0;
	std::string itemName = "";
	int amountOfSaturation = 0;
	int amountOfHeal = 0;
	int hitDamage = 0;
	std::string ghData;
	int gh = 0;
	std::string desc = {};
	//名前と説明文の入った配列
	std::string itemDesc[2] = {};
	//描画座標
	t2k::Vector3 pos = { 0,0,0 };
	//マップ座標
	t2k::Vector3 mapPos = { 0,0,0 };

	bool isLive = true;

	//移動補完系
	t2k::Vector3 goalPos;
	t2k::Vector3 nowPos;
	t2k::Vector3 move;

	//飛んでいく速さ
	float moveSpeed = 0.08f;

	bool init = false;
	
};

