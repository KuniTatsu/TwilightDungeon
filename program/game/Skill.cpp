#include "Skill.h"
#include"GameManager.h"

extern GameManager* gManager;

Skill::Skill(int SkillId, int SkillType, int DamageAmount, int HealAmount, std::string GhPass, int AllNum
	, int XNum, int YNum, int XSize, int YSize)
{
	skillId = SkillId;
	skillType = SkillType;

	damageAmount = DamageAmount;
	healAmount = HealAmount;

	skillNum[0] = damageAmount;
	skillNum[1] = healAmount;
	ghPass = GhPass;
	gManager->LoadDivGraphEx(ghPass, AllNum, XNum, YNum, XSize, YSize, animGh);

}

Skill::~Skill()
{
}
