#pragma once
#include"../../library/t2klib.h"
#include<vector>
#include<string>
#include<list>

class Enemy;

class EnemyManager {

public:
	EnemyManager();

	std::vector < std::vector<std::string>> loadEnemy;

	//id‚²‚Æ‚É•ª‚¯‚ç‚ê‚½enemyƒf[ƒ^
	std::vector<std::vector<std::string>> enemyList;

	std::list<std::shared_ptr<Enemy>> liveEnemyList;


	void CreateEnemy(const int Id, const int Floor);

private:
	void LoadEnemyData();

	bool canCreateEnemy = true;

	std::shared_ptr<Enemy> GetEnemyStatus(const int num, const int Floor);

};