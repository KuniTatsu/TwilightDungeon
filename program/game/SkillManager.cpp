#include "SkillManager.h"
#include"../library/t2klib.h"
#include"Skill.h"
using namespace std;

SkillManager::SkillManager()
{

	skillMaster.resize(3);
}

SkillManager::~SkillManager()
{
}

Skill* SkillManager::GetSkill(int SkillId)
{
	return nullptr;
}

void SkillManager::LoadCsv()
{
	loadSkill = t2k::loadCsv("Csv/Skill.csv");

	/*int SkillId, int SkillType,std::string SkillName, int DamageAmount, int HealAmount, std::string GhPass, int AllNum
		, int XNum, int YNum, int XSize, int YSize*/

	for (int i = 1; i < loadSkill.size(); ++i) {
		int id = atoi(loadSkill[i][0].c_str());
		int type = atoi(loadSkill[i][1].c_str());
		int damage = atoi(loadSkill[i][2].c_str());

		int heal = atoi(loadSkill[i][4].c_str());
		int allNum = atoi(loadSkill[i][6].c_str());
		int xNum = atoi(loadSkill[i][7].c_str());
		int yNum = atoi(loadSkill[i][8].c_str());
		int xSize = atoi(loadSkill[i][9].c_str());
		int ySize = atoi(loadSkill[i][10].c_str());

		Skill* newSkill = new Skill(id, type, loadSkill[i][3], damage, heal, loadSkill[i][5], allNum, xNum, yNum, xSize, ySize);
		skillMaster[type].emplace_back(newSkill);
	}






}
