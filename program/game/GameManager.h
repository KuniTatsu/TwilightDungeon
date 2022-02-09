#pragma once
#include<vector>
#include<string>
#include<unordered_map>
#include "../library/t2klib.h"
#include<memory>
#include<list>
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

class GameManager {

public:

	GameManager();
	~GameManager();

	/*SceneManager* sManager = nullptr;
	FadeControl* fControl = nullptr;*/
	Sound* sound = nullptr;
	Map* map = nullptr;
	Player* player = nullptr;
	Camera* camera = nullptr;
	ItemManager* iManager = nullptr;
	HaveItem* haveItem = nullptr;
	Inventory* inventory = nullptr;

	//インベントリが入った配列
	std::vector<Inventory*> inventories;
	//アイテムを追加する際の格納可能なインベントリ配列番号
	int inventoryNum = 0;

	//インベントリにアイテムを追加する関数
	void AddItemToInventory(int itemId);

	double graphEx = 1;

	//マップの幅(チップ数)
	const int MAPWIDTH = 60;//default:50
	//マップの縦幅
	const int MAPHEIGHT = 48;//default:36

	//一度読み込んだghを保存するmap
	std::unordered_map<std::string, int> ghmap;

	//itemすべての配列
	//std::list<Item*> itemList;

	std::list<int> haveItemList;
	//itemIdからItem*を取得する関数
	Item* GetItemData(int ItemId);

	//ItemIdが存在するかどうか確認する関数
	bool OutOfRangeInItem(int ItemId);


	//ドロップアイテムリストから特定のアイテムをpopする関数
	bool PopDetectItem(Item* item, std::list<Item*>& list);

	float deitatime_;


	void Update();
	void Draw();

	//GameManagerの初期化
	void initGameManager();

	//画像を読み込んでmapに入れる関数
	//すでにあるghならそれを返す
	int LoadGraphEx(std::string gh);

	//a~bの間の乱数取得
	int GetRandEx(int a, int b);

	//部屋の再生成
	void ReCreate();

	//将来的にMapに橋渡しする関数群はMapのポインタだけ渡してあとは勝手にやれって感じの関数にまとめたい

	//mapのどのチップか取得する 描画座標からマップ座標への変換
	t2k::Vector3 WorldToLocal(t2k::Vector3 Pos);
	//マップ座標から描画座標への変換
	t2k::Vector3 LocalToWorld(int MapX, int MapY);

	//マップのチップの情報を取得する
	int GetMapChip(t2k::Vector3 PInChip);

	//特定の描画座標座標が部屋のどこかにあるか確認する関数
	int CheckIsThereFromWorld(t2k::Vector3 Pos);

	//特定のマップ座標が部屋のどこかにあるか確認する関数
	int CheckIsThere(t2k::Vector3 Pos);

	//カメラの移動
	void CameraMove(Player* p);

	//マップ内のランダムな部屋を取得→部屋の中のランダムな座標を取得→座標を描画座標に変換して返す
	//setType 0:プレイヤー初期座標,1:階段,2:enemy
	t2k::Vector3 SetStartPos(int setType);

	//部屋番号ごとの通路の座標を格納するvector
	std::vector< std::vector<t2k::Vector3>>wayPoint;

	//wayPointVectorを初期化する関数
	void InitWayPointVector(int initroomNum);

	////部屋の外周1マス外側を探索してvectorに格納する関数 //使わない
	//void CheckRoomWayPoint(int roomId);

	//通路作成時にvectorに格納する関数
	void SetRoomWayPoint(t2k::Vector3 pos, int roomId);

	//部屋番号と引数の座標から一番遠い部屋の出口を取得する関数
	t2k::Vector3 GetFarPoint(int roomId, t2k::Vector3 pos);

	//特定の範囲のチップを変更する関数
	void SetChip(int Left, int Up, int Right, int Down);

	//部屋の大きさ取得
	t2k::Vector3 GetRoomValue(int roomNum);

	//特定のEnemyとPlayerが隣り合っているか確認する関数
	bool CheckNearByPlayer(std::shared_ptr<Enemy>enemy);

	t2k::Vector3 GetRoomStartPos(int roomNum);

	//enemylist
	std::list<std::shared_ptr<Enemy>> hoge;

	//全てのenemyとplayerが一定範囲内にいるかどうか確認する関数
	bool CheckNearByPlayerToAllEnemy(int range);


	//debug切り替え
	bool isDebug = true;

	int playerRoomNum = 0;

	void setPlayerRoomNum(int roomNum);

	Player* GetPlayer();

	//短形とのマウスクリック感知
	bool CheckMousePointToRect(int MouseX, int MouseY, int RectLeftTopX, int RectWidth, int RectTopY, int RectHeight);

	//アイテム総数取得
	void SetItemNum(int num);

	int GetItemNum();

private:

	int test = 0;
	//アイテムの総数
	int itemNum=0;
	//std::vector<int> haveItemList;

	void Zoom(double* zoomEx);

};