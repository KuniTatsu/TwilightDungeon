///*****Description*****
///�X�L���Ɋւ���N���X
///�v���C���[�ƃG�l�~�[�o���������Ƃ��o����X�L���̏��,������ێ�����
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

