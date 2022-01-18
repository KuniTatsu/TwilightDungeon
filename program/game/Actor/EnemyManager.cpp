#include "EnemyManager.h"
#include"Enemy.h"
#include<memory>
#include"../GameManager.h"

extern GameManager* gManager;

EnemyManager::EnemyManager()
{
	enemyList.resize(6);
	LoadEnemyData();
}

void EnemyManager::CreateEnemy(int Id)
{
	std::shared_ptr<Enemy> enemy;

	switch (Id)
	{
	case 100:
		enemy = GetEnemyStatus(0);
		//(*enemy)->skill = {};
		break;
	case 101:
		enemy = GetEnemyStatus(1);
		break;
	case 102:
		enemy = GetEnemyStatus(2);
		break;
	case 103:
		enemy = GetEnemyStatus(3);
		break;
	case 104:
		enemy = GetEnemyStatus(4);
		break;
	case 105:
		enemy = GetEnemyStatus(5);
		break;
	default:
		break;
	}
	//敵同士の座標被りチェック
	while (1) {
		//座標セット
		enemy->pos = gManager->SetStartPos(2);
		//まだenemyが一体も作られていなければすぐ抜ける
		if (liveEnemyList.empty())break;
		for (auto liveEnemy : liveEnemyList) {

			if (liveEnemy->pos.x != enemy->pos.x && liveEnemy->pos.y != enemy->pos.y)
			{
				canCreateEnemy = true;
			}
			else {
				canCreateEnemy = false;
			}
		}
		if (canCreateEnemy)break;
	}
	liveEnemyList.emplace_back(enemy);
}

void EnemyManager::LoadEnemyData()
{
	loadEnemy = t2k::loadCsv("Csv/Enemy.csv");

	//id(int)	EnemyType(int)	Enemy_Name(std::string)	HP(int)	Atack(int)	Defence(int)	Speed(int)	Gh(std::string)	Exp(int) 
	for (int i = 1; i < loadEnemy.size(); ++i) {

		enemyList[i - 1].emplace_back(loadEnemy[i][0]);
		enemyList[i - 1].emplace_back(loadEnemy[i][1]);
		enemyList[i - 1].emplace_back(loadEnemy[i][2]);
		enemyList[i - 1].emplace_back(loadEnemy[i][3]);
		enemyList[i - 1].emplace_back(loadEnemy[i][4]);
		enemyList[i - 1].emplace_back(loadEnemy[i][5]);
		enemyList[i - 1].emplace_back(loadEnemy[i][6]);
		enemyList[i - 1].emplace_back(loadEnemy[i][7]);
		enemyList[i - 1].emplace_back(loadEnemy[i][8]);
	}
}
std::shared_ptr<Enemy> EnemyManager::GetEnemyStatus(int num)
{
	int id = std::atoi(loadEnemy[num][0].c_str());
	int type = std::atoi(loadEnemy[num][1].c_str());

	int hp = std::atoi(loadEnemy[num][3].c_str());
	int atack = std::atoi(loadEnemy[num][4].c_str());
	int defence = std::atoi(loadEnemy[num][5].c_str());
	int speed = std::atoi(loadEnemy[num][6].c_str());

	int exp = std::atoi(loadEnemy[num][8].c_str());


	return std::make_shared<Enemy>(id, type, loadEnemy[num][2], hp, atack, defence, speed, loadEnemy[num][7], exp);
}
