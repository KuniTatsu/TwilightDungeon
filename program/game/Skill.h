///*****Description*****
///スキルに関するクラス
///プレイヤーとエネミー双方が持つことが出来るスキルの情報,処理を保持する
///*********************
#pragma once
#include<string>
#include<vector>
class Skill
{
public:
	Skill(int SkillId, int SkillType, int DamageAmount, int HealAmount, std::string GhPass, int AllNum
		, int XNum, int YNum, int XSize, int YSize);
	~Skill();

	inline int* GetSkillAmount() {
		return skillNum;
	}

private:

	int skillId = 0;
	int skillType = 0;

	int damageAmount = 0;
	int healAmount = 0;

	int skillNum[2];

	std::string ghPass = "";
	int gh = 0;
	std::vector<int>animGh;
};

