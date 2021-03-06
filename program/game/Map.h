///*****Description*****
///ダンジョン内のマップに関するクラス
///区域分割法によるマップランダム生成と、
///穴掘り法のよるマップの一筆書き防止処理の２つによってマップが生成される
///また、全てのセルの情報を持ち、壁か壁でないかなどを返す関数を持つ
///*********************
#pragma once
//#define _HAS_STD_BYTE 0
#include<vector>
#include"../library/t2klib.h"
#include"DxLib.h"
#include<memory>


class Camera;
class Player;

class Map {

public:
	//マップ情報の初期化(すべてを壁にする)
	Map(const int Width, const int Height, std::vector<int>Handles);

	//マップの背景
	std::vector<std::vector<int>> ground;
	//ミニマップ用表示判定配列
	std::vector < std::vector<bool>>visited;

	//ミニマップ用 通路表示判定変更関数
	void ChangeWayVisit(t2k::Vector3 localPos);

	//ミニマップ用 部屋表示判定変更関数
	void ChangeRoomVisit(t2k::Vector3 localPos);
	//ミニマップ用 座標から表示していいチップか判定する関数
	inline bool CheckCanDraw(t2k::Vector3 localPos) {
		return visited[localPos.y][localPos.x];
	}


	//部屋の左上と右下の座標のバッファー
	t2k::Vector3 leftTop;
	t2k::Vector3 rightBottom;

	std::shared_ptr<Player> player = nullptr;

	//チップ一つの大きさ
	const int SIZE = 20;

	//部屋の数

	int sumRoomNum = 0;

	enum mapElements {
		WALL,
		PASSWAY,
		CHARACTOR,
		STAIRS,
		SHOP,
	};

	//マップチップを取得
	inline int GetChip(const int x, const int y) {
		if (IsOutOfRange(x, y))return OUTOFRANGE;
		return ground[y][x];
	}

	void SetShop();

	//範囲内のマップチップを取得
	void GetAllChip(const int roomNum, std::vector<std::vector<int>>& chips);

	//マップチップをSetChipで書き換え
	void SetChip(const int x, const int y, const int SetChip);
	//一定範囲すべてを道で書き換え
	void SetAllChip(const int Left, const int Up, const int Right, const int Down);
	//エリア分割開始
	void DivideStart(const int Width, const int Height, Map* map);


	//描画座標をローカル座標に変換
	t2k::Vector3 WorldToMap(const int WorldX, const int WorldY);
	//ローカル座標を描画座標に変換
	t2k::Vector3 MapToWorld(const int MapX, const int MapY);

	//外部から"部屋の数"を取得するときの関数
	inline int GetRoomNum() {
		return divideRoom.size() - 1;
	}

	//外部から"特定の部屋"を取得するときの関数
	inline std::vector<int> GetRoom(int roomNum) {
		return divideRoom[roomNum];
	}

	//外部から"特定の部屋の大きさ"を取得するときの関数
	t2k::Vector3 GetRoomValue(const int roomNum);

	//部屋座標系の0,0を取得する関数
	t2k::Vector3 GetRoomStartPos(const int roomNum);
	//特定の部屋の左上と右下の座標を取得する関数
	void SetCornerPos(int roomNum, t2k::Vector3& LeftTop, t2k::Vector3& RightBottom);

	//特定のマップ座標が部屋のどこかに存在するか確認する関数 返り値は部屋番号
	int CheckIsThere(const int x, const int y);


	//階段テスト用関数
	void AddStairList(t2k::Vector3 pos);
	//階段テスト用クリア関数
	inline void ResetStairList() {
		stairs.clear();
	}
	std::vector<t2k::Vector3> stairs;


	//マップ全体の描画
	void MapDraw();



	//ミニマップ描画
	void MiniMapDraw();

	//今表示されている全ての部屋の上下左右座標をする描画するDebug関数
	void DrawAllRoomPos(const std::vector<std::vector<int>>RoomList);

	inline void SetDoneBuy() {
		if (!doneBuyItem)doneBuyItem = true;
	}

	//マップ内のショップで買い物をしたかどうか取得する関数
	inline bool GetDoneBuy() {
		return doneBuyItem;
	}

private:


	int color_red = GetColor(255, 0, 0);
	int color_green = GetColor(0, 255, 0);
	int color_blue = GetColor(0, 0, 255);
	int color_yellow = GetColor(200, 200, 0);
	int color_purple = GetColor(200, 0, 200);

	int colors[5] = { color_red,color_green ,color_blue,color_yellow,color_purple };

	//初回分割時のためのフラグ
	bool doneFirstDivide = false;
	//買い物済みショップのフラグ
	bool doneBuyItem = false;

	//ショップローカル座標
	t2k::Vector3 localPos;

	//部屋の最小幅
	const int roomMinWidth = 8;
	//部屋の最小高さ
	const int roomMinHeight = 5;
	//部屋の最大数
	const int roomMaxNum = 8;


	//マップの幅(コンストラクタで使用)
	int width = 0;
	//マップの高さ(コンストラクタで使用)
	int height = 0;
	//範囲外判定用
	const int OUTOFRANGE = -1;

	//マップチップ
	int wall = 0;
	int floor = 0;
	int stair = 0;
	int shop = 0;

	//オートタイル用列挙体
	enum graphicPattern
	{
		ROOMWALL,
		ROOMTOP,
		ROOMRIGHT,
		ROOMBOTTOM,
		ROOMLEFT,
		ROOMLEFTTOP,
		ROOMRIGHTTOP,
		ROOMLEFTBOTTOM,
		ROOMRIGHTBOTTOM,
		MAXNUM
	};

	//オートタイル用チップ
	int autoTileChip[graphicPattern::MAXNUM];

	int ue = 0;
	int migi = 0;
	int shita = 0;
	int hidari = 0;

	int voidGh = 0;

	//

	//ミニマップ描画用マップチップ
	int miniMapChip[3] = {};
	//ミニマッププレイヤーgh
	int miniPlayer = 0;
	////ミニマップエネミーgh
	//int miniEnemy = 0;

	//区間分割(区間)
	std::vector<std::vector<int>> divideArea;

	//分割した区間を一旦保存するvector
	std::vector<std::vector<int>> dumpDivideArea;

	//区間分割(区分線)
	std::vector<std::vector<int>> divideLine;
	//区間分割(部屋) 左,上,右,下,id
	std::vector<std::vector<int>> divideRoom;


	//分割方向
	enum mapDir {
		VERTICAL,
		HORIZONTAL,
	};

	bool IsOutOfRange(const int x, const int y);

	//区間分割法
	//上下左右の座標と部屋番号を取得してvectorに格納
	void SetDivideArea(const int Left, const int Up, const int Right, const int Down, const int Id);

	//分割したエリアの大きい方を格納する関数
	void SetLargeDivideArea(const int Left, const int Up, const int Right, const int Down, const int Id);

	//全エリアを線で分割
	//始点,終点のxy,方向
	void SetDivideLine(const int Start_x, const int Start_y, const int Goal_x, const int Goal_y, const int Dir);

	//部屋の格納
	void SetDivideRoom(const int Left, const int Up, const int Right, const int Down, const int RoomId);


	//区画の分割
	void AreaDivide();
	//部屋の作成
	void CreateRoom();
	//通路の作成
	void CreatePassWay();



	//引数の辺に通路があるかどうか確認する関数
	//arg1:動かさない座標(縦に検索ならx座標,横に検索ならy座標
	//arg2:検索する辺の最小の座標
	//arg3:検索する辺の最大の座標
	//arg4:部屋のどっち方向の壁か 0:上,1:右,2:下,3:左
	bool CheckPassWay(const int roomPos_set, const int roomPos_moveStart, const int roomPos_moveGoal, const int dir);

	//最初と最後の部屋から特定の分割線方向への入り口を作る関数 
	t2k::Vector3 RandomPoint(const int roomId, int& dir);
	//t2k::Vector3 Map::RandomPoint(int roomId, int& dir,vector<int>setDivideLine);

	//入り口から隣接する部屋へと通路を伸ばす関数
	bool CreateSecondWay(const int x, const int y, const int dir, const int roomId);


	//セルから周囲の通路を確認し、描画すべき画像が何番か取得する関数 
	int CheckAroundWay(const int x, const int y);

	//セルから見て周囲に通路があるかどうか確かめる関数
	bool CheckAround(const int x, const int y);
	//この座標のセルが通路だったらtrueを返す関数
	bool CheckThisCell(const int x, const int y);

	//セル(x,y)からみてdir方向に垂直なセル2つとdir方向のセルが壁か通路か確認する関数
	bool CheckChip(const int x, const int y, const int nextDir);

	//色コードからstringを返す関数
	std::string GetColorName(const int code);


};
