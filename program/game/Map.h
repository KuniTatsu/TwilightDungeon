#pragma once
#define _HAS_STD_BYTE 0
#include<vector>
#include"../library/t2klib.h"
#include"DxLib.h"

using namespace std;

class Camera;

class Map {

public:
	//マップ情報の初期化(すべてを壁にする)
	Map(int Width, int Height);

	//マップの背景
	vector<vector<int>> ground;

	//部屋の数

	int sumRoomNum = 0;

	enum mapElements {
		WALL,
		PASSWAY,
		CHARACTOR,
		STAIRS,
		EXPASSWAY,
	};

	//マップチップを取得
	int GetChip(int x, int y);

	//範囲内のマップチップを取得
	void GetAllChip(int roomNum, std::vector<std::vector<int>>& chips);

	//マップチップをSetChipで書き換え
	void SetChip(int x, int y, int SetChip);
	//一定範囲すべてを道で書き換え
	void SetAllChip(int Left, int Up, int Right, int Down);

	void DivideStart(int Width, int Height, Map* map);

	t2k::Vector3 WorldToMap(int WorldX, int WorldY);
	t2k::Vector3 MapToWorld(int MapX, int MapY);

	//外部から"部屋の数"を取得するときの関数
	int GetRoomNum();

	//外部から"特定の部屋"を取得するときの関数
	vector<int> GetRoom(int roomNum);

	//外部から"特定の部屋の大きさ"を取得するときの関数
	t2k::Vector3 GetRoomValue(int roomNum);

	//部屋座標系の0,0を取得する関数
	t2k::Vector3 GetRoomStartPos(int roomNum);


	//特定のマップ座標が部屋のどこかに存在するか確認する関数
	int CheckIsThere(int x, int y);

	//マップ全体の描画
	void MapDraw();

	//ミニマップ描画
	void MiniMapDraw();

	//今表示されている全ての部屋の上下左右座標をする描画するDebug関数
	void DrawAllRoomPos(vector<vector<int>>RoomList);

private:

	int color_red = GetColor(255, 0, 0);
	int color_green = GetColor(0, 255, 0);
	int color_blue = GetColor(0, 0, 255);
	int color_yellow = GetColor(200, 200, 0);
	int color_purple = GetColor(200, 0, 200);

	int colors[5] = { color_red,color_green ,color_blue,color_yellow,color_purple };


	bool doneFirstDivide = false;

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
	int outOfRange = -1;

	//マップチップ
	int mapChip[4] = {};



	//区間分割(区間)
	vector<vector<int>> divideArea;

	//分割した区間を一旦保存するvector
	vector<vector<int>> dumpDivideArea;

	//区間分割(区分線)
	vector<vector<int>> divideLine;
	//区間分割(部屋) 左,上,右,下,id
	vector<vector<int>> divideRoom;



	enum mapDir {
		VERTICAL,
		HORIZONTAL,
	};

	bool IsOutOfRange(int x, int y);

	//区間分割法
	//上下左右の座標と部屋番号を取得してvectorに格納
	void SetDivideArea(int Left, int Up, int Right, int Down, int Id);

	//分割したエリアの大きい方を格納する関数
	void SetLargeDivideArea(int Left, int Up, int Right, int Down, int Id);

	//全エリアを線で分割
	//始点,終点のxy,方向
	void SetDivideLine(int Start_x, int Start_y, int Goal_x, int Goal_y, int Dir);

	//部屋の格納
	void SetDivideRoom(int Left, int Up, int Right, int Down,int RoomId);


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
	bool CheckPassWay(int roomPos_set, int roomPos_moveStart, int roomPos_moveGoal,int dir);

	//最初と最後の部屋から特定の分割線方向への入り口を作る関数 
	t2k::Vector3 Map::RandomPoint(int roomId, int& dir);
	//t2k::Vector3 Map::RandomPoint(int roomId, int& dir,vector<int>setDivideLine);

	//入り口から隣接する部屋へと通路を伸ばす関数
	bool Map::CreateSecondWay(int x, int y, int dir, int roomId);

	//セル(x,y)からみてdir方向に垂直なセル2つとdir方向のセルが壁か通路か確認する関数
	bool Map::CheckChip(int x, int y, int nextDir);

	//色コードからstringを返す関数
	std::string GetColorName(int code);
	
};
