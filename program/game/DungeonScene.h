#pragma once
#include"Scene.h"
#include "../library/t2klib.h"
#include<memory>

class Map;
class Menu;
class MenuWindow;
class EnemyManager;
class Item;
class Inventory;
class Player;
class Enemy;

class DungeonScene :public BaseScene {

public:
	DungeonScene();
	~DungeonScene();


	//num体敵を作る関数
	void RandEnemyCreate(int num);

	void Update();

	void Draw();

	int GetDungeonLevel();
	void SetDungeonLevel(int addLevel);

	void MoveLevel(int addLevel);

private:
	//階層
	int dungeonLevel = 1;

	int alfa = 0;
	//次のenemyの行動までのインターバルフレーム
	const int ENEMYACTINTERVAL = 30;
	//更新インターバルフレーム
	int enemyActTimer = 30;

	std::list<std::shared_ptr<Enemy>>atackEnemies;
	std::list<std::shared_ptr<Enemy>>::iterator itr;

	//次の回に行けるwindow
	Menu* nextLevelWindow = nullptr;
	Menu* menuOpen = nullptr;
	Menu* inventory = nullptr;
	Menu* log = nullptr;
	Menu* desc = nullptr;
	Menu* playerStatus = nullptr;
	MenuWindow* use_usable = nullptr;
	MenuWindow* use_equip = nullptr;

	MenuWindow* use_nowEquip = nullptr;

	enum useType {
		USABLE,
		EQUIP,
		NOWEQUIP
	};
	useType usetype = USABLE;

	MenuWindow* firstMenu = nullptr;
	std::shared_ptr<Player> player = nullptr;
	
	//飛んでいるアイテムリスト
	std::list<Item*> throwItem;

	std::list<std::shared_ptr<Item>>throwedItemList;

	/*
	Menu* hoge=new Menu(30,50,175,390,gh(int))
	Menu* hoge=new Menu(220,50,440,390,gh(int))
	*/

	std::shared_ptr<EnemyManager>eManager = nullptr;

	//**debug
	t2k::Vector3 playerPos = {};

	t2k::Sequence<DungeonScene*> main_sequence =
		t2k::Sequence<DungeonScene*>(this, &DungeonScene::Seq_Main);

	//プレイヤー動作シークエンス
	bool Seq_Main(const float deltatime);
	//エネミー動作シークエンス
	bool Seq_EnemyAct(const float deltatime);
	//１つ目のメニュー操作
	bool Seq_FirstMenu(const float deltatime);
	//Inventory描画シークエンス
	bool Seq_InventoryOpen(const float deltatime);
	//Inventoryアイテム使用確認シークエンス
	bool Seq_InventoryUse(const float deltatime);
	//物が飛んでいるシークエンス
	bool Seq_ThrowItemMove(const float deltatime);

	//debug
	bool Seq_CameraMove(const float deltatime);
	//シークエンスの列挙体
	enum class sequence {
		MAIN,
		ENEMYACT,
		FIRSTMENU,
		INVENTORY_OPEN,
		INVENTORY_USE,
		THROWITEMMOVE,
		CAMERA

	};
	sequence nowSeq = sequence::MAIN;
	//Sequenceを移動させる関数,enumも一緒に変更する
	void ChangeSequence(sequence seq);
	//シーケンス確認関数
	void DrawNowSequence(sequence seq);

	//enemyのデータの確認関数
	void DrawEnemyData();
	//描画中のインベントリページ
	int inventoryPage = 0;
	//選択中のインベントリを描画する関数
	void DrawInventory();
	//インベントリの別ページに移動する関数
	void ChangeInventory();

	int mouseX = 0;
	int mouseY = 0;

	//アイテムをスポーンさせる関数
	void SpawnItem(int ItemId);

	//落ちているアイテムリスト
	std::list<Item*>dropItems;

	//落ちているアイテムの描画
	void DrawPopItem();

	//インベントリで選んだアイテムを一時的に取得する変数
	Item* itemBuf = nullptr;

	//アイテムの使用関数
	void ItemUse(/*int selectNum,Inventory* inventory, */int inventoryPage);

	void ItemThrow(int inventoryPage);

	void DeleteDeadEnemy();

	/*std::string Log[9] = {};
	void addLog(const std::string log);
	void LogDraw();*/
	

};