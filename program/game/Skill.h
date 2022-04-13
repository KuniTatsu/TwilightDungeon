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
	Skill(int SkillId, int SkillType,std::string SkillName, float DamageRate, float HealRate, std::string GhPass, int AllNum
		, int XNum, int YNum, int XSize, int YSize,int ActSpeed);
	~Skill();

	inline const float* GetSkillAmount() {
		return floatData;
	}
	inline const std::string& GetSkillName() {
		return name;
	}

	inline const int& GetGraphicAllNum() {
		return graphicAllNum;
	}

	inline const int& GetActSpeed() {
		return actSpeed;
	}

	inline const std::vector<int>&GetGraphicHandle() {
		return animGh;
	}

private:
	//�X�L��Id
	int skillId = 0;
	//�X�L���^�C�v
	int skillType = 0;
	//�X�L���̖��O
	std::string name = "";

	//�U���͂ɂ�����{��
	float damageRate = 0;
	//�_���[�W�ɂ�����{��
	float healRate = 0;
	//��Q��Z�߂��z��
	float floatData[2];

	//�t�H���_�p�X
	std::string ghPass = "";
	//DivGraph�ŕ��������摜�n���h��
	std::vector<int>animGh;

	//�摜�̏��
	int graphicAllNum = 0;
	int xNum = 0;
	int yNum = 0;

	int xSize = 0;
	int ySize = 0;

	int actSpeed = 0;
};

