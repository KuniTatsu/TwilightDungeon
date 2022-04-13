#include "Skill.h"
#include"GameManager.h"

extern GameManager* gManager;

Skill::Skill(int SkillId, int SkillType, std::string SkillName, float DamageRate, float HealRate, std::string GhPass, int AllNum
	, int XNum, int YNum, int XSize, int YSize, int ActSpeed)
{
	skillId = SkillId;
	skillType = SkillType;
	name = SkillName;

	damageRate = DamageRate;
	healRate = HealRate;

	floatData[0] = damageRate;
	floatData[1] = healRate;
	ghPass = GhPass;
	gManager->LoadDivGraphEx(ghPass, AllNum, XNum, YNum, XSize, YSize, animGh);

	graphicAllNum = AllNum;
	xNum = XNum;
	yNum = YNum;

	xSize = XSize;
	ySize = YSize;

	actSpeed = ActSpeed;
}

Skill::~Skill()
{
}
