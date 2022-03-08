///*****Description*****
///ゲーム機能全般に関するクラス
///ゲーム全体に対して関連する処理及びインスタンスを保有するクラス
/// シーンマネージャのUpdate,DrawをこのクラスのUpdate,Drawで行い、gameMainで呼び出すことでゲームを動かす
///*********************
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
class Inventory;
class Actor;
class ResourceManager;

class GameManager {

public:

	GameManager();
	~GameManager();

	FadeControl* fControl = nullptr;
	Sound* sound = nullptr;
	Map* map = nullptr;

	std::shared_ptr<Player>player = nullptr;

	Camera* camera = nullptr;
	ItemManager* iManager = nullptr;
	Inventory* inventory = nullptr;
	ResourceManager* resource = nullptr;

	//std::shared_ptr<Inventory>shared_inventory;

	//debug
	bool minimapDraw = true;
	//
	//文字色
	int red = 0;
	int blue = 0;

	//マップチップの一枚の大きさ
	const int GRAPHICSIZE = 20;
	float nowGraphicSize = GRAPHICSIZE;

	double graphEx = 1;
	//画面中心
	const t2k::Vector3 WINDOWCENTER = t2k::Vector3(512, 384, 0);

	//マップの幅(チップ数)
	const int MAPWIDTH = 60;//default:50
	//マップの縦幅
	const int MAPHEIGHT = 48;//default:36


	//LoadDivGraphのMaxIndexデータ attack,player,enemy
	std::vector<std::vector<std::string>>maxIndex;
	//各LoadDivGraphのMaxIndex読み込み
	void LoadMaxIndex();

	enum class index : uint32_t {
		ATTACK,
		PLAYER,
		ENEMY
	};

	inline const int& GetMaxIndex(index IndexName) {
		//std::string hoge = maxIndex[static_cast<uint32_t>(IndexName)][1].c_str();
		return std::atoi(maxIndex[static_cast<uint32_t>(IndexName)][1].c_str());
	}

	//インベントリが入った配列
	std::vector<Inventory*> inventories;

	//std::vector<std::shared_ptr<Inventory>>sharedInventories;

	//アイテムを追加する際の格納可能なインベントリ配列番号
	int inventoryNum = 0;

	//インベントリにアイテムを追加する関数
	void AddItemToInventory(const int itemId);

	//インベントリからアイテムを削除する関数
	void PopItemFromInventory(const int NowInventoryId);

	//現在装備中のアイテムとインベントリ内のアイテムのステータスの差を取得する関数
	int GetDifStatus(int subId,int equipType,int amount);

	//装備しているか確認する関数
	bool CheckEquipItem(int subId);

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

	enum class SpawnScene {
		Camp,
		Dungeon
	};

	//プレイヤーの生成
	void MakePlayer(SpawnScene nowScene);

	

	enum class Dungeon : uint32_t {
		TOWER,
		FOREST,
		WATERWAY,
		DUNGEONMAX
	};
	Dungeon nowDungeon;

	const std::string DUNGEONNAME[3] = { "塔のダンジョン","森のダンジョン","灼熱のダンジョン" };

	inline std::string GetDungeonName(Dungeon dungeonEnum) {
		//if (dungeonEnum > Dungeon::DUNGEONMAX)return "範囲外";
		return DUNGEONNAME[static_cast<uint32_t>(dungeonEnum)];
	}
	//ダンジョンマップ内で使う画像ハンドルの取得
	std::vector<int>& GetGraphicHandles(Dungeon dungeonName);
	//ダンジョンの生成
	void CreateDungeon(Dungeon dungeonName);

	//ダンジョンマップの生成
	void CreateMap(Dungeon dungeonName);

	//ダンジョンマップごとのBGM再生
	void RunDungeonBgm();

	//BGM停止
	void StopBgm();

	//効果音
	void RunSystemSound();

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

	//カメラをキャラを中心にリセット
	void CameraReset();

	//カメラの移動
	void CameraMove(int width, int height, int dir);

	//特殊なウィンドウの表示関数
	//arg1 windowX 最大に開いた時の横の大きさ
	//arg2 windowY 最大に開いた時の縦の大きさ
	//arg3 xSpeed  横に開く際のタイマースピード
	//arg4 ySpeed　縦に開く際のタイマースピード
	//arg5 centerX 画像描画中心X座標
	//arg6 centerY 画像描画中心Y座標
	//arg7 gh	   表示する画像のグラフィックハンドル
	void DrawOpenWindow(float windowX,float windowY,int xCount,float yCount,int gh);
	//ウィンドウが開く際のタイマー
	float openTimer = 0;

	enum class setStartPosType {
		PLAYER,
		STAIR,
		ENEMY,
		ITEM
	};

	//マップ内のランダムな部屋を取得→部屋の中のランダムな座標を取得→座標を描画座標に変換して返す
	//setType 0:プレイヤー初期座標,1:階段,2:enemy
	t2k::Vector3 SetStartPos(setStartPosType num);

	const t2k::Vector3 spawnPos = { 720,360,0 };

	//campの特定の座標にプレイヤーを設置
	inline const t2k::Vector3& SpawnPlayerCamp() {
		return spawnPos;
	}


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
	bool isDebug = false;

	int playerRoomNum = 0;

	void setPlayerRoomNum(const int roomNum);
	//プレイヤーの取得
	std::shared_ptr<Player> GetPlayer();
	t2k::Vector3 GetPlayerLocalPos();

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

	const t2k::Vector3 Vectores[4]{
		t2k::Vector3(0,-1,0),//上
		t2k::Vector3(1,0,0),//右
		t2k::Vector3(0,1,0),//下
		t2k::Vector3(-1,0,0),//左
	};

	enum Vec {
		UP,
		RIGHT,
		DOWN,
		LEFT
	};

	inline const t2k::Vector3& GetVecter(int vec) {
		return Vectores[vec];
	}
	inline const t2k::Vector3 GetMultipleVector(int vec, int multiple) {
		const float x = Vectores[vec].x * multiple;
		const float y = Vectores[vec].y * multiple;
		return t2k::Vector3(x, y, 0);
	}
	inline Camera* GetCamera() {
		return camera;
	}

private:
	std::string Log[7] = {};

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