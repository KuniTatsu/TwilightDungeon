#include "SkillManager.h"
#include"../library/t2klib.h"
#include"Skill.h"
#include"PassiveMod.h"
using namespace std;

SkillManager::SkillManager()
{

	skillMaster.resize(3);
	ModMaster.resize(3);
	LoadSkillCsv();
	LoadModCsv();
}

SkillManager::~SkillManager()
{
	for (int i = 0; i < skillMaster.size(); ++i) {
		for (int k = 0; k < skillMaster[i].size(); ++k) {
			if (skillMaster[i].empty())continue;
			delete skillMaster[i][k];
		}
	}
	for (int i = 0; i < ModMaster.size(); ++i) {
		for (int k = 0; k < ModMaster[i].size(); ++k) {
			if (ModMaster[i].empty())continue;
			delete ModMaster[i][k];
		}
	}
}

Skill* SkillManager::GetSkill(int SkillType, int SkillId)
{
	return skillMaster[SkillType][SkillId];
}

void SkillManager::LoadSkillCsv()
{
	loadSkill = t2k::loadCsv("Csv/Skill.csv");

	/*int SkillId, int SkillType,std::string SkillName, int DamageAmount, int HealAmount, std::string GhPass, int AllNum
		, int XNum, int YNum, int XSize, int YSize*/

	for (int i = 1; i < loadSkill.size(); ++i) {
		int id = stoi(loadSkill[i][0]);
		int type = stoi(loadSkill[i][1]);

		float damage = stof(loadSkill[i][3]);
		float heal = stof(loadSkill[i][4]);
		int allNum = stoi(loadSkill[i][6]);
		int xNum = stoi(loadSkill[i][7]);
		int yNum = stoi(loadSkill[i][8]);
		int xSize = stoi(loadSkill[i][9]);
		int ySize = stoi(loadSkill[i][10]);
		int actSpeed = stoi(loadSkill[i][11]);

		//id,type,name,damage,heal,gh,allnum,xnum,ynum,xsize,ysize
		Skill* newSkill = new Skill(id, type, loadSkill[i][2], damage, heal, loadSkill[i][5], allNum, xNum, yNum, xSize, ySize, actSpeed, loadSkill[i][12]);
		skillMaster[type].emplace_back(newSkill);
	}
}

void SkillManager::LoadModCsv()
{
	//id(int)	ModType	ModName	ModEffect	AttackRate	AddAttack	DefRate	AddDef	SpeedRate	AddSpeed	HpRate	ExtraDamageRate	AddGold	AddExp	Gh(std::string)

	loadMod = t2k::loadCsv("Csv/PassiveMod.csv");
	for (int i = 1; i < loadMod.size(); ++i) {
		int id = stoi(loadMod[i][0]);
		int type = stoi(loadMod[i][1]);

		float attackRate = stof(loadMod[i][4]);
		float defRate = stof(loadMod[i][5]);
		float speedRate = stof(loadMod[i][6]);
		float addAttack = stof(loadMod[i][7]);
		float addDef = stof(loadMod[i][8]);
		float addSpeed = stof(loadMod[i][9]);

		float extraDamage = stof(loadMod[i][10]);
		float extraExp = stof(loadMod[i][11]);

		int addGold = stoi(loadMod[i][12]);
		int addExp = stoi(loadMod[i][13]);

		//id,type,name,damage,heal,gh,allnum,xnum,ynum,xsize,ysize
		PassiveMod* newMod = new PassiveMod(id, type, loadMod[i][2], attackRate, defRate, speedRate, addAttack, addDef, addSpeed, extraDamage, extraExp, addGold, addExp);

		ModMaster[type].emplace_back(newMod);
	}
	if (ModMaster[0].size() > 5)t2k::debugTrace("\nmod‚Ì“Ç‚İ‚İ‚ªI‚í‚è‚Ü‚µ‚½\n");
}
