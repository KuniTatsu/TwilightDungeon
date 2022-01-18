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

	//id���Ƃɕ�����ꂽenemy�f�[�^
	std::vector<std::vector<std::string>> enemyList;

	std::list<std::shared_ptr<Enemy>> liveEnemyList;


	void CreateEnemy(int Id);

private:
	void LoadEnemyData();

	bool canCreateEnemy = true;

	std::shared_ptr<Enemy> GetEnemyStatus(int num);

};