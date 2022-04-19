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
	//--------�X�L���̃��[�h------------------------------//
	void LoadSkillCsv();
	std::vector<std::vector<std::string>>loadSkill;

	std::vector<std::vector<Skill*>> skillMaster;

	//----------------------------------------------------//

	//--------�p�b�V�u���ʂ̃��[�h------------------------------//
	void LoadModCsv();
	std::vector<std::vector<std::string>>loadMod;

	std::vector<std::vector<PassiveMod*>>ModMaster;

	//PassiveMod.csv

};

