#pragma once
#include<vector>
#include<string>
#include<unordered_map>
class SceneManager;

class Item;
class FadeControl;
class Sound;
class Map;

class GameManager {

public:

	GameManager();
	~GameManager();

	/*SceneManager* sManager = nullptr;
	FadeControl* fControl = nullptr;*/
	//Sound* sound = nullptr;
	Map* map = nullptr;

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
private:
	
	//アイテム情報をexcelから読み取る関数
	void loadItem();

	std::vector<std::vector<std::string>> loadItemCsv;

	//マップの幅
	const int MAPWIDTH = 50;
	//マップの縦幅
	const int MAPHEIGHT = 50;

	
};