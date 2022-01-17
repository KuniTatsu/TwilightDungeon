#pragma once
#include<vector>
#include<string>
#include<unordered_map>
#include "../library/t2klib.h"
class SceneManager;

class Item;
class FadeControl;
class Sound;
class Map;
class Player;
class Camera;

class GameManager {

public:

	GameManager();
	~GameManager();

	/*SceneManager* sManager = nullptr;
	FadeControl* fControl = nullptr;*/
	//Sound* sound = nullptr;
	Map* map = nullptr;
	Player* player = nullptr;
	Camera* camera = nullptr;

	double graphEx = 1;
	
	//一度読み込んだghを保存するmap
	std::unordered_map<std::string, int> ghmap;

	//[itemid][haveItemNum]アイテムを持っている数とidの二次元配列
	std::vector<std::vector<int>> haveItem;
	//itemすべての配列
	std::vector<std::vector<Item*>> itemList;

	float deitatime_;

	
	void Update();
	void Draw();

	//GameManagerの初期化
	void initGameManager();

	//画像を読み込んでmapに入れる関数
	//すでにあるghならそれを返す
	int LoadGraphEx(std::string gh);

	//アイテムの初期所持数を決める関数
	void haveItemInit();

	void setitem(int ItemId, int addNum);

	//a~bの間の乱数取得
	int GetRandEx(int a, int b);

	void ReCreate();
	//mapのどのチップか取得する 描画座標からマップ座標への変換
	t2k::Vector3 WorldToLocal(t2k::Vector3 Pos);
	//マップ座標から描画座標への変換
	t2k::Vector3 LocalToWorld(int MapX,int MapY);

	//マップのチップの情報を取得する
	int GetMapChip(t2k::Vector3 PInChip);

	//カメラの移動
	void CameraMove(Player* p);

private:
	
	//アイテム情報をexcelから読み取る関数
	void loadItem();

	std::vector<std::vector<std::string>> loadItemCsv;

	//マップの幅(チップ数)
	const int MAPWIDTH = 50;
	//マップの縦幅
	const int MAPHEIGHT = 38;
	//マップ内のランダムな部屋を取得→部屋の中のランダムな座標を取得→座標を描画座標に変換して返す
	t2k::Vector3 SetStartPos();


	void Zoom(double* zoomEx);
	
};