#include "GameManager.h"
#include"SceneManager.h"
#include"DxLib.h"
#include "../library/t2klib.h"
#include"../support/Support.h"
#include<string>
#include"map.h"
#include<time.h>

//#include"Item.h"
//#include "FadeControl.h"
//#include "SoundManager.h"



GameManager::GameManager()
{

}

GameManager::~GameManager()
{

}

void GameManager::Update()
{
	SceneManager::Update();
}
void GameManager::Draw()
{
	SceneManager::Render();
}

void GameManager::initGameManager()
{
	SRand(time(0));
	map = new Map(MAPWIDTH,MAPHEIGHT);
	map->DivideStart(MAPWIDTH, MAPHEIGHT, map);

	//sound = new Sound();
	//fControl = new FadeControl();

	deitatime_ = 0;
	SceneManager::ChangeScene(SceneManager::SCENE::DUNGEON);

	/*itemList.resize(3);
	loadItem();

	haveItem.resize(38);

	haveItemInit();*/

}

int GameManager::LoadGraphEx(std::string gh)
{

	auto it = ghmap.find(gh);
	if (it != ghmap.end()) {
		return ghmap[gh];
	}

	else {
		int loadgh = LoadGraph(gh.c_str());
		ghmap.insert(std::make_pair(gh, loadgh));
	}


	return ghmap[gh];
}

void GameManager::haveItemInit()
{
	for (int i = 0; i < haveItem.size(); ++i) {
		//ƒAƒCƒeƒ€i”Ô‚ðZZŒÂ‚Å‰Šú‰»‚·‚é
		haveItem[i].emplace_back(1);
	}
}

void GameManager::setitem(int ItemId, int addNum)
{
	haveItem[ItemId][0] += addNum;
}

int GameManager::GetRandEx(int a, int b)
{
	if (a > b) {
		int hoge = a - b;
		int random = GetRand(hoge) + b;
		return random;
	}
	else {
		int hoge = b - a;
		int random = GetRand(hoge) + a;
		return random;
	}
	return 0;
}



void GameManager::loadItem()
{
	loadItemCsv = t2k::loadCsv("Csv/Item.csv");
	for (int i = 1; i < loadItemCsv.size(); ++i) {

		//id
		int a = std::atoi(loadItemCsv[i][0].c_str());
		//ItemType
		int b = std::atoi(loadItemCsv[i][1].c_str());
		//setDay
		int c = std::atoi(loadItemCsv[i][2].c_str());
		//addStatus
		int d = std::atoi(loadItemCsv[i][3].c_str());
		//addStatusNum
		int e = std::atoi(loadItemCsv[i][4].c_str());
		//setAbility
		int f = std::atoi(loadItemCsv[i][5].c_str());
		//setAbilityType
		int g = std::atoi(loadItemCsv[i][6].c_str());
		//num
		int h = std::atoi(loadItemCsv[i][10].c_str());


		//Item* abi = new Item(a, b, c, d, e, f, g, loadItemCsv[i][7], loadItemCsv[i][8], loadItemCsv[i][9], h);

		//abilitytype‚²‚Æ‚ÉƒŠƒXƒg‚ÉŠi”[
		//itemList[b].emplace_back(abi);
	}
}

void GameManager::ReCreate()
{
	delete map;
	map = nullptr;

	map = new Map(MAPWIDTH, MAPHEIGHT);
	map->DivideStart(MAPWIDTH, MAPHEIGHT, map);
}

