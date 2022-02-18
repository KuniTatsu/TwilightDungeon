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
	////mydir�擾
	//inline dir getDir() {
	//	return mydir;
	//}
	
	//�U���A�j���[�V����
	void DrawAttackAnim();
	//Hp�̕ϓ����O������^����ꂽ���Ɏg���֐�
	void TakeHpEffect(const int HpMove);

	virtual void Atack();
	virtual bool Move();
	void Anim();

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

	//��E�����̏���3������
	int gh[12];

	int level = 1;

	float nowHp = hp;

	int nowHpVar_gh = 0;
	int hpVar_gh = 0;

	const float VARWIDTH = 30;
	double nowHpVarWidth = nowHp / hp;


	const int ACT_SPEED = 20;
	const int MAX_MOTION_INDEX = 3;
	int act_wait = ACT_SPEED;
	int act_index = 0;

	int drawGh = 0;


	int moveTimer = 0;
	const int MOVETIME = 60;



};