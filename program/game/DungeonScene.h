///*****Description*****
///ダンジョン内の全てを行うシーンクラス
///UpdateとDraw関数がSceneManagerで行われる
///UpdateとDraw関数の中ではシークエンスで毎フレーム行われる処理が分岐する
///各シークエンス間はChangeSequence関数によって次フレームから走るシークエンスを移動する
///*********************

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
class Animation;

class DungeonScene :public BaseScene {

public:
	DungeonScene();
	~DungeonScene();


	void Update();

	void Draw();

	int GetDungeonLevel();
	void SetDungeonLevel(const int addLevel);

	void MoveLevel(const int addLevel);

private:
	bool firstIn = true;

	//階層
	int dungeonLevel = 1;

	//1階層に設置するアイテム数
	const int spawnItemNum = 5;

	int EButton = 0;

	/*int alfa = 0;*/
	//次のenemyの行動までのインターバルフレーム
	const int ENEMYACTINTERVAL = 30;
	//更新インターバルフレーム
	int enemyActTimer = 30;

	std::list<std::shared_ptr<Enemy>>atackEnemies;
	std::list<std::shared_ptr<Enemy>>::iterator itr;

	//****************************************************
	//UI関連のクラスのインスタンス
	
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
	MenuWindow* firstMenu = nullptr;
	//****************************************************

	enum useType {
		USABLE,
		EQUIP,
		NOWEQUIP
	};
	useType usetype = USABLE;
	std::shared_ptr<Player> player = nullptr;

	//飛んでいるアイテムリスト
	std::list<Item*> throwItem;

	std::list<std::shared_ptr<Item>>throwedItemList;

	std::shared_ptr<EnemyManager>eManager = nullptr;

	std::list<std::shared_ptr<Animation>>drawAnimationList;
	void initDungeonScene();

	void UpdateAnimation();
	void DrawAnimation();

	void CheckAnimLive();
	const int ATTACKEFFECTSPEED = 10;
	//int ATTACKEFFECTINDEX = 5;

	void ReturnCamp();

	t2k::Vector3 playerPos = {};

	t2k::Sequence<DungeonScene*> mainSequence =
		t2k::Sequence<DungeonScene*>(this, &DungeonScene::SeqDescFade);

	//プレイヤー動作シークエンス
	bool SeqMain(const float deltatime);
	//プレイヤー攻撃シークエンス
	bool SeqPlayerAttack(const float deltatime);
	//エネミー動作シークエンス
	bool SeqEnemyAct(const float deltatime);
	//エネミー攻撃シークエンス
	bool SeqEnemyAttack(const float deltatime);
	//１つ目のメニュー操作
	bool SeqFirstMenu(const float deltatime);
	//Inventory描画シークエンス
	bool SeqInventoryOpen(const float deltatime);
	//Inventoryアイテム使用確認シークエンス
	bool SeqInventoryUse(const float deltatime);
	//物が飛んでいるシークエンス
	bool SeqThrowItemMove(const float deltatime);
	//アニメーションシークエンス
	bool SeqAnimation(const float deltatime);
	//フェードインシークエンス
	bool SeqFadeIn(const float deltatime);
	//フェードアウトシークエンス
	bool SeqFadeOut(const float deltatime);
	//エリア移動時の文字フェードシークエンス
	bool SeqDescFade(const float deltatime);

	//debug
	bool SeqCameraMove(const float deltatime);
	//シークエンスの列挙体
	enum class sequence {
		MAIN,
		PLAYERATTACK,
		ENEMYACT,
		ENEMYATTACK,
		FIRSTMENU,
		INVENTORY_OPEN,
		INVENTORY_USE,
		THROWITEMMOVE,
		ANIMATION,
		FADEIN,
		FADEOUT,
		FADEDESC,
		CAMERA

	};
	sequence nowSeq = sequence::FADEDESC;
	sequence lastSeq = sequence::FADEDESC;

	enum class DescType {
		DUNGEONIN,
		MOVELEVEL

	};

	//毎秒デルタタイムを足す
	float descFadeCount = 0;
	//文字を描画する時間
	const float DESCFADETIME = 3;
	//今のダンジョン文字列
	std::string nowDungeonName = "";

	//drawstring用の中心座標補正
	const float OFFSET = 30.0f;

	//fadeDescシークエンスで文字を描画する関数
	void DrawFadeDesc();


	//Sequenceを移動させる関数,enumも一緒に変更する
	void ChangeSequence(const sequence seq);
	//シーケンス確認関数
	void DrawNowSequence(const sequence seq);

	//num体敵を作る関数
	void RandEnemyCreate(const int num);

	//enemyのデータの確認関数
	void DrawEnemyData();
	//描画中のインベントリページ
	int drawInventoryPage = 0;
	//選択中のインベントリを描画する関数
	void DrawInventory();
	//インベントリの別ページに移動する関数
	void ChangeInventory();

	int mouseX = 0;
	int mouseY = 0;

	//アイテムをスポーンさせる関数
	void SpawnItem(const int ItemId);

	//落ちているアイテムリスト
	std::list<Item*>dropItems;

	//落ちているアイテムの描画
	void DrawPopItem();

	//インベントリで選んだアイテムを一時的に取得する変数
	Item* itemBuf = nullptr;

	//アイテムの使用関数
	void ItemUse(const int inventoryPage);

	void ItemThrow(const int inventoryPage);

	void DeleteDeadEnemy();

};