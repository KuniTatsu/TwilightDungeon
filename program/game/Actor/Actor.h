///*****Description*****
///�������Ɋւ���N���X
///�v���C���[�ƃG�l�~�[��h����Ɏ��B
/// �e��X�e�[�^�X�ƕ`��֐�������
///*********************
#pragma once
#include"../../library/t2klib.h"
#include<string>

class Actor {
public:
	Actor();
	virtual ~Actor();

	bool isLive = true;

	enum dir {
		UP,
		RIGHT,
		DOWN,
		LEFT
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

	virtual void Atack();
	virtual bool Move();

	const enum class AnimPattern {
		WALK,
		EFFECT
	};

	//�A�j���[�V������������gh�z��,�`��X�s�[�h,�`��ő喇���������Ɏ��A�j���[�V�����֐�
	void Anim(int* DrawGhArr, int Speed, int MaxIndex, int& DrawGh);

	virtual void Update();
	virtual void Draw();
protected:
	int actId;
	//enum dir {
	//	UP,
	//	RIGHT,
	//	DOWN,
	//	LEFT
	//};
	//dir mydir = UP;

	int id = 0;
	int type = 0;
	std::string name = "";
	//����, �A�C�e���ɂ��ǉ��X�e�[�^�X
	int equipHp = 0;
	int equipAtack = 0;
	int equipDefence = 0;
	int equipSpeed = 0;

	//���̑��v���ɂ��ǉ��X�e�[�^�X
	int exHp = 0;
	int exAtack = 0;
	int exDefence = 0;
	int exSpeed = 0;

	//��b�X�e�[�^�X
	int baseHp = 0;
	int baseAtack = 0;
	int baseDefence = 0;
	int baseSpeed = 0;
	//�`��X�e�[�^�X
	int hp = 0;
	int atack = 0;
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
	double nowHpVarWidth = nowHp / hp;


	int actSpeed = 20;
	int maxMotionIndex = 3;


	//�ʏ펞�̕`�悷��A�j���[�V�����摜
	int drawGh = 0;
	int actWait = actSpeed;
	int actIndex = 0;

	t2k::Vector3 animPos = { 0,0,0 };


	int moveTimer = 0;
	const int MOVETIME = 60;



};