#pragma once
#include<vector>
#include<string>
#include<unordered_map>
#include "../library/t2klib.h"
#include<memory>
#include<list>
#include<string>
class SceneManager;

class Item;
class FadeControl;
class Sound;
class Map;
class Player;
class Camera;
class Enemy;
class ItemManager;
class HaveItem;
class Inventory;
class Actor;

class GameManager {

public:

	GameManager();
	~GameManager();

	//FadeControl* fControl = nullptr;
	Sound* sound = nullptr;
	Map* map = nullptr;

	std::shared_ptr<Player>player = nullptr;

	Camera* camera = nullptr;
	ItemManager* iManager = nullptr;
	HaveItem* haveItem = nullptr;
	Inventory* inventory = nullptr;

	//std::shared_ptr<Inventory>shared_inventory;

	//debug
	bool minimapDraw = true;
	//


	//インベントリが入った配列
	std::vector<Inventory*> inventories;

	//std::vector<std::shared_ptr<Inventory>>sharedInventories;

	//アイテムを追加する際の格納可能なインベントリ配列番号
	int inventoryNum = 0;

	//インベントリにアイテムを追加する関数
	void AddItemToInventory(const int itemId);

	//インベントリからアイテムを削除する関数
	void PopItemFromInventory(const int NowInventoryId);

	const int graphicSize = 20;
	float nowGraphicSize = graphicSize;

	double graphEx = 1;

	//マップの幅(チップ数)
	const int MAPWIDTH = 60;//default:50
	//マップの縦幅
	const int MAPHEIGHT = 48;//default:36

	//一度読み込んだghを保存するmap
	std::unordered_map<std::string, int> ghmap;

	std::list<int> haveItemList;
	//itemIdからItem*を取得する関数
	Item* GetItemData(const int ItemId);

	//ItemIdが存在するかどうか確認する関数
	bool OutOfRangeInItem(const int ItemId);

	//ドロップアイテムリストから特定のアイテムをpopする関数
	bool PopDetectItem(Item* item, std::list<Item*>& list);

	float deitatime_;

	void Update();
	void Draw();
	//マップの描画
	void MapDraw();

	//GameManagerの初期化
	void initGameManager();

	//画像を読み込んでmapに入れる関数
	//すでにあるghならそれを返す
	int LoadGraphEx(const std::string gh);

	//a~bの間の乱数取得
	int GetRandEx(const int a, const int b);

	//部屋の再生成
	void ReCreate();

	//将来的にMapに橋渡しする関数群はMapのポインタだけ渡してあとは勝手にやれって感じの関数にまとめたい

	//mapのどのチップか取得する 描画座標からマップ座標への変換
	t2k::Vector3 WorldToLocal(const t2k::Vector3 Pos);
	//マップ座標から描画座標への変換
	t2k::Vector3 LocalToWorld(const int MapX, const int MapY);

	//マップのチップの情報を取得する
	int GetMapChip(const t2k::Vector3 PInChip);

	//特定の描画座標座標が部屋のどこかにあるか確認する関数
	int CheckIsThereFromWorld(const t2k::Vector3 Pos);

	//特定のマップ座標が部屋のどこかにあるか確認する関数
	int CheckIsThere(const t2k::Vector3 Pos);

	//カメラの移動
	void CameraMove();

	enum class setStatrPosType {
		PLAYER,
		STAIR,
		ENEMY,
		ITEM
	};

	//マップ内のランダムな部屋を取得→部屋の中のランダムな座標を取得→座標を描画座標に変換して返す
	//setType 0:プレイヤー初期座標,1:階段,2:enemy
	t2k::Vector3 SetStartPos(setStatrPosType num);


	//部屋番号ごとの通路の座標を格納するvector
	std::vector< std::vector<t2k::Vector3>>wayPoint;

	//wayPointVectorを初期化する関数
	void InitWayPointVector(const int initroomNum);

	////部屋の外周1マス外側を探索してvectorに格納する関数 //使わない
	//void CheckRoomWayPoint(int roomId);

	//通路作成時にvectorに格納する関数
	void SetRoomWayPoint(const t2k::Vector3 pos, const int roomId);

	//部屋番号と引数の座標から一番遠い部屋の出口を取得する関数
	t2k::Vector3 GetFarPoint(const int roomId, const t2k::Vector3 pos);

	//特定の範囲のチップを変更する関数
	void SetChip(const int Left, const int Up, const int Right, const int Down);

	//部屋の大きさ取得
	t2k::Vector3 GetRoomValue(const int roomNum);

	//特定のEnemyとPlayerが隣り合っているか確認する関数
	bool CheckNearByPlayer(std::shared_ptr<Enemy>enemy);
	//隣り合ったPlayerとの相対位置を取得する関数 0:上,1:右,2:下,3:左
	int GetPlayerVec(std::shared_ptr<Enemy>enemy);

	t2k::Vector3 GetRoomStartPos(const int roomNum);

	//enemylist
	std::list<std::shared_ptr<Enemy>> liveEnemyList;

	//生存しているenemyを同期する関数
	void SetLiveEnemyList(std::list<std::shared_ptr<Enemy>> list);

	//全てのenemyとplayerが一定範囲内にいるかどうか確認する関数
	bool CheckNearByPlayerToAllEnemy(const int range);
	//特定の座標にenemyがいるかどうか確認する関数
	bool CheckIsThereEnemyToDir(const t2k::Vector3 Pos);
	//特定の座標のenemyを取得する関数
	std::shared_ptr<Enemy> GetIsThereEnemyToDir(const t2k::Vector3 Pos);

	inline void ForceInventoryChange(int& num) {
		num--;
		isDeleteInventory = false;
	}
	bool isDeleteInventory = false;


	//目の前の対象にダメージを与える
	void TakeDamageToTarget(Actor* hoge, const t2k::Vector3 Pos);


	//debug切り替え
	bool isDebug = true;

	int playerRoomNum = 0;

	void setPlayerRoomNum(const int roomNum);

	std::shared_ptr<Player> GetPlayer();

	//短形とのマウスクリック感知
	bool CheckMousePointToRect(const int MouseX, const int MouseY, const int RectLeftTopX, const  int RectWidth, const int RectTopY, const int RectHeight);

	//アイテム総数を入力
	void SetItemNum(const int num);
	//ロードしたアイテムの総数を取得する関数
	int GetItemNum();

	//ダメージ計算
	float CalcDamage(const int Attack, const int Defence);

	//ダメージ処理
	void RunDamageEvent(const float Damage, std::shared_ptr<Actor>actor);

	//ログ関連

	void addLog(const std::string log);
	void LogDraw(const int x, const int y);
	void DrawStringLows(const int lowNum);

private:
	std::string Log[7] = {};

	const t2k::Vector3 WINDOWCENTER = t2k::Vector3(512, 384, 0);

	int test = 0;
	//アイテムの総数
	int itemNum = 0;
	//std::vector<int> haveItemList;

	void Zoom();

	//test
	std::string strings[5][3]{
		{"こんにちは","私の名前は","hogehogeです"},
		{"","",""},
		{"","",""},
		{"","",""},
		{"","",""}
	};

};