//������v���C���[,�G�ɕt�^�����p�b�V�u���ʂ̃N���X
//�����͍ő�5��,�v���C���[�ƓG�͍ő吔�Ȃ�,���ʏ��ł܂ł̃^�[������
//�{���l:�U���͂ɏ�Z�����,���Z�l:�U���͂ɉ��Z�����

#pragma once
#include<string>

class PassiveMod
{
public:
	PassiveMod(int ModId, int ModType, std::string ModName,float AttackRate,float DefRate,float SpeedRate,float AddAttack,float AddDef,float AddSpeed,
		float ExtraDamage,float ExtraHp,int AddGold,int AddExp);
	~PassiveMod();

private:
	//ModId
	int modId = 0;
	//Mod�^�C�v
	int modType = 0;
	//Mod�̖��O
	std::string name = "";

	//�U���{���l
	float attackRate = 0;
	//�U�����Z�l
	float addAttack = 0;

	//�h��{���l
	float defRate = 0;
	//�h����Z�l
	float addDef = 0;

	//���x�{���l
	float speedRate = 0;
	//���x���Z�l
	float addSpeed = 0;

	//�ǉ��_���[�W�{���l
	float extraDamageRate = 0;
	//�ő�Hp�ǉ��{���l
	float extraHpRate = 0;

	//�l��Gold���Z
	int addGold = 0;
	//�l���o���l���Z
	int addExp = 0;




};
