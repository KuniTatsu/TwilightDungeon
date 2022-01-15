#pragma once
#define _HAS_STD_BYTE 0
#include<vector>

using namespace std;

class Camera;

class Map {

public:
	//マップ情報の初期化(すべてを壁にする)
	Map(int Width, int Height);

	


	//マップチップを取得
	int GetChip(int x, int y);
	//マップチップをSetChipで書き換え
	void SetChip(int x, int y, int SetChip);
	//一定範囲すべてを書き換え
	void SetAllChip(int Left, int Up, int Right, int Down);

	void DivideStart(int Width, int Height, Map* map);

	//マップ全体の描画
	void MapDraw();

private:

	Camera* camera = nullptr;

	//部屋の最小幅
	const int roomMinWidth = 5;
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
	int mapChip[3] = {};


	//マップの背景
	vector<vector<int>> ground;
	//区間分割(区間)
	vector<vector<int>> divideArea;

	//分割した区間を一旦保存するvector
	vector<vector<int>> dumpDivideArea;

	//区間分割(区分線)
	vector<vector<int>> divideLine;
	//区間分割(部屋)
	vector<vector<int>> divideRoom;

	enum mapElements {
		WALL,
		ROUTE,
		CHARACTOR,
	};

	enum mapDir {
		VERTICAL,
		HORIZONTAL,
	};

	bool IsOutOfRange(int x, int y);

	//区間分割法
	//上下左右の座標と部屋番号を取得してvectorに格納
	void SetDivideArea(int Left, int Up, int Right, int Down, int Id);

	//分割したエリアの大きい方を格納する関数
	void setLargeDivideArea(int Left, int Up, int Right, int Down, int Id);

	//全エリアを線で分割
	//始点,終点のxy,方向
	void SetDivideLine(int Start_x, int Start_y, int Goal_x, int Goal_y, int Dir);

	//部屋の格納
	void SetDivideRoom(int Left, int Up, int Right, int Down);


	//区画の分割
	void AreaDivide();
	//部屋の作成
	void CreateRoom();
	//通路の作成
	void CreatePassWay();


	
};
