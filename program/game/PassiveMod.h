//������v���C���[,�G�ɕt�^�����p�b�V�u���ʂ̃N���X
//�����͍ő�5��,�v���C���[�ƓG�͍ő吔�Ȃ�,���ʏ��ł܂ł̃^�[������
//�{���l:�U���͂ɏ�Z�����,���Z�l:�U���͂ɉ��Z�����

#pragma once
#include<string>

class PassiveMod
{
public:
	PassiveMod();
	~PassiveMod();

private:
	//ModId
	int skillId = 0;
	//Mod�^�C�v
	int skillType = 0;
	//Mod�̖��O
	std::string name = "";

	//�U���{���l
	float damageRate = 0;
	//�U�����Z�l
	float addDamage = 0;
	//�h��{���l
	float defRate = 0;
	//�h����Z�l
	float addDef = 0;
	//���x�{���l
	float SpeedRate = 0;
	//���x���Z�l
	float addSpeed = 0;

	//�l��Gold���Z
	int addGold = 0;
	//�l���o���l���Z
	int addExp = 0;




};
