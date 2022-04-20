///*****Description*****
///�������Ɋւ���N���X
///�v���C���[�ƃG�l�~�[��h����Ɏ��B
/// �e��X�e�[�^�X�ƕ`��֐�������
///*********************
#pragma once
#include"../../library/t2klib.h"
#include<string>

class Skill;
class PassiveMod;

class Actor {
public:
	Actor();
	virtual ~Actor();

	bool isLive = true;

	enum dir {
		UP,
		RIGHT,
		DOWN,
		LEFT,
		MAX
	};
	dir mydir = UP;

	const dir dires[4]{ dir::UP,dir::RIGHT,dir::DOWN,dir::LEFT };

	void setDir(const int dir);

	t2k::Vector3 pos = {};
	t2k::Vector3 skill = {};

	//�O����X�e�[�^�X���擾����֐� StatusNum:0->HP,1->Atack,2->Defence,3->Speed,4->level
	int GetStatus(const int StatusNum);

	//���O�̎擾�֐�
	inline std::string GetName() {
		return name;
	};

	const enum  ChangeStatusType {
		EFFECT,
		EQUIP
	};

	//�X�e�[�^�X�㏸�̊֐� StatusNum:0->HP,1->Atack,2->Defence,3->Speed,StatusType->0:effect,1:equipment
	void ChangeStatus(const int StatusNum, const int MoveNum, const int StatusType);

	//�����X�e�[�^�X�̏���
	void RemoveStatus(const int StatusNum, const int MoveNum);

	//�����X�e�[�^�X�̑S����
	inline void RemoveAllEquipStatus() {
		equipHp = 0;
		equipAttack = 0;
		equipDefence = 0;
		equipSpeed = 0;
	}

	//���x���X�e�[�^�X�̑S����
	inline void RemoveAllLevelStatus() {
		exHp = 0;
		exAttack = 0;
		exDefence = 0;
		exSpeed = 0;
	}

	//actId�擾
	inline int GetActId() {
		return actId;
	}

	//�A�j���[�V�������W�̐ݒ�
	void SetAnimPos();

	t2k::Vector3 effectPos;

	//�U���A�j���[�V�����̌���
	bool SetDrawAnim();
	//�U���A�j���[�V�����̕`��
	void DrawAttackAnim();

	//Hp�̕ϓ����O������^����ꂽ���Ɏg���֐�
	void TakeHpEffect(const int HpMove);

	virtual void Attack();
	virtual void SkillAttack(Skill* usedSkill);

	virtual bool Move();

	const enum class AnimPattern {
		WALK,
		EFFECT
	};

	//�A�j���[�V������������gh�z��,�`��X�s�[�h,�`��ő喇���������Ɏ��A�j���[�V�����֐�
	void Anim(int* DrawGhArr, int Speed, int MaxIndex, int& DrawGh);

	//�����X�L�����X�g���擾����
	inline std::vector<Skill*>& GetSkillList() {
		return mySkill;
	}

	virtual void Update();
	virtual void Draw();
protected:
	int actId;

	int id = 0;
	int type = 0;
	std::string name = "";
	//����, �A�C�e���ɂ��ǉ��X�e�[�^�X
	int equipHp = 0;
	int equipAttack = 0;
	int equipDefence = 0;
	int equipSpeed = 0;

	//���̑��v���ɂ��ǉ��X�e�[�^�X
	int exHp = 0;
	int exAttack = 0;
	int exDefence = 0;
	int exSpeed = 0;

	//��b�X�e�[�^�X
	int baseHp = 0;
	int baseAttack = 0;
	int baseDefence = 0;
	int baseSpeed = 0;
	//�`��X�e�[�^�X
	int hp = 0;
	int attack = 0;
	int defence = 0;
	int speed = 0;

	//��E�����̏���3������ �L�����摜
	int gh[12];

	//�U���G�t�F�N�g
	int effectGh[5];

	int level = 1;

	float nowHp = hp;

	int nowHpVar_gh = 0;
	int hpVar_gh = 0;

	const float VARWIDTH = 30;
	double nowHpVarWidth = static_cast<double>(nowHp / hp);


	int actSpeed = 20;
	int maxMotionIndex = 3;


	//�ʏ펞�̕`�悷��A�j���[�V�����摜
	int drawGh = 0;
	int actWait = actSpeed;
	int actIndex = 0;

	t2k::Vector3 animPos = { 0,0,0 };


	int moveTimer = 0;
	const int MOVETIME = 60;

	//�X�L���֘A
	//�����X�L��
	std::vector<Skill*>mySkill = {};

	//�����p�b�V�u����
	std::vector<PassiveMod*>myMod = {};

};