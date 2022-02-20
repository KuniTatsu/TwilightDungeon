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
		//��̃_���W�����Ɏg���摜��6�� 6�����Ƃ�vecor�Ɋi�[����
		for (int k = i * 6 + 1; k < i * 6 + 7; ++k) {
			std::string hoge = dungeonMapChip[k][1];
			dungeonMapChipGh[i].emplace_back(gManager->LoadGraphEx(hoge));
		}
	}

	/*
	mapChip[0] = gManager->LoadGraphEx("graphics/floor.png");//��
	mapChip[1] = gManager->LoadGraphEx("graphics/Wall.png");//�����̕�
	ue = gManager->LoadGraphEx("graphics/Wall_top.png");//top
	shita = gManager->LoadGraphEx("graphics/Wall_bottom.png");//bottom
	hidari = gManager->LoadGraphEx("graphics/Wall_leftside.png");//left
	migi = gManager->LoadGraphEx("graphics/Wall_rightside.png");//right
	*/

}
