#include "PassiveMod.h"

PassiveMod::PassiveMod(int ModId, int ModType, std::string ModName, float AttackRate, float DefRate, float SpeedRate, float AddAttack, float AddDef, float AddSpeed,
	float ExtraDamage, float ExtraHp, int AddGold, int AddExp)
{
	modId = ModId;
	modType = ModType;
	name = ModName;

	attackRate = AttackRate;
	addAttack = AddAttack;

	defRate = DefRate;
	addDef = AddDef;

	speedRate = SpeedRate;
	addSpeed = AddSpeed;

	extraDamageRate = ExtraDamage;
	extraHpRate = ExtraHp;

	addGold = AddGold;
	addExp = AddExp;

}

PassiveMod::~PassiveMod()
{
}
