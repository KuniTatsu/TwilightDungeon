#pragma once
#include<vector>
#include<string>

class Skill;
class PassiveMod;

class SkillManager
{
public:
	SkillManager();
	~SkillManager();

	Skill* GetSkill(int SkillType, int SkillId);

private:
	//--------スキルのロード------------------------------//
	void LoadSkillCsv();
	std::vector<std::vector<std::string>>loadSkill;

	std::vector<std::vector<Skill*>> skillMaster;

	//----------------------------------------------------//

	//--------パッシブ効果のロード------------------------------//
	void LoadModCsv();
	std::vector<std::vector<std::string>>loadMod;

	std::vector<std::vector<PassiveMod*>>ModMaster;

	//PassiveMod.csv

};

