#include "ResourcesManager.h"
#include"../library/t2klib.h"
#include"GameManager.h"

extern GameManager* gManager;

ResourceManager::ResourceManager()
{
	dungeonMapChipGh.resize(3);
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::LoadResource()
{
	dungeonMapChip = t2k::loadCsv("Csv/Resources.csv");

	for (int i = 0; i < 2; ++i) {
		//一つのダンジョンに使う画像は6枚 6枚ごとにvecorに格納する
		for (int k = i * 6 + 1; k < i * 6 + 7; ++k) {
			std::string hoge = dungeonMapChip[k][1];
			dungeonMapChipGh[i].emplace_back(gManager->LoadGraphEx(hoge));
		}
	}

	/*
	mapChip[0] = gManager->LoadGraphEx("graphics/floor.png");//床
	mapChip[1] = gManager->LoadGraphEx("graphics/Wall.png");//ただの壁
	ue = gManager->LoadGraphEx("graphics/Wall_top.png");//top
	shita = gManager->LoadGraphEx("graphics/Wall_bottom.png");//bottom
	hidari = gManager->LoadGraphEx("graphics/Wall_leftside.png");//left
	migi = gManager->LoadGraphEx("graphics/Wall_rightside.png");//right
	*/

}
