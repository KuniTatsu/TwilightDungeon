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

	//���[�h�����G�l�~�[�̐����������[�v������ID����v����G��I�яo���悤�ɂւ񂱂�����@
	for (int i = 0; i < enemyList.size(); ++i) {
		//����Id���ꏏ�Ȃ�enemy�ɓ����
		if (Id == std::atoi(enemyList[i][0].c_str())) {
			enemy = GetEnemyStatus(i, Floor);
			break;
		}
	}
	
	//�G���m�̍��W���`�F�b�N
	while (1) {
		//���W�Z�b�g
		enemy->pos = gManager->SetStartPos(GameManager::setStartPosType::ENEMY);
		//�܂�enemy����̂�����Ă��Ȃ���΂���������
		if (liveEnemyList.empty())break;
		//�S�Ă̐����ς݂�enemy�Ɣ�ׂ�
		for (auto liveEnemy : liveEnemyList) {
			//���W��肵�Ă��Ȃ����true�ɂ���
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
	//gManager->hoge.emplace_back(enemy);
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


	return std::make_shared<Enemy>(id, type, enemyList[num][2], hp, atack, defence, speed, enemyList[num][7], exp, Floor,1);
}
