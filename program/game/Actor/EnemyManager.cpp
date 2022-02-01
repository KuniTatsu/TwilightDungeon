#include "EnemyManager.h"
#include"Enemy.h"
#include<memory>
#include"../GameManager.h"
#include"../Player.h"

extern GameManager* gManager;

EnemyManager::EnemyManager()
{
	enemyList.resize(6);
	LoadEnemyData();
}

void EnemyManager::CreateEnemy(int Id, int Floor)
{
	std::shared_ptr<Enemy> enemy;

	switch (Id)
	{
	case 100:
		enemy = GetEnemyStatus(0, Floor);
		//(*enemy)->skill = {};
		break;
	case 101:
		enemy = GetEnemyStatus(1, Floor);
		break;
	case 102:
		enemy = GetEnemyStatus(2, Floor);
		break;
	case 103:
		enemy = GetEnemyStatus(3, Floor);
		break;
	case 104:
		enemy = GetEnemyStatus(4, Floor);
		break;
	case 105:
		enemy = GetEnemyStatus(5, Floor);
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
			//座標被りしていなければtrueにする
			if (liveEnemy->pos.x != enemy->pos.x && liveEnemy->pos.y != enemy->pos.y&&
				gManager->player->pos.x != enemy->pos.x&& gManager->player->pos.y != enemy->pos.y)
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
	gManager->hoge.emplace_back(enemy);
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
std::shared_ptr<Enemy> EnemyManager::GetEnemyStatus(int num,int Floor)
{
	int id = std::atoi(enemyList[num][0].c_str());
	int type = std::atoi(enemyList[num][1].c_str());

	int hp = std::atoi(enemyList[num][3].c_str());
	int atack = std::atoi(enemyList[num][4].c_str());
	int defence = std::atoi(enemyList[num][5].c_str());
	int speed = std::atoi(enemyList[num][6].c_str());

	int exp = std::atoi(enemyList[num][8].c_str());


	return std::make_shared<Enemy>(id, type, enemyList[num][2], hp, atack, defence, speed, enemyList[num][7], exp, Floor);
}
