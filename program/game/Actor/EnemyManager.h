///*****Description*****
///エネミー生成に関するクラス
///全てのエネミーのマスターデータを持つ
///エネミーの生成もこのクラスの関数で行う
///*********************
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

	//idごとに分けられたenemyデータ
	std::vector<std::vector<std::string>> enemyList;

	std::list<std::shared_ptr<Enemy>> liveEnemyList;


	void CreateEnemy(const int Id, const int Floor);

private:
	void LoadEnemyData();

	bool canCreateEnemy = true;

	std::shared_ptr<Enemy> GetEnemyStatus(const int num, const int Floor);

};