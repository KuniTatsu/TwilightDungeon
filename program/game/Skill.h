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
		, int XNum, int YNum, int XSize, int YSize,int ActSpeed, std::string IconGhPass);
	~Skill();

	//�_���[�W�Ɖ񕜗ʂ̃f�[�^���擾����
	inline const float* GetSkillAmount() {
		return floatData;
	}
	//�X�L���̖��O���擾����
	inline const std::string& GetSkillName() {
		return name;
	}
	//�A�j���[�V�����摜���������擾����
	inline const int& GetGraphicAllNum() {
		return graphicAllNum;
	}
	//�A�j���[�V�����^�C�}�[�b�����擾����
	inline const int& GetActSpeed() {
		return actSpeed;
	}
	//�A�j���[�V�����摜�̃n���h��Vector���擾����
	inline const std::vector<int>&GetGraphicHandle() {
		return animGh;
	}
	//�X�L���A�C�R���摜��Gh���擾����
	inline const int& GetSkillIconGh() {
		return iconGh;
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

	//icon�t�H���_�p�X
	std::string iconPass="";
	//iconGh
	int iconGh = 0;

	//�摜�̏��
	int graphicAllNum = 0;
	int xNum = 0;
	int yNum = 0;

	int xSize = 0;
	int ySize = 0;

	int actSpeed = 0;
};

