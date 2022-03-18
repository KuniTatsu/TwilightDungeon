#pragma once
#include<vector>
#include<string>

class Skill;

class SkillManager
{
public:
	SkillManager();
	~SkillManager();

private:

	void LoadCsv();
	std::vector<std::vector<std::string>>loadSkill;

	std::vector<std::vector<Skill*>> skillMaster;

};

